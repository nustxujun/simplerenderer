#include "DefaultPipeline.h"
#include "RendererHeader.h"

namespace RCP
{
	DefaultPipeline::DefaultPipeline()
	{
		mPlane[0].x =  1;
		mPlane[1].x = -1;
		mPlane[2].y =  1;
		mPlane[3].y = -1;
		mPlane[4].z =  1;
		mPlane[5].z = -1;
	}

	DefaultPipeline::~DefaultPipeline()
	{}

	void DefaultPipeline::initImpl() 
	{
		const RendererParameters& rp = getRendererParameters();
		mRasterizer.initialize(rp.backBufferWidth, rp.backBufferHeight, rp.backBufferPixelFormat);
	}

	void DefaultPipeline::execute(const RenderData& renderData,RenderTarget* target)
	{
		//預處理
		//mPositionList.clear();
		mPrimitiveVector.clear();

		const RenderData::RenderElementList& elems = renderData.getRenderElementList();
		size_t elemSize = elems.size();
		if (mVertexList.size() < elemSize)
			mVertexList.resize(elemSize);

		//show time.
		RenderData::RenderElementList::const_iterator i,endi = elems.end();
		VertexList::iterator verIter = mVertexList.begin();
		for (i = elems.begin(); i != endi; ++i,++verIter)
		{
			//verIter->clear();
			if (i->vertexBuffer->getVertexCount() > verIter->size())
				verIter->resize(i->vertexBuffer->getVertexCount());//這裡就直接初始化了，下面就不再push_back了
			vertexProcessing(*i,*verIter);
			primitiveAssembly(*i,*verIter);
		}

		PrimitiveVector::iterator priIter, priEndIter = mPrimitiveVector.end();
		Primitive priResult[2];
		for (priIter = mPrimitiveVector.begin(); priIter != priEndIter; ++priIter)
		{
			//返回true则说明通过，false则剔除
			 if ((*priIter).type == Primitive::ERROR  || !culling(*priIter) )
				 continue;
			 cliping(*priIter,priResult);
			 //产生的clip新结果插入
			 for (unsigned int k = 0; k < 2; ++k)
			 {
				 //这里将进行
				 //齐次坐标归一（透视除法。。。怎么这么多名字）
				 //视口映射
				 afterCliping(priResult[k]);
				 priResult[k].type = Primitive::ERROR;
			 }

			 
		}

		rasterization(target);
		notifyCompleted();
	}

	
	void DefaultPipeline::rasterization(RenderTarget* target)
	{
		mRasterizer.flush(target);
	}

	void DefaultPipeline::vertexProcessing(const RenderElement& elem, VertexVector& verVec)
	{
		const VertexBuffer* vb = elem.vertexBuffer;
		unsigned int vertexSize = vb->getVertexDeclaration().getSizeInBytes();
		//這裡取的是elem的vertexCount，注意，和VertexBuffer裏不一樣
		size_t dataSize = vb->getVertexCount() * vertexSize;
		//各个数据在一個頂點中的偏移大小
		const VertexDeclaration& verDecl = vb->getVertexDeclaration();
		unsigned int posDataOffset = verDecl.getElementOffsetInBytes(VES_POSITION);
		unsigned int diffuseDataOffset = verDecl.getElementOffsetInBytes(VES_DIFFUSE);
		unsigned int specularDataOffset = verDecl.getElementOffsetInBytes(VES_SPECULAR);
		unsigned int normalDataOffset = verDecl.getElementOffsetInBytes(VES_NORMAL);
		unsigned int texCroodDataOffset[8];
		for (unsigned char i = 0; i < 8; ++i)
		{
			texCroodDataOffset[i] = verDecl.getElementOffsetInBytes(VES_TEXTURE_COORDINATES,i);
		}
		//尋不到Position信息就傻了
		assert(posDataOffset != -1);
		//獲取4個權重的偏移
		unsigned int weightDataOffet[4] ;
		for (int i = 0; i < 4; ++i )
			weightDataOffet[i]= verDecl.getElementOffsetInBytes(VES_BLEND_WEIGHTS,i);

		//原始数据
		const unsigned char* data = vb->getData() ;
		//const unsigned char* end = vb->getData() + dataSize;
		Matrix4X4 transMat;
		float weight[4] = {1,0,0,0};
		Colour diffuse,specular,diffuseBlend,specularBlend;
		float power;
		bool enable = false;
		Vector3 normVec;
		Vector3 posVec;
		Vector4 posVec4;
		for (size_t i = 0; i < vb->getVertexCount(); ++i)
		{
			//定位頂點position
			const Position* posData =(const Position*)( data + i * vertexSize + posDataOffset);
			posVec4.x = posData->x;
			posVec4.y = posData->y;
			posVec4.z = posData->z;
			posVec4.w= 1.0f;

			//-----------------------------------------------------
			//這裡注意！！！！
			//暫時默認為4個weight的混合
			//-----------------------------------------------------

			//獲取4個權重
			for (int j = 0; j < 4; ++j)
			{
				
				if (weightDataOffet[j] == -1)//儅該權重不存在
				{
					//計算權重
					weight[j] = 1;
					for (int k = 0; k < j ; ++k )
						weight[j] = weight[j] - weight[k];
					break;
				}
				const float* weightData = (const float*)( data + i * vertexSize + weightDataOffet[j]);
				weight[j] = *weightData;
			}
			//計算混合
			transMat = elem.matWorld[TS_WORLD] * weight[0] + 
				elem.matWorld[TS_WORLD2] * weight[1] + 
				elem.matWorld[TS_WORLD3] * weight[2] + 
				elem.matWorld[TS_WORLD4] * weight[3];
			//矩阵变换
			posVec4 =  transMat * posVec4 ;//* elem.matWorld[TS_PROJECTION]  ;
			
			//记录下变换前的坐标 进行光照计算
			posVec.x = posVec4.x;
			posVec.y = posVec4.y;
			posVec.z = posVec4.z;

			//继续变换
			posVec4 = elem.matWorld[TS_PROJECTION] * (elem.matWorld[TS_VIEW] * posVec4);

			//記錄下頂點的坐标
			verVec[i].pos = posVec4;	
			

			//获取颜色(材质ye处理) 
			diffuse = elem.material.diffuse;
			specular = elem.material.specular;
			power = elem.material.power;
			if (diffuseDataOffset != -1 && elem.material.isDiffuseVertexColorEnable())
			{
				if (verDecl.getElementSizeInBytes(VES_DIFFUSE) == 4)
				{
					const int* colorData =(const int*)( data + i * vertexSize + diffuseDataOffset);
					diffuse = Colour().getFromARGB(*colorData) * diffuse ;
					
				}
				else if (verDecl.getElementSizeInBytes(VES_DIFFUSE) == 16)
				{
					const Color* colorData =(const Color*)( data + i * vertexSize + diffuseDataOffset);

					diffuse = diffuse * Colour(colorData->r,colorData->g,colorData->b,colorData->a);
				}
				
			}
			if (specularDataOffset != -1 && elem.material.isSpecularVertexColorEnable())
			{
				if (verDecl.getElementSizeInBytes(VES_SPECULAR) == 4)
				{
					const int* colorData =(const int*)( data + i * vertexSize + specularDataOffset);
					specular = specular + Colour().getFromARGB(*colorData);
					
				}
				else if (verDecl.getElementSizeInBytes(VES_SPECULAR) == 16)
				{
					const Color* colorData =(const Color*)( data + i * vertexSize + specularDataOffset);

					diffuse = diffuse + Colour(colorData->r,colorData->g,colorData->b,colorData->a);
				}
			}


			//法线
			if (normalDataOffset != -1)
			{
				const Position* normalData = (const Position*)( data + i * vertexSize + normalDataOffset);
				verVec[i].norm.x = normalData->x;
				verVec[i].norm.y = normalData->y;
				verVec[i].norm.z = normalData->z;
			}

			//光照计算
			diffuseBlend.getFromABGR(0);
			specular.getFromABGR(0);
			enable = false;
			for (unsigned int index = 0; index < 8; ++index)
			{
				if (!elem.light[index].isEnable())
					continue;
				enable = true;
				normVec = elem.light[index].position - posVec;
				normVec.normalise();
				diffuseBlend += elem.light[index].diffuse * 
					std::max<float>(0,verVec[i].norm.dotProduct(normVec));
			}

			//如果使用了灯光。
			if (enable)
			{
				diffuseBlend.r = diffuseBlend.r > 1? 1: diffuseBlend.r;
				diffuseBlend.g = diffuseBlend.g > 1? 1: diffuseBlend.g;
				diffuseBlend.b = diffuseBlend.b > 1? 1: diffuseBlend.b;
				diffuseBlend.a = diffuseBlend.a > 1? 1: diffuseBlend.a;

				diffuse = diffuse * diffuseBlend;
			}
			//temp
			verVec[i].color = diffuse;
			

			//纹理坐标
			for (unsigned char k = 0; k < 8; ++k )
			{
				if (texCroodDataOffset[k] != -1)
				{
					const TexCrood* croodData = (const TexCrood*) ( data + i * vertexSize + texCroodDataOffset[k]);
					verVec[i].texCrood[k].x = croodData->u;
					verVec[i].texCrood[k].y = croodData->v;
				}
			}


		}
	}

	void DefaultPipeline::primitiveAssembly(const RenderElement& elem,const VertexVector& verVec)
	{
		const IndexBuffer* ib = elem.indexBuffer;
		const VertexBuffer* vb = elem.vertexBuffer;
		unsigned int skipVertexCount = 0;
		size_t realVertexCount = 0;
		unsigned int type = 0;
		//得到实际需要绘制的顶点数
		if (ib != NULL)
			realVertexCount = ib->getIndexCount();
		else
			realVertexCount = vb->getVertexCount();

		//图元需要的顶点数
		unsigned int vertexNumInGeom ;
		switch (elem.ptType)
		{
		case PT_POINTLIST:
			vertexNumInGeom = 1;
			type = Primitive::POINT;
			skipVertexCount = elem.beginPrimitiveOffset;
			break;
		case PT_LINELIST:
			vertexNumInGeom = 2;
			type = Primitive::LINE;
			skipVertexCount = elem.beginPrimitiveOffset;
			break;
		case PT_LINESTRIP:
			vertexNumInGeom = 2;
			type = Primitive::LINE;
			skipVertexCount = elem.beginPrimitiveOffset * 2;
			break;
		case PT_TRIANGLELIST:
		case PT_TRIANGLEFAN:
			type = Primitive::TRIANGLE;
			vertexNumInGeom = 3;
			skipVertexCount = elem.beginPrimitiveOffset ;
			break;
		case PT_TRIANGLESTRIP:
			type = Primitive::TRIANGLE;
			vertexNumInGeom = 3;
			skipVertexCount = elem.beginPrimitiveOffset * 3;
			break;
		default:
			assert(0);
			break;
		}

		//保证剩余顶点数足够形成图元
		assert(realVertexCount - skipVertexCount >= vertexNumInGeom*elem.primitiveCount );

		if (mPrimitiveVector.capacity() < elem.primitiveCount + mPrimitiveVector.size())
			mPrimitiveVector.reserve(elem.primitiveCount + mPrimitiveVector.size());
		Primitive* pri = NULL;
		for (unsigned int i = 0; i < elem.primitiveCount; ++i)
		{
			mPrimitiveVector.push_back(Primitive());
			pri = &*mPrimitiveVector.rbegin();
			pri->type = type;
			pri->vp = &elem.viewport;
			memcpy(pri->tex, elem.texture,sizeof(Texture*) * 8);
			for (unsigned int j = 0; j < vertexNumInGeom; ++j)
			{
				if (ib != NULL)
				{
					pri->vertex[j] = verVec[(*ib)[skipVertexCount + i*vertexNumInGeom + j]];
				}
				else
				{
					pri->vertex[j] = verVec[skipVertexCount + i*vertexNumInGeom + j];
				}
			}
				
		}
	}

	bool DefaultPipeline::culling(const Primitive& prim)
	{
		if (prim.type != Primitive::TRIANGLE)
			return true;
		
		
		//临时
		return true;

		const Vector4& pos0 = prim.vertex[0].pos;
		const Vector4& pos1 = prim.vertex[1].pos;
		const Vector4& pos2 = prim.vertex[2].pos; 
		//暂时默认逆时针剔除
		float result = (float)(pos0.w * (pos1.x * pos2.y - pos2.x * pos1.y) + 
			pos1.w * (pos2.x * pos0.y - pos0.x * pos2.y) + 
			pos2.w * (pos0.x * pos1.y - pos1.x * pos0.y)) ;
		if (result > 0)
			return true;
		else
			return false;
		
	}

	void DefaultPipeline::cliping(const Primitive& prim,Primitive prims[2])
	{
		switch (prim.type)
		{
		case Primitive::POINT:
			{
				const Vector4& pos = prim.vertex[0].pos;
				//被裁减
				if ( !checkPointInScreen(pos) )
					 return;
				prims[0] = prim;
				break;
			}
		case Primitive::LINE:
			{
				clipingLine(prim,prims[0]);
				break;
			}
		case Primitive::TRIANGLE:
			{
				clipingTriangle(prim,prims);
				break;
			}
		default:
			assert(0);
		}
	
	}

	void DefaultPipeline::afterCliping(Primitive& prim)
	{
		if (prim.type == Primitive::ERROR)
			return;

		unsigned int num = 0;
		switch(prim.type)
		{
		case Primitive::POINT:
			num = 1;
			break;
		case Primitive::LINE:
			num = 2;
			break;
		case Primitive::TRIANGLE:
			num = 3;
			break;
		default:
			assert(0);
		}

		for (unsigned int i= 0; i < num; ++i)
		{
			homogeneousDivide(prim.vertex[i].pos);
			viewportMapping(prim.vertex[i].pos,prim.vp);			
		}
		mRasterizer.pushPrimitive(prim);

	}

	void DefaultPipeline::homogeneousDivide(Vector4& pos)
	{
		pos /= pos.w;
	}

	void DefaultPipeline::viewportMapping(Vector4& pos,const Viewport* vp)
	{
		vp->mapping(pos);
	}




	template<class T>
	void DefaultPipeline::Interpolate(T& result, const T& vec1, const T& vec2, float scale)
	{
		result = (vec2 - vec1) * scale + vec1;
	}

	void DefaultPipeline::generateNewVertex(Vertex& newVertex,const Vertex& vert1, const Vertex& vert2,const Vector4& plane)
	{
		Vector4 pos1(vert1.pos.x/vert1.pos.w, vert1.pos.y/vert1.pos.w, vert1.pos.z/vert1.pos.w,1);
		Vector4 pos2(vert2.pos.x/vert2.pos.w, vert2.pos.y/vert2.pos.w, vert2.pos.z/vert2.pos.w,1);
		Vector4 normal(plane.x, plane.y, plane.z,0);

		float dist1 = fabs(pos1.dotProduct( normal) + plane.w);
		float dist2 = fabs(pos2.dotProduct( normal) + plane.w);

		Vector4 result = pos1 - pos2;
		float length1 = sqrt(result.dotProduct( result));

		float length2 = dist1 * length1 / (dist1 + dist2) ;

		float scale = length2/length1;

		Interpolate(newVertex.color,vert1.color,vert2.color,scale);
		Interpolate(newVertex.norm,vert1.norm,vert2.norm,scale);
		Interpolate(newVertex.pos,vert1.pos,vert2.pos,scale);
		for (unsigned int i = 0 ; i < 8; ++i)
		{
			Interpolate(newVertex.texCrood[i],vert1.texCrood[i],vert2.texCrood[i],scale);
		}


	}

	bool DefaultPipeline::checkPointInScreen(const Vector4& point)
	{
		//临时
		return true;
		if ( /*fabs(point.x ) > point.w ||
			fabs(point.y ) > point.w ||
			point.z > point.w ||*/
			point.z <= 0
			)
			return false;
		return true;
	}

	void DefaultPipeline::clipingLine(const Primitive& prim,Primitive& resultPrim)
	{
		const Vector4& pos1 = prim.vertex[0].pos;
		const Vector4& pos2 = prim.vertex[1].pos;

		bool point1 = checkPointInScreen(pos1);
		bool point2 = checkPointInScreen(pos2);

		if (point1 && point2)
		{
			resultPrim = prim; 
			return;
		}
		else if ( !point1 && !point2)
			return;

		resultPrim.type = prim.type;
		resultPrim.vertex[0] = point1?prim.vertex[0]:prim.vertex[1];

		//暂时只考虑nearPlane的情况
		generateNewVertex(resultPrim.vertex[1],prim.vertex[0],prim.vertex[1],mPlane[5]);

	}

	void DefaultPipeline::clipingTriangle(const Primitive& prim,Primitive prims[2])
	{
		int reserveIndex[3];
		int disusedIndex[3];

		unsigned int reserveCount = 0;
		unsigned int disusedCount = 0;

		bool points[3] ;
		points[0] = checkPointInScreen(prim.vertex[0].pos);
		points[1] = checkPointInScreen(prim.vertex[1].pos);
		points[2] = checkPointInScreen(prim.vertex[2].pos);

		if (points[0] && points[1] && points[2])
		{
			prims[0] = prim; 
			return;
		}
		else if (!points[0] && !points[1] && !points[2])
		{
			return;
		}

		//为顶点分组
		for (unsigned int i = 0; i < 3 ; ++i)
		{
			if (points[i])
				reserveIndex[reserveCount++] = i;
			else
				disusedIndex[disusedCount++] = i;
		}

		//生成新的图元（已经cull了就无所谓了顺序了）
		if (reserveCount == 2)//2个点在里面
		{
			prims[0].type = prim.type;
			prims[0].vertex[0] = prim.vertex[reserveIndex[0]];
			prims[0].vertex[1] = prim.vertex[reserveIndex[1]];

			generateNewVertex(prims[0].vertex[2],prim.vertex[disusedIndex[0]],
				prim.vertex[reserveIndex[0]],mPlane[5]);

			prims[1].type = prim.type;
			prims[1].vertex[0] = prim.vertex[reserveIndex[1]];
			prims[1].vertex[0] = prims[0].vertex[2];//新点

			generateNewVertex(prims[0].vertex[2],prim.vertex[disusedIndex[0]],
				prim.vertex[reserveIndex[1]],mPlane[5]);
		}
		else//两个顶点在外面
		{
			prims[0].type = prim.type;
			prims[0].vertex[0] = prim.vertex[reserveIndex[0]];

			generateNewVertex(prims[0].vertex[1],prim.vertex[disusedIndex[0]],
				prim.vertex[reserveIndex[0]],mPlane[5]);

			generateNewVertex(prims[0].vertex[2],prim.vertex[disusedIndex[1]],
				prim.vertex[reserveIndex[0]],mPlane[5]);

		}
	}

}
#include "DefaultPipeline.h"
#include "RendererHeader.h"
#include <sstream>
namespace RCP
{
	DefaultPipeline::DefaultPipeline():
		mVertexShader(NULL)
	{

	}

	DefaultPipeline::~DefaultPipeline()
	{}

	void DefaultPipeline::initImpl() 
	{
		mPlane[0]= Vector4(1,0,0,1);
		mPlane[1]= Vector4(-1,0,0,1);
		mPlane[2]= Vector4(0,1,0,1);
		mPlane[3]= Vector4(0,-1,0,1);
		mPlane[4]= Vector4(0,0,1,1);
		mPlane[5]= Vector4(0,0,-1,1);

		const RendererParameters& rp = getRendererParameters();
		mRasterizer.initialize(rp.backBufferWidth, rp.backBufferHeight, rp.backBufferPixelFormat);

	
	}

	void DefaultPipeline::execute(const RenderData& renderData)
	{

		const RenderParameter& rp = renderData.renderParameter;
		 
		//设置自定义渲染状态
		setOtherState(rp.propertys);
		if (rp.vertexBuffer->getVertexCount() > mVertexList.size())
			mVertexList.resize(rp.vertexBuffer->getVertexCount());//這裡就直接初始化了，下面就不再push_back了
		vertexProcessing(renderData,mVertexList);
		primitiveAssembly(renderData,mVertexList);
		mRasterizer.flush(*rp.frameBuffer,rp.renderState);

	}

	

	void DefaultPipeline::vertexProcessing(const RenderData& elem, VertexList& verVec)
	{
		const VertexBuffer* vb = elem.renderParameter.vertexBuffer;
		unsigned int vertexSize = vb->getVertexDeclaration().getSizeInBytes();
		//這裡取的是elem的vertexCount，注意，和VertexBuffer裏不一樣
		size_t dataSize = vb->getVertexCount() * vertexSize;
		//各个数据在一個頂點中的偏移大小
		const VertexDeclaration& verDecl = vb->getVertexDeclaration();
		unsigned int posDataOffset = verDecl.getElementOffsetInBytes(VES_POSITION);
		unsigned int diffuseDataOffset = verDecl.getElementOffsetInBytes(VES_DIFFUSE);
		unsigned int specularDataOffset = verDecl.getElementOffsetInBytes(VES_SPECULAR);
		unsigned int normalDataOffset = verDecl.getElementOffsetInBytes(VES_NORMAL);
		unsigned int tangentDataOffset = verDecl.getElementOffsetInBytes(VES_TANGENT);
		unsigned int binormalDataOffset = verDecl.getElementOffsetInBytes(VES_BINORMAL);
		unsigned int texCroodDataOffset[8];
		for (unsigned char i = 0; i < 8; ++i)
		{
			texCroodDataOffset[i] = verDecl.getElementOffsetInBytes(VES_TEXTURE_COORDINATES,i);
		}
		//尋不到Position信息就傻了
		if(posDataOffset == -1)
			THROW_EXCEPTION("未找到顶点位置信息，因为没有多流，因此顶点处理失败");
		//獲取4個權重的偏移
		unsigned int weightDataOffet[4] ;
		for (int i = 0; i < 4; ++i )
			weightDataOffet[i]= verDecl.getElementOffsetInBytes(VES_BLEND_WEIGHTS,i);

		//原始数据
		const unsigned char* data = vb->getData() ;
		//const unsigned char* end = vb->getData() + dataSize;
		Matrix4X4 transMat;
		float weight[4] = {1,0,0,0};
		Colour diffuse,specular,ambient, diffuseBlend,specularBlend, ambientBlend;
		float power;
		bool enable = false;
		Vector3 posVec;
		Vector4 normVec4;
		Vector3 H,L,V,lightPos,normVec3;
		const unsigned char *vertexData;
		for (size_t i = 0; i < vb->getVertexCount(); ++i)
		{
			vertexData = data + i * vertexSize;

			//定位頂點position
			mDataCollector.getData(posVec,vertexData + posDataOffset);
			verVec[i].pos = posVec;
			//点
			verVec[i].pos.w= 1.0f;
	


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
				mDataCollector.getData(weight[j],vertexData + weightDataOffet[j]);
			}

			//获取颜色(材质ye处理) 
			diffuse = elem.renderParameter.material.diffuse;
			specular = elem.renderParameter.material.specular;
			power = elem.renderParameter.material.power;
			ambient = elem.renderParameter.material.ambient;
			if (diffuseDataOffset != -1 && elem.renderParameter.material.isDiffuseVertexColorEnable())
			{
				if (verDecl.getElementSizeInBytes(VES_DIFFUSE) == 4)
				{
					int c;
					mDataCollector.getData(c,vertexData + diffuseDataOffset);
					diffuse = Colour().getFromARGB(c) * diffuse ;
					
				}
				else if (verDecl.getElementSizeInBytes(VES_DIFFUSE) == 16)
				{
					Vector4 c;
					mDataCollector.getData(c,vertexData + diffuseDataOffset);

					diffuse = diffuse * Colour(c.x,c.y,c.z,c.w);
				}
				
			}
			if (specularDataOffset != -1 && elem.renderParameter.material.isSpecularVertexColorEnable())
			{
				if (verDecl.getElementSizeInBytes(VES_SPECULAR) == 4)
				{
					int c;
					mDataCollector.getData(c,vertexData + specularDataOffset);
					specular = specular + Colour().getFromARGB(c);
					
				}
				else if (verDecl.getElementSizeInBytes(VES_SPECULAR) == 16)
				{
					Vector4 c;
					mDataCollector.getData(c,vertexData + specularDataOffset);
					diffuse = diffuse * Colour(c.x,c.y,c.z,c.w);
				}
			}


			//法线
			if (normalDataOffset != -1)
			{
				mDataCollector.getData(verVec[i].norm,vertexData + normalDataOffset);			
			}

			//纹理坐标
			for (unsigned char k = 0; k < 8; ++k )
			{
				if (texCroodDataOffset[k] != -1 && elem.renderParameter.sampler[k].texture != NULL)
				{
					mDataCollector.getData(verVec[i].texCrood[k],vertexData + texCroodDataOffset[k]);
					//取样时才计算寻址
					//elem.sampler[k].assignUV(verVec[i].texCrood[k].x,verVec[i].texCrood[k].y);					
				}
			}

			//切线
			if (tangentDataOffset != -1)
			{
				mDataCollector.getData(verVec[i].tan,vertexData + tangentDataOffset);			
			}
			//次法线
			if (binormalDataOffset != -1)
			{
				mDataCollector.getData(verVec[i].bino,vertexData + binormalDataOffset);			
			}


			//以上为数据采集
			if (mVertexShader != NULL)
			{
				mVertexShader->execute(verVec[i]);
				continue;
			}

			//以下为数据计算

			//計算混合
			transMat = elem.renderParameter.matrices[TS_WORLD] * weight[0] + 
				elem.renderParameter.matrices[TS_WORLD2] * weight[1] + 
				elem.renderParameter.matrices[TS_WORLD3] * weight[2] + 
				elem.renderParameter.matrices[TS_WORLD4] * weight[3];
			//矩阵变换
			verVec[i].pos = elem.renderParameter.matrices[TS_VIEW] *( transMat * verVec[i].pos );//* elem.matrices[TS_PROJECTION]  ;
			
			//记录下变换前的坐标 进行光照计算
			posVec.x = verVec[i].pos.x;
			posVec.y = verVec[i].pos.y;
			posVec.z = verVec[i].pos.z;

			//继续变换
			verVec[i].pos = elem.renderParameter.matrices[TS_PROJECTION] *  verVec[i].pos;	
			


			//光照计算
			diffuseBlend.getFromABGR(0);
			specularBlend.getFromABGR(0);
			ambientBlend.getFromABGR(0);
			enable = false;
			for (unsigned int index = 0; index < 8; ++index)
			{
				if (!elem.renderParameter.light[index].isEnable())
					continue;
				enable = true;
				
				//注意这里灯的坐标和法向量都进行了变换。
				//因为此时要的到顶点指向摄像机的向量需要摄像机坐标，而这里没有提供逆矩阵
				//于是将法向量和灯变换到摄像机坐标系。
				//当然法向量的话 还有一个世界坐标系的变换。
				lightPos = elem.renderParameter.matrices[TS_VIEW] *(elem.renderParameter.light[index].position);
				L = lightPos - posVec;
				L.normalise();
				V = - posVec;
				V.normalise();
				H = L + V;
				H.normalise();
				
				//注意这里有个隐藏操作是normVec4.w设为了0，不是1
				normVec4 = verVec[i].norm;
				//normal的话只需要旋转方向
				normVec4 = elem.renderParameter.matrices[TS_VIEW] * (transMat * normVec4);
				normVec3.x = normVec4.x;
				normVec3.y = normVec4.y;
				normVec3.z = normVec4.z;

				diffuseBlend += elem.renderParameter.light[index].diffuse * 
					std::max<float>(0,normVec3.dotProduct(L));

				specularBlend += elem.renderParameter.light[index].specular * 
					pow(std::max<float>(0,H.dotProduct(normVec3)),power);
				ambientBlend += elem.renderParameter.light[index].ambient;
			}

			//如果使用了灯光。
			if (enable)
			{
				float a;
				a = diffuse.a;
				diffuse = diffuse * diffuseBlend;
				diffuse.a = a;
				a = ambient.a;
				ambient = ambient * ambientBlend;
				ambient.a = a;
				a = specular.a;
				specular = specular * specularBlend;
				specular.a = a;

				//diffuse.clip();
				//ambient.clip();
				//specular.clip();
			}
			//temp
			verVec[i].color[0] = diffuse + ambient;
			verVec[i].specular = specular;
			
		}
	}

	void DefaultPipeline::primitiveAssembly(const RenderData& elem,const VertexList& verVec)
	{
		const IndexBuffer* ib = elem.renderParameter.indexBuffer;
		const VertexBuffer* vb = elem.renderParameter.vertexBuffer;
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
			type = Primitive::PT_POINT;
			skipVertexCount = elem.beginPrimitiveOffset;
			break;
		case PT_LINELIST:
			vertexNumInGeom = 2;
			type = Primitive::PT_LINE;
			skipVertexCount = elem.beginPrimitiveOffset;
			break;
		case PT_LINESTRIP:
			vertexNumInGeom = 2;
			type = Primitive::PT_LINE;
			skipVertexCount = elem.beginPrimitiveOffset * 2;
			break;
		case PT_TRIANGLELIST:
		case PT_TRIANGLEFAN:
			type = Primitive::PT_TRIANGLE;
			vertexNumInGeom = 3;
			skipVertexCount = elem.beginPrimitiveOffset ;
			break;
		case PT_TRIANGLESTRIP:
			type = Primitive::PT_TRIANGLE;
			vertexNumInGeom = 3;
			skipVertexCount = elem.beginPrimitiveOffset * 3;
			break;
		default:
			assert(0);
			break;
		}

		//保证剩余顶点数足够形成图元
		if (realVertexCount - skipVertexCount < vertexNumInGeom*elem.primitiveCount )
			THROW_EXCEPTION("顶点数量不足以绘制指定数量图元");

		Primitive prim ;
		Primitive priResult[5];
		for (unsigned int i = 0; i < elem.primitiveCount; ++i)
		{
			prim.type = type;
			prim.vp = &elem.renderParameter.viewport;
			
			for (short j = 0; j < 8; ++j)
			{
				prim.sampler[j] = elem.renderParameter.sampler[j];
			}
			
			
			//输入顶点
			for (unsigned int j = 0; j < vertexNumInGeom; ++j)
			{
				if (ib != NULL)
				{
					prim.vertex[j] = verVec[(*ib)[skipVertexCount + i*vertexNumInGeom + j]];
				}
				else
				{
					prim.vertex[j] = verVec[skipVertexCount + i*vertexNumInGeom + j];
				}
			}
						
			//返回true则说明通过，false则剔除
			if (prim.type == Primitive::PT_ERROR  || !culling(prim,elem.renderParameter.renderState.cullMode) )
				 continue;
			 //同时把齐次归一，视口映射给做了，因为在顶点级可以少做几个顶点
			 clipping(prim,priResult);

			 //产生的clip新结果插入
			 for ( int k = 0; k < 5; ++k)
			 {
				 rasterize(priResult[k]);
				 priResult[k].type = Primitive::PT_ERROR;
			 }

		}
	}

	bool DefaultPipeline::culling(const Primitive& prim,CullMode cm)
	{
		if (prim.type != Primitive::PT_TRIANGLE || cm == CM_DISABLE)
			return true;
		

		const Vector4& pos0 = prim.vertex[0].pos;
		const Vector4& pos1 = prim.vertex[1].pos;
		const Vector4& pos2 = prim.vertex[2].pos; 
		//暂时默认逆时针剔除
		float result = (float)(pos0.w * (pos1.x * pos2.y - pos2.x * pos1.y) + 
			pos1.w * (pos2.x * pos0.y - pos0.x * pos2.y) + 
			pos2.w * (pos0.x * pos1.y - pos1.x * pos0.y)) ;
		if (result > 0)
			return cm == CM_CCW ;
		else
			return cm != CM_CCW;
		
	}

	void DefaultPipeline::clipping(const Primitive& prim,Primitive prims[5])
	{
		switch (prim.type)
		{
		case Primitive::PT_POINT:
			{
				clippingPoint(prim,prims[0]);
				break;
			}
		case Primitive::PT_LINE:
			{
				clippingLine(prim,prims[0]);
				break;
			}
		case Primitive::PT_TRIANGLE:
			{
				clippingTriangle(prim,prims);
				break;
			}
		default:
			assert(0);
		}
	
	}

	void DefaultPipeline::rasterize(Primitive& prim)
	{
		if (prim.type == Primitive::PT_ERROR)
			return;

		unsigned int num = 0;
		switch(prim.type)
		{
		case Primitive::PT_POINT:
			num = 1;
			break;
		case Primitive::PT_LINE:
			num = 2;
			break;
		case Primitive::PT_TRIANGLE:
			num = 3;
			break;
		default:
			assert(0);
		}

		mRasterizer.pushPrimitive(prim);

	}

	void DefaultPipeline::homogeneousDivide(Vertex& vert)
	{
		assert ( fabs(vert.pos.w) > 0.01f );
		float invw = 1.0f/vert.pos.w;

		vert.pos *= invw;
		vert.pos.w = invw;
		//vert.color *= invw;
		vert.specular *= invw;

		for (unsigned int i = 0; i < 8; ++i)
		{
			if(vert.texCrood.isUsed(i))
				vert.texCrood[i] *= invw;
			if(vert.color.isUsed(i))
				vert.color[i] *= invw;
		}
	}

	void DefaultPipeline::viewportMapping(Vector4& pos,const Viewport* vp)
	{
		vp->mapping(pos);
	}

	void DefaultPipeline::generateNewVertex(Vertex& newVertex,const Vertex& vert1, const Vertex& vert2, float dist1, float dist2)
	{
		float scale = (0 - dist1) / (dist2 - dist1);
		lerp(newVertex.specular,scale,vert1.specular,vert2.specular);
		lerp(newVertex.norm,scale,vert1.norm,vert2.norm);
		lerp(newVertex.tan,scale,vert1.tan,vert2.tan);
		lerp(newVertex.bino,scale,vert1.bino,vert2.bino);
		lerp(newVertex.pos,scale,vert1.pos,vert2.pos);
		for (unsigned int i = 0 ; i < 8; ++i)
		{
			if (vert1.texCrood.isUsed(i) || vert2.texCrood.isUsed(i))
				lerp(newVertex.texCrood[i],scale,vert1.texCrood[i],vert2.texCrood[i]);
			if (vert1.color.isUsed(i) || vert2.color.isUsed(i))
				lerp(newVertex.color[i],scale,vert1.color[i],vert2.color[i]);

		}
	}

	bool DefaultPipeline::checkPointInScreen(const Vector4& point)
	{
		if ( fabs(point.x ) > point.w ||
			fabs(point.y ) > point.w ||
			fabs(point.z )  > point.w 
			)
			return false;
		return true;
	}

	void DefaultPipeline::clippingPoint(const Primitive& prim,Primitive& resultPrim)
	{
		const Vector4& pos = prim.vertex[0].pos;
		//被裁减
		if ( !checkPointInScreen(pos) )
			return;

		resultPrim = prim;
		homogeneousDivide(resultPrim.vertex[0]);
		viewportMapping(resultPrim.vertex[0].pos,prim.vp);
		
	}

	void DefaultPipeline::clippingLine(const Primitive& prim,Primitive& resultPrim)
	{
		assert(prim.type == Primitive::PT_LINE);

		Vertex vertices[2][2];
		//使用的顶点下标
		int beforeClip = 0;
		int afterClip = 1;

		float d1,d2;

		for (int i =0; i < 2; ++i)
		{
			vertices[beforeClip][i] = prim.vertex[i];
		}


		int numPlane = 6;
		for(int plane = 0; plane < numPlane; ++plane)
		{

			d1 = vertices[beforeClip][0].pos.dotProduct(mPlane[plane]);
			d2 = vertices[beforeClip][1].pos.dotProduct(mPlane[plane]);

			if (d1 >= 0.0f)
			{
				vertices[afterClip][0] = vertices[beforeClip][0];
				if (d2 < 0.0f)
				{
					//lerp
					generateNewVertex( vertices[afterClip][1], 
						vertices[beforeClip][0], vertices[beforeClip][1], d1,d2);
				}
				else
				{
					vertices[afterClip][1] = vertices[beforeClip][1];
				}
			}
			else
			{
				if (d2 >= 0.0f)
				{
					vertices[afterClip][1] = vertices[beforeClip][1];
					//lerp
					generateNewVertex( vertices[afterClip][0], 
						vertices[beforeClip][0], vertices[beforeClip][1], d1,d2);
				}
				else
				{
					return;
					//都被裁剪
				}
			}
				
			//交换组
			(++beforeClip) &= 1; 
			(++afterClip) &= 1; 

		
		}//plane


		//齐次坐标归一（透视除法） & 视口映射
		for ( int i =0; i < 2; ++i)
		{
			homogeneousDivide(vertices[afterClip][i]);
			viewportMapping(vertices[afterClip][i].pos,prim.vp);
		}

		//生成新primitive

		resultPrim = prim;
		resultPrim.vertex[0] = vertices[afterClip][0];
		resultPrim.vertex[1] = vertices[afterClip][1];

	

	}

	void DefaultPipeline::clippingTriangle(const Primitive& prim,Primitive prims[5])
	{
		assert(prim.type == Primitive::PT_TRIANGLE);
		//2组顶点，一组裁剪前一组裁剪后，交替使用
		//三角形最多被3个面裁剪，生成6个顶点,特殊情况是，再多包含一个两个面的交点 也就是7个点
		Vertex vertices[2][7];
		//2组顶点的数量
		int numVertices[2];
		//使用的顶点下标
		int beforeClip = 0;
		int afterClip = 1;

		float d1,d2;

		if (checkPointInScreen(prim.vertex[0].pos) && 
			checkPointInScreen(prim.vertex[1].pos) && 
			checkPointInScreen(prim.vertex[2].pos))
		{
			for (int i =0; i < 3; ++i)
				vertices[afterClip][i] = prim.vertex[i];
			numVertices[afterClip] = 3;
			//用了goto，真是抱歉，不想看到长长的if {} else {}
			goto result;
		}

		for (int i =0; i < 3; ++i)
		{
			vertices[beforeClip][i] = prim.vertex[i];
		}
		numVertices[beforeClip] = 3;

		int numPlane = 6;
		beforeClip = 1;
		afterClip = 0;
		for(int plane = 0; plane < numPlane; ++plane)
		{

			//交換組
			(++beforeClip) &= 1;
			(++afterClip) &= 1;

			//全部被裁剪
			if (numVertices[beforeClip] == 0)
				return;

			numVertices[afterClip]= 0;

			//先把第一个顶点的dist算出来，后面每向后移一位就把之前的d2给d1以减少计算
			d1 = vertices[beforeClip][0].pos.dotProduct(mPlane[plane]);
			
			for (int i = 0,j= 1;i < numVertices[beforeClip]; ++i,j = (i+1) % numVertices[beforeClip])
			{
				d2 = vertices[beforeClip][j].pos.dotProduct(mPlane[plane]);

				if (d1 >= 0.0f)
				{
					vertices[afterClip][numVertices[afterClip]] = vertices[beforeClip][i];
					++numVertices[afterClip];

					if (d2 < 0.0f)
					{
						//lerp
						generateNewVertex( vertices[afterClip][numVertices[afterClip]], 
							vertices[beforeClip][i], vertices[beforeClip][j], d1,d2);
						++numVertices[afterClip];
					}
					else
					{
						//没有被裁剪，下轮推入verteices[afterClip]
					}
				}
				else
				{
					if (d2 >= 0.0f)
					{
						//lerp
						generateNewVertex( vertices[afterClip][numVertices[afterClip]], 
							vertices[beforeClip][i], vertices[beforeClip][j], d1,d2);
						++numVertices[afterClip];
					}
					else
					{
						//都被裁剪
					}
				}

				d1 = d2;
			
			}//verteices				
		}//plane

//如果所有点都在屏幕内直接跳转到这里
result:
		assert(numVertices[afterClip] < 8);
		

		if (numVertices[afterClip] < 3)
			return;

		//齐次坐标归一（透视除法） & 视口映射
		for ( int i =0; i < numVertices[afterClip]; ++i)
		{
			homogeneousDivide(vertices[afterClip][i]);
			viewportMapping(vertices[afterClip][i].pos,prim.vp);
		}

		//生成新primitive
		int num = numVertices[afterClip] - 1;
		for (int i = 1,j = 0; i < num; ++i,++j )
		{
			prims[j] = prim;
			prims[j].vertex[0] = vertices[afterClip][0];
			prims[j].vertex[1] = vertices[afterClip][i];
			prims[j].vertex[2] = vertices[afterClip][ (i + 1)% numVertices[afterClip] ];

		}

	}

	void DefaultPipeline::setVertexShader(VertexShader* vs)
	{
		mVertexShader = vs;
	}
	void DefaultPipeline::setPixelShader(PixelShader* ps)
	{
		mRasterizer.setPixelShader(ps);
	}

	void DefaultPipeline::setOtherState(const std::map<std::string,Any>& p)
	{
		VertexShader* vs = NULL;
		PixelShader* ps = NULL;
		std::map<std::string,Any>::const_iterator i,endi = p.end();
		i = p.find("VertexShader");
		if (i != endi)
		{
			vs = any_cast<VertexShader*>(i->second);
			setVertexShader(vs);
		}
		i = p.find("PixelShader");
		if (i != endi)
		{
			ps = any_cast<PixelShader*>(i->second);
			setPixelShader(ps);
		}

		std::string mhead ="Matrix";
		std::string vhead ="Vector";
		std::string param;
		for (unsigned int index = 0; index < 16; ++index)
		{
			std::ostringstream convert;
			convert << (int)index;
			//matrix
			param = mhead + convert.str();
			i = p.find(param);
			if (i != endi)
			{
				if (vs != NULL)
					vs->matrix4X4[index] = any_cast< Matrix4X4>(i->second);
				if(ps != NULL)
					ps->matrix4X4[index] = any_cast<Matrix4X4>(i->second);
			}
			//vector
			param = vhead + convert.str();
			i = p.find(param);
			if (i != endi)
			{
				if (vs != NULL)
					vs->vector4[index] = any_cast<Vector4>(i->second);
				if(ps != NULL)
					ps->vector4[index] = any_cast<Vector4>(i->second);
			}
				

		}
	}

}
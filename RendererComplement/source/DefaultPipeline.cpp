#include "DefaultPipeline.h"
#include "RendererHeader.h"
#include "Timer.h"
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
		FILE* fp;
		fp = fopen("timer.txt","a");
		Timer t;
		t.reset();
		
		const RenderData::RenderElementList& elems = renderData.getRenderElementList();
		size_t elemSize = elems.size();
		if (mVertexList.size() < elemSize)
			mVertexList.resize(elemSize);

		//show time.
		RenderData::RenderElementList::const_iterator i,endi = elems.end();
		VertexList::iterator verIter = mVertexList.begin();
		for (i = elems.begin(); i != endi; ++i,++verIter)
		{
			//�����Զ�����Ⱦ״̬
			setOtherState(i->propertys);
			if (i->vertexBuffer->getVertexCount() > verIter->size())
				verIter->resize(i->vertexBuffer->getVertexCount());//�@�e��ֱ�ӳ�ʼ���ˣ�����Ͳ���push_back��
			vertexProcessing(*i,*verIter);
			primitiveAssembly(*i,*verIter);
			mRasterizer.flush(i->frameBuffer,i->renderState);
		}
		notifyCompleted();

	}

	

	void DefaultPipeline::vertexProcessing(const RenderElement& elem, VertexVector& verVec)
	{
		const VertexBuffer* vb = elem.vertexBuffer;
		unsigned int vertexSize = vb->getVertexDeclaration().getSizeInBytes();
		//�@�eȡ����elem��vertexCount��ע�⣬��VertexBuffer�Y��һ��
		size_t dataSize = vb->getVertexCount() * vertexSize;
		//����������һ����c�е�ƫ�ƴ�С
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
		//������Position��Ϣ��ɵ��
		assert(posDataOffset != -1);
		//�@ȡ4�����ص�ƫ��
		unsigned int weightDataOffet[4] ;
		for (int i = 0; i < 4; ++i )
			weightDataOffet[i]= verDecl.getElementOffsetInBytes(VES_BLEND_WEIGHTS,i);

		//ԭʼ����
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

			//��λ��cposition
			mDataCollector.getData(posVec,vertexData + posDataOffset);
			verVec[i].pos = posVec;
			//��
			verVec[i].pos.w= 1.0f;
	


			//-----------------------------------------------------
			//�@�eע�⣡������
			//���rĬ�J��4��weight�Ļ��
			//-----------------------------------------------------

			//�@ȡ4������
			for (int j = 0; j < 4; ++j)
			{
				if (weightDataOffet[j] == -1)//�}ԓ���ز�����
				{
					//Ӌ�����
					weight[j] = 1;
					for (int k = 0; k < j ; ++k )
						weight[j] = weight[j] - weight[k];
					break;
				}			
				mDataCollector.getData(weight[j],vertexData + weightDataOffet[j]);
			}

			//��ȡ��ɫ(����ye����) 
			diffuse = elem.material.diffuse;
			specular = elem.material.specular;
			power = elem.material.power;
			ambient = elem.material.ambient;
			if (diffuseDataOffset != -1 && elem.material.isDiffuseVertexColorEnable())
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
			if (specularDataOffset != -1 && elem.material.isSpecularVertexColorEnable())
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


			//����
			if (normalDataOffset != -1)
			{
				mDataCollector.getData(verVec[i].norm,vertexData + normalDataOffset);			
			}

			//��������
			for (unsigned char k = 0; k < 8; ++k )
			{
				if (texCroodDataOffset[k] != -1 && elem.sampler[k].texture != NULL)
				{
					mDataCollector.getData(verVec[i].texCrood[k],vertexData + texCroodDataOffset[k]);
					elem.sampler[k].assignUV(verVec[i].texCrood[k].x,verVec[i].texCrood[k].y);					
				}
			}

			//����Ϊ���ݲɼ�
			if (mVertexShader != NULL)
			{
				mVertexShader->execute(verVec[i]);
				continue;
			}

			//����Ϊ���ݼ���

			//Ӌ����
			transMat = elem.matWorld[TS_WORLD] * weight[0] + 
				elem.matWorld[TS_WORLD2] * weight[1] + 
				elem.matWorld[TS_WORLD3] * weight[2] + 
				elem.matWorld[TS_WORLD4] * weight[3];
			//����任
			verVec[i].pos = elem.matWorld[TS_VIEW] *( transMat * verVec[i].pos );//* elem.matWorld[TS_PROJECTION]  ;
			
			//��¼�±任ǰ������ ���й��ռ���
			posVec.x = verVec[i].pos.x;
			posVec.y = verVec[i].pos.y;
			posVec.z = verVec[i].pos.z;

			//�����任
			verVec[i].pos = elem.matWorld[TS_PROJECTION] *  verVec[i].pos;	
			


			//���ռ���
			diffuseBlend.getFromABGR(0);
			specularBlend.getFromABGR(0);
			ambientBlend.getFromABGR(0);
			enable = false;
			for (unsigned int index = 0; index < 8; ++index)
			{
				if (!elem.light[index].isEnable())
					continue;
				enable = true;
				
				//ע������Ƶ�����ͷ������������˱任��
				//��Ϊ��ʱҪ�ĵ�����ָ���������������Ҫ��������꣬������û���ṩ�����
				//���ǽ��������͵Ʊ任�����������ϵ��
				//��Ȼ�������Ļ� ����һ����������ϵ�ı任��
				lightPos = elem.matWorld[TS_VIEW] *(transMat * elem.light[index].position);
				L = lightPos - posVec;
				L.normalise();
				V = - posVec;
				V.normalise();
				H = L + V;
				H.normalise();
				
				//ע�������и����ز�����normVec4.w��Ϊ��0������1
				normVec4 = verVec[i].norm;
				//normal�Ļ�ֻ��Ҫ��ת����
				normVec4 = elem.matWorld[TS_VIEW] *  normVec4;
				normVec3.x = normVec4.x;
				normVec3.y = normVec4.y;
				normVec3.z = normVec4.z;

				diffuseBlend += elem.light[index].diffuse * 
					std::max<float>(0,normVec3.dotProduct(L));

				specularBlend += elem.light[index].specular * 
					pow(std::max<float>(0,H.dotProduct(normVec3)),power);
				ambientBlend += elem.light[index].ambient;
			}

			//���ʹ���˵ƹ⡣
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

	void DefaultPipeline::primitiveAssembly(const RenderElement& elem,const VertexVector& verVec)
	{
		const IndexBuffer* ib = elem.indexBuffer;
		const VertexBuffer* vb = elem.vertexBuffer;
		unsigned int skipVertexCount = 0;
		size_t realVertexCount = 0;
		unsigned int type = 0;
		//�õ�ʵ����Ҫ���ƵĶ�����
		if (ib != NULL)
			realVertexCount = ib->getIndexCount();
		else
			realVertexCount = vb->getVertexCount();

		//ͼԪ��Ҫ�Ķ�����
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

		//��֤ʣ�ඥ�����㹻�γ�ͼԪ
		assert(realVertexCount - skipVertexCount >= vertexNumInGeom*elem.primitiveCount );

		Primitive prim ;
		Primitive priResult[5];
		for (unsigned int i = 0; i < elem.primitiveCount; ++i)
		{
			prim.type = type;
			prim.vp = &elem.viewport;
			
			for (short j = 0; j < 8; ++j)
			{
				prim.sampler[j] = elem.sampler[j];
			}
			
			
			//���붥��
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
						
			//����true��˵��ͨ����false���޳�
			 if (prim.type == Primitive::PT_ERROR  || !culling(prim) )
				 continue;
			 //ͬʱ����ι�һ���ӿ�ӳ������ˣ���Ϊ�ڶ��㼶����������������
			 cliping(prim,priResult);

			 //������clip�½������
			 for ( int k = 0; k < 5; ++k)
			 {
				 rasterize(priResult[k]);
				 priResult[k].type = Primitive::PT_ERROR;
			 }

		}
	}

	bool DefaultPipeline::culling(const Primitive& prim)
	{
		if (prim.type != Primitive::PT_TRIANGLE)
			return true;
		

		const Vector4& pos0 = prim.vertex[0].pos;
		const Vector4& pos1 = prim.vertex[1].pos;
		const Vector4& pos2 = prim.vertex[2].pos; 
		//��ʱĬ����ʱ���޳�
		float result = (float)(pos0.w * (pos1.x * pos2.y - pos2.x * pos1.y) + 
			pos1.w * (pos2.x * pos0.y - pos0.x * pos2.y) + 
			pos2.w * (pos0.x * pos1.y - pos1.x * pos0.y)) ;
		if (result > 0)
			return true;
		else
			return false;
		
	}

	void DefaultPipeline::cliping(const Primitive& prim,Primitive prims[5])
	{
		switch (prim.type)
		{
		case Primitive::PT_POINT:
			{
				const Vector4& pos = prim.vertex[0].pos;
				//���ü�
				if ( !checkPointInScreen(pos) )
					 return;
				prims[0] = prim;
				break;
			}
		case Primitive::PT_LINE:
			{
				clipingLine(prim,prims[0]);
				break;
			}
		case Primitive::PT_TRIANGLE:
			{
				clipingTriangle(prim,prims);
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
			vert.texCrood[i] *= invw;
			vert.color[i] *= invw;
		}
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

	void DefaultPipeline::generateNewVertex(Vertex& newVertex,const Vertex& vert1, const Vertex& vert2, float dist1, float dist2)
	{
		float scale = (0 - dist1) / (dist2 - dist1);
		Interpolate(newVertex.specular,vert1.specular,vert2.specular,scale);
		Interpolate(newVertex.norm,vert1.norm,vert2.norm,scale);
		Interpolate(newVertex.pos,vert1.pos,vert2.pos,scale);
		for (unsigned int i = 0 ; i < 8; ++i)
		{
			Interpolate(newVertex.texCrood[i],vert1.texCrood[i],vert2.texCrood[i],scale);
			Interpolate(newVertex.color[i],vert1.color[i],vert2.color[i],scale);

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

	void DefaultPipeline::clipingLine(const Primitive& prim,Primitive& resultPrim)
	{
		assert(prim.type == Primitive::PT_LINE);

		Vertex vertices[2][2];
		//ʹ�õĶ����±�
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
					//�����ü�
				}
			}
				
			//������
			(++beforeClip) &= 1; 
			(++afterClip) &= 1; 

		
		}//plane


		//��������һ��͸�ӳ����� & �ӿ�ӳ��
		for ( int i =0; i < 2; ++i)
		{
			homogeneousDivide(vertices[afterClip][i]);
			viewportMapping(vertices[afterClip][i].pos,prim.vp);
		}

		//������primitive

		resultPrim = prim;
		resultPrim.vertex[0] = vertices[afterClip][0];
		resultPrim.vertex[1] = vertices[afterClip][1];

	

	}

	void DefaultPipeline::clipingTriangle(const Primitive& prim,Primitive prims[5])
	{
		assert(prim.type == Primitive::PT_TRIANGLE);
		//2�鶥�㣬һ��ü�ǰһ��ü��󣬽���ʹ��
		//��������౻3����ü�������6������,��������ǣ��ٶ����һ��������Ľ��� Ҳ����7����
		Vertex vertices[2][7];
		//2�鶥�������
		int numVertices[2];
		//ʹ�õĶ����±�
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
			//����goto�����Ǳ�Ǹ�����뿴��������if {} else {}
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

			//���Q�M
			(++beforeClip) &= 1;
			(++afterClip) &= 1;

			//ȫ�����ü�
			if (numVertices[beforeClip] == 0)
				return;

			numVertices[afterClip]= 0;

			//�Ȱѵ�һ�������dist�����������ÿ�����һλ�Ͱ�֮ǰ��d2��d1�Լ��ټ���
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
						//û�б��ü�����������verteices[afterClip]
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
						//�����ü�
					}
				}

				d1 = d2;
			
			}//verteices				
		}//plane

//������е㶼����Ļ��ֱ����ת������
result:
		assert(numVertices[afterClip] < 8);
		

		if (numVertices[afterClip] < 3)
			return;

		//��������һ��͸�ӳ����� & �ӿ�ӳ��
		for ( int i =0; i < numVertices[afterClip]; ++i)
		{
			homogeneousDivide(vertices[afterClip][i]);
			viewportMapping(vertices[afterClip][i].pos,prim.vp);
		}

		//������primitive
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
		std::map<std::string,Any>::const_iterator i,endi = p.end();
		i = p.find("VertexShader");
		if (i != endi)
		{
			setVertexShader(any_cast<VertexShader*>(i->second));
		}
		i = p.find("PixelShader");
		if (i != endi)
		{
			setPixelShader(any_cast<PixelShader*>(i->second));
		}
	}

}
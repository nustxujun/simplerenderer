#include "TangentUtil.h"
#include "RendererHeader.h"
#include "DataCollector.h"
namespace RCP
{
	VertexBuffer* TangentUtil::calculateTangentSpace(Renderer* renderer, const VertexBuffer* vb,const IndexBuffer* ib)
	{
		VertexDeclaration newVD = vb->getVertexDeclaration();
		bool hasNormal = newVD.hasElement(VES_NORMAL);
		bool hasTangent = newVD.hasElement(VES_TANGENT);
		bool hasBinormal = newVD.hasElement(VES_BINORMAL);

		if (!hasNormal)
			newVD.addElement(VET_FLOAT3,VES_NORMAL);
		if (!hasTangent)
			newVD.addElement(VET_FLOAT3,VES_TANGENT);
		if (!hasBinormal)
			newVD.addElement(VET_FLOAT3,VES_BINORMAL);

		VertexBuffer* newVB = renderer->createVertexBuffer(vb->getVertexCount(),newVD);

		int oldVertexSize = vb->getVertexDeclaration().getSizeInBytes();
		int newVertexSize = newVD.getSizeInBytes();
		//实际绘制顶点数（要根据三角形来算）
		int vertexCount = vb->getVertexCount();
		if (ib != NULL)
			vertexCount = ( int)ib->getIndexCount();
		DataCollector collector;
		unsigned int posDataOffset = vb->getVertexDeclaration().getElementOffsetInBytes(VES_POSITION);
		unsigned int uvDataOffset = vb->getVertexDeclaration().getElementOffsetInBytes(VES_TEXTURE_COORDINATES);

		std::vector<Result> vertexList;
		vertexList.resize(vb->getVertexCount());

		Vector3 p1,p2,p3;
		Vector2 tc1,tc2,tc3;
		Result result;
		for (int i = 0; i < vertexCount; i+= 3 )
		{
			unsigned int index[3] = {i,i+1,i+2};
			if (ib != NULL)
			{
				index[0] = (*ib)[index[0]];
				index[1] = (*ib)[index[1]];
				index[2] = (*ib)[index[2]];
			}

			const unsigned char* ov1 = vb->getData() + index[0] * oldVertexSize;
			const unsigned char* ov2 = vb->getData() + index[1] * oldVertexSize;
			const unsigned char* ov3 = vb->getData() + index[2] * oldVertexSize;

			collector.getData(p1,ov1 + posDataOffset);
			collector.getData(p2,ov2 + posDataOffset);
			collector.getData(p3,ov3 + posDataOffset);
			if (uvDataOffset != -1)		
			{
				//有uv的情况
				collector.getData(tc1,ov1 + uvDataOffset);
				collector.getData(tc2,ov2 + uvDataOffset);
				collector.getData(tc3,ov3 + uvDataOffset);

				result = calculateTangent(p1,p2,p3,tc1,tc2,tc3);
			}
			else
			{

				result = calculateTangent(p1,p2,p3);
			}

			vertexList[index[0]].merge(result);
			vertexList[index[1]].merge(result);
			vertexList[index[2]].merge(result);
		}

		unsigned char* vertex = new unsigned char[newVertexSize];
		unsigned char* r = new unsigned char[4 * 3 * 3];
		for (unsigned int i = 0; i < vb->getVertexCount(); ++i)
		{
			//copy旧数据
			memcpy(vertex,vb->getData() + i * oldVertexSize,oldVertexSize);
			vertexList[i].calculate();

			((float*)r)[0] =  vertexList[i].normal.x;
			((float*)r)[1] =  vertexList[i].normal.y;
			((float*)r)[2] =  vertexList[i].normal.z;

			((float*)r)[3] =  vertexList[i].tangent.x;
			((float*)r)[4] =  vertexList[i].tangent.y;
			((float*)r)[5] =  vertexList[i].tangent.z;

			((float*)r)[6] =  vertexList[i].binormal.x;
			((float*)r)[7] =  vertexList[i].binormal.y;
			((float*)r)[8] =  vertexList[i].binormal.z;

			int counter = 0;
			if (!hasNormal)
			{
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 0,4);
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 1,4);
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 2,4);
			}
			if (!hasTangent)
			{
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 3,4);
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 4,4);
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 5,4);
			}
			if (!hasBinormal)
			{
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 6,4);
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 7,4);
				memcpy(vertex + oldVertexSize + counter++ * 4,r + 4 * 8,4);
			}
			newVB->fill(i,vertex);
		}
		delete vertex;
		delete r;
		return newVB;
	}


	TangentUtil::Result TangentUtil::calculateTangent(const Vector3& p1,const Vector3& p2, const Vector3& p3, const Vector2& tc1, const Vector2& tc2, const Vector2& tc3)
	{
		Result result;
		//side0 is the vector along one side of the triangle of vertices passed in, 
		//and side1 is the vector along another side. Taking the cross product of these returns the normal.
		Vector3 side0 = p1 - p2;
		Vector3 side1 = p3 - p1;
		//Calculate face normal
		Vector3 normal = side1.crossProduct(side0);
		normal.normalise();
		//Now we use a formula to calculate the tangent. 
		float deltaV0 = tc1.x - tc2.x;
		float deltaV1 = tc3.x - tc1.x;
		Vector3 tangent =  side0 * deltaV1 - side1 * deltaV0 ;
		tangent.normalise();
		//Calculate binormal
		float deltaU0 = tc1.y - tc2.y;
		float deltaU1 = tc3.y - tc1.y;
		Vector3 binormal = side0 * deltaU1 - side1 * deltaU0;
		binormal.normalise();
		//Now, we take the cross product of the tangents to get a vector which 
		//should point in the same direction as our normal calculated above. 
		//If it points in the opposite direction (the dot product between the normals is less than zero), 
		//then we need to reverse the s and t tangents. 
		//This is because the triangle has been mirrored when going from tangent space to object space.
		//reverse tangents if necessary
		Vector3 tangentCross = tangent.crossProduct(binormal);
		if (tangentCross.dotProduct(normal) < 0.0f)
		{
			tangent = -tangent;
			binormal = -binormal;
		}

		result.used = 1;
		result.normal = normal;
		result.tangent = tangent;
		result.binormal = binormal;
		return result;
	}

	TangentUtil::Result TangentUtil::calculateTangent(const Vector3& p1,const Vector3& p2, const Vector3& p3)
	{
		return calculateTangent(p1,p2,p3,generateUV(p1),generateUV(p2),generateUV(p3));
	}

	Vector2 TangentUtil::generateUV(const Vector3& pos)
	{
		int face = 0;
		float s, t, m;

		float x = pos.x;
		float y = pos.y;
		float z = pos.z;

		float ax = abs(x);
		float ay = abs(y);
		float az = abs(z);

		if(ax > ay && ax > az)
		{
			// x max
			m = ax;
			if(x > 0){
				//+x
				s = 0.5f * (z / m + 1.0f);
				t = 0.5f * (-y / m + 1.0f);
				face = 0;
			} else {
				//-x

				s = 0.5f * (-z / m + 1.0f);
				t = 0.5f * (-y / m + 1.0f);
				face = 1;
			}
		} else {

			if(ay > ax && ay > az){
				m = ay;
				if(y > 0){
					//+y
					s =0.5f * (-x / m + 1.0f);
					t = 0.5f * (z / m + 1.0f);
					face = 2;
				} else {
					s = 0.5f * (-x / m + 1.0f);
					t = 0.5f * (-z / m + 1.0f);
					face = 3;
				}
			} else {
				m = az;
				if(z > 0){
					//+z
					s = 0.5f * (-x / m + 1.0f);
					t = 0.5f * (-y / m + 1.0f);
					face = 4;
				} else {
					s = 0.5f * (x / m + 1.0f);
					t = 0.5f * (-y / m + 1.0f);
					face = 5;
				}
			}
		}	
		return Vector2(s,t);
	}
}
#ifndef _TangentUtil_H_
#define _TangentUtil_H_

#include"Prerequisites.h"

namespace RCP
{
	class TangentUtil
	{
	public:
		static VertexBuffer* calculateTangentSpace(Renderer* renderer, const VertexBuffer* vb,const IndexBuffer* ib = NULL);
	private:
		struct Result
		{
			float used;
			Vector3 tangent;
			Vector3 binormal;
			Vector3 normal;
			Result():
				used(0)
			{}

			void merge(const Result& r)
			{
				if (used == 0)
				{
					normal = r.normal;
					tangent = r.tangent;
					binormal = r.binormal;
					
				}
				else
				{
					normal = normal+ r.normal;
					tangent = tangent + r.tangent;
					binormal = binormal + r.binormal;
				}
				++used; 
			}

			void calculate()
			{
				if (used != 0)
				{
					normal /= used;
					normal.normalise();
					tangent /= used;
					tangent.normalise();
					binormal /= used;
					binormal.normalise();
				}
			}
		};
		static Result calculateTangent(const Vector3& p1,const Vector3& p2, const Vector3& p3, const Vector2& tc1, const Vector2& tc2, const Vector2& tc3);
		static Result calculateTangent(const Vector3& p1,const Vector3& p2, const Vector3& p3);
		static Vector2 generateUV(const Vector3& pos);

	};
}
#endif//_TangentUtil_H_

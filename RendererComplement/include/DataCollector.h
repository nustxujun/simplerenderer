#ifndef _DataCollector_H_
#define _DataCollector_H_

#include"Prerequisites.h"
namespace RCP
{
	class DataCollector
	{
	public:
		template<class T>
		void getData(T& t,const void* data)
		{
			getDataImpl(t,data);
		}
	private:
		void getDataImpl(Vector4& vec4, const void* data);
		void getDataImpl(Vector3& vec3, const void* data);
		void getDataImpl(Vector2& vec2, const void* data);
		void getDataImpl(float& f, const void* data);
		void getDataImpl(int& d, const void* data);
	};
}
#endif//_DataCollector_H_

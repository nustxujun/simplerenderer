#include "DataCollector.h"

namespace RCP
{
	void DataCollector::getDataImpl(Vector4& vec4, const void* data)
	{
		const float* d = (const float* )data;

		vec4.x = d[0];
		vec4.y = d[1];
		vec4.z = d[2];
		vec4.w = d[3];

	}

	void DataCollector::getDataImpl(Vector3& vec3, const void* data)
	{
		const float* d = (const float* )data;
		vec3.x = d[0];
		vec3.y = d[1];
		vec3.z = d[2];
	}

	void DataCollector::getDataImpl(Vector2& vec2, const void* data)
	{
		const float* d = (const float* )data;
		vec2.x = d[0];
		vec2.y = d[1];
	}


	void DataCollector::getDataImpl(float& f, const void* data)
	{
		memcpy(&f,data,sizeof(f));
	}

	void DataCollector::getDataImpl(int& d, const void* data)
	{
		memcpy(&d,data,sizeof(d));
	}
}
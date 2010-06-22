#ifndef _MatrixUtil_H_
#define _MatrixUtil_H_

#include "Matrix4X4.h"

namespace RCP
{
	class MatrixUtil
	{
	public :
		static void getViewSpace(Matrix4X4& output,const Vector3&  eye, const Vector3& at, const Vector3& up);
		static void getPerspectiveProjectionSpace(Matrix4X4& output, float fov, float aspect, float near, float far);
	};
}
#endif//_MatrixUtil_H_

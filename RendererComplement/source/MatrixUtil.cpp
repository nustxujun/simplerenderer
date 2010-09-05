#include "MatrixUtil.h"

namespace RCP
{
	void MatrixUtil::getViewSpace(Matrix4X4& output,const Vector3&  eye, const Vector3& at, const Vector3& up)
	{
		/*
		zaxis = normal(Eye - At)
		xaxis = normal(cross(Up, zaxis))
		yaxis = cross(zaxis, xaxis)

		*/

		Vector3 xaxis,yaxis,zaxis;
		//当view matrix为单位矩阵的时候摄像机朝着z的负轴
		//所以这里eye - at才是摄像机方向
		zaxis = (eye - at);
		zaxis.normalise();
		xaxis = up.crossProduct(zaxis);
		xaxis.normalise();
		yaxis = zaxis.crossProduct(xaxis);

		//直接把轴按转置摆放，直接求逆
		output.m[0][0] = xaxis.x;
		output.m[0][1] = xaxis.y;
		output.m[0][2] = xaxis.z;
		output.m[1][0] = yaxis.x;
		output.m[1][1] = yaxis.y;
		output.m[1][2] = yaxis.z;
		output.m[2][0] = zaxis.x;
		output.m[2][1] = zaxis.y;
		output.m[2][2] = zaxis.z;
		output.m[3][0] = 0;
		output.m[3][1] = 0;
		output.m[3][2] = 0;
		output.m[3][3] = 1;

		Vector3 trans = output *( -eye);
		output.m[0][3] = trans.x;
		output.m[1][3] = trans.y;
		output.m[2][3] = trans.z;
	}

	void MatrixUtil::getPerspectiveProjectionSpace(Matrix4X4& output, float fov, float aspect, float near, float far)
	{
		assert(fov != 0.0);
		float xScale ,yScale;
		yScale = 1.0f / tan(fov / 2);
		xScale = yScale / aspect;

		/*
		[ A   0   C   0  ]
		[ 0   B   D   0  ]
		[ 0   0   q   qn ]
		[ 0   0   -1  0  ]
		*/
		output = Matrix4X4::IDENTITY;
		output.m[0][0] = xScale;
		output.m[1][1] = yScale;
		output.m[2][2] = (far + near) / (near - far);
		output.m[3][3] = 0;
		output.m[2][3] = 2 * far * near / (near - far);
		output.m[3][2] = -1;

	}

	void MatrixUtil::getCubeMapViewMatrix(Matrix4X4& output, unsigned int face)
	{
		Vector3 vEyePt   = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3 vLookDir,vUpDir;

		switch( face )
		{
		case 0:
			vLookDir = Vector3( 1.0f, 0.0f, 0.0f );
			vUpDir   = Vector3( 0.0f, 1.0f, 0.0f );
			break;
		case 1:
			vLookDir = Vector3(-1.0f, 0.0f, 0.0f );
			vUpDir   = Vector3( 0.0f, 1.0f, 0.0f );
			break;
		case 2:
			vLookDir = Vector3( 0.0f, 1.0f, 0.0f );
			vUpDir   = Vector3( 0.0f, 0.0f,-1.0f );
			break;
		case 3:
			vLookDir = Vector3( 0.0f,-1.0f, 0.0f );
			vUpDir   = Vector3( 0.0f, 0.0f, 1.0f );
			break;
		case 4:
			vLookDir = Vector3( 0.0f, 0.0f, 1.0f );
			vUpDir   = Vector3( 0.0f, 1.0f, 0.0f );
			break;
		case 5:
			vLookDir = Vector3( 0.0f, 0.0f,-1.0f );
			vUpDir   = Vector3( 0.0f, 1.0f, 0.0f );
			break;
		}
		MatrixUtil::getViewSpace(output,vEyePt,vLookDir,vUpDir);
	}
}
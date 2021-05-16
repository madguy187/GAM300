#ifndef MATH_LIB_H
#define MATH_LIB_H
#include <cmath>
#include "Vec.h"
#include "Matrix4.h"
#include "Quaternion.h"
#define PI                 3.14159265358979323846f
#define DEG_TO_RAD(_a) ((_a)*PI/180.0f)
#define RAD_TO_DEG(_a) ((_a)*180.0f/PI)
#define RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define FLOAT_SMALL            1.0e-37f
#define TOLERANCE              2e-37f
#define E                      2.71828182845904523536f
#define LOG10E                 0.4342944819032518f
#define LOG2E                  1.442695040888963387f
#define PIOVER2                1.57079632679489661923f
#define PIOVER4                0.785398163397448309616f
#define PIOVER360              0.008726646259971647884f
#define PIOVER180              0.0174532924f
#define _180OVERPI              57.29578f
#define PIX2                   6.28318530717958647693f
#define EPSILON                0.000001f
#define MATRIX_SIZE ( sizeof(float) * 16)
#define CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
////////////////////////////////////////////////////////////////////////
///
///                 MATRIX3D DEFINITIONS AND FUNCTIONS -START
///
//////////////////////////////////////////////////////////////////////

class Matrix3D
{

private:

	int row = 0;
	int column = 0;

public:

	struct 
	{
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	}mtx;

	float m[3][3];

	// Default Constructor for m[3][3] = 0;
	Matrix3D()
	{
		m[0][0] = mtx.m00 = 0.0f;
		m[0][1] = mtx.m01 = 0.0f;
		m[0][2] = mtx.m02 = 0.0f;
		m[1][0] = mtx.m10 = 0.0f;
		m[1][1] = mtx.m11 = 0.0f;
		m[1][2] = mtx.m12 = 0.0f;
		m[2][0] = mtx.m20 = 0.0f;
		m[2][1] = mtx.m21 = 0.0f;
		m[2][2] = mtx.m22 = 0.0f;
	}

	// Constructors
	Matrix3D(Matrix3D& Matrix, const float* Array);
	Matrix3D(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21, float _22);

	// Operator Functions
	Matrix3D operator * (const Matrix3D& rhs);
	Matrix3D& operator = (const Matrix3D& rhs);
	Matrix3D& operator *= (const Matrix3D& rhs);
	Matrix3D& DivideRow(Matrix3D& rhs, float divisor);

	// Matrices Functions
	static void MatrixSetIdentity(Matrix3D& Result);
	static void MatrixTranslate(Matrix3D& Result, float x, float y);
	static void MatrixScale(Matrix3D& Result, float x, float y);
	static void MatrixTranspose(Matrix3D& Result, Matrix3D& Matrix);
	static void MatrixInverse(Matrix3D& Result, Matrix3D& Matrix);
	static void MatrixRotDeg(Matrix3D& Result, float angle);
	static void MatrixRotRad(Matrix3D& Result, float angle);
	static Matrix3D MatrixConcat(Matrix3D* Result, Matrix3D* PMatrix_Receieve, Matrix3D* pMatrix_IN);
	static void setTransform(Matrix3D& Transform,Matrix3D& MTrans, Matrix3D& MRot, Matrix3D& MScale);

	// Others
	static void PrintMatrix(const Matrix3D Matrix);
	static const float GetEntry(Matrix3D& Matrix, int rowIndex, int colIndex);

};

////////////////////////////////////////////////////////////////////////
///
///                 MATRIX3D DEFINITIONS AND FUNCTIONS -END
///
//////////////////////////////////////////////////////////////////////




#endif //MATH_LIB_H


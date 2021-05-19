/**********************************************************************************
* \file            Matrix4.h
*
* \brief        Definition of the Matrix4 class function(s).
*
* \author        Tian Yu
*
* \email		t.yu\@digipen.edu
*
* \date			1 aug 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef MATRIX4_H
#define MATRIX4_H
#include "Math_Lib.h"

///
///                     MUST READ I AM DOING COLUMN MAJOR ARRAY EG.
///                              1   2   3   4
///                              5   6   7   8
///                              9   10  11  12
///                              13  14  15  16
///
///                     THIS WILL BE IN 1D ARRAY FORM : 1 5 9 13 2 6 10 14 3 7 11 15 4 8 12 16

////////////////////////////////////////////////////////////////////////
///
///                 MATRIX4 DEFINITIONS AND FUNCTIONS -START
///
//////////////////////////////////////////////////////////////////////
class Quaternion;
class matrix4
{

public:

	/**************************************************************************/
	/*!
		Define the 1D array for the matrix
	*/
	/**************************************************************************/
	float m[16] = { 0 };
	
	static const float identityMatrix[16];
	
	/**************************************************************************/
	/*!
		Constructor for matrix setting it to an identity matrix
	*/
	/**************************************************************************/
	matrix4();

	/**************************************************************************/
	/*!
		Constructor for matrix setting each of the individual value for the matrix
	*/
	/**************************************************************************/
	matrix4(float m00, float m01, float m02, float m03,      //first row
		float m10, float m11, float m12, float m13,      //second row
		float m20, float m21, float m22, float m23,      //third row
		float m30, float m31, float m32, float m33);     //forth row

/**************************************************************************/
/*!
	Create a matrix init to the specified array (column major)
*/
/**************************************************************************/
	matrix4(const float* inputmatrix);

	/**************************************************************************/
	/*!
		Create a matrix by copying from another matrix
	*/
	/**************************************************************************/
	matrix4(const matrix4& copy);

	/**************************************************************************/
	/*!
		Destructor
	*/
	/**************************************************************************/
	~matrix4();

	/**************************************************************************/
	/*!
		Return an identity matrix
	*/
	/**************************************************************************/
	static const matrix4& Identity();

	/**************************************************************************/
	/*!
	   Return matrix that is all zero
	*/
	/**************************************************************************/
	static const matrix4& AllZero();

	/**************************************************************************/
	/*!
		Set the values of the matrix
	*/
	/**************************************************************************/
	void set(float m00, float m01, float m02, float m03,      //first row
			 float m10, float m11, float m12, float m13,      //second row
			 float m20, float m21, float m22, float m23,      //third row
			 float m30, float m31, float m32, float m33);     //forth row

	/**************************************************************************/
	/*!
	   Set the values of the matrix base on a matrix[]
	*/
	/**************************************************************************/
	void set(const float* matrix);

	/**************************************************************************/
	/*!
		 Set the values of the matrix base on a matrix
	*/
	/**************************************************************************/
	void set(const matrix4& matrix);

	/**************************************************************************/
	/*!
		 Set the identity matrix
	*/
	/**************************************************************************/
	void setIdentity();

	/**************************************************************************/
	/*!
		 Set the matrix component values all to 0
	*/
	/**************************************************************************/
	void setAllZero();

	/**************************************************************************/
	/*!
	   Make a scale matrix
	*/
	/**************************************************************************/
	static void makeScaleM(const vec3& scale, matrix4* output);

	/**************************************************************************/
	/*!
	   Make a scale matrix(overloaded func to scale x y and z individually)
	*/
	/**************************************************************************/
	static void makeScaleM(float x, float y, float z, matrix4* output);

	/**************************************************************************/
	/*!
	   Create rotation matrix from input axis and angle(rad)
	*/
	/**************************************************************************/
	static void createRotation(const vec3& axis, float angle, matrix4* output);

	/**************************************************************************/
	/*!
	   Create rotation matrix from input quaternion and store in output
	*/
	/**************************************************************************/
	static void createRotation(const Quaternion& input, matrix4* output);
	
	/**************************************************************************/
	/*!
	   Create rotation matrix from angle(overloaded )
	*/
	/**************************************************************************/
	static void createRotationXaxis(float angle, matrix4* output);

	/**************************************************************************/
	/*!
	   Create rotation matrix from angle(overloaded )
	*/
	/**************************************************************************/
	static void createRotationYaxis(float angle, matrix4* output);

	/**************************************************************************/
	/*!
	   Create rotation matrix from angle(overloaded )
	*/
	/**************************************************************************/
	static void createRotationZaxis(float angle, matrix4* output);

	/**************************************************************************/
	/*!
		Create rotation matrix from quaternion
	*/
	/**************************************************************************/
	static void createRotationQuaternion(const Quaternion& quat, matrix4* output);

	/**************************************************************************/
	/*!
		Create matrix holds the rotations of yaw(radians), pitch(radians) and roll(radians)
	*/
	/**************************************************************************/
	static void createMatrixEuler(float yaw, float pitch, float roll, matrix4* output);

	/**************************************************************************/
	/*!
		Create translation matrix based on vector 3
	*/
	/**************************************************************************/
	static void createTranslation(const vec3& translationVector, matrix4* output);

	/**************************************************************************/
	/*!
		Create translation matrix based on the x, y and z
	*/
	/**************************************************************************/
	static void createTranslation(float x, float y, float z, matrix4* output);

	/**************************************************************************/
	/*!
		Add scalar to each component in the matrix
	*/
	/**************************************************************************/
	void add(float scalar);

	/**************************************************************************/
	/*!
		Add scalar to each component in the matrix and store in the output matrix
	*/
	/**************************************************************************/
	void add(float scalar, matrix4* output);

	/**************************************************************************/
	/*!
		Add matrix with matrix
	*/
	/**************************************************************************/
	void add(const matrix4& input);

	/**************************************************************************/
	/*!
		Add the 2 input matrix and store in output
	*/
	/**************************************************************************/
	void add(const matrix4& m1, const matrix4& m2, matrix4* output);

	/**************************************************************************/
	/*!
		 Subtract a matrix from the current matrix
	*/
	/**************************************************************************/
	void subtract(const matrix4& input);

	/**************************************************************************/
	/*!
		 Subtract matrix 1 - 2 and store it in output
	*/
	/**************************************************************************/
	static void subtract(const matrix4& m1, const matrix4& m2, matrix4* output);

	/**************************************************************************/
	/*!
	   Multiply matrix with scalar
	*/
	/**************************************************************************/
	void multiply(float scalar);

	/**************************************************************************/
	/*!
	   Multiply matrix with scalar and store it in output
	*/
	/**************************************************************************/
	void multiply(float scalar, matrix4* output)const;

	/**************************************************************************/
	/*!
	   Multiply a specific matrix with scalar and store it in output
	*/
	/**************************************************************************/
	static void multiply(const matrix4& inputmatrix, float scalar, matrix4* output);

	/**************************************************************************/
	/*!
	   Multiply matrix with matrix
	*/
	/**************************************************************************/
	void multiply(const matrix4& inputmatrix);

	/**************************************************************************/
	/*!
	   Multiply matrix1 and 2  and store it in output
	*/
	/**************************************************************************/
	static void multiply(const matrix4& inputmatrix1, const matrix4& inputmatrix2, matrix4* output);

	/**************************************************************************/
	/*!
		Decompose scale translation and rotation of the matrix
	*/
	/**************************************************************************/
	bool decomposeMatrix(vec3* scale, Quaternion* rotation, vec3* translation)const;

	/**************************************************************************/
	/*!
		Get the determinant of matrix
	*/
	/**************************************************************************/
	float determinant()const;

	/**************************************************************************/
	/*!
		Get the euler angle
	*/
	/**************************************************************************/
	vec3 getEulerAngle()const;

	/**************************************************************************/
	/*!
	   Get scalar of matrix
	*/
	/**************************************************************************/
	void getScale(vec3* scale)const;

	/**************************************************************************/
	/*!
	   Get rotation of matrix
	*/
	/**************************************************************************/
	bool getRotation(Quaternion* rotation)const;

	/**************************************************************************/
	/*!
	   Get translation of matrix
	*/
	/**************************************************************************/
	void getTranslation(vec3* translation)const;

	/**************************************************************************/
	/*!
	   Get up vector of matrix
	*/
	/**************************************************************************/
	void getUpVector(vec3* output)const;

	/**************************************************************************/
	/*!
	   Get down vector of matrix
	*/
	/**************************************************************************/
	void getDownVector(vec3* output)const;

	/**************************************************************************/
	/*!
	   Get left vector of matrix
	*/
	/**************************************************************************/
	void getLeftVector(vec3* output)const;

	/**************************************************************************/
	/*!
	   Get Right vector of matrix
	*/
	/**************************************************************************/
	void getRightVector(vec3* output)const;

	/**************************************************************************/
	/*!
	   Get forward vector of matrix
	*/
	/**************************************************************************/
	void getForwardVector(vec3* output)const;

	/**************************************************************************/
	/*!
	   Get backward vector of matrix
	*/
	/**************************************************************************/
	void getBackwardVector(vec3* output)const;

	/**************************************************************************/
	/*!
	   Inverse matrix
	*/
	/**************************************************************************/
	bool inverseMatrix();

	/**************************************************************************/
	/*!
	   Inverse matrix(overload) and stored into output
	*/
	/**************************************************************************/
	bool inverseMatrix(matrix4* output)const;

	/**************************************************************************/
	/*!
	   Check if matrix is same as identity (FOR DEBUG PURPOSE)
	*/
	/**************************************************************************/
	bool isIdentity()const;

	/**************************************************************************/
	/*!
	   negative matrix
	*/
	/**************************************************************************/
	void negativeMatrix();

	/**************************************************************************/
	/*!
	   Negate  matrix and store it in output
	*/
	/**************************************************************************/
	void negativeMatrix(matrix4* output)const;

	/**************************************************************************/
	/*!
	   Rotate input
	*/
	/**************************************************************************/
	void rotate(const Quaternion& input);

	/**************************************************************************/
	/*!
	   Rotate input and store into output
	*/
	/**************************************************************************/
	void rotate(const Quaternion& input, matrix4* output)const;

	/**************************************************************************/
	/*!
	   Rotate the axis base on the angle
	*/
	/**************************************************************************/
	void rotate(const vec3& axis, float angle);

	/**************************************************************************/
	/*!
	   Rotate the axis base on the angle and store into output
	*/
	/**************************************************************************/
	void rotate(const vec3& axis, float angle, matrix4* output);

	/**************************************************************************/
	/*!
		Rotate matrix on angle around x axis
	*/
	/**************************************************************************/
	void rotateX(float angle);

	/**************************************************************************/
	/*!
	   Rotate matrix on angle and store it in output around x axis
	*/
	/**************************************************************************/
	void rotateX(float angle, matrix4* output);

	/**************************************************************************/
	/*!
		Rotate matrix on angle around y axis
	*/
	/**************************************************************************/
	void rotateY(float angle);

	/**************************************************************************/
	/*!
	   Rotate matrix on angle and store it in output around y axis
	*/
	/**************************************************************************/
	void rotateY(float angle, matrix4* output);

	/**************************************************************************/
	/*!
		Rotate matrix on angle around z axis
	*/
	/**************************************************************************/
	void rotateZ(float angle);

	/**************************************************************************/
	/*!
	   Rotate matrix on angle and store it in output around z axis
	*/
	/**************************************************************************/
	void rotateZ(float angle, matrix4* output);

	/**************************************************************************/
	/*!
	   Scale all axis base on input
	*/
	/**************************************************************************/
	void scale(float input);

	/**************************************************************************/
	/*!
	   Scale all axis base on input and store in output
	*/
	/**************************************************************************/
	void scale(float input, matrix4* output)const;

	/**************************************************************************/
	/*!
	   Scale all axis base on x y and z input
	*/
	/**************************************************************************/
	void scale(float x, float y, float z);

	/**************************************************************************/
	/*!
	   Scale all axis base on input and store in output
	*/
	/**************************************************************************/
	void scale(float x, float y, float z, matrix4* output)const;

	/**************************************************************************/
	/*!
	   Scale all axis base on input vector
	*/
	/**************************************************************************/
	void scale(const vec3& inputvector);

	/**************************************************************************/
	/*!
	   Scale all axis base on input vector and store into output
	*/
	/**************************************************************************/
	void scale(const vec3& inputvector, matrix4* output);

	/**************************************************************************/
	/*!
		Transform a point by the matrix
	*/
	/**************************************************************************/
	void transformPoint(vec3* input)const;

	/**************************************************************************/
	/*!
		Transform a point by the matrix and store it in output
	*/
	/**************************************************************************/
	void transformPoint(const vec3& input, vec3* output)const;

	/**************************************************************************/
	/*!
		Transform a vector by the matrix
	*/
	/**************************************************************************/
	void transformVector(vec3* input)const;

	/**************************************************************************/
	/*!
		Transform a vector by the matrix and store it in output
	*/
	/**************************************************************************/
	void transformVector(const vec3& input, vec3* output)const;

	/**************************************************************************/
	/*!
		Transform a vector by the matrix and store into output
	*/
	/**************************************************************************/
	 void transformVector(float x, float y, float z, float w, vec3* output)const;

	/**************************************************************************/
	/*!
	   Transform a vector by the matrix
	*/
	/**************************************************************************/
	void transformVector(vec4* input)const;

	/**************************************************************************/
	/*!
	   Transform a vector by the matrix and store into output
	*/
	/**************************************************************************/
	void transformVector(const vec4& input, vec4* output)const;

	/**************************************************************************/
	/*!
	   Translate the matrix along the x and y and z axis
	*/
	/**************************************************************************/
	void translate(float x, float y, float z);

	/**************************************************************************/
	/*!
	   Translate the matrix along the x and y and z axis and store in output
	*/
	/**************************************************************************/
	void translate(float x, float y, float z, matrix4* output)const;

	/**************************************************************************/
	/*!
	   Translate the matrix along the x and y and z axis from vec3
	*/
	/**************************************************************************/
	void translate(const vec3& input);

	/**************************************************************************/
	/*!
	   Translate the matrix along the x and y and z axis from vec3 and
	   store in output
	*/
	/**************************************************************************/
	void translate(const vec3& input, matrix4* output)const;

	/**************************************************************************/
	/*!
		Transpose matrix
	*/
	/**************************************************************************/
	void transpose();

	/**************************************************************************/
	/*!
		Transpose matrix and store in output
	*/
	/**************************************************************************/
	void transpose(matrix4* output) const;

	/**************************************************************************/
	/*!
		Overload operator +
	*/
	/**************************************************************************/
	inline const matrix4 operator+(const matrix4& input) const;

	/**************************************************************************/
	/*!
		Overload operator +=
	*/
	/**************************************************************************/
	inline matrix4& operator+=(const matrix4& input);

	/**************************************************************************/
	/*!
		Overload operator -
	*/
	/**************************************************************************/
	inline const matrix4 operator-(const matrix4& input) const;

	/**************************************************************************/
	/*!
		Overload operator -=
	*/
	/**************************************************************************/
	inline matrix4& operator-=(const matrix4& input);

	/**************************************************************************/
	/*!
		Overload operator +
	*/
	/**************************************************************************/
	inline const matrix4 operator-() const;

	/**************************************************************************/
	/*!
		Overload operator *
	*/
	/**************************************************************************/
	inline const matrix4 operator*(const matrix4& input) const;

	/**************************************************************************/
	/*!
		Overload operator *=
	*/
	/**************************************************************************/
	inline matrix4& operator*=(const matrix4& input);

};

/**************************************************************************/
/*!
	In line helper functions(making my life easier cos i lazy copy paste)
*/
/**************************************************************************/
inline void addMatrix_inline(const float* inputmatrix, float scalar, float* output);
inline void addMatrix_inline(const float* input1, const float* input2, float* output);
inline void subtractMatrix_inline(const float* input1, const float* input2, float* output);
inline void multiplyMatrix_inline(const float* input, float scalar, float* output);
inline void multiplyMatrix_inline(const float* input1, const float* input2, float* output);
inline void negativeMatrix_inline(const float* input, float* output);
inline void transformVector_inline(const float* matrix, float x, float y, float z, float w, float* output);
inline void transformVector_inline(const float* matrix, const float* vector, float* output);
inline void transposeMatrix_inline(const float* matrix, float* output);
////////////////////////////////////////////////////////////////////////
///
///                 MATRIX4 DEFINITIONS AND FUNCTIONS -START
///
//////////////////////////////////////////////////////////////////////
#endif //MATRIX4_H
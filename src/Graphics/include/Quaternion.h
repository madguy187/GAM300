/**********************************************************************************
* \file            Quaternion.h
*
* \brief        Definition of the Quaternion class function(s).
*
* \author        Tian Yu
*
* \email		t.yu\@digipen.edu
*
* \date			20 aug 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef QUATERNION_H
#define QUATERNION_H
#include "Math_Lib.h"

class matrix4;
class Quaternion
{
public:

	float x, y, z, w;
	
	/**************************************************************************/
	/*!
		Default constructor for quaternion (0,0,0,1)
	*/
	/**************************************************************************/
	Quaternion();

	/**************************************************************************/
	/*!
		Overloaded constructor to make quaternion to (x,y,z,w)
	*/
	/**************************************************************************/
	Quaternion(float x, float y, float z, float w);

	/**************************************************************************/
	/*!
		Overload constructor to make quaternion base on array
	*/
	/**************************************************************************/
	Quaternion(float* inputArray);

	/**************************************************************************/
	/*!
		Overload constructor to make quaternion base on the rotational part
		of a matrix
	*/
	/**************************************************************************/
	Quaternion(const matrix4& inputMatrix);

	/**************************************************************************/
	/*!
		Overload constructor to make quaternion base on the eulerAngle
		x = roll , y = pitch, z = yaw
	*/
	/**************************************************************************/
	Quaternion(const vec3& eulerAngle);

	/**************************************************************************/
	/*!
		Overload constructor to make a quaternion base on the rotation
		of a axis and angle
	*/
	/**************************************************************************/
	Quaternion(const vec3& axis, float angle);

	/**************************************************************************/
	/*!
		Overload constructor to make a quaternion base on the
		different between the start and end direction vector's rotation
	*/
	/**************************************************************************/
	Quaternion(const vec3& startDir, const vec3& endDir);

	/**************************************************************************/
	/*!
		Overload constructor to make a copy of another quaternion
	*/
	/**************************************************************************/
	Quaternion(const Quaternion& outputCopy);

	/**************************************************************************/
	/*!
		 Destructor for quaternion 
	*/
	/**************************************************************************/
	~Quaternion();

	/**************************************************************************/
	/*!
		Check if quaternion is identity quaternion
	*/
	/**************************************************************************/
	bool isIdentity()const;

	/**************************************************************************/
	/*!
		Check if quaternion is all zero 
	*/
	/**************************************************************************/
	bool isZero();
	
	/**************************************************************************/
	/*!
		Set if quaternion to identity(0,0,0,1)
	*/
	/**************************************************************************/
	static const Quaternion& setIdentity();

	/**************************************************************************/
	/*!
		Set if quaternion is all zero
	*/
	/**************************************************************************/
	static const Quaternion& setAllZero();

	/**************************************************************************/
	/*!
		Make a quaternion equal to the rotation difference between the start and
		end direction and store it in the output
	*/
	/**************************************************************************/
	static void makeQuaternion_Rotation(const vec3& startDir, const vec3& endDir, Quaternion* output);

	/**************************************************************************/
	/*!
		Make a quaternion equal to the rotation from a euler angle and
		store it in the output
	*/
	/**************************************************************************/
	static void makeQuaternion_EulerAngle(const vec3& eulerAngle, Quaternion* output);

	/**************************************************************************/
	/*!
		Make a quaternion equal to the rotation from a matrix and
		store it in the output
	*/
	/**************************************************************************/
	static void makeQuaternion_Maxtrix4(const matrix4& inputMatrix, Quaternion* output);

	/**************************************************************************/
	/*!
		Make a quaternion equal to the rotation from a axis angle and
		store it in the output
	*/
	/**************************************************************************/
	static void makeQuaternion_AxisAngle(const vec3& axis, float angle, Quaternion* output);


	/**************************************************************************/
	/*!
		Get euler angle from quaternion rotation
	*/
	/**************************************************************************/
	void getEulerAngle(vec3* eulerAngles) const;

	/**************************************************************************/
	/*!
		Set all quaternion points to negative
	*/
	/**************************************************************************/
	void setNegative();

	/**************************************************************************/
	/*!
		Set all quaternion points to negative and store into output
	*/
	/**************************************************************************/
	void setNegative(Quaternion* output)const;

	/**************************************************************************/
	/*!
		Set quaternion to inverse return true if can inverse
	*/
	/**************************************************************************/
	bool inverse();

	/**************************************************************************/
	/*!
		Set quaternion to inverse and store in output
		return true if can inverse 
	*/
	/**************************************************************************/
	bool inverse(Quaternion* output) const;

	/**************************************************************************/
	/*!
		Multiply this quaternion to input and store it back to this
	*/
	/**************************************************************************/
	void multiply(const Quaternion& input);

	/**************************************************************************/
	/*!
		Multiply input1 and input 2 and store into output
	*/
	/**************************************************************************/
	static void multiply(const Quaternion& input1, const Quaternion& input2, Quaternion* output);

	/**************************************************************************/
	/*!
		Normalize quaternion
	*/
	/**************************************************************************/
	void normalize();

	/**************************************************************************/
	/*!
		Normalize quaternion and store in output
	*/
	/**************************************************************************/
	void normalize(Quaternion* output)const;

	/**************************************************************************/
	/*!
		Rotate point by this quaternion and store in output
	*/
	/**************************************************************************/
	void rotatePoint(const vec3& point, vec3* output)const;

	/**************************************************************************/
	/*!
		Set x y z w of the quaternion
	*/
	/**************************************************************************/
	void set(float x, float y, float z, float w);

	/**************************************************************************/
	/*!
		Set quaternion base on array
	*/
	/**************************************************************************/
	void set(float* array);

	/**************************************************************************/
	/*!
		Set quaternion base on matrix4
	*/
	/**************************************************************************/
	void set(const matrix4& matrixInput);

	/**************************************************************************/
	/*!
		Set quaternion base on euler
	*/
	/**************************************************************************/
	void set(const vec3& eulerAngle);

	/**************************************************************************/
	/*!
		Set quaternion base on axis and angle
	*/
	/**************************************************************************/
	void set(const vec3& axis, float angle);

	/**************************************************************************/
	/*!
		Copy quaternion base on another quaternion
	*/
	/**************************************************************************/
	void set(const Quaternion& input);

	/**************************************************************************/
	/*!
		Convert the quaternion to axis angle
	*/
	/**************************************************************************/
	float ConvertToAxisAngle(vec3* input);

	/**************************************************************************/
	/*!
		Interpolates between 2 quaternion using linear interpolation
	*/
	/**************************************************************************/
	static void lerp(const Quaternion& input1, const Quaternion& input2, float interpolationt, Quaternion* output);

	/**************************************************************************/
	/*!
		Overload *
	*/
	/**************************************************************************/
	inline const Quaternion operator*(const Quaternion& q) const;

	/**************************************************************************/
	/*!
		Overload *=
	*/
	/**************************************************************************/
	inline Quaternion& operator*=(const Quaternion& q);

	/**************************************************************************/
	/*!
		Overload ==
	*/
	/**************************************************************************/
	inline bool operator==(const Quaternion& rhs) const;

	/**************************************************************************/
	/*!
		Overload !=
	*/
	/**************************************************************************/
	inline bool operator!=(const Quaternion& rhs) const;
	
};




















#endif//QUATERNION_H

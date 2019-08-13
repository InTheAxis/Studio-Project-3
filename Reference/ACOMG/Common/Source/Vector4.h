/******************************************************************************/
/*!
\file	Vector4.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Struct to define a 3D vector
*/
/******************************************************************************/

#ifndef Vector4_H
#define Vector4_H

#include "MyMath.h"
#include <iostream>
#include "Vector3.h"

#pragma warning( disable: 4290 ) //for throw(DivideByZero)

/******************************************************************************/
/*!
		Class Vector4:
\brief	Defines a 3D vector and its methods
*/
/******************************************************************************/
struct Vector4
{
	float x, y, z, w;

	Vector4(float a = 0.0, float b = 0.0, float c = 0.0, float d = 1.0);
	Vector4(const Vector4 &rhs);
	Vector4(const Vector3 &rhs, float d = 1.0);
	~Vector4();

	void Set(float a = 0, float b = 0, float c = 0, float d = 1); //Set all data
	void SetZero(void); //Set all data to zero
	bool IsZero(void) const; //Check if data is zero

	Vector4 operator+(const Vector4& rhs) const; //Vector addition
	Vector4& operator+=(const Vector4& rhs);

	Vector4 operator-(const Vector4& rhs) const; //Vector subtraction
	Vector4& operator-=(const Vector4& rhs);

	Vector4 operator-(void) const; //Unary negation

	Vector4 operator*(float scalar) const; //Scalar multiplication
	Vector4& operator*=(float scalar);

	bool operator==(const Vector4& rhs) const; //Equality check
	bool operator!= (const Vector4& rhs) const; //Inequality check

	Vector4& operator=(const Vector4& rhs); //Assignment operator

	float Length(void) const; //Get magnitude
	float LengthSquared(void) const; //Get square of magnitude

	//Return a copy of this vector, normalized
	//Throw a divide by zero exception if normalizing a zero vector
	Vector4 Normalized(void) const throw(DivideByZero);

	//Normalize this vector and return a reference to it
	//Throw a divide by zero exception if normalizing a zero vector
	Vector4& Normalize(void) throw(DivideByZero);

	friend std::ostream& operator<<(std::ostream& os, Vector4& rhs); //print to ostream

	friend Vector4 operator*(float scalar, const Vector4& rhs); //what is this for?
};

#endif //Vector4_H

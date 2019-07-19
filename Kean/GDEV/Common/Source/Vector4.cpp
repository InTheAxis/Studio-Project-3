/******************************************************************************/
/*!
\file	Vector4.cpp
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Struct to define a 3D vector
*/
/******************************************************************************/
#include <cmath>
#include "Vector4.h"

/******************************************************************************/
/*!
\brief	Vector4 default constructor

\param	a
	x value
\param	b
	y value
\param	c
	z value
*/
/******************************************************************************/
Vector4::Vector4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d)
{
}

/******************************************************************************/
/*!
\brief	Vector4 copy constructor

\param	rhs
	Vector to copy from
*/
/******************************************************************************/
Vector4::Vector4(const Vector4 &rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{
}

Vector4::Vector4(const Vector3 & rhs, float d) : x(rhs.x), y(rhs.y), z(rhs.z), w(d)
{
}

/******************************************************************************/
/*!
\brief	Vector4 destructor
*/
/******************************************************************************/
Vector4::~Vector4()
{
}

/******************************************************************************/
/*!
\brief	Set the elements of this vector

\param	a
	x value
\param	b
	y value
\param	c
	z value
*/
/******************************************************************************/
void Vector4::Set(float a, float b, float c, float d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

/******************************************************************************/
/*!
\brief	Set all elements to zero
*/
/******************************************************************************/
void Vector4::SetZero(void)
{
	x = y = z = w = 0.0f;
}

/******************************************************************************/
/*!
\brief	Check if vector is zero, using Math::Epsilon

\return TRUE if vector is zero
*/
/******************************************************************************/
bool Vector4::IsZero(void) const
{
	return Math::FIsEqual(x, 0.f) && Math::FIsEqual(y, 0.f) && Math::FIsEqual(z, 0.f) && Math::FIsEqual(w, 0.f);
}

/******************************************************************************/
/*!
\brief
operator+ overload for vector addition

\param rhs
	Vector4 to add with
\return
	Resulting vector
*/
/******************************************************************************/
Vector4 Vector4::operator+(const Vector4& rhs) const
{
	return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

/******************************************************************************/
/*!
\brief
operator+= overload for vector addition and assignment

\param rhs
	Vector4 to add with
\return
	Resulting vector
*/
/******************************************************************************/
Vector4& Vector4::operator+=(const Vector4& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
}

/******************************************************************************/
/*!
\brief
operator- overload for vector subtraction

\param rhs
	Vector4 to subtract with
\return Resulting vector
*/
/******************************************************************************/
Vector4 Vector4::operator-(const Vector4& rhs) const
{
	return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

/******************************************************************************/
/*!
\brief
operator-= overload for vector subtraction and assignment

\param rhs
	Vector4 to subtract with
\return
	Resulting vector
*/
/******************************************************************************/
Vector4& Vector4::operator-=(const Vector4& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
}

/******************************************************************************/
/*!
\brief
operator- overload for unary negation

\return
	Resulting vector
*/
/******************************************************************************/
Vector4 Vector4::operator-(void) const
{
	return Vector4(-x, -y, -z, -w);
}

/******************************************************************************/
/*!
\brief
operator* overload for scalar multiplication

\param scalar - float to multiply with

\exception None
\return Resulting vector
*/
Vector4 Vector4::operator*(float scalar) const
{
	return Vector4(scalar * x, scalar * y, scalar * z, scalar * w);
}

/******************************************************************************/
/*!
\brief
operator*= overload for scalar multiplication and assignment

\param scalar
	float to multiply with
\return
	Resulting vector
*/
/******************************************************************************/
Vector4& Vector4::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

/******************************************************************************/
/*!
\brief
operator== overload for equality check, using Math::EPSILON

\param rhs
	vector to compare with

\exception None
\return Resulting vector
*/
/******************************************************************************/
bool Vector4::operator==(const Vector4& rhs) const
{
	return Math::FIsEqual(x, rhs.x) && Math::FIsEqual(y, rhs.y) && Math::FIsEqual(z, rhs.z) && Math::FIsEqual(w, rhs.w);
}

/******************************************************************************/
/*!
\brief
operator!= overload for inequality check, using Math::EPSILON

\param rhs
	vector to compare with
\return
	Resulting vector
*/
/******************************************************************************/
bool Vector4::operator!= (const Vector4& rhs) const
{
	return !Math::FIsEqual(x, rhs.x) || !Math::FIsEqual(y, rhs.y) || !Math::FIsEqual(z, rhs.z) || !Math::FIsEqual(w, rhs.w);
}

/******************************************************************************/
/*!
\brief	Assignment operation (assign a vector to this vector)

\param	rhs
	Vector to assign
\return
	A reference to this vector
*/
/******************************************************************************/
Vector4& Vector4::operator=(const Vector4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

/******************************************************************************/
/*!
\brief
Return length of vector

\return
	Length
*/
/******************************************************************************/
float Vector4::Length(void) const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

/******************************************************************************/
/*!
\brief
Return square of length of vector

\return
	Length square
*/
/******************************************************************************/
float Vector4::LengthSquared(void) const
{
	return x * x + y * y + z * z + w * w;
}

/******************************************************************************/
/*!
\brief
Return a copy of this vector, normalized

\param rhs
	Point to calculate distance from
\exception Divide by zero
	thrown if normalizing a zero vector
\return
	Resulting normalized vector
*/
/******************************************************************************/
Vector4 Vector4::Normalized(void) const throw(DivideByZero)
{
	float d = Length();
	if (d <= Math::EPSILON && -d <= Math::EPSILON)
		throw DivideByZero();
	return Vector4(x / d, y / d, z / d, w / d);
}

/******************************************************************************/
/*!
\brief
Normalize this vector and return a reference to it

\param rhs
	Point to calculate distance from
\exception Divide by zero
	thrown if normalizing a zero vector
\return
	Reference to this vector
*/
/******************************************************************************/
Vector4& Vector4::Normalize(void) throw(DivideByZero)
{
	float d = Length();
	if (d <= Math::EPSILON && -d <= Math::EPSILON)
		throw DivideByZero();
	x /= d;
	y /= d;
	z /= d;
	w /= d;
	return *this;
}

std::ostream& operator<< (std::ostream& os, Vector4& rhs)
{
	os << "[ " << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w << " ]";
	return os;
}

/******************************************************************************/
/*!
\brief
Scalar product of scalar with vector

\param scalar
	scalar value
\param vector
	vector
\return
	Resulting vector
*/
/******************************************************************************/
Vector4 operator*(float scalar, const Vector4& rhs)
{
	return rhs * scalar;
}

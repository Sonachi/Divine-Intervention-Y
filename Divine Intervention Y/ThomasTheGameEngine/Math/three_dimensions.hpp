#pragma once
#include "two_dimensions.hpp"

class Vec3 {
public:
	float x, y, z;

	//--------------------------------Constructors and Factories------------------------------------\\

	Vec3(void);
	Vec3(float, float, float);
	static Vec3 Zero();
	static Vec3 One();
	static Vec3 BasisX();
	static Vec3 BasisY();
	static Vec3 BasisZ();

	//--------------------------------Operator Overloading-------------------------------------------\\


	 Vec3 operator+(const Vec3&) const;
	 bool operator==(const Vec3&) const;
	 bool operator!=(const Vec3&) const;
	 Vec3 operator+=(const Vec3&);
	 void operator=(const Vec3&);
	 Vec3 operator-(const Vec3&) const;
	 Vec3 operator-=(const Vec3&);
	 Vec3 operator*(const float&) const;
	 Vec3 operator*=(const float&);
	 Vec3 operator/(const float&) const;
	 Vec3 operator/=(const float&);
	 std::string toString();

	 friend const Vec3 operator*(const float&, const Vec3&);

	//--------------------------------Actual Maths----------------------------------------------------\\

	static float length(Vec3);
	float length();
	float magnitude();
	void NormalizeThis();
	Vec3 Normalized();
	static float dot(Vec3, Vec3);
	static Vec3 cross(Vec3, Vec3);
};

class Matrix3 {
public:
	float values[9];

	//--------------------------------Constructors and Factories------------------------------------\\

	Matrix3(float=1, float=0, float=0, float=0, float=1, float=0, float=0, float=0, float=1); //9 for building a custom matrix
	static Matrix3 Identity();
	static Matrix3 Rotate(float, int); //Angle - Axis (0 = x, 1 = y, 2 = z) This rotate is only for rotation about a basis axis (x/y/z)
	static Matrix3 Translate(float, float);
	//Matrix3 Skew(); //Not sure how this works yet but will check it.

	//---------------------------------Operator Overloading-------------------------------------------\\

	 Matrix3 operator+(const Matrix3&);
	 Matrix3 operator+=(const Matrix3&);
	 Matrix3 operator-(const Matrix3&);
	 Matrix3 operator-=(const Matrix3&);
	 Matrix3 operator*(const float&);
	 Matrix3 operator*=(const float&);
	 Matrix3 operator/(const float&);
	 Matrix3 operator/=(const float&);
	 Matrix3 operator=(const Matrix3&);
	 Matrix3 operator*(const Matrix3&);
	 Matrix3 operator*=(const Matrix3&);
	 Vec3 operator*(const Vec3&);
	 std::string toString();

	//------------------------------------Actual Maths----------------------------------------------\\

	Matrix3 transpose();
	Matrix3 inverse();
	Matrix3 cofactor();
	Matrix3 adjoint();
	Matrix2 minor(int);

	float det();

	Vec3 getRowVector(int) const;
	Vec3 getColVector(int) const;
};
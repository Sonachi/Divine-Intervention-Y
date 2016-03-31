#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

class Vec2 {
public:
	float x, y;

	//--------------------------------Constructors and Factories------------------------------------\\

	Vec2();
	Vec2(float, float);
	static Vec2 Zero();
	static Vec2 BasisX();
	static Vec2 BasisY();

	//--------------------------------Operator Overloading-------------------------------------------\\

	 Vec2 operator+(const Vec2&) const;
	 Vec2 operator+=(const Vec2&);
	 void operator=(const Vec2&);
	 Vec2 operator-(const Vec2&) const;
	 Vec2 operator-=(const Vec2&);
	 Vec2 operator*(const float&) const;
	 Vec2 operator*=(const float&);
	 Vec2 operator/(const float&) const;
	 Vec2 operator/=(const float&);
	std::string toString();

	//--------------------------------Actual Maths----------------------------------------------------\\

	void NormalizeThis();
	static float length(Vec2);
	Vec2 Normalized();
	static float dot(Vec2, Vec2);
	static Vec2 ortho(Vec2);
};

class Matrix2 {
public:
	float values[4];

	//--------------------------------Constructors and Factories------------------------------------\\

	Matrix2(float=1, float=0, float=0, float=1); //4 in for 2x2
	static Matrix2 Identity();
	static Matrix2 Rotate(float);

	//--------------------------------Operator Overloading-------------------------------------------\\

	 Matrix2 operator+(const Matrix2&);
	 Matrix2 operator+=(const Matrix2&);
	 Matrix2 operator-(const Matrix2&);
	 Matrix2 operator-=(const Matrix2&);
	 Matrix2 operator*(const float&);
	 Matrix2 operator*=(const float&);
	 Matrix2 operator/(const float&);
	 Matrix2 operator/=(const float&);
	 Matrix2 operator=(const Matrix2&);
	 Matrix2 operator*(const Matrix2&);
	 Matrix2 operator*=(const Matrix2&);
	 std::string toString();

	//------------------------------------Actual Maths----------------------------------------------\\

	float minor(int index);
	float det();
	Vec2 getRowVector(int) const;
	Vec2 getColVector(int) const;

};
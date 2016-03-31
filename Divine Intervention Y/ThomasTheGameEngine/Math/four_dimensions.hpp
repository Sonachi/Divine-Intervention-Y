#pragma once
#include "three_dimensions.hpp"

class Quat { //q = {w,{x,y,z}} ==> q = {cos(9/2), {x sin (9/2), y sin(9/2), z sin(9/2)}}
public:
	float w;
	Vec3 vector;

	//--------------------------------Constructors and Factories------------------------------------\\

	Quat();
	Quat(float, Vec3);
	Quat(float, float, float, float);
	static Quat Identity();

	//--------------------------------Operator Overloading-------------------------------------------\\

	Quat operator*(const Quat&);
	Quat operator*(const float&);
	Quat operator*(const Vec3&);
	Quat operator+(const Quat&);
	Quat operator-(const Quat&);
	Quat operator/(const float&);
	std::string Quat::toString();

	//--------------------------------Actual Maths----------------------------------------------------\\

	Quat conjugate();
	Quat inverse();
	Quat NormalizeThis();

	static Vec3 rotate(Quat, Vec3);
	static float length(Quat);
	float length();
	static float dot(Quat, Quat);

};

class Vec4 {
public:
	float w, x, y, z;

	//--------------------------------Constructors and Factories------------------------------------\\

	Vec4();
	Vec4(float _w, float _x, float _y, float _z);
	static Vec4 Zero();
	static Vec4 BasisW();
	static Vec4 BasisX();
	static Vec4 BasisY();
	static Vec4 BasisZ();
	static Vec4 HomoVec3(Vec3);
	static Vec3 DownCast(Vec4);

	//--------------------------------Operator Overloading-------------------------------------------\\

	Vec4 operator+(const Vec4&);
	Vec4 operator+=(const Vec4&);
	Vec4 operator-(const Vec4&);
	Vec4 operator-=(const Vec4&);
	Vec4 operator*(const float&);
	Vec4 operator*=(const float&);
	Vec4 operator/(const float&);
	Vec4 operator/=(const float&);
	void operator=(const Vec4&);
	std::string Vec4::toString();

	//--------------------------------Actual Maths----------------------------------------------------\\

	void NormalizeThis();
	static float length(Vec4);
	Vec4 Normalized();
	static float dot(Vec4, Vec4);
};

class Matrix4 {
public:
	float values[16];

	//--------------------------------Constructors and Factories------------------------------------\\

	//Matrix4(); // no need for this... 

	//Default identity matrix
	Matrix4(
		float = 1, float = 0, float = 0, float = 0,
		float = 0, float = 1, float = 0, float = 0,
		float = 0, float = 0, float = 1, float = 0,
		float = 0, float = 0, float = 0, float = 1);

	Matrix4(Quat _quaternion);

	static Matrix4 Rotate(Quat); //TODO -- Take other stuff??
	static Matrix4 Rotate(float angle_x, float angle_y, float angle_z); //Rotation matrix (from my code -- Mat)
	static Matrix4 Translate(float, float, float); //x,y,z
	static Matrix4 Scale(float _x, float _y, float _z);
	static Matrix4 Zero();
	static Matrix4 Identity();

	//--------------------------------Operator Overloading-------------------------------------------\\

	Matrix4 operator+(const Matrix4&);
	Matrix4 operator+=(const Matrix4&);
	Matrix4 operator-(const Matrix4&);
	Matrix4 operator-=(const Matrix4&);
	Matrix4 operator*(const Matrix4&);
	Matrix4 operator*=(const Matrix4&);
	Matrix4 operator*(const float&);
	Matrix4 operator*=(const float&);
	void operator=(const Matrix4&);
	Vec4 operator*(const Vec4&);
	std::string toString();

	//Entry access
	struct Matrix4Entry
	{
		const int index;
		const Matrix4* m;

		Matrix4Entry(const int _index, const Matrix4* _m) : index(_index), m(_m){}

		inline float operator[](const int& i2) const 
		{
			return m->values[(index * 4) + i2];
		}
	};

	inline const Matrix4Entry& operator[](const int& i1) const
	{
		int index = i1;

		return Matrix4Entry(index, this);
	}

	//--------------------------------Actual Maths----------------------------------------------------\\

	Matrix3 minor(int);
	Matrix4 transpose();
	Matrix4 inverse();
	Matrix4 cofactor();
	Matrix4 adjoint();
	Vec4 getRowVector(int) const;
	Vec4 getColVector(int) const;
	float det();
};
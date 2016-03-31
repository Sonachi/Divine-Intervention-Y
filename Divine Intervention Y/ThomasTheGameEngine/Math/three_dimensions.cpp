#include "three_dimensions.hpp"
#include "four_dimensions.hpp"

//------------------------------------------Vector----------------------------------------\\

//Constructors and Factories

Vec3::Vec3() {
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

Vec3 Vec3::Zero() {
	return Vec3(0, 0, 0);
}

Vec3 Vec3::One() {
	return Vec3(1, 1, 1);
}

Vec3 Vec3::BasisX() {
	return Vec3(1, 0, 0);
}

Vec3 Vec3::BasisY() {
	return Vec3(0, 1, 0);
}

Vec3 Vec3::BasisZ() {
	return Vec3(0, 0, 1);
}

//Operator Overloads

 Vec3 Vec3::operator+(const Vec3& value) const {
	return Vec3(x + value.x, y + value.y, z + value.z);
}

 bool Vec3::operator==(const Vec3 &other) const
 {
	 return (x == other.x) && (y == other.y) && (z == other.z);
 }

 bool Vec3::operator!=(const Vec3 &other) const
 {
	 return !((x == other.x) && (y == other.y) && (z == other.z));
 }

 Vec3 Vec3::operator+=(const Vec3& value) {
	x += value.x;
	y += value.y;
	z += value.z;
	return *this;
}

 Vec3 Vec3::operator-(const Vec3& value) const {
	return Vec3(x - value.x, y - value.y, z - value.z);
}

 Vec3 Vec3::operator-=(const Vec3& value) {
	x -= value.x;
	y -= value.y;
	z -= value.z;
	return *this;
}

 Vec3 Vec3::operator*(const float& value) const {
	return Vec3(x * value, y * value, z * value);
}

 const Vec3 operator*(const float& value, const Vec3& _this)
 {
	 return Vec3(_this.x * value, _this.y * value, _this.z * value);
 }

 Vec3 Vec3::operator*=(const float& value) {
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

 void Vec3::operator=(const Vec3& value) {
	x = value.x;
	y = value.y;
	z = value.z;
};

 Vec3 Vec3::operator/(const float& value) const {
	return Vec3(x / value, y / value, z / value);
}

 Vec3 Vec3::operator/=(const float& value) {
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

//Actual Maths

float Vec3::length(Vec3 value) {
	return sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
}

float Vec3::length()
{
	return sqrt(x * x + y * y + z * z);
}

float Vec3::magnitude()
{
	return length(*this);
}

void Vec3::NormalizeThis() {
	float size = Vec3::length(*this);
	x = x / size;
	y = y / size;
	z = z / size;
}

Vec3 Vec3::Normalized() {
	float size = Vec3::length(*this);
	return Vec3(x / size, y / size, z / size);
}

float Vec3::dot(Vec3 first, Vec3 second) {
	return first.x * second.x + first.y * second.y + first.z * second.z;
}

Vec3 Vec3::cross(Vec3 first, Vec3 second) {
	return Vec3(first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x);
}

std::string Vec3::toString() {
	return ("<" + std::to_string(x) + " , " + std::to_string(y) + " , " + std::to_string(z) + ">");
}

//--------------------------------------------------------Matrix------------------------------------------------------------------------\\

//Constructors and Factories

Matrix3::Matrix3(float _0, float _1, float _2, float _3, float _4, float _5, float _6, float _7, float _8) {
	values[0] = _0;
	values[1] = _1;
	values[2] = _2;
	values[3] = _3;
	values[4] = _4;
	values[5] = _5;
	values[6] = _6;
	values[7] = _7;
	values[8] = _8;
}

Matrix3 Matrix3::Identity() {
	return Matrix3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1);
}

Matrix3 Matrix3::Rotate(float angle, int axis) {
	if (axis = 0) {
		return Matrix3(
			1, 0, 0,
			0, cos(angle), -sin(angle),
			0, sin(angle), cos(angle));
	} else if (axis = 1) {
		return Matrix3(
			cos(angle), 0, -sin(angle),
			0, 1, 0,
			sin(angle), 0, cos(angle));
	} else {
		return Matrix3(
			cos(angle), -sin(angle), 0,
			sin(angle), cos(angle), 0,
			0, 0, 1);
	}
	return Matrix3::Identity();
}

std::string Matrix3::toString() {
	return (
		"{" + std::to_string(values[0]) + "," + std::to_string(values[1]) + "," + std::to_string(values[2]) + "\n" +
		std::to_string(values[3]) + "," + std::to_string(values[4]) + "," + std::to_string(values[5]) + "\n" +
		std::to_string(values[6]) + "," + std::to_string(values[7]) + "," + std::to_string(values[8]));
}

//Operator Overloading

 Matrix3 Matrix3::operator+(const Matrix3& other) {
	return Matrix3(
		other.values[0] + values[0], other.values[1] + values[1], other.values[2] + values[2],
		other.values[3] + values[3], other.values[4] + values[4], other.values[5] + values[5],
		other.values[6] + values[6], other.values[7] + values[7], other.values[8] + values[8]);
}

 Matrix3 Matrix3::operator+=(const Matrix3& other) {
	for (int i = 0; i < 9; i++) {
		values[i] = values[i] + other.values[i];
	}
	return *this;
}

 Matrix3 Matrix3::operator-(const Matrix3& other) {
	return Matrix3(
		values[0] - other.values[0], values[1] - other.values[1], values[2] - other.values[2],
		values[3] - other.values[3], values[4] - other.values[4], values[5] - other.values[5],
		values[6] - other.values[6], values[7] - other.values[7], values[8] - other.values[8]);
}

 Matrix3 Matrix3::operator-=(const Matrix3& other) {
	for (int i = 0; i < 9; i++) {
		values[i] = values[i] - other.values[i];
	}
	return *this;
}

 Matrix3 Matrix3::operator*(const float& other) {
	return Matrix3(
		values[0] * other, values[1] * other, values[2] * other,
		values[3] * other, values[4] * other, values[5] * other,
		values[6] * other, values[7] * other, values[8] * other);
}

 Matrix3 Matrix3::operator*=(const float& other) {
	for (int i = 0; i < 9; i++) {
		values[i] = values[i] * other;
	}
	return *this;
}

 Matrix3 Matrix3::operator/(const float& other) {
	return Matrix3(
		values[0] / other, values[1] / other, values[2] / other,
		values[3] / other, values[4] / other, values[5] / other,
		values[6] / other, values[7] / other, values[8] / other);
}

 Matrix3 Matrix3::operator/=(const float& other) {
	for (int i = 0; i < 9; i++) {
		values[i] = values[i] / other;
	}
	return *this;
}
 Matrix3 Matrix3::operator=(const Matrix3& other) {
	for (int i = 0; i < 9; i++) {
		values[i] = other.values[i];
	}
	return *this;
}

 Matrix3 Matrix3::operator*(const Matrix3& other) {
	return Matrix3(
		Vec3::dot(getRowVector(0), other.getColVector(0)), Vec3::dot(getRowVector(0), other.getColVector(1)), Vec3::dot(getRowVector(0), other.getColVector(2)),
		Vec3::dot(getRowVector(1), other.getColVector(0)), Vec3::dot(getRowVector(1), other.getColVector(1)), Vec3::dot(getRowVector(1), other.getColVector(2)),
		Vec3::dot(getRowVector(2), other.getColVector(0)), Vec3::dot(getRowVector(2), other.getColVector(1)), Vec3::dot(getRowVector(2), other.getColVector(2)));
}

 Matrix3 Matrix3::operator*=(const Matrix3& other) {
	for (int i = 0; i < 9; i++) {
		values[i] = Vec3::dot(getRowVector(i / 3), other.getColVector(i % 3));
	}
	return *this;
}

 Vec3 Matrix3::operator*(const Vec3& other){
	 return Vec3(
		 Vec3::dot(getRowVector(0), other),
		 Vec3::dot(getRowVector(1), other),
		 Vec3::dot(getRowVector(2), other));
 }

//Actual Maths
Matrix3 Matrix3::Translate(float x, float y) {
	return Matrix3(
		1, 0, x,
		0, 1, y,
		0, 0, 1);
}

Matrix3 Matrix3::transpose() {
	return Matrix3(
		values[0], values[3], values[6],
		values[1], values[4], values[7],
		values[2], values[5], values[8]);
}

Matrix3 Matrix3::inverse() {
	return adjoint() * 1 / det();
}

Matrix3 Matrix3::adjoint() {
	return cofactor().transpose();
}

Matrix3 Matrix3::cofactor() {
	Matrix3 cofactorMatrix;
	for (int i = 0; i < 9; i++) {
		cofactorMatrix.values[i] = pow(-1, i % 3 + i / 3) * minor(i).det();
	}
	return cofactorMatrix;
}

Matrix2 Matrix3::minor(int pos) {
	Matrix2 minorSquare;
	int oldIndex = 0;
	for (int i = 0; i < 4; i++) {
		while (oldIndex % 3 == pos % 3 || oldIndex / 3 == pos / 3) {
			oldIndex++;
		}
		minorSquare.values[i] = values[oldIndex];
		oldIndex++;
	}
	return minorSquare;
}

float Matrix3::det() {
	float determ = 0;
	for (int i = 0; i < 3; i++) {
		determ += pow(-1, i % 3 + i / 3) * values[i] * minor(i).det();
	}
	return determ;
}

Vec3 Matrix3::getRowVector(int row) const {
	return Vec3(values[0 + row * 3], values[1 + row * 3], values[2 + row * 3]);
}

Vec3 Matrix3::getColVector(int col) const {
	return Vec3(values[0 + col], values[3 + col], values[6 + col]);
}
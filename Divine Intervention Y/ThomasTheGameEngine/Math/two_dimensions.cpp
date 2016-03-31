#include "two_dimensions.hpp"

//-----------------------------------------------Vectors--------------------------------\\

//---------Constructors and Factories--------\\

Vec2::Vec2() {
	x = 0;
	y = 0;
}

Vec2::Vec2(float _x, float _y) {
	x = _x;
	y = _y;
}

Vec2 Vec2::Zero() {
	return Vec2(0, 0);
}

Vec2 Vec2::BasisX() {
	return Vec2(1, 0);
}

Vec2 Vec2::BasisY() {
	return Vec2(0, 1);
}

//------------Operators---------------\\

 Vec2 Vec2::operator+(const Vec2& value) const {
	return Vec2(x + value.x, y + value.y);
}

 Vec2 Vec2::operator+=(const Vec2& value) {
	x += value.x;
	y += value.y;
	return *this;
}

 Vec2 Vec2::operator-(const Vec2& value) const {
	return Vec2(x - value.x, y - value.y);
}

 Vec2 Vec2::operator-=(const Vec2& value) {
	x -= value.x;
	y -= value.y;
	return *this;
}

 Vec2 Vec2::operator*(const float& value) const {
	return Vec2(x * value, y * value);
}

 Vec2 Vec2::operator*=(const float& value) {
	x *= value;
	y *= value;
	return *this;
}

 Vec2 Vec2::operator/(const float& value) const {
	return Vec2(x / value, y / value);
}

 Vec2 Vec2::operator/=(const float& value) {
	x /= value;
	y /= value;
	return *this;
}

 void Vec2::operator=(const Vec2& value) {
	x = value.x;
	y = value.y;
};

std::string Vec2::toString() {
	return ("<" + std::to_string(x) + "," + std::to_string(y) + ">");
}

//-----------Actual Maths---------\\

void Vec2::NormalizeThis() {
	float size = Vec2::length(*this);
	x = x / size;
	y = y / size;
}


float Vec2::length(Vec2 value) {
	return sqrt(value.x * value.x + value.y * value.y);
}

Vec2 Vec2::Normalized() {
	float size = Vec2::length(*this);
	return Vec2(x / size, y / size);
}

float Vec2::dot(Vec2 first, Vec2 second) {
	return first.x * second.x + first.y * second.y;
}

Vec2 Vec2::ortho(Vec2 value) { //Check back to confirm which one is negative
	return Vec2(value.y, -value.x);
}

//-----------------------------------------------Matrix--------------------------------\\

//--------------------------------Constructors and Factories------------------------------------\\

Matrix2::Matrix2(float _0, float _1, float _2, float _3) {
	values[0] = _0;
	values[1] = _1;
	values[2] = _2;
	values[3] = _3;
}

Matrix2 Matrix2::Identity() {
	return Matrix2(
		1, 0,
		0, 1);
}

Matrix2 Matrix2::Rotate(float angle) {
	return Matrix2(
		cos(angle), -sin(angle),
		sin(angle), cos(angle));
}

//--------------------------------Operator Overloading-------------------------------------------\\

 Matrix2 Matrix2::operator+(const Matrix2& other) {
	return Matrix2(
		other.values[0] + values[0], other.values[1] + values[1],
		other.values[2] + values[2], other.values[3] + values[3]);
}

 Matrix2 Matrix2::operator+=(const Matrix2& other) {
	for (int i = 0; i < 4; i++) {
		values[i] = values[i] + other.values[i];
	}
	return *this;
}

 Matrix2 Matrix2::operator-(const Matrix2& other) {
	return Matrix2(
		values[0] - other.values[0], values[1] - other.values[1],
		values[2] - other.values[2], values[3] - other.values[3]);
}

 Matrix2 Matrix2::operator-=(const Matrix2& other) {
	for (int i = 0; i < 4; i++) {
		values[i] = values[i] - other.values[i];
	}
	return *this;
}

 Matrix2 Matrix2::operator*(const float& other){
	return Matrix2(
		values[0] * other, values[1] * other,
		values[2] * other, values[3] * other);
}

 Matrix2 Matrix2::operator*=(const float& other){
	for (int i = 0; i < 4; i++) {
		values[i] = values[i] * other;
	}
	return *this;
}

 Matrix2 Matrix2::operator/(const float& other){
	return Matrix2(
		values[0] / other, values[1] / other,
		values[2] / other, values[3] / other);
}

 Matrix2 Matrix2::operator/=(const float& other) {
	for (int i = 0; i < 4; i++) {
		values[i] = values[i] / other;
	}
	return *this;
}
 Matrix2 Matrix2::operator=(const Matrix2& other) {
	for (int i = 0; i < 4; i++) {
		values[i] = other.values[i];
	}
	return *this;
}

 Matrix2 Matrix2::operator*(const Matrix2& other) {
	return Matrix2(
		Vec2::dot(getRowVector(0), other.getColVector(0)), Vec2::dot(getRowVector(0), other.getColVector(1)),
		Vec2::dot(getRowVector(1), other.getColVector(0)), Vec2::dot(getRowVector(1), other.getColVector(1)));
}

 Matrix2 Matrix2::operator*=(const Matrix2& other) {
	for (int i = 0; i < 4; i++) {
		values[i] = Vec2::dot(getRowVector(i / 2), other.getColVector(i % 2));
	}
	return *this;
}

 std::string Matrix2::toString() {
	 return (
		 "{" + std::to_string(values[0]) + "," + std::to_string(values[1]) + "\n" +
		std::to_string(values[2]) + "," + std::to_string(values[3]) + "}");
 }

//------------------------------------Actual Maths----------------------------------------------\\

float Matrix2::minor(int index) {
	for (int i = 0; i < 4; i++) {
		if (i % 2 != index % 2 && i / 2 != index / 2) {
			return i;
		}
	}

	//Should never get here, will check with tests
	return 0.0f;
}

float Matrix2::det() {
	return values[0] * values[3] - values[1] * values[2];
}

Vec2 Matrix2::getRowVector(int row) const {
	return Vec2(values[0 + row * 2], values[1 + row * 2]);
}

Vec2 Matrix2::getColVector(int col) const {
	return Vec2(values[0 + col], values[2 + col]);
}
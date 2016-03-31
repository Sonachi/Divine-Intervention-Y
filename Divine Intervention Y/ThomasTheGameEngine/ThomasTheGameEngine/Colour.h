//Author:	Mathieu Violette
//Date:		12/01/2015

#pragma once

#include <string>
#include <SDL.h>

class Colour
{
public:
	Colour(float _r = 1, float _g = 1, float _b = 1, float _a = 1);
	~Colour();

	float r, g, b, a;

	/* Factories */
	static Colour Black();			static Colour Red();		static Colour Yellow();		static Colour Orange();			static Colour Lime();
	static Colour White();			static Colour Green();		static Colour Cyan();		static Colour Pink();			static Colour SpringGreen();
	static Colour Transparent();	static Colour Blue();		static Colour Magenta();	static Colour ManganeseBlue();	static Colour Fuchsia();
	static Colour Gray();

	Colour operator+(const Colour&) const;
	Colour operator+=(const Colour&);
	void operator=(const Colour&);
	Colour operator-(const Colour&) const;
	Colour operator-=(const Colour&);
	Colour operator*(const float&) const;
	Colour operator*=(const float&);
	Colour operator/(const float&) const;
	Colour operator/=(const float&);
	std::string toString();

	friend const Colour operator*(const float&, const Colour&);

	operator SDL_Color() const;

private:
	//Returns a value between 1 and 0
	float Limit(const float _value) const;
	void Normalize();
};
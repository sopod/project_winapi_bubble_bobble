#include "fPoint.h"
#include <cmath>


fPoint::fPoint() { }

fPoint::fPoint(float _x, float _y)
{
	x = _x;
	y = _y;
}

fPoint::~fPoint() { }


fPoint fPoint::Norm()
{
	float r = 1 / std::sqrt(x * x + y * y);
	return { r * x, r * y };
}

fPoint fPoint::Abs()
{
	return { std::abs(x), std::abs(y) };
}

fPoint fPoint::operator+(const fPoint & p) const
{
	return { x + p.x , y + p.y };
}

fPoint fPoint::operator+(float f) const
{
	return { x + f , y + f };
}

fPoint fPoint::operator-(const fPoint & p) const
{
	return { x - p.x , y - p.y };
}

fPoint fPoint::operator-(float f) const
{
	return { x - f , y - f };
}

fPoint fPoint::operator*(const fPoint & p) const
{
	return { x * p.x , y * p.y };
}

fPoint fPoint::operator*(float f) const
{
	return { f * x , f * y };
}

fPoint fPoint::operator/(const fPoint & p) const
{
	return { x / p.x , y / p.y };
}

fPoint fPoint::operator/(float f) const
{
	return { x / f, y / f };
}

const fPoint fPoint::operator+=(const fPoint & p)
{
	x += p.x;
	y += p.y;
	return *this;
}

const fPoint fPoint::operator+=(float f)
{
	x += f;
	y += f;
	return *this;
}

const fPoint fPoint::operator-=(const fPoint & p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

const fPoint fPoint::operator*=(const fPoint & p)
{
	x *= p.x;
	y *= p.y;
	return *this;
}

const fPoint fPoint::operator/=(const fPoint & p)
{
	x /= p.x;
	y /= p.y;
	return *this;
}

const bool fPoint::operator!=(const fPoint & p)
{
	if (x != p.x || y != p.y) return true;
	return false;

}

const bool fPoint::operator==(const fPoint & p)
{
	if (x == p.x && y == p.y) return true;
	return false;
}

#include "FPOINT.h"
#include <cmath>


FPOINT::FPOINT()
{
}

FPOINT::FPOINT(float _x, float _y)
{
	x = _x;
	y = _y;
}

FPOINT::~FPOINT()
{
}


FPOINT FPOINT::Norm()
{
	float r = 1 / std::sqrt(x * x + y * y);
	return { r * x, r * y };
}

FPOINT FPOINT::Abs()
{
	return { std::abs(x), std::abs(y) };
}

FPOINT FPOINT::operator+(const FPOINT & p) const
{
	return { x + p.x , y + p.y };
}

FPOINT FPOINT::operator+(float f) const
{
	return { x + f , y + f };
}

FPOINT FPOINT::operator-(const FPOINT & p) const
{
	return { x - p.x , y - p.y };
}

FPOINT FPOINT::operator-(float f) const
{
	return { x - f , y - f };
}

FPOINT FPOINT::operator*(const FPOINT & p) const
{
	return { x * p.x , y * p.y };
}

FPOINT FPOINT::operator*(float f) const
{
	return { f * x , f * y };
}

FPOINT FPOINT::operator/(const FPOINT & p) const
{
	return { x / p.x , y / p.y };
}

FPOINT FPOINT::operator/(float f) const
{
	return { x / f, y / f };
}

const FPOINT FPOINT::operator+=(const FPOINT & p)
{
	x += p.x;
	y += p.y;
	return *this;
}

const FPOINT FPOINT::operator+=(float f)
{
	x += f;
	y += f;
	return *this;
}

const FPOINT FPOINT::operator-=(const FPOINT & p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

const FPOINT FPOINT::operator*=(const FPOINT & p)
{
	x *= p.x;
	y *= p.y;
	return *this;
}

const FPOINT FPOINT::operator/=(const FPOINT & p)
{
	x /= p.x;
	y /= p.y;
	return *this;
}
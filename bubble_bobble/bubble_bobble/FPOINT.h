#pragma once

class fPoint
{
public:
	float x;
	float y;

public:
	fPoint();
	fPoint(float _x, float _y);
	~fPoint();

	fPoint Norm();
	fPoint Abs();

	fPoint operator+(const fPoint & p) const;

	fPoint operator+(float f) const;

	fPoint operator-(const fPoint & p) const;

	fPoint operator-(float f) const;

	friend fPoint operator-(float f, const fPoint & p)
	{
		return { f - p.x , f - p.y };
	}

	fPoint operator*(const fPoint & p) const;

	fPoint operator*(float f) const;

	friend fPoint operator*(float f, const fPoint & p)
	{
		return { f * p.x , f * p.y };
	}

	fPoint operator/(const fPoint & p) const;

	fPoint operator/(float f) const;

	friend fPoint operator/(float f, const fPoint & p)
	{
		return { f / p.x , f / p.y };
	}

	const fPoint operator+=(const fPoint & p);

	const fPoint operator+=(float f);

	const fPoint operator-=(const fPoint & p);

	const fPoint operator*=(const fPoint & p);

	const fPoint operator/=(const fPoint & p);

	const bool operator!=(const fPoint & p);

	const bool operator==(const fPoint & p);

};
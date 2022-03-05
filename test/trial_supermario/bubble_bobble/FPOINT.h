#pragma once

class FPOINT
{
public:
	float x;
	float y;

public:
	FPOINT();
	FPOINT(float _x, float _y);
	~FPOINT();

	FPOINT Norm();
	FPOINT Abs();

	FPOINT operator+(const FPOINT & p) const;

	FPOINT operator+(float f) const;

	FPOINT operator-(const FPOINT & p) const;

	FPOINT operator-(float f) const;

	friend FPOINT operator-(float f, const FPOINT & p)
	{
		return { f - p.x , f - p.y };
	}

	FPOINT operator*(const FPOINT & p) const;

	FPOINT operator*(float f) const;

	friend FPOINT operator*(float f, const FPOINT & p)
	{
		return { f * p.x , f * p.y };
	}

	FPOINT operator/(const FPOINT & p) const;

	FPOINT operator/(float f) const;

	friend FPOINT operator/(float f, const FPOINT & p)
	{
		return { f / p.x , f / p.y };
	}

	const FPOINT operator+=(const FPOINT & p);

	const FPOINT operator+=(float f);

	const FPOINT operator-=(const FPOINT & p);

	const FPOINT operator*=(const FPOINT & p);

	const FPOINT operator/=(const FPOINT & p);

};
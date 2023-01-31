#pragma once
#include <iostream>

class SVector2D
{
public:
	float x;
	float y;

	SVector2D();
	SVector2D(float x, float y);

	SVector2D& Add(const SVector2D& vec);
	SVector2D& Subtract(const SVector2D& vec);
	SVector2D& Multiply(const SVector2D& vec);
	SVector2D& Divide(const SVector2D& vec);

	friend SVector2D& operator+(SVector2D& v1, const SVector2D& v2);
	friend SVector2D& operator-(SVector2D& v1, const SVector2D& v2);
	friend SVector2D& operator*(SVector2D& v1, const SVector2D& v2);
	friend SVector2D& operator/(SVector2D& v1, const SVector2D& v2);

	SVector2D& operator+=(const SVector2D& vec);
	SVector2D& operator-=(const SVector2D& vec);
	SVector2D& operator*=(const SVector2D& vec);
	SVector2D& operator/=(const SVector2D& vec);

	SVector2D& operator*(const int& i);
	SVector2D& Zero();

	friend std::ostream& operator<<(std::ostream& stream, const SVector2D& vec);
};

using SPoint = SVector2D;

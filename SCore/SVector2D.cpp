#include "SVector2D.h"

SVector2D::SVector2D()
{
	x = 0.0f;
	y = 0.0f;
}

SVector2D::SVector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

SVector2D& SVector2D::Add(const SVector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

SVector2D& SVector2D::Subtract(const SVector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

SVector2D& SVector2D::Multiply(const SVector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

SVector2D& SVector2D::Divide(const SVector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

SVector2D& operator+(SVector2D& v1, const SVector2D& v2)
{
	return v1.Add(v2);
}

SVector2D& operator-(SVector2D& v1, const SVector2D& v2)
{
	return v1.Subtract(v2);
}

SVector2D& operator*(SVector2D& v1, const SVector2D& v2)
{
	return v1.Multiply(v2);
}

SVector2D& operator/(SVector2D& v1, const SVector2D& v2)
{
	return v1.Divide(v2);
}

SVector2D& SVector2D::operator+=(const SVector2D& vec)
{
	return this->Add(vec);
}

SVector2D& SVector2D::operator-=(const SVector2D& vec)
{
	return this->Subtract(vec);
}

SVector2D& SVector2D::operator*=(const SVector2D& vec)
{
	return this->Multiply(vec);
}

SVector2D& SVector2D::operator/=(const SVector2D& vec)
{
	return this->Divide(vec);
}

SVector2D& SVector2D::operator*(const int& i)
{
	this->x *= i;
	this->y *= i;

	return *this;
}

SVector2D& SVector2D::Zero()
{
	this->x = 0;
	this->y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const SVector2D& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}
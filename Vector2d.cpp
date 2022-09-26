#include <math.h>
#include "Vector2d.hpp"

Vector2d Vector2d::operator+(const Vector2d &other)
{
    return Vector2d(x + other.x, y + other.y);
}

Vector2d Vector2d::operator-()
{
    return Vector2d(-x, -y);
}

Vector2d Vector2d::operator-(const Vector2d &other)
{
    return Vector2d(x - other.x, y - other.y);
}

Vector2d Vector2d::operator*(double multiplier)
{
    return Vector2d(x * multiplier, y * multiplier);
}

Vector2d Vector2d::operator/(double divider)
{
    return Vector2d(x / divider, y / divider);
}

Vector2d &Vector2d::operator+=(const Vector2d &other)
{
    x += other.x;
    y += other.y;

    return *this;
}

Vector2d &Vector2d::operator-=(const Vector2d &other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

Vector2d &Vector2d::operator*=(double multiplier)
{
    x *= multiplier;
    y *= multiplier;

    return *this;
}

Vector2d &Vector2d::operator/=(double divider)
{
    x /= divider;
    y /= divider;

    return *this;
}

double Vector2d::length()
{
    return sqrt(x * x + y * y);
}

void Vector2d::normalize()
{
    if (x == 0 && y == 0)
        return;

    double len = length();
    x /= len;
    y /= len;
}

sf::Vector2f Vector2d::toSFMLVector()
{
    return sf::Vector2f(x, y);
}
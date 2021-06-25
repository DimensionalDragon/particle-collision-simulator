#include<iostream>
#include<cmath>

#include "Util/Vector2f.hpp"
#include "Util/Matrix2x2f.hpp"
#include "Util/Functions.hpp"

//Vector
Vector2f::Vector2f()
{
    x = 0.0f; y = 0.0f;
}

Vector2f::Vector2f(float pX, float pY)
{
    x = pX; y = pY;
}

void Vector2f::operator=(Vector2f vector)
{
    x = vector.x;
    y = vector.y;
}

Vector2f Vector2f::operator+(Vector2f other)
{
    return Vector2f((x + other.x), (y + other.y));
}

Vector2f Vector2f::operator-(Vector2f other)
{
    return Vector2f((x - other.x), (y - other.y));
}

Vector2f Vector2f::operator*(float scalar)
{
    return Vector2f((x * scalar), (y * scalar));
}

float Vector2f::dotProduct(Vector2f other)
{
    return (x * other.x) + (y * other.y);
}

float Vector2f::getSquaredMagnitude()
{
    return (square(x) + square(y));
}

void Vector2f::print()
{
    std::cout << "{" << x << ", " << y << "}" << std::endl;
}

//Matrix
Matrix2x2f::Matrix2x2f()
{
    a = 0.0f; b = 0.0f; c = 0.0f; d = 0.0f;
}

Matrix2x2f::Matrix2x2f(float pA, float pB, float pC, float pD)
{
    a = pA; b = pB; c = pC; d = pD;
}

void Matrix2x2f::operator=(Matrix2x2f matrix)
{
    a = matrix.a;
    b = matrix.b;
    c = matrix.c;
    d = matrix.d;
}

Vector2f Matrix2x2f::operator*(Vector2f vector)
{
    return Vector2f(
        (a * vector.x + b * vector.y),
        (c * vector.x + d * vector.y));
}

Matrix2x2f Matrix2x2f::operator*(float scalar)
{
    return Matrix2x2f((a * scalar), (b * scalar), (c * scalar), (d * scalar));
}

void Matrix2x2f::print()
{
    std::cout << "{" << a << " " << b << "}" << std::endl;
    std::cout << "{" << c << " " << d << "}" << std::endl;
}

//Functions
float square(float x)
{
    return x * x;
}
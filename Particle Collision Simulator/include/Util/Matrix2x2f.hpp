#pragma once

#include "Util/Vector2f.hpp"

class Matrix2x2f
{
    public:
        Matrix2x2f();
        Matrix2x2f(float pA, float pB, float pC, float pD);
        void operator=(Matrix2x2f matrix);
        Vector2f operator*(Vector2f vector);
        Matrix2x2f operator*(float scalar);
        void print();
        
        float a, b, c, d;
};
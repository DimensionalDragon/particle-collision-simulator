#include<iostream>

#include "Border.hpp"

Border::Border(Vector2f upperRightCornerPos, float width, float height)
: position(upperRightCornerPos), borderWidth(width), borderHeight(height)
{}

Vector2f Border::getPosition()
{
    return position;
}

float Border::getWidth()
{
    return borderWidth;
}

float Border::getHeight()
{
    return borderHeight;
}
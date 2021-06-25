#pragma once

#include "Util/Vector2f.hpp"
#include "Entity.hpp"

class Border
{
    public:
        Border(Vector2f upperRightCornerPos, float width, float height);
        Vector2f getPosition();
        float getWidth();
        float getHeight();
    private:
        Vector2f position;
        float borderWidth, borderHeight;
};
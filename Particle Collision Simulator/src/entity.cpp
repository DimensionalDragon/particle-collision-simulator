#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>

#include "Entity.hpp"
#include "Util/Vector2f.hpp"

Entity::Entity(Vector2f pos, float pWidth, float pHeight, SDL_Texture* tex)
    :position(pos), texture(tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 1200;
    currentFrame.h = 1200;
    width = pWidth;
    height = pHeight;
}

Vector2f& Entity::getPosition()
{
    return position;
}

SDL_Texture* Entity::getTexture()
{
    return texture;
}

SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}

float Entity::getWidth()
{
    return width;
}

float Entity::getHeight()
{
    return height;
}

#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include "Util/Vector2f.hpp"

class Entity
{
    public:
        Entity(Vector2f pos, float pWidth, float pHeight, SDL_Texture* tex);
        Vector2f& getPosition();
        SDL_Texture* getTexture();
        SDL_Rect getCurrentFrame();
        float getWidth();
        float getHeight();
    protected:
        Vector2f position;
        SDL_Rect currentFrame;
        SDL_Texture* texture;
        float width, height;
};
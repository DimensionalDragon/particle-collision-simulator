#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Particle.hpp"

RenderWindow::RenderWindow(const char* title, int width, int height)
    :window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if(window == NULL) std::cout << "[ERROR] NULL Window: " << SDL_GetError() << std::endl;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    windowHeight = height;
    windowWidth = width; 
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    if(texture == NULL) std::cout << "[ERROR] Texture Load Failed: " << SDL_GetError() << std::endl;
    return texture;
}

int RenderWindow::getRefreshRate()
{
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode);
    return mode.refresh_rate;
}

int RenderWindow::getWidth()
{
    return windowWidth;
}

int RenderWindow::getHeight()
{
    return windowHeight;
}

void RenderWindow::clean()
{
    SDL_DestroyWindow(window);
}

void RenderWindow::clearScreen()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& entity)
{
    SDL_Rect src;
    src.x = entity.getCurrentFrame().x;
    src.y = entity.getCurrentFrame().y;
    src.w = entity.getCurrentFrame().w;
    src.h = entity.getCurrentFrame().h;

    SDL_Rect dest;
    dest.x = entity.getPosition().x;
    dest.y = entity.getPosition().y;
    dest.w = entity.getWidth();
    dest.h = entity.getHeight();

    SDL_RenderCopy(renderer, entity.getTexture(), &src, &dest);
}

void RenderWindow::render(Particle& particle)
{
    SDL_Rect src;
    src.x = particle.getCurrentFrame().x;
    src.y = particle.getCurrentFrame().y;
    src.w = particle.getCurrentFrame().w;
    src.h = particle.getCurrentFrame().h;

    SDL_Rect dest;
    dest.x = particle.getPosition().x;
    dest.y = particle.getPosition().y;
    dest.w = particle.getDiameter();
    dest.h = particle.getDiameter();

    SDL_RenderCopy(renderer, particle.getTexture(), &src, &dest);
}

void RenderWindow::render(Border& border)
{
    SDL_Rect box;
    box.x = border.getPosition().x;
    box.y = border.getPosition().y;
    box.w = border.getWidth();
    box.h = border.getHeight();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &box);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}
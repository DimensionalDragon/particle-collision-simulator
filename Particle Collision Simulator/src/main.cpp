#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<utility>
#include<algorithm>
#include<vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Particle.hpp"
#include "Border.hpp"
#include "Util/Functions.hpp"
#include "Util/Vector2f.hpp"
#include "Util/Matrix2x2f.hpp"
#include "Util/Timestep.hpp"

#define RANDOM_ZERO_TO_ONE 0.5 * (SDL_sinf(rand()) + 1)
#define NUMBER_OF_PARTICLES 250

int main(int argc, char *argv[])
{
    //SDL Init
    if(SDL_Init(SDL_INIT_EVERYTHING) > 0) std::cout << "[ERROR] Failed Init: " << SDL_GetError() << std::endl;
    if(!IMG_Init(IMG_INIT_PNG)) std::cout << "[ERROR] Failed Image Init: " << SDL_GetError() << std::endl;
    
    //Window and loading textures
    RenderWindow window("Particle Collision Sim v1.0", 1280, 720);

    std::vector<SDL_Texture*> particleTextures;

    Border border(Vector2f(60, 60), 600, 600);

    particleTextures.push_back(window.loadTexture("res/images/circle_yellow.png"));
    particleTextures.push_back(window.loadTexture("res/images/circle_blue.png"));
    particleTextures.push_back(window.loadTexture("res/images/circle_red.png"));
    particleTextures.push_back(window.loadTexture("res/images/circle_green.png"));

    SDL_Texture* resetTexture = window.loadTexture("res/images/reset_particles.png");
    Entity resetText(Vector2f(720, 100), 500, 500, resetTexture);

    //Particles container
    std::vector<Particle> particles;

    //Regions container
    std::vector<std::pair<Vector2f, Vector2f> > regions;

    //Scope to give lifetime to the position constant variables
    {
        //Important positions
        Vector2f origin(border.getPosition());
        const float borderWidth = border.getWidth();
        const float borderHeight = border.getHeight();
        const float halfWidth = border.getWidth() / 2;
        const float halfHeight = border.getHeight() / 2;

        //List of regions
        regions.push_back(std::make_pair(origin, Vector2f(origin.x + halfWidth, origin.y + halfHeight)));
        regions.push_back(std::make_pair(Vector2f(origin.x + halfWidth, origin.y), Vector2f(origin.x + borderWidth, origin.y + halfHeight)));
        regions.push_back(std::make_pair(Vector2f(origin.x, origin.y + halfHeight), Vector2f(origin.x + halfWidth, origin.y + borderHeight)));
        regions.push_back(std::make_pair(Vector2f(origin.x + halfWidth, origin.y + halfHeight), Vector2f(origin.x + borderWidth, origin.y + borderHeight)));
    }

resetParticle:
    //Particle generator
    for(int i = 0; i < NUMBER_OF_PARTICLES; i++)
    {
        float mass = 1.5 * RANDOM_ZERO_TO_ONE + 1.5; //random value from 1.5 to 3
        
        Particle particle(Vector2f(0, 0), Vector2f(2 * SDL_sinf(rand()), 1.5 * SDL_sinf(rand())), Vector2f(0, 0), particleTextures[i % 4], mass); //create particle
        particle.getPosition().x = (border.getWidth() - particle.getDiameter()) * RANDOM_ZERO_TO_ONE + border.getPosition().x; //update with random x-value from border width start to border width end
        particle.getPosition().y = (border.getHeight() - particle.getDiameter()) * RANDOM_ZERO_TO_ONE + border.getPosition().y;  //update with random y-value from border height start to border height end
        particle.updateCenter();

        for(std::vector<Particle>::size_type j = 0; j < particles.size(); j++)
        {   
            //check if the particle spawned on top of other particle
            if((particle.getCenter() - particles[j].getCenter()).getSquaredMagnitude() < square(particle.getRadius() + particles[j].getRadius()))
            {
                particle.getPosition().x = (border.getWidth() - particle.getDiameter()) * RANDOM_ZERO_TO_ONE + border.getPosition().x;
                particle.getPosition().y = (border.getHeight() - particle.getDiameter()) * RANDOM_ZERO_TO_ONE + border.getPosition().y;
                particle.updateCenter();
                j = -1;
            }
        }

        particles.push_back(particle);
    }

    //Program starts
    bool isRunning = true;

    SDL_Event event;

    //Program loop
    while(isRunning)
    {
        int startTick = SDL_GetTicks();

        //Event handler
        while(SDL_PollEvent(&event))
        {
            const Uint8* key = SDL_GetKeyboardState(NULL);
            if(event.type == SDL_QUIT) isRunning = false;
            else if(event.type == SDL_KEYDOWN)
            {
                if(key[SDL_SCANCODE_SPACE])
                {
                    particles.clear();
                    goto resetParticle;
                }
            }
        }

        window.clearScreen();

        //Update for every particle and render all objects
        for(std::vector<Particle>::size_type i = 0; i < particles.size(); i++)
        {
            std::vector<Particle*> others;
            for(std::vector<Particle>::size_type j = i + 1; j < particles.size(); j++)
            {
                //Only check for collision if 2 particles are in the same region (currently there are 4 hardcoded regions)
                for(int k = 0; k < 4; k++)
                {
                    if(particles[i].isInsideRectangle(regions[k]) && particles[j].isInsideRectangle(regions[k]))
                    {
                        others.push_back(&particles[j]);
                        break;
                    }
                }
            }
            particles[i].update(others);
            window.render(particles[i]);
        }
        window.render(resetText);
        window.render(border);

        window.display();

        //Timestep management
        int frameTicks = SDL_GetTicks() - startTick;
        if(frameTicks < 1000 / window.getRefreshRate())
            SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
    }

    window.clean();
    SDL_Quit();

    return 0;
}

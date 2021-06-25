#include<iostream>
#include<vector>
#include<utility>

#include "Particle.hpp"
#include "Util/Vector2f.hpp"
#include "Util/Matrix2x2f.hpp"
#include "Util/Functions.hpp"

#define SCALING_FACTOR 250

Particle::Particle(Vector2f pPos, Vector2f pSpeed, Vector2f pAccel, SDL_Texture* pTexture, float pMass)
    :Entity(pPos, 1200, 1200, pTexture)
{
    speed = pSpeed;
    acceleration = pAccel;
    mass = pMass;
    diameter = currentFrame.w * 2 * mass / SCALING_FACTOR;
    radius = diameter / 2;
    position = position - Vector2f(radius, radius);
    center = position + Vector2f(radius, radius);
}

Vector2f Particle::getSpeed()
{
    return speed;
}

Vector2f Particle::getCenter()
{
    return center;
}

float Particle::getDiameter()
{
    return diameter;
}

float Particle::getRadius()
{
    return radius;
}

bool Particle::isInsideRectangle(const std::pair<Vector2f, Vector2f>& rectangle)
{
    Vector2f topLeft = position;
    Vector2f topRight = position + Vector2f(diameter, 0);
    Vector2f botLeft = position + Vector2f(0, diameter);
    Vector2f botRight = position + Vector2f(diameter, diameter);
    return
    (topLeft.x > rectangle.first.x || topLeft.y > rectangle.first.y) ||
    (topLeft.x - diameter < rectangle.second.x || topLeft.y < rectangle.second.y) ||
    (topRight.x > rectangle.first.x || topRight.y > rectangle.first.y) ||
    (topRight.x - diameter < rectangle.second.x || topRight.y < rectangle.second.y) ||
    (botLeft.x > rectangle.first.x || botLeft.y > rectangle.first.y) ||
    (botLeft.x - diameter < rectangle.second.x || botLeft.y < rectangle.second.y) ||
    (botRight.x > rectangle.first.x || botRight.y > rectangle.first.y) ||
    (botRight.x - diameter < rectangle.second.x || botRight.y < rectangle.second.y);
}

void Particle::handleWallCollision()
{
    if(position.x <= 60 || position.x >= 720 - 60 - diameter) 
    {
        position.x = (position.x <= 60) ? 60 : 720 - 60 - diameter;
        speed.x *= -1;
    }
    if(position.y <= 60 || position.y >= 720 - 60 - diameter)
    {
        position.y = (position.y <= 60) ? 60 : 720 - 60 - diameter;
        speed.y *= -1;
    }
}

void Particle::handleParticleCollision(Particle& other)
{
    float squaredDistance = square(center.x - other.center.x) + square(center.y - other.center.y);
    if(squaredDistance <= square(radius + other.radius))
    {
        //Differences
        Vector2f speedDifference = speed - other.speed;
        Vector2f positionDifference = other.position - position;

        //Prevent a double update (a condition where the particles still overlaps after an update)
        if(speedDifference.dotProduct(positionDifference) >= 0)
        {
            //Angle calculation
            const float angle = -SDL_atan2f((other.center.y - center.y), (other.center.x - center.x));

            //Rotation matrix (and the inverse)
            Matrix2x2f rotation = Matrix2x2f(SDL_cosf(angle), -SDL_sinf(angle),
                                             SDL_sinf(angle),  SDL_cosf(angle)); //rotation matrix
            Matrix2x2f inverseRotation = Matrix2x2f(SDL_cosf(-angle), -SDL_sinf(-angle),
                                                    SDL_sinf(-angle),  SDL_cosf(-angle)); //rotation matrix but uses (-angle) as argument to inverse it

            //Masses
            float m1 = mass, m2 = other.mass;

            //Rotated speeds
            Vector2f u1 = rotation * speed;
            Vector2f u2 = rotation * other.speed;

            //Calculate 1d collision
            Vector2f v1 = u1, v2 = u2;
            v1.x = ((u1.x * (m1 - m2)) + (2 * u2.x * m2)) / (m1 + m2); //by 1D collision formula
            v2.x = ((u2.x * (m2 - m1)) + (2 * u1.x * m1)) / (m1 + m2);

            //Inverse rotate the speeds
            Vector2f v1Final = inverseRotation * v1;
            Vector2f v2Final = inverseRotation * v2;

            //Apply change of speed
            speed = v1Final;
            other.speed = v2Final;
        }
    }
}

void Particle::updateCenter()
{
    center = position + Vector2f(radius, radius);
}

void Particle::update(std::vector<Particle*> others)
{
    //Handle collisions
    handleWallCollision();
    for(Particle* p : others)
    {
        handleParticleCollision(*p);
    }

    //Update speed and position
    speed.x += speed.x >= 0 ? acceleration.x : -1 * acceleration.x;
    speed.y += speed.y >= 0 ? acceleration.y : -1 * acceleration.y;
    position.x += speed.x;
    position.y += speed.y;
    updateCenter();
}
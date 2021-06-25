#pragma once
#include<vector>
#include<utility>

#include "Entity.hpp"
#include "Util/Vector2f.hpp"
#include "Util/Matrix2x2f.hpp"

class Particle : public Entity
{
    public:
        Particle(Vector2f pPos, Vector2f pSpeed, Vector2f pAccel, SDL_Texture* pTexture, float pMass);
        Vector2f getSpeed();
        Vector2f getCenter();
        float getDiameter();
        float getRadius();
        bool isInsideRectangle(const std::pair<Vector2f, Vector2f>& rectangle);
        void handleWallCollision();
        void handleParticleCollision(Particle& other);
        void updateCenter();
        void update(std::vector<Particle*> others);
    private:
        float mass, diameter, radius;
        Vector2f center, speed, acceleration;
};
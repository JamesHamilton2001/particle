#include "ParticleLife.h"

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>


ParticleLife::ParticleLife(int count, int size, int innerRange, int outerRange, float resistance)
{
    this->count = count;
    this->innerRange = innerRange;
    this->outerRange = outerRange;
    this->diameter = 2.0f * outerRange;
    this->resistance = resistance;
    this->bounds = Vector2({(float) (size*outerRange), (float) (size*outerRange)});
    this->particles = new Particle[count];

    for (int i = 0; i < 3; i++)
        rangeColours[i].a = 0;
    
    attractions[0][0] = 0.2f; attractions[0][1] = 0.5f; attractions[0][2] = 0.7f;
    attractions[1][0] = 0.7f; attractions[1][1] = 0.6f; attractions[1][2] = 0.5f;
    attractions[2][0] = 0.5f; attractions[2][1] = 0.2f; attractions[2][2] = 0.6f;

    for (int i = 0; i < count; i++) {
        float angle = (float) (GetRandomValue(0, 360));
        particles[i].type = (i + 3) % 3;
        particles[i].pos.x = GetRandomValue((int)(-bounds.x), (int)(bounds.x));
        particles[i].pos.y = GetRandomValue((int)(-bounds.y), (int)(bounds.y));
        particles[i].vel = { -cos(angle), sin(angle) };
    }
}

ParticleLife::~ParticleLife()
{
    delete [] particles;
}

void ParticleLife::update()
{
    for (int i = 0; i < count; i++) {
        Particle& particle = particles[i];

        for (int j = 0; j < count; j++) {
            if (i == j) continue;
            Particle& other = particles[j];

            float xDist = particle.pos.x - other.pos.x;
            float yDist = particle.pos.y - other.pos.y;
            float distance = sqrtf(xDist*xDist + yDist*yDist);

            if (distance <= innerRange) {
                float xNorm = (other.pos.x - particle.pos.x) / distance;
                float yNorm = (other.pos.y - particle.pos.y) / distance;
                float reactionCoef = distance / outerRange / innerRange - 1.0f;
                float xForce = xNorm * reactionCoef;
                float yForce = yNorm * reactionCoef;
                particle.vel.x += xForce;
                particle.vel.y += yForce;
                other.vel.x -= xForce;
                other.vel.y -= yForce;
            }
            else if (distance <= outerRange) {
                float xNorm = (other.pos.x - particle.pos.x) / distance;
                float yNorm = (other.pos.y - particle.pos.y) / distance;
                float reactionCoef = attractions[particle.type][other.type] * (1.0f - abs(2*distance/outerRange - 1.0f - innerRange) / 1.0f - innerRange);
                float xForce = xNorm * reactionCoef;
                float yForce = yNorm * reactionCoef;
                particle.vel.x += xForce;
                particle.vel.y += yForce;
                other.vel.x -= xForce;
                other.vel.y -= yForce;
            }
        }

        if (abs(particle.pos.x) > bounds.x)
            particle.pos.x = (particle.pos.x > 0) ? -bounds.x : bounds.x;
        if (abs(particle.pos.y) > bounds.y)
            particle.pos.y = (particle.pos.y > 0) ? -bounds.y : bounds.y;
    

        particle.vel.x *= (1.0f - resistance);
        particle.vel.y *= (1.0f - resistance);

        particle.pos = Vector2Add(particle.pos, particle.vel);
    }
}

void ParticleLife::draw()
{
    for (int i = 0; i < count; i++) {
        Particle& particle = particles[i];

        rlBegin(RL_TRIANGLES);
            Color colour = colours[particle.type];
            int segSize = 36;
            for (int i = 0; i < 360; i+= segSize) {
                rlColor4ub(colour.r, colour.g, colour.b, 255);
                rlVertex2f(particle.pos.x, particle.pos.y);
                rlColor4ub(colour.r, colour.g, colour.b, 0);
                rlVertex2f(particle.pos.x + sinf(DEG2RAD*i)*outerRange, particle.pos.y + cosf(DEG2RAD*i)*outerRange);
                rlColor4ub(colour.r, colour.g, colour.b, 0);
                rlVertex2f(particle.pos.x + sinf(DEG2RAD*(i + segSize))*outerRange, particle.pos.y + cosf(DEG2RAD*(i + segSize))*outerRange);
            }
        rlEnd();
    }
}
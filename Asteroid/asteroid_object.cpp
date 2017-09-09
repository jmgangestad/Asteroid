#include "asteroid_object.h"

AsteroidObject::AsteroidObject() : GameObject(), Radius(12.5f), Destroyed(false) {}

AsteroidObject::AsteroidObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, velocity), Radius(radius), Destroyed(false)
{}
#ifndef ASTEROIDOBJECT_h
#define ASTEROIDOBJECT_h

#define GLEWSTATIC
#include <glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "game_object.h"

class AsteroidObject : public GameObject {
public:
	GLfloat Radius;
	bool Destroyed;

	AsteroidObject();
	AsteroidObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	~AsteroidObject() {}
};

#endif
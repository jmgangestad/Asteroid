
#include <tuple>
#include <ctime>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "asteroid_object.h"
#include <cstdlib>
#include <irrklang/irrKlang.h>
#include <vector>

using namespace irrklang;

SpriteRenderer *Renderer;
GameObject *Ship;


int num_asteroids = 5;
//GameObject *Enemy;
//GameObject *Obstacle;
//ParticleGenerator *Particles;

//const glm::vec2 INITIAL_VELOCITY(100.0f, -350.0f);

//const GLfloat BALL_RADIUS = 12.5f;

//typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

ISoundEngine *SoundEngine = createIrrKlangDevice();

/*Direction VectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};

	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++) {
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}
*/

/*
GLboolean CheckCollision(GameObject &one, GameObject &two) {
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
	return collisionX && collisionY;
}
*/

enum {
	a, b, x, y, lb, rb, back, start, lclick, rclick, Dup, Dright, Ddown, Dleft
};

enum {
	left_joy_x, left_joy_y, right_joy_x, right_joy_y, L_trigger, R_trigger
};

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height){

}

Game::~Game() {
	delete Renderer;
	delete Ship;
  //delete Particles;
}

void Game::Init() {
	srand(time(NULL));
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", 0, "sprite");
	ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.frag", 0, "particle");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/ship.png", GL_TRUE, "ship");
	ResourceManager::LoadTexture("textures/asteroid.jpg", GL_TRUE, "asteroid");
	ResourceManager::LoadTexture("textures/shot.png", GL_TRUE, "shot");



	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	

	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height/2 - PLAYER_SIZE.y);
	Ship = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("ship"));
	

	for (int i = 0; i < num_asteroids; i++)
	{
		AsteroidObject obj(glm::vec2(rand() % this->Width, rand() % this->Height), 15 + rand() % 40, glm::vec2(-50.0f + rand() % 100, -50.0f + rand() % 100), ResourceManager::GetTexture("asteroid"));
		obj.AngularRate = -50.0f + rand() % 100;
		this->Asteroids.push_back(obj);
	}

	
	//SoundEngine->play2D("audio/breakout.mp3", GL_TRUE);
}

void Game::Update(GLfloat dt) {

	Ship->Move(dt, this->Width, this->Height);
	
	for (AsteroidObject &tile : this->Asteroids)
	{
		tile.Move(dt, this->Width, this->Height);
	}

	for (GameObject &tile : this->Shots)
	{
		tile.Move(dt, this->Width, this->Height);
		
		if (tile.ScreenPasses > 2)
		{
			this->Shots.erase(tile.);
		}
	}
	
	//this->DoCollisions();

	//Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	
}

void Game::Render() {
	if (this->State == GAME_ACTIVE) {
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		Ship->Draw(*Renderer, this->Width, this->Height);




		for (AsteroidObject &tile : this->Asteroids)
		{
			tile.Draw(*Renderer, this->Width, this->Height);
		}

		for (GameObject &tile : this->Shots)
		{
			tile.Draw(*Renderer, this->Width, this->Height);
		}

		//Particles->Draw();
	}
}

void Game::ProcessInput(GLfloat dt, GLFWwindow *window) {
	int count;
	const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	const unsigned char *button = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

	
	if (this->State == GAME_ACTIVE) {
		GLfloat velocity = PLAYER_VELOCITY * dt;

		if (this->State == GAME_ACTIVE)
		{
			GLfloat velocity = PLAYER_VELOCITY * dt;



			if (button[back]) {
				glfwSetWindowShouldClose(window, GL_TRUE);
			}


			if (this->Keys[GLFW_KEY_A] || button[Dleft]) {
				/*if (Ship->Position.x >= 0) {
					Ship->Position.x -= velocity;
				}*/

				Ship->Rotation -= 1.2f;
			}
			if (this->Keys[GLFW_KEY_D] || button[Dright]) {
				/*if (Ship->Position.x <= this->Width - Ship->Size.x) {
					Ship->Position.x += velocity;
				}*/
				Ship->Rotation += 1.2f;
			}

			if (this->Keys[GLFW_KEY_W] || button[Dup]) {
				Ship->Velocity += glm::vec2(1.2*sin(Ship->Rotation*3.11415 / 180.0), -1.2*cos(Ship->Rotation*3.11415 / 180.0));
			}

			if (this->Keys[GLFW_KEY_S] || button[Ddown]) {
				Ship->Velocity -= glm::vec2(1.2*sin(Ship->Rotation*3.11415 / 180.0), -1.2*cos(Ship->Rotation*3.11415 / 180.0));
			}

			//a, b, x, y, lb, rb, back, start, lclick, rclick, Dup, Dright, Ddown, Dleft

			if (button[a] && !last_button_a && Shots.size() < 5) {

				glm::vec2 new_shot_pos = glm::vec2(Ship->Position.x + 0.5*Ship->Size.x + sin(Ship->Rotation*3.11415 / 180.0)*Ship->Size.y / 2.0, Ship->Position.y + 0.5*Ship->Size.y + 1.0 - cos(Ship->Rotation*3.11415 / 180.0)*Ship->Size.y / 2.0);

				float vel = 700.0f;
				glm::vec2 shot_vel = glm::vec2(vel * sin(Ship->Rotation*3.1415f/180.0f), -vel * cos(Ship->Rotation*3.1415/180.0f));
				shot_vel += Ship->Velocity;

				GameObject obj(new_shot_pos, glm::vec2(5.0f, 20.0f), ResourceManager::GetTexture("shot"), shot_vel);
				obj.Rotation = Ship->Rotation;
				this->Shots.push_back(obj);
				last_button_a = true;
			}
			else if(!button[a])
			{
				last_button_a = false;
			}


			if (axes[left_joy_x] != 0 ) {
				Ship->Rotation += 1.2 * axes[left_joy_x];
			}
			
			
			if (axes[left_joy_y] != 0) {
				Ship->Velocity += glm::vec2(1.2*axes[left_joy_y]*sin(Ship->Rotation*3.11415/180.0), -1.2*axes[left_joy_y]*cos(Ship->Rotation*3.11415 / 180.0));
			}

		}
		

		/*
		if (button[lb] && level_switch_down) {
		this->Level--;
		if (this->Level < 0) {
		this->Level = 3;
		}
		level_switch_down = false;
		}
		else if (!button[lb]) {
		level_switch_down = true;
		}
		*/
	}
}


void Game::DoCollisions() {
	
}


void Game::ResetPlayer() {
	
}
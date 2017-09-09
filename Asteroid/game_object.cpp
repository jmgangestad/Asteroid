#include "game_object.h"


GameObject::GameObject()
	:Position(0, 0), Size(1, 1), Velocity(0.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false), AngularRate(0.0f), ScreenPasses(0) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec2 velocity)
	: Position(pos), Size(size), Velocity(velocity), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false), AngularRate(0.0f), ScreenPasses(0) {}

void GameObject::Draw(SpriteRenderer &renderer, GLuint window_width, GLuint window_height) {
	
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation);
	
	if (this->Position.x + this->Size.x >= window_width)
	{
		renderer.DrawSprite(this->Sprite, glm::vec2(this->Position.x - window_width, this->Position.y), this->Size, this->Rotation);
	}

	if (this->Position.y + this->Size.y >= window_height)
	{
		renderer.DrawSprite(this->Sprite, glm::vec2(this->Position.x, this->Position.y - window_height), this->Size, this->Rotation);
	}

	if (this->Position.x + this->Size.x >= window_width && this->Position.y + this->Size.y >= window_height)
	{
		renderer.DrawSprite(this->Sprite, glm::vec2(this->Position.x - window_width, this->Position.y - window_height), this->Size, this->Rotation);
	}

//void SpriteRenderer::DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color) {


	
	/*
	glm::mat4 mat;
	mat = glm::translate(mat, glm::vec3(0.5f*this->Size.x, 0.5f*this->Size.y, 0.0f));
	//mat = glm::rotate(mat, this->Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	//mat = glm::translate(mat, glm::vec3(-0.5f*this->Size.x, -0.5f*this->Size.y, 0.0f));

	glm::vec4 new_pos_temp = mat*glm::vec4(this->Position.x, this->Position.y, 0.0f, 0.0f);

	glm::vec2 new_pos = glm::vec2(new_pos_temp.x, new_pos_temp.y);

	renderer.DrawSprite(this->Sprite, new_pos, this->Size, 0.0f);
	*/


}

//glm::vec2 Move(GLfloat dt, GLuint window_width, GLuint window_height);

glm::vec2 GameObject::Move(GLfloat dt, GLuint window_width, GLuint window_height)
{
	this->Position += this->Velocity * dt;
	this->Rotation += this->AngularRate * dt;

	if (this->Position.x < 0.0f)
	{
		this->Position.x = window_width;
		ScreenPasses++;
	}
	else if (this->Position.x > window_width)
	{
		this->Position.x = 0.0f;
		ScreenPasses++;
	}

	if (this->Position.y < 0.0f)
	{
		this->Position.y = window_height;
		ScreenPasses++;
	}
	else if (this->Position.y > window_height)
	{
		this->Position.y = 0.0f;
		ScreenPasses++;
	}

	return this->Position;
}
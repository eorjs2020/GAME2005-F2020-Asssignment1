#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/Circle.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(26.5f, 600.0f - 58 / 2);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
	
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	if (m_bThrow)
	{
		if (getRigidBody()->isColliding)
		{
			getTransform()->position = glm::vec2(26.5f, 600.0f - 58 / 2);
			getRigidBody()->isColliding = false;
			getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
			m_bThrow = false;
		}
		m_move();
		m_checkBounds();
	}
}

void Target::clean()
{
}

void Target::doThrow()
{
	getRigidBody()->acceleration += glm::vec2(0, 9.8f);//external forces, gravity
	getTransform()->position = throwPos;
	getRigidBody()->velocity = throwSpeed;
}

void Target::reset()
{
	getTransform()->position = glm::vec2(26.5f, 600.0f - 58 / 2);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
}

void Target::m_move()
{
	float deltaTime = 1.0f / 60.0f;
	float pixelPerMeter = 1.0f;
	
	getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime * pixelPerMeter;
}

void Target::m_checkBounds()
{
	if (getTransform()->position.x > 810 || getTransform()->position.y > 600 - getWidth() / 2 )
	{
		getRigidBody()->isColliding = true;
	}
}

void Target::m_reset()
{
}

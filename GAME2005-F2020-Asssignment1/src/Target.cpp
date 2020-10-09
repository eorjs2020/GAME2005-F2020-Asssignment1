#include "Target.h"
#include "TextureManager.h"



Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/ball.png","circle");

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
			reset();
		
		}
		m_move();
		m_checkBounds();
		if (m_maxdis < -1 * (this->getTransform()->position.y - Config::SCREEN_HEIGHT))
			m_maxdis = -1 * (this->getTransform()->position.y - Config::SCREEN_HEIGHT) ;
	}
	force();
}

void Target::clean()
{
}

void Target::doThrow()
{
	getRigidBody()->acceleration += glm::vec2(0.0f, gravity);
	getTransform()->position = throwPos;
	getRigidBody()->velocity.x += throwSpeed * cos(angle * (3.14f/180));
	getRigidBody()->velocity.y += -throwSpeed * sin(angle * (3.14f / 180));
}

void Target::reset()
{

	getTransform()->position = glm::vec2(26.5f, 600.0f - 58 / 2);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	m_bThrow = false;
}

void Target::force()
{
	m_force = sqrt(Util::magnitude(getRigidBody()->acceleration) * mass);
}

void Target::m_move()
{
	float deltaTime = 1.0f / 60.0f;
	float pixelPerMeter = 1.0f;
	getRigidBody()->velocity += getRigidBody()->acceleration  * deltaTime;
	getTransform()->position += getRigidBody()->velocity * deltaTime * pixelPerMeter;
	std::cout << cos(angle * (3.14f / 180)) << "  " << sin(angle * (3.14f / 180)) << std::endl;
}

void Target::m_checkBounds()
{
	if (getTransform()->position.x > 1010 || getTransform()->position.y > 600 - getWidth() / 2 )
	{
		getRigidBody()->isColliding = true;
	}
}

void Target::m_reset()
{
}

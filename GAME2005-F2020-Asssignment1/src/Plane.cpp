#include "Plane.h"
#include "TextureManager.h"

Plane::Plane()
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/atlas.txt",
		"../Assets/sprites/atlas.png", 
		"spritesheet");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("spritesheet"));

	// set frame width
	setWidth(65);

	// set frame height
	setHeight(65);

	getTransform()->position = glm::vec2(800.0f - 65.0f, 600.0f - 65/2);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);

	m_buildAnimations();
}

Plane::~Plane()
= default;

void Plane::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	
	
	// draw the plane sprite with simple propeller animation
	switch (m_currentAnimationState)
	{
	case PLANE_IDLE:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("plane"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLANE_EXPLOSION:
		TextureManager::Instance()->playAnimation("spritesheet", getAnimation("explosion"),
			x, y, 0.12f, 0, 255, true);
		break;

	default:
		break;
	}
}

void Plane::update()
{
	

}

void Plane::clean()
{
}

void Plane::setAnimationState(PlaneAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

bool Plane::isColliding(GameObject* pOther)
{
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;
	
	return (getDistance(pOther) <= myRadius + otherRadius);
}

float Plane::getDistance(GameObject* pOther)
{
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}

void Plane::m_buildAnimations()
{
	Animation planeAnimation = Animation();

	planeAnimation.name = "plane";
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane1"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane2"));
	planeAnimation.frames.push_back(getSpriteSheet()->getFrame("plane3"));

	setAnimation(planeAnimation);


	Animation explosionAnimation = Animation();
	explosionAnimation.name = "explosion";
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion1"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion2"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion3"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion4"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion5"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion6"));
	explosionAnimation.frames.push_back(getSpriteSheet()->getFrame("explosion7"));

	setAnimation(explosionAnimation);


}
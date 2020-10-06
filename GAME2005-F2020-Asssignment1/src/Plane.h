#pragma once
#ifndef __PLANE__
#define __PLANE__

#include "Sprite.h"
#include "PlaneAnimationState.h"

class Plane final : public Sprite
{
public:
	Plane();
	~Plane();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
	void setAnimationState(PlaneAnimationState);
	bool isColliding(GameObject*);
	float getDistance(GameObject*);
private:
	void m_buildAnimations();
	bool collision = false;
	PlaneAnimationState m_currentAnimationState;
	
};

#endif /* defined (__PLANE__) */

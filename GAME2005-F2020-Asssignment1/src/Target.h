#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"
#include "Util.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	glm::vec2 throwPos;
	float throwSpeed = 95.0f, angle = 15 , mass = 2.2f, m_force;
	float gravity = 9.8f;
	bool m_bThrow = false;
	void doThrow();
	void reset();
	void force();
	float getForce() { return m_force; };
private:
	void m_move();
	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */
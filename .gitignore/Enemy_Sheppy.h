#ifndef ENEMY_SHEPPY_H
#define ENEMY_SHEPPY_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Enemy_Sheppy : public Actor
{

	protected:
	int move;
	bool m_bJump;
	int m_iJumpCounter;

	public:
	Enemy_Sheppy();

	void Update();
	void Animation();



};


#endif

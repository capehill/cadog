#ifndef ENEMY_COCOA_H
#define ENEMY_COCOA_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"
#include "Player.h"

class Enemy_Cocoa : public Actor
{

	protected:


	public:
	Enemy_Cocoa();

	int  m_iShake;
	int  m_iDelay;
	int  m_fOldX;
	int  m_fOldY;
	
	bool m_bFalling;

	bool m_bNew;

	void Update();
	void Animation();


};


#endif

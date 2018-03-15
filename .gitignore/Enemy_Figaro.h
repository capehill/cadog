#ifndef ENEMY_FIGARO_H
#define ENEMY_FIGARO_H

#include <stdio.h>

#include "Global.h"
#include "Actor.h"

class Enemy_Figaro : public Actor
{

	protected:
	float m_fOldX;
	float m_fOldY;

	float m_fTurn;
	float m_fTurnMove;

	float m_fCircle;
	float m_fEffect;
	int   m_iMove;
	bool  m_bFistCall;


	public:
	Enemy_Figaro();
	void Update();
	void Animation();


};


#endif

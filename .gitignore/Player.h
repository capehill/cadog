#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>

#include "Actor.h"
#include "Global.h"
#include "NumberDraw.h"

#define IDLE			0
#define WALK_RIGHT		2	
#define WALK_LEFT		4
#define JUMP			8

#define DUCK			16
#define RUN				32


#define ANIMATION_SPEED 4 //Once every X frame

class Player : public Actor
{
	public:

	int	  m_iLife;
	float m_fForce;
	int   m_iAction;
	
	bool   m_bOnFloor;

	bool  m_bJumping;
	bool  m_bJumpReady;
	int   m_iJumpHold;

	int   m_bWalking;
    int	  m_iJumpState; //For animation
	int	  m_iWalkState;

	float m_fSpeedUp;

	bool  m_bDuck;
	int   m_iDuck;

	int	  m_iFlash;
	int	  m_iFlashCounter;

	int	  m_iBoostJump;

	float m_fCY;
	

	GLuint	m_pExtraTexture; //För playerns extra stuff

	int  MoveX( float fDX );
	

	void SetXY_CXCY( float fX, float fY) { m_fCX = fX; m_fCY = fY; SetXY(fX,fY); }
	void Reset();

	void ForceJump();
	void Flash();
	void ReleaseTextures();
	void LoadTextures();
	void PlayerAction( int iAction );
	void Update();
	bool Render();
	void Animation(); 
	static Player* Instance() {return &I;}

	void Die();

	Player();

	private:
	static Player I;
};

#define Player1 Player::Instance()



#endif

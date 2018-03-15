#ifndef ACTOR_H
#define ACTOR_H

#include <stdio.h>

#include "Global.h"
#include "WorldMap.h"


#ifndef WINVER // defined in windef.h which also defines POINT
struct POINT {
	int x, y;
};
#endif

class Actor
{

	protected:

	QUADVERT			m_QuadVerts[6];
	GLuint	m_pTexture;

	public:
	int		m_iHealth;
	int		m_iScore;

	int		iID;

	float   m_fCX;
	float	m_fX;
	float	m_fY;
	bool    m_bFacingRight;

	bool	m_bDead;
	
	bool	m_bInWater;
	bool	m_bOnFloor;
	float   m_fForce;
	
	bool	m_bActive;
	bool  m_bInvinsible;
	int		m_iSpecial;
	int		m_iAnimation;
	float	m_z;

	POINT	m_ptCollision[4];
				//	1.  2.
				//	3.  4.
	

	 Actor();
	~Actor();

 
	
	void CleanUp();
	void SetTexture( GLuint pTexture );

	void SetXY(float fX, float fY) { m_fX = fX; m_fY = fY; };
	void SetAABB( int iX1, int iY1, int iX2, int iY2, int iX3, int iY3, int iX4, int iY4 );

	virtual void SetFacingRight( bool bFacingRight ) { m_bFacingRight = bFacingRight; };
	virtual int MoveX( float fDX);
	virtual int MoveY( float fDY);
	virtual void CheckIfActive(float fX);
	virtual void AI() {};
	virtual void Update();
	virtual void Animation();
	virtual bool Render();


};


#endif

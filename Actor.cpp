#include "Actor.h"


Actor::Actor()
{
	m_pTexture = 0;
	m_bInWater = false;
	m_bDead = false;
	m_bActive = false;
	iID = 0;
	m_fX = 0.0f;
	m_fY = 0.0f;
	
	m_bFacingRight = true;
	m_bInvinsible = false;

	m_iHealth = 100;
	m_iAnimation = 0;

	m_bInWater = false;
	m_bOnFloor = false;
    m_fForce = 0.0f;

	m_iSpecial = 0;

	m_fCX = 0.0f;

	m_z = -10.0f;

	memset(m_QuadVerts,0,sizeof(QUADVERT) * 6);

}

Actor::~Actor()
{
	CleanUp();	

}


void Actor::CleanUp()
{


}


void Actor::SetAABB( int iX1, int iY1, int iX2, int iY2, int iX3, int iY3, int iX4, int iY4 )
{
	m_ptCollision[0].x = iX1;
	m_ptCollision[0].y = iY1,

	m_ptCollision[1].x = iX2;
	m_ptCollision[1].y = iY2,

	m_ptCollision[2].x = iX3;
	m_ptCollision[2].y = iY3,

	m_ptCollision[3].x = iX4;
	m_ptCollision[3].y = iY4;
}

int Actor::MoveX( float fDX )
{	
	int i;




	for(i = 0; i < 4; i++)
	{
		int iTileID = World->GetTile(int(m_fX + fDX + m_ptCollision[i].x)/32, int(m_fY + m_ptCollision[i].y + 32)/32);
		if( iTileID > 0 && iTileID < SOLID )
		{
		  if( i == 0 || i == 2) m_fX = int(m_fX + fDX) / 32 * 32 + 32 - m_ptCollision[i].x + 1.0f;
		  else m_fX = int(m_fX + fDX)/32*32 + (32 - m_ptCollision[i].x) - 1.0f;
	      return i;
		}
	}

	m_fX += fDX;
	
	return -1;
}



int Actor::MoveY( float fDY )
{	
	int i;

	m_bInWater = false;

	for(i = 0; i < 4; i++)
	{
		int iTileID = World->GetTile(int(m_fX + m_ptCollision[i].x)/32, int(m_fY + m_ptCollision[i].y + fDY + 32)/32);
		
		if(!m_bInWater) m_bInWater = (iTileID == WATER);
		if( iTileID > 0 && (iTileID <  SOLID || ( (int(m_fY + m_ptCollision[i].y + fDY + 32)/32) != (int(m_fY + m_ptCollision[i].y + 32)/32) && fDY < 0.0f && iTileID >= SOLID && iTileID < SEMI_SOLID && i > 1)))
		{
		    if( i > 1 ) m_fY = int(m_fY + fDY+32) / 32 * 32  - m_ptCollision[i].y;
			
			return i;
		}
	}

	m_fY += fDY;

	return -1;
}

void Actor::CheckIfActive(float fX)
{
	/*

		VERY IMPORTANT!

		Checks if an Actor is close enough to be seen (And in use)
	*/
	m_bActive = ( (int(fabs(fX - m_fX))/256) * 256 < 1000 );

}
void Actor::Update( )
{
	
 
	m_fForce += (GRAVITY - (m_bInWater ? WATER_PULL_UP : 0.0f));

	if(m_fForce >= 32.0f) m_fForce = 31.0f;
	if(m_bInWater && m_fForce >= 2.0f) m_fForce = 1.9f;


	int ir = MoveY(-m_fForce);
	
	m_bOnFloor = false;

	if( ir != -1)
	if( ir < 2 )
	{
		m_fForce = 0.0f;
	}
	else
	{
		m_fForce = 0.0f;
		m_bOnFloor = true;
		
	}

}

void Actor::Animation()
{
	/*
		Omg, this is totally freaky code... allways overwritten so it doesn't really matter
	*/

	float anims[4] = {0,1,0,2};

	float anim = 0.0f; //temp

	anim = (int(m_fX) /16)% 4;

	anim = anims[int(anim)];
	
	if( m_bFacingRight )
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
		m_QuadVerts[0].u = 0.0f + (0.25f * anim);
		m_QuadVerts[0].v = 0.25f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
		m_QuadVerts[1].u = 0.25f + (0.25f * anim);
		m_QuadVerts[1].v = 0.25f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
		m_QuadVerts[2].u = 0.0f + (0.25f * anim);
		m_QuadVerts[2].v = 0.0f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
		m_QuadVerts[3].u = 0.25f + (0.25f * anim);
		m_QuadVerts[3].v = 0.25f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
		m_QuadVerts[4].u = 0.25f + (0.25f * anim);
		m_QuadVerts[4].v = 0.0f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
		m_QuadVerts[5].u = 0.0f + (0.25f * anim);
		m_QuadVerts[5].v = 0.0f;
	}
	else
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[0].specular = 0x0;
		m_QuadVerts[0].u = 0.0f + (0.25f * anim);
		m_QuadVerts[0].v = 0.25f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[1].specular = 0x0;
		m_QuadVerts[1].u = 0.25f + (0.25f * anim);
		m_QuadVerts[1].v = 0.25f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[2].specular = 0x0;
		m_QuadVerts[2].u = 0.0f + (0.25f * anim);
		m_QuadVerts[2].v = 0.0f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[3].specular = 0x0;
		m_QuadVerts[3].u = 0.25f + (0.25f * anim);
		m_QuadVerts[3].v = 0.25f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[4].specular = 0x0;
		m_QuadVerts[4].u = 0.25f + (0.25f * anim);
		m_QuadVerts[4].v = 0.0f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[5].specular = 0x0;
		m_QuadVerts[5].u = 0.0f + (0.25f * anim);
		m_QuadVerts[5].v = 0.0f;
	}




}

void Actor::SetTexture( GLuint pTexture )
{
	m_pTexture = pTexture;
}



bool Actor::Render()
{ 
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_pTexture);
	drawQuadVerts(m_QuadVerts,0,1,4,2);
	return true;
}

#include <stdlib.h>
#include "Enemy_Sheppy.h"

Enemy_Sheppy::Enemy_Sheppy()
{
	m_iJumpCounter = 150;
	m_bJump = false;

}
void Enemy_Sheppy::Update()
{
//	static int move = -10;
	int ir;

	if(!m_bDead)
	{
		//sndJump stuff
		
		m_iJumpCounter--;

		if(m_iJumpCounter< 0 && !m_bJump)
		{
			
			m_iJumpCounter = (rand() % 150) + 20;
			m_bJump = true;
			m_fForce = - 8 - float(rand() % 8);
			
		}

		//Gravity shit
		m_fForce += (GRAVITY - (m_bInWater ? WATER_PULL_UP : 0.0f)); 

		if(m_fForce >= 32.0f) m_fForce = 31.0f;
		if(m_bInWater && m_fForce >= 2.0f) m_fForce = 1.9f;

		 

		ir = MoveY(-m_fForce);
		

		m_bOnFloor = false;

	 /*
		if( ir != -1)
		{*/
			if( ir != -1)
			if( ir < 2 )
			{
				m_fForce = 0.0f;
			}
			else
			{
				m_fForce = 0.0f;
				m_bOnFloor = true;
				m_bJump = false;
				
			}
	
			m_iAnimation = (m_iAnimation + 1) % 20; //This fellah's animation is allways player

			if(move == 0) move = 1;
			
			//OMG AI!
			if(move > 0)
			{
				SetFacingRight(false);
			
				if( World->GetTile( int((m_fX + 32)/32), int((m_fY - 1.0f + 32.0f)/32) ) < SEMI_SOLID &&
					World->GetTile( int((m_fX + 32)/32), int((m_fY - 1.0f + 32.0f)/32) ) != 0 || m_bJump )
				{
					if(MoveX(1.4f) != -1) move = -1;
				}else if(!m_bJump) move = -1;

			}
			else if( move < 0)
			{
				SetFacingRight(true);
				if( World->GetTile( int((m_fX - 1)/32), int((m_fY - 1.0f + 32.0f)/32 )) < SEMI_SOLID &&
					World->GetTile( int((m_fX - 1)/32), int((m_fY - 1.0f + 32.0f)/32 )) != 0 || m_bJump )
				{
					if(MoveX(-1.4f) != -1) move = 1;
				}else if(!m_bJump) move = 1;
				
			}
		
	}
	else
	{
		m_fForce += GRAVITY;
		m_fY -= m_fForce;
	}

}
void Enemy_Sheppy::Animation()
{
	float tempY = 0.0f;

	float aniarr [] = { 0,1,0,2,0};

	float anim = m_iAnimation / 5;
	
	anim = aniarr[int(anim)];

	tempY = m_fY;

	m_fY -= 2.0f;

	if(m_bJump) anim = 3.0f;

	if( m_bFacingRight )
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
//		m_QuadVerts[0].specular = 0x0;
		m_QuadVerts[0].u = 0.0f + (0.25f * anim);
		m_QuadVerts[0].v = 0.75f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+38)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+38)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[1].specular = 0x0;
		m_QuadVerts[1].u = 0.25f + (0.25f * anim);
		m_QuadVerts[1].v = 0.75f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[2].specular = 0x0;
		m_QuadVerts[2].u = 0.0f + (0.25f * anim);
		m_QuadVerts[2].v = 0.50f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+38)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+38)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[3].specular = 0x0;
		m_QuadVerts[3].u = 0.25f + (0.25f * anim);
		m_QuadVerts[3].v = 0.75f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+38)) * (float)(RADIE  + 38 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+38.0f)) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[4].specular = 0x0;
		m_QuadVerts[4].u = 0.25f + (0.25f * anim);
		m_QuadVerts[4].v = 0.50f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
//		m_QuadVerts[5].specular = 0x0;
		m_QuadVerts[5].u = 0.0f + (0.25f * anim);
		m_QuadVerts[5].v = 0.50f;
	}
	else
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX+38)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX+38)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[0].specular = 0x0;
		m_QuadVerts[0].u = 0.0f + (0.25f * anim);
		m_QuadVerts[0].v = 0.75f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[1].specular = 0x0;
		m_QuadVerts[1].u = 0.25f + (0.25f * anim);
		m_QuadVerts[1].v = 0.75f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX+38 )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX+38 )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[2].specular = 0x0;
		m_QuadVerts[2].u = 0.0f + (0.25f * anim);
		m_QuadVerts[2].v = 0.50f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[3].specular = 0x0;
		m_QuadVerts[3].u = 0.25f + (0.25f * anim);
		m_QuadVerts[3].v = 0.75f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 38 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 38 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[4].specular = 0x0;
		m_QuadVerts[4].u = 0.25f + (0.25f * anim);
		m_QuadVerts[4].v = 0.50f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX+38 )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX+38 )) * (float)(RADIE + 38 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[5].specular = 0x0;
		m_QuadVerts[5].u = 0.0f + (0.25f * anim);
		m_QuadVerts[5].v = 0.50f;
	}
	
	m_fY = tempY;



}

#include <stdlib.h>
#include "Enemy_Parrot.h"


Enemy_Parrot::Enemy_Parrot()
{
	m_fOldX = 0.0f;
	m_fOldY = 0.0f;

	m_fCircle = 0.0f;
	m_fEffect = 16.0f + float(rand() % 64);
	m_iMove = (rand() % 512) - 256;
	
	m_fTurnMove = 2.3f;
	m_bFistCall = false;
}


void Enemy_Parrot::Update()
{
//	static int move = -10;
	if(!m_bFistCall)
	{
		m_bFistCall = true;
		m_fOldY = m_fY;
		m_fOldX = m_fX;
	}

	if(!m_bDead)
	{
		if( m_fX < m_fOldX - 256.0f )
		{
			m_iMove = rand() % 512;
			m_fTurn = M_PI - 0.3f;
			
		}
		else if( m_fX > m_fOldX + 256.0f ) 
		{
			m_iMove = -(rand() % 512);
			m_fTurn = M_PI - 0.3f;
		}

		m_bFacingRight = m_iMove > 0;
		
		m_fCircle += 0.08f;

		if(m_fCircle > M_PI * 2)
		{
			m_fCircle = 0.0f;
			m_fEffect = 8.0f + float(rand() % 16);
		}
	
		if( m_iMove < 0 )
		{
			m_iMove++;
			m_fX -= 3.0f;
		}
		else if( m_iMove > 0 )
		{
			m_iMove--;
			m_fX += 3.0f;
		} 
		else
		{	
			m_iMove = (rand() % 512) - 256;
		}
	
		float dE = 1.0f;

		 

		m_fY = m_fOldY + sinf(m_fCircle) * m_fEffect * dE; 



		m_iAnimation = (m_iAnimation + 1) % 20; //This fellah's animation is allways player
	}
	else
	{	
		m_fForce += GRAVITY;
		if(m_fForce > 30) m_fForce = 30;
		m_fY -= m_fForce;
	}


}
void Enemy_Parrot::Animation()
{
	float anim = float(m_iAnimation / 10);

	if( m_bFacingRight )
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
//		m_QuadVerts[0].specular = 0x0;
		m_QuadVerts[0].u = 0.0f + (0.5f * anim);
		m_QuadVerts[0].v = 0.5f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[1].specular = 0x0;
		m_QuadVerts[1].u = 0.5f + (0.5f * anim);
		m_QuadVerts[1].v = 0.5f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[2].specular = 0x0;
		m_QuadVerts[2].u = 0.0f + (0.5f * anim);
		m_QuadVerts[2].v = 0.0f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[3].specular = 0x0;
		m_QuadVerts[3].u = 0.5f + (0.5f * anim);
		m_QuadVerts[3].v = 0.5f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[4].specular = 0x0;
		m_QuadVerts[4].u = 0.5f + (0.5f * anim);
		m_QuadVerts[4].v = 0.0f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
//		m_QuadVerts[5].specular = 0x0;
		m_QuadVerts[5].u = 0.0f + (0.5f * anim);
		m_QuadVerts[5].v = 0.0f;
	}
	else
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[0].specular = 0x0;
		m_QuadVerts[0].u = 0.0f + (0.5f * anim);
		m_QuadVerts[0].v = 0.5f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[1].specular = 0x0;
		m_QuadVerts[1].u = 0.5f + (0.5f * anim);
		m_QuadVerts[1].v = 0.5f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[2].specular = 0x0;
		m_QuadVerts[2].u = 0.0f + (0.5f * anim);
		m_QuadVerts[2].v = 0.0f;;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[3].specular = 0x0;
		m_QuadVerts[3].u = 0.5f + (0.5f * anim);
		m_QuadVerts[3].v = 0.5f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[4].specular = 0x0;
		m_QuadVerts[4].u = 0.5f + (0.5f * anim);
		m_QuadVerts[4].v = 0.0f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
	//	m_QuadVerts[5].specular = 0x0;
		m_QuadVerts[5].u = 0.0f + (0.5f * anim);
		m_QuadVerts[5].v = 0.0f;
	}





}

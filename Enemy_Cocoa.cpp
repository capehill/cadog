#include <stdlib.h>
#include "Enemy_Cocoa.h"



Enemy_Cocoa::Enemy_Cocoa()
{
	m_bNew = true;
	m_bFalling = false;
	m_iSpecial = 0;
	m_iShake = 0;
	m_fForce = 0;
	m_iDelay = 0;

	 m_fOldX = 0;
	 m_fOldY = 0;
	



}
void Enemy_Cocoa::Update()
{
//	static int move = -10;
	
	if(m_bNew)
	{
		m_fY +=30;
		m_bNew = false;

		 m_fOldX = m_fX;
		 m_fOldY = m_fY;
	}

		if(!m_bFalling)
		{
			if(m_iDelay <= 0)
			{
				if(m_iShake == 0)
				{
					int iDist = abs(Player1->m_fX - m_fX);

					if(iDist < 80) 
					{
						m_iShake = rand() % 105;
					}
				}
				else
				{
					m_iShake--;

					if(m_iShake % 3 == 0)
					{
						m_fX = m_fOldX - 3 + (rand() % 3);
						m_fY = m_fOldY - 3 + (rand() % 3);
					}

					if(m_iShake <= 0)
					{
						m_bFalling = true;
						m_iSpecial = 1;
					}
				}
			}
			else
			{
				m_iDelay--;

				if(m_iDelay <= 0)
				{
					m_fX = m_fOldX;
					m_fY = m_fOldY;
				}
			}
		}
		else
		{
			m_fForce += (GRAVITY/2);
			if(m_fForce > 8) m_fForce = 8;

			if(MoveY(-m_fForce) != -1) 
			{
				m_fForce = 0;
				m_bFalling = false;
				m_iSpecial = 0;
				m_iDelay = 75;
		
				m_fX = m_fOldX;
				m_fY = 512;

			}

		}





}
void Enemy_Cocoa::Animation()
{

		
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
		m_QuadVerts[0].u = 0.75f;
		m_QuadVerts[0].v = 1.0f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
		m_QuadVerts[1].u = 1.0f;
		m_QuadVerts[1].v = 1.0f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
		m_QuadVerts[2].u = 0.75f;
		m_QuadVerts[2].v = 0.75f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
		m_QuadVerts[3].u = 1.0f;
		m_QuadVerts[3].v = 1.0f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
		m_QuadVerts[4].u = 1.0f;
		m_QuadVerts[4].v = 0.75f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;

		m_QuadVerts[5].u = 0.75f;
		m_QuadVerts[5].v = 0.75f;



}

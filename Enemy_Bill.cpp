#include "Enemy_Bill.h"


void Enemy_Bill::CheckIfActive(float fX)
{
	/*

		VERY IMPORTANT!

		Checks if an Actor is close enough to be scene (And in use)
	*/
	m_bActive = ( fabs(fX - m_fX) < 1000 );
	if(!m_bDead)
	{
		m_bDead = !m_bActive;
		if(m_bDead) m_fY = -256;
	}
}

void Enemy_Bill::Update()
{
//	static int move = -10;
	
	if(!m_bDead)
	{
		
		if(!m_bFacingRight){ m_fX+=6.0f;  }
		else { m_fX-=6.0f;  }
	}
	else
	{	
		if(!m_bFacingRight){  m_fX+=6.0f;  }
		else { m_fX-=6.0f;  }
		m_fForce += GRAVITY;
		if(m_fForce > 30) m_fForce = 30;
		m_fY -= m_fForce;
	}

	if(m_fX < -64) m_bDead = true;
	if(m_fX > World->m_iWorldMapWidth * 32) m_bDead = true;

}
void Enemy_Bill::Animation()
{
	
	if( m_bFacingRight )
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
		m_QuadVerts[0].u = 0.0f;
		m_QuadVerts[0].v = 0.5f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
		m_QuadVerts[1].u = 0.5f;
		m_QuadVerts[1].v = 0.5f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
		m_QuadVerts[2].u = 0.0f;
		m_QuadVerts[2].v = 0.0f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
		m_QuadVerts[3].u = 0.5f;
		m_QuadVerts[3].v = 0.5f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
		m_QuadVerts[4].u = 0.5f;
		m_QuadVerts[4].v = 0.0f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;

		m_QuadVerts[5].u = 0.0f;
		m_QuadVerts[5].v = 0.0f;
	}
	else
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
		m_QuadVerts[0].u = 0.0f;
		m_QuadVerts[0].v = 0.5f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
		m_QuadVerts[1].u = 0.5f;
		m_QuadVerts[1].v = 0.5f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
		m_QuadVerts[2].u = 0.5f;
		m_QuadVerts[2].v = 0.5f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
		m_QuadVerts[3].u = 0.5f;
		m_QuadVerts[3].v = 0.5f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
		m_QuadVerts[4].u = 0.5f;
		m_QuadVerts[4].v = 0.5f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
		m_QuadVerts[5].u = 0.0f;
		m_QuadVerts[5].v = 0.0f;
	}




}

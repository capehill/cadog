#include "Enemy_Spikey.h"



void Enemy_Spikey::Update()
{
	m_iSpecial = (Player1->m_fForce > 0) ? 1 : 0;


}
void Enemy_Spikey::Animation()
{

		
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = 0xFFFFFFFF;
		m_QuadVerts[0].u = 0.5f;
		m_QuadVerts[0].v = 1.0f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = 0xFFFFFFFF;
		m_QuadVerts[1].u = 1.0f;
		m_QuadVerts[1].v = 1.0f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = 0xFFFFFFFF;
		m_QuadVerts[2].u = 0.5f;
		m_QuadVerts[2].v = 0.02f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = 0xFFFFFFFF;
		m_QuadVerts[3].u = 1.0f;
		m_QuadVerts[3].v = 1.0f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + 64 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = 0xFFFFFFFF;
		m_QuadVerts[4].u = 1.0f;
		m_QuadVerts[4].v = 0.02f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = 0xFFFFFFFF;
		m_QuadVerts[5].u = 0.5f;
		m_QuadVerts[5].v = 0.02f;



}

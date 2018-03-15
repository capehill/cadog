#include "Coin.h"



Coin::Coin()
{
	die = 0;
	m_iType = 0;
	aa = 0.0f;
}

void Coin::Update()
{
	m_iType = iID - 51;
	
	if(!m_bDead)
	{
	
	}
	else
	{	
		die += 16;
		if(die >= 256) m_fY = -300.0f;
	}

	m_iAnimation = int(sinf(aa) * 16);

	aa += 0.1f;
	if(aa > M_PI) aa = 0.0f;



}
void Coin::Animation()
{
	float anim = 0.0f;
	GLuint color = 0xFFFFFFFF;

	
	anim = (16.0f + m_iAnimation) / 2.0f;

	if(die > 0)
	{
		color = 0x00FFFFFF + ((256 - die) << 24); 
	}


		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX + 16.0f - anim)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = color;
		m_QuadVerts[0].u = 0.0f + (m_iType % 2) * 0.50f;
		m_QuadVerts[0].v = 0.50f + (m_iType / 2) * 0.50f;;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+ 16.0f + anim )) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = color;
		m_QuadVerts[1].u = 0.50f + (m_iType % 2) * 0.50f;
		m_QuadVerts[1].v = 0.50f + (m_iType / 2) * 0.50f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX + 16.0f - anim )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = color;
		m_QuadVerts[2].u = 0.0f + (m_iType % 2) * 0.50f;;
		m_QuadVerts[2].v = 0.0f + (m_iType / 2) * 0.50f;;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+ 16.0f + anim )) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = color;
		m_QuadVerts[3].u = 0.50f + (m_iType % 2) * 0.50f;;
		m_QuadVerts[3].v = 0.50f + (m_iType / 2) * 0.50f;;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+ 16.0f + anim )) * (float)(RADIE  + 32 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+32.0f)) * (float)(RADIE  + 32 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = color;
		m_QuadVerts[4].u = 0.50f + (m_iType % 2) * 0.50f;;
		m_QuadVerts[4].v = 0.0f + (m_iType / 2) * 0.50f;;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX + 16.0f - anim )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 32 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = color;
		m_QuadVerts[5].u = 0.0f + (m_iType % 2) * 0.50f;;
		m_QuadVerts[5].v = 0.0f + (m_iType / 2) * 0.50f;;




}

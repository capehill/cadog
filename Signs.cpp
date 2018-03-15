#include "Signs.h"



Signs::Signs()
{

	bChecked = false;
}

void Signs::Update()
{
	
	
	if(!m_bDead)
	{
	
	}
	else
	{	
	
	}

}

void Signs::Animation()
{

	GLuint color = 0xFFFFFFFF;

	float tempY = m_fY;

	m_fY -= 4;

	if( iID == 55)
	{
		m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
		m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
		m_QuadVerts[0].z = m_z;
		m_QuadVerts[0].diffuse = color;
		m_QuadVerts[0].u = 0.0f;
		m_QuadVerts[0].v = 0.50f;

		m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + m_fY);
		m_QuadVerts[1].z = m_z;
		m_QuadVerts[1].diffuse = color;
		m_QuadVerts[1].u = 0.50f;
		m_QuadVerts[1].v = 0.50f;

		m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[2].z = m_z;
		m_QuadVerts[2].diffuse = color;
		m_QuadVerts[2].u = 0.0f;
		m_QuadVerts[2].v = 0.0f;

		m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY);
		m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY );
		m_QuadVerts[3].z = m_z;
		m_QuadVerts[3].diffuse = color;
		m_QuadVerts[3].u = 0.50f;
		m_QuadVerts[3].v = 0.50f;

		m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + 64 + m_fY );
		m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + 64 + m_fY);
		m_QuadVerts[4].z = m_z;
		m_QuadVerts[4].diffuse = color;
		m_QuadVerts[4].u = 0.50f;
		m_QuadVerts[4].v = 0.0f;

		m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
		m_QuadVerts[5].z = m_z;
		m_QuadVerts[5].diffuse = color;
		m_QuadVerts[5].u = 0.0f;
		m_QuadVerts[5].v = 0.0f;
	}
	else
	{
		if(m_iSpecial == 0)
		{
			m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
			m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
			m_QuadVerts[0].z = m_z;
			m_QuadVerts[0].diffuse = color;
			m_QuadVerts[0].u = 0.0f;
			m_QuadVerts[0].v = 1.00f;

			m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY );
			m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + m_fY);
			m_QuadVerts[1].z = m_z;
			m_QuadVerts[1].diffuse = color;
			m_QuadVerts[1].u = 0.50f;
			m_QuadVerts[1].v = 1.00f;

			m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[2].z = m_z;
			m_QuadVerts[2].diffuse = color;
			m_QuadVerts[2].u = 0.0f;
			m_QuadVerts[2].v = 0.5f;

			m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY);
			m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY );
			m_QuadVerts[3].z = m_z;
			m_QuadVerts[3].diffuse = color;
			m_QuadVerts[3].u = 0.50f;
			m_QuadVerts[3].v = 1.00f;

			m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + 64 + m_fY );
			m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + 64 + m_fY);
			m_QuadVerts[4].z = m_z;
			m_QuadVerts[4].diffuse = color;
			m_QuadVerts[4].u = 0.50f;
			m_QuadVerts[4].v = 0.5f;

			m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[5].z = m_z;
			m_QuadVerts[5].diffuse = color;
			m_QuadVerts[5].u = 0.0f;
			m_QuadVerts[5].v = 0.5f;
		}
		else
		{
			m_QuadVerts[0].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY);
			m_QuadVerts[0].y = -cosf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + m_fY );
			m_QuadVerts[0].z = m_z;
			m_QuadVerts[0].diffuse = color;
			m_QuadVerts[0].u = 0.5f;
			m_QuadVerts[0].v = 1.00f;

			m_QuadVerts[1].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY );
			m_QuadVerts[1].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + m_fY);
			m_QuadVerts[1].z = m_z;
			m_QuadVerts[1].diffuse = color;
			m_QuadVerts[1].u = 1.00f;
			m_QuadVerts[1].v = 1.00f;

			m_QuadVerts[2].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[2].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[2].z = m_z;
			m_QuadVerts[2].diffuse = color;
			m_QuadVerts[2].u = 0.5f;
			m_QuadVerts[2].v = 0.5f;

			m_QuadVerts[3].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY);
			m_QuadVerts[3].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE + m_fY );
			m_QuadVerts[3].z = m_z;
			m_QuadVerts[3].diffuse = color;
			m_QuadVerts[3].u = 1.00f;
			m_QuadVerts[3].v = 1.00f;

			m_QuadVerts[4].x = -sinf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + 64 + m_fY );
			m_QuadVerts[4].y = -cosf( (M_PI/2560.0f) * -float(m_fX+64.0f)) * (float)(RADIE  + 64 + m_fY);
			m_QuadVerts[4].z = m_z;
			m_QuadVerts[4].diffuse = color;
			m_QuadVerts[4].u = 1.00f;
			m_QuadVerts[4].v = 0.5f;

			m_QuadVerts[5].x = -sinf( (M_PI/2560.0f) * -float(m_fX)) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[5].y = -cosf( (M_PI/2560.0f) * -float(m_fX )) * (float)(RADIE + 64 + m_fY);
			m_QuadVerts[5].z = m_z;
			m_QuadVerts[5].diffuse = color;
			m_QuadVerts[5].u = 0.5f;
			m_QuadVerts[5].v = 0.5f;
		}
	}

	m_fY = tempY;



}

#include "Menu.h"
#include "sdlgl.h"

CMenu CMenu::I;

CMenu::CMenu()
{	
	for(int i = 0; i < 7; i++)
		m_pTexture[i] = 0;
	
	m_iSubSelected = 0;
	m_iSelected = 0;
	m_bNoButtonsPressed = false;
	m_iKeyDown = 0;
	m_iShowBox = 0;
	m_iCheckPoint = 0;

	m_iState = 0;

	m_iAnimation[0] = 0; 
	m_iAnimation[1] = 0;
	m_iAnimation[2] = 0;
	m_iAnimation[3] = 0;

	m_iSubAnimation[0] = 0; 
	m_iSubAnimation[1] = 0;
	m_iSubAnimation[2] = 0;
	m_iSubAnimation[3] = 0;

	LOG("DEBUG: Menu initialized\n");
}

void CMenu::Reset()
{	
	m_iSubSelected = 0;
	m_iSelected = 0;
	m_bNoButtonsPressed = false;
	m_iKeyDown = 1; //Otherwice it detect no keypress this frame and automatically exit (cause esc is down)
	m_iShowBox = 0;
	m_iState = 0;

	m_iAnimation[0] = 0; 
	m_iAnimation[1] = 0;
	m_iAnimation[2] = 0;
	m_iAnimation[3] = 0;

	m_iSubAnimation[0] = 0; 
	m_iSubAnimation[1] = 0;
	m_iSubAnimation[2] = 0;
	m_iSubAnimation[3] = 0;
}



CMenu::~CMenu()
{
	CleanUp();
}

void CMenu::CleanUp()
{
	ReleaseTextures();

}

	
void CMenu::LoadTextures()
{

	
	Pack->OpenPackFile("Data/Data1.pak");
				

	m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("menu_bg_1.tga"), Pack->GetSizeByName("menu_bg_1.tga"), 0, 0);
	m_pTexture[1] = loadTextureFromFP(Pack->GetFPByName("menu_bg_2.tga"), Pack->GetSizeByName("menu_bg_2.tga"), 0, 0);
	m_pTexture[2] = loadTextureFromFP(Pack->GetFPByName("menu_1.tga"), Pack->GetSizeByName("menu_1.tga"), 0, 0);
	m_pTexture[3] = loadTextureFromFP(Pack->GetFPByName("menu_2.tga"), Pack->GetSizeByName("menu_2.tga"), 0, 0);
	m_pTexture[4] = loadTextureFromFP(Pack->GetFPByName("menu_3.tga"), Pack->GetSizeByName("menu_3.tga"), 0, 0);
	m_pTexture[5] = loadTextureFromFP(Pack->GetFPByName("menu_options.tga"), Pack->GetSizeByName("menu_options.tga"), 0, 0);
	m_pTexture[6] = loadTextureFromFP(Pack->GetFPByName("menu_level.tga"), Pack->GetSizeByName("menu_level.tga"), 0, 0);

	LOG("DEBUG: Menu's Textures Loaded\n");


	Pack->ClosePackFile();
}


		
void CMenu::ReleaseTextures()
{

	for(int i = 0; i < 7; i++)
		if( m_pTexture[i] != 0 )
		{
			glDeleteTextures(1,&m_pTexture[i]);
			m_pTexture[i] = 0;
		}


	LOG("DEBUG: Menu's Textures Released\n");
}

void CMenu::Update()
{
	if(m_iKeyDown == 0)
	{
		m_bNoButtonsPressed = true;
	}
	else if(m_bNoButtonsPressed)
	{
		m_bNoButtonsPressed = false;

		switch(m_iKeyDown)
		{
			case 1:
				if(m_iShowBox != 1)
				{
					if(m_iSelected > 0)
					{
						m_iSelected--;
						m_iShowBox = 0;
						m_iState = NOTHING;
						playSound(sndBack);
					}
				}
				else
				{

					if(m_iSubSelected > 0)
					{
						m_iSubSelected--;
						playSound(sndBack);
					}
				}
			break;

			case 2:
				if(m_iShowBox != 1)
				{
					if(m_iSelected < 3)
					{
						m_iSelected++;
						m_iShowBox = 0;
						m_iState = NOTHING;
						playSound(sndBack);
					}
				}
				else
				{
					if(m_iSubSelected < m_iCheckPoint)
					{
						m_iSubSelected++;
						playSound(sndBack);
					}

				}
			break;

			case 3:
			if(m_iShowBox != 1)
			{
				if(m_iSelected < 3)
				{
					playSound(sndSelect);
					m_iSubSelected = 0;
					m_iSubAnimation[0] = 0; 
					m_iSubAnimation[1] = 0;
					m_iSubAnimation[2] = 0;
					m_iSubAnimation[3] = 0;
					m_iShowBox = m_iSelected + 1;
				}
				else m_iState = EXIT_GAME;
			}
			else
			{
				playSound(sndSelect);
				m_iState = START_GAME + m_iSubSelected;
			}
			break;

			case 4:
			if(m_iShowBox != 0)
			{
				m_iShowBox = 0;
				playSound(sndBack);
			}
			else m_iState = EXIT_GAME;
			break;

			default:
			break;

		}
	}

	for(int i = 0; i < 4; i++)
	{
		if(m_iSelected != i)
			if(m_iAnimation[i] > 0) m_iAnimation[i]-=4;

		if(m_iSubSelected != i)
			if(m_iSubAnimation[i] > 0) m_iSubAnimation[i]-=4;
	}

	if(m_iAnimation[m_iSelected] < 24) m_iAnimation[m_iSelected]+=4;
	if(m_iSubAnimation[m_iSubSelected] < 24) m_iSubAnimation[m_iSubSelected]+=4;

	m_iKeyDown = 0;
	
}

void CMenu::Escape()
{
	m_iKeyDown = 4;
}
void CMenu::MoveDown()
{
	m_iKeyDown = 2;
}

void CMenu::MoveUp()
{
	m_iKeyDown = 1;
}
	
void CMenu::Enter()
{
	m_iKeyDown = 3;
}

int  CMenu::GetState()
{
	
	return m_iState;
}

void CMenu::Render()
{

	//Menu Background
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU , D3DTADDRESS_CLAMP  );
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV , D3DTADDRESS_CLAMP  );

//	pd3dDevice->SetTexture(0,m_pTexture[1]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_pTexture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	RenderQuad(640-480,0,480,480,0,0,1,1,0xFFFFFFFF);

//	pd3dDevice->SetTexture(0,m_pTexture[0]);
	glBindTexture(GL_TEXTURE_2D, m_pTexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	RenderQuad(0,0,480,480,0,0,1,1,0xFFFFFFFF);


	//Options
//	pd3dDevice->SetTexture(0,m_pTexture[5]);
	glBindTexture(GL_TEXTURE_2D, m_pTexture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	RenderQuad(32+m_iAnimation[0],200,192,63,0,0,0.75f,0.25f,0xFFFFFFFF);
	RenderQuad(32+m_iAnimation[1],248,192,63,0,0.25f,0.75f,0.50f,0xFFFFFFFF);
	RenderQuad(32+m_iAnimation[2],296,256,63,0,0.50f,1,0.75f,0xFFFFFFFF);
	RenderQuad(32+m_iAnimation[3],344,192,63,0,0.75f,0.75f,1,0xFFFFFFFF);

	if(m_iShowBox > 0)
	{

//		pd3dDevice->SetTexture(0,m_pTexture[m_iShowBox+1]);
		glBindTexture(GL_TEXTURE_2D, m_pTexture[m_iShowBox+1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		RenderQuad(640 - 10 - 300,480 - 10 - 300,300,300,0,0,1,1,0xFFFFFFFF);

		if(m_iShowBox == 1)
		{
//			pd3dDevice->SetTexture(0,m_pTexture[6]);
			glBindTexture(GL_TEXTURE_2D, m_pTexture[6]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			RenderQuad(640 - 10 - 300 + 22+m_iSubAnimation[0] ,480 - 10 - 300 + 70,160,40,0,0,1,0.25f,0xFFFFFFFF);

			if(m_iCheckPoint > 0)
				RenderQuad(640 - 10 - 300 + 22+m_iSubAnimation[1] ,480 - 10 - 300 + 144,160,40,0,0.25f,1,0.5f,0xFFFFFFFF);
			
			if(m_iCheckPoint > 1)
				RenderQuad(640 - 10 - 300 + 22+m_iSubAnimation[2] ,480 - 10 - 300 + 144+40,160,40,0,0.5f,1,0.75f,0xFFFFFFFF);

			if(m_iCheckPoint > 2)
				RenderQuad(640 - 10 - 300 + 22+m_iSubAnimation[3] ,480 - 10 - 300 + 144+80,160,40,0,0.75f,1,1,0xFFFFFFFF);
		}
	}

//	pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU , D3DTADDRESS_WRAP  );
//	pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV , D3DTADDRESS_WRAP  );
}

void CMenu::RenderQuad(float x, float y, float w, float h, float u1, float v1, float u2, float v2, GLuint color)
{
	float z = -10;
/*	QUADVERT quadverts[6] = {
		{x, y, z, color,  u1, v1},
		{x+w, y, z, color, u2, v1},
		{x, y+h, z, color,  u1, v2},

		{x, y+h, z, color, u1, v2},
		{x+w, y, z, color, u2, v1},
		{x+w, y+h, z, color, u2, v2},
	};
	
	pd3dDevice->SetVertexShader( D3DFVF_QUADVERT );

	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)quadverts, sizeof(QUADVERT));
*/
	glColor4ub((color>>16)&0xFF,(color>>8)&0xFF,color&0xFF,(color>>24)&0xFF);
	glBegin(GL_QUADS);
	glTexCoord2f(u1,v1); glVertex3f(x,y,z);
	glTexCoord2f(u2,v1); glVertex3f(x+w,y,z);
	glTexCoord2f(u2,v2); glVertex3f(x+w,y+h,z);
	glTexCoord2f(u1,v2); glVertex3f(x,y+h,z);
	glEnd();


}


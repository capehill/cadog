#include "NumberDraw.h"
#include "sdlgl.h"

NumberDraw NumberDraw::I;

NumberDraw::NumberDraw()
{
	m_pTexture[0] = 0;
	m_pTexture[1] = 0;
	m_pTexture[2] = 0;
	int i;
	float z = -10;

	//Setup numbers
	for( i = 0; i < 16; i++)
	{
		m_pQuadVert[i][0].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i][0].z = z;
		m_pQuadVert[i][0].u = (i % 4) * 0.25f;
		m_pQuadVert[i][0].v = (i / 4) * 0.25f;
		m_pQuadVert[i][0].x = 0.0f;
		m_pQuadVert[i][0].y = 0.0f;

		m_pQuadVert[i][1].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i][1].z = z;
		m_pQuadVert[i][1].u = (i % 4) * 0.25f + 0.25f;
		m_pQuadVert[i][1].v = (i / 4) * 0.25f;
		m_pQuadVert[i][1].x = 32.0f;
		m_pQuadVert[i][1].y = 0.0f;

		m_pQuadVert[i][2].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i][2].z = z;
		m_pQuadVert[i][2].u = (i % 4) * 0.25f;
		m_pQuadVert[i][2].v = (i / 4) * 0.25f + 0.25f;
		m_pQuadVert[i][2].x = 0.0f;
		m_pQuadVert[i][2].y = 32.0f;


		m_pQuadVert[i][3].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i][3].z = z;
		m_pQuadVert[i][3].u = (i % 4) * 0.25f + 0.25f;
		m_pQuadVert[i][3].v = (i / 4) * 0.25f;
		m_pQuadVert[i][3].x = 32.0f;
		m_pQuadVert[i][3].y = 0.0f;

		m_pQuadVert[i][4].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i][4].z = z;
		m_pQuadVert[i][4].u = (i % 4) * 0.25f + 0.25f;
		m_pQuadVert[i][4].v = (i / 4) * 0.25f + 0.25f;
		m_pQuadVert[i][4].x = 32.0f;
		m_pQuadVert[i][4].y = 32.0f;
		
		m_pQuadVert[i][5].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i][5].z = z;
		m_pQuadVert[i][5].u = (i % 4) * 0.25f;
		m_pQuadVert[i][5].v = (i / 4) * 0.25f + 0.25f;
		m_pQuadVert[i][5].x = 0.0f;
		m_pQuadVert[i][5].y = 32.0f;

	}

	for( i = 0; i < 16; i++)
	{
		m_pQuadVert[i+16][0].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i+16][0].z = z;
		m_pQuadVert[i+16][0].u = (i % 4) * 0.25f;
		m_pQuadVert[i+16][0].v = (i / 4) * 0.25f;
		m_pQuadVert[i+16][0].x = 0.0f;
		m_pQuadVert[i+16][0].y = 0.0f;

		m_pQuadVert[i+16][1].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i+16][1].z = z;
		m_pQuadVert[i+16][1].u = (i % 4) * 0.25f + 0.25f;
		m_pQuadVert[i+16][1].v = (i / 4) * 0.25f;
		m_pQuadVert[i+16][1].x = 24.0f;
		m_pQuadVert[i+16][1].y = 0.0f;

		m_pQuadVert[i+16][2].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i+16][2].z = z;
		m_pQuadVert[i+16][2].u = (i % 4) * 0.25f;
		m_pQuadVert[i+16][2].v = (i / 4) * 0.25f + 0.25f;
		m_pQuadVert[i+16][2].x = 0.0f;
		m_pQuadVert[i+16][2].y = 24.0f;


		m_pQuadVert[i+16][3].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i+16][3].z = z;
		m_pQuadVert[i+16][3].u = (i % 4) * 0.25f + 0.25f;
		m_pQuadVert[i+16][3].v = (i / 4) * 0.25f;
		m_pQuadVert[i+16][3].x = 24.0f;
		m_pQuadVert[i+16][3].y = 0.0f;

		m_pQuadVert[i+16][4].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i+16][4].z = z;
		m_pQuadVert[i+16][4].u = (i % 4) * 0.25f + 0.25f;
		m_pQuadVert[i+16][4].v = (i / 4) * 0.25f + 0.25f;
		m_pQuadVert[i+16][4].x = 24.0f;
		m_pQuadVert[i+16][4].y = 24.0f;
		
		m_pQuadVert[i+16][5].diffuse = 0xFFFFFFFF;
		m_pQuadVert[i+16][5].z = z;
		m_pQuadVert[i+16][5].u = (i % 4) * 0.25f;
		m_pQuadVert[i+16][5].v = (i / 4) * 0.25f + 0.25f;
		m_pQuadVert[i+16][5].x = 0.0f;
		m_pQuadVert[i+16][5].y = 24.0f;

	}

	for(int i = 0; i < 32; i++)
		m_fnum[i].bInUse = false;

	LOG("DEBUG: NumberDraw initialized\n");
}

NumberDraw::~NumberDraw()
{
	CleanUp();
}

void NumberDraw::CleanUp()
{
	ReleaseTextures();

}

	
void NumberDraw::LoadTextures()
{
	Pack->OpenPackFile("Data/Data2.pak");
				

	
	if(!bLowRes)
	{
		m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("Numbers.tga"), Pack->GetSizeByName("Numbers.tga"), 0, 0);
		m_pTexture[1] = loadTextureFromFP(Pack->GetFPByName("Numbers2.tga"), Pack->GetSizeByName("Numbers2.tga"), 0, 0);
		m_pTexture[2] = loadTextureFromFP(Pack->GetFPByName("Numbers3.tga"), Pack->GetSizeByName("Numbers3.tga"), 0, 0);
	}
	else
	{
		m_pTexture[0] = loadTextureFromFP(Pack->GetFPByName("Numbers.tga"), Pack->GetSizeByName("Numbers.tga"), 128, 128);
		m_pTexture[1] = loadTextureFromFP(Pack->GetFPByName("Numbers2.tga"), Pack->GetSizeByName("Numbers2.tga"), 128, 128);
		m_pTexture[2] = loadTextureFromFP(Pack->GetFPByName("Numbers3.tga"), Pack->GetSizeByName("Numbers3.tga"), 128, 128);
	}

	Pack->ClosePackFile();
	LOG("DEBUG: NumberDraw's Textures Loaded\n");
}

void NumberDraw::Clear()
{
	for(int i = 0; i < 32; i++)
		m_fnum[i].bInUse = false;
}
		
void NumberDraw::ReleaseTextures()
{
	for(int i = 0; i < 3; i++)
	if( m_pTexture[i] != 0 )
	{
		glDeleteTextures(1,&m_pTexture[i]);
		m_pTexture[i] = 0;
	}


	LOG("DEBUG: NumberDraw's Textures Released\n");
}

void NumberDraw::AddFadeNumber(int iNumber, bool bShowProcent) //temp to show +/- numbers
{
	for(int i = 0; i < 32; i++)
		if(!m_fnum[i].bInUse)
		{
			int size = 40;

			if( (iNumber > 9 && iNumber < 100) || (iNumber > -10 && iNumber < 0) ) size = 60;
			if( (iNumber > 99 && iNumber < 1000) || (iNumber > -100 && iNumber < -9) ) size = 80;
			if( (iNumber > 900 && iNumber < 10000) || (iNumber > -1000 && iNumber < -99) ) size = 100;
			if( (iNumber > -10000 && iNumber < -999) ) size = 120;
			
			
			if(	!m_fnum[i].bShowProcent ) size -= 20;

			m_fnum[i].fX = 320 - (size / 2); //Player1->m_fX;
			m_fnum[i].fY = 220; //Player1->m_fY + 64.0f;
			m_fnum[i].bShowProcent = bShowProcent;
			m_fnum[i].bInUse = true;
			m_fnum[i].iFade = 255;
			m_fnum[i].iNumber = iNumber;
			
			break;

		}
}


void NumberDraw::UpdateFadeNumbers()
{
	for(int i = 0; i < 32; i++)
		if(m_fnum[i].bInUse)
		{
			m_fnum[i].iFade-=8;
			m_fnum[i].fY -= 2.0f;

			if(m_fnum[i].iFade < 0)
				m_fnum[i].bInUse = false;
		}
}
void NumberDraw::DrawFadeNumbers()
{
	for(int i = 0; i < 32; i++)
		if(m_fnum[i].bInUse)
			DrawProcentNumberEx( m_fnum[i].fX, m_fnum[i].fY, m_fnum[i].iNumber, (m_fnum[i].iNumber > 0), m_fnum[i].iFade, m_fnum[i].bShowProcent, ((m_fnum[i].iNumber > 0) ? 2 : 1));
}

void NumberDraw::DrawNumber(int iX, int iY, int iNumber, int iTexture)
{

	 char chNum[16];
	 int  iDraw = 0,i = 0;
//	 int  iOldX, iOldY;
	 int  iZeros = 8;

	 if(iNumber > 99999999) iNumber = 99999999;
	 int iTemp = iNumber;

	 while(iTemp != 0)
	 {
		 iTemp /= 10;
		 iZeros--;
	 }

	 memset(chNum,0,sizeof(chNum));

	 if(iZeros == 0) sprintf(chNum, "%i", abs(iNumber));
	 else if(iZeros == 1) sprintf(chNum, "0%i", abs(iNumber));
	 else if(iZeros == 2) sprintf(chNum, "00%i", abs(iNumber));
	 else if(iZeros == 3) sprintf(chNum, "000%i", abs(iNumber));
	 else if(iZeros == 4) sprintf(chNum, "0000%i", abs(iNumber));
	 else if(iZeros == 5) sprintf(chNum, "00000%i", abs(iNumber));
	 else if(iZeros == 6) sprintf(chNum, "000000%i", abs(iNumber));
     else if(iZeros == 7) sprintf(chNum, "0000000%i", abs(iNumber));
	 else if(iZeros == 8) sprintf(chNum, "00000000");

//     pd3dDevice->SetTexture( 0, m_pTexture[iTexture] );
	glBindTexture(GL_TEXTURE_2D, m_pTexture[iTexture]);
	glEnable(GL_TEXTURE_2D);

//	 pd3dDevice->SetVertexShader(D3DFVF_QUADVERT);

	 
/*	 D3DMATRIX matMove = { 1,0,0,0,
						   0,1,0,0,
						   0,0,1,0,
						   iX,iY,0,1 };
    iOldX = -iX;
	iOldY = -iY;

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMove);	
*/
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(iX,iY,0);
/*	D3DMATRIX matNewChar = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      20,0,0,1 };
*/
	while(chNum[i] != '\0')
	{
		switch(chNum[i])
		{
			case '0':
		    iDraw = 0;
			break;

			case '1':
		    iDraw = 1;
			break;

			case '2':
		    iDraw = 2;
			break;

			case '3':
		    iDraw = 3;
			break;

			case '4':
		    iDraw = 4;
			break;

			case '5':
		    iDraw = 5;
			break;

			case '6':
		    iDraw = 6;
			break;

			case '7':
		    iDraw = 7;
			break;

			case '8':
		    iDraw = 8;
			break;

			case '9':
		    iDraw = 9;
			break;

			default:
			iDraw = 0;
			break;
		}
	
//		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[iDraw], sizeof(QUADVERT));
		drawQuadVerts(m_pQuadVert[iDraw],0,1,4,2);

		i++;


//		pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matNewChar);	
//		iOldX += -20;
		glTranslatef(20,0,0);

	}

/*	D3DMATRIX matMoveBack = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      iOldX,iOldY,0,1 };

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMoveBack);	
*/
	glPopMatrix();
}
void NumberDraw::DrawNumberSmall(int iX, int iY, int iNumber, int iTexture)
{

	 char chNum[16];
	 int  iDraw = 0,i = 0;
//	 int  iOldX, iOldY;
	 int  iZeros = 8;

	 if(iNumber > 99999999) iNumber = 99999999;
	 int iTemp = iNumber;

	 while(iTemp != 0)
	 {
		 iTemp /= 10;
		 iZeros--;
	 }

	 memset(chNum,0,sizeof(chNum));

	 if(iZeros == 0) sprintf(chNum, "%i", abs(iNumber));
	 else if(iZeros == 1) sprintf(chNum, "0%i", abs(iNumber));
	 else if(iZeros == 2) sprintf(chNum, "00%i", abs(iNumber));
	 else if(iZeros == 3) sprintf(chNum, "000%i", abs(iNumber));
	 else if(iZeros == 4) sprintf(chNum, "0000%i", abs(iNumber));
	 else if(iZeros == 5) sprintf(chNum, "00000%i", abs(iNumber));
	 else if(iZeros == 6) sprintf(chNum, "000000%i", abs(iNumber));
     else if(iZeros == 7) sprintf(chNum, "0000000%i", abs(iNumber));
	 else if(iZeros == 8) sprintf(chNum, "00000000");


//     pd3dDevice->SetTexture( 0, m_pTexture[iTexture] );

//	 pd3dDevice->SetVertexShader(D3DFVF_QUADVERT);
	glBindTexture(GL_TEXTURE_2D, m_pTexture[iTexture]);
	glEnable(GL_TEXTURE_2D);

	 
/*	 D3DMATRIX matMove = { 1,0,0,0,
						   0,1,0,0,
						   0,0,1,0,
						   iX,iY,0,1 };
     iOldX = -iX;
	 iOldY = -iY;

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMove);	

	 D3DMATRIX matNewChar = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      15,0,0,1 };
*/
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(iX,iY,0);

	while(chNum[i] != '\0')
	{
		switch(chNum[i])
		{
			case '0':
		    iDraw = 0;
			break;

			case '1':
		    iDraw = 1;
			break;

			case '2':
		    iDraw = 2;
			break;

			case '3':
		    iDraw = 3;
			break;

			case '4':
		    iDraw = 4;
			break;

			case '5':
		    iDraw = 5;
			break;

			case '6':
		    iDraw = 6;
			break;

			case '7':
		    iDraw = 7;
			break;

			case '8':
		    iDraw = 8;
			break;

			case '9':
		    iDraw = 9;
			break;

			default:
			iDraw = 0;
			break;
		}

		if( i == 0)
		{
			drawQuadVerts(m_pQuadVert[15],0,1,4,2);
//			pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[15], sizeof(QUADVERT));
			glTranslatef(30,0,0);
//			pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matNewChar);	
//			pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matNewChar);
//			iOldX -= 30;

		}
//		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[iDraw+16], sizeof(QUADVERT));
		drawQuadVerts(m_pQuadVert[iDraw+16],0,1,4,2);
		i++;


//		pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matNewChar);	
//		iOldX -= 15;
		glTranslatef(15,0,0);

	}

/*	D3DMATRIX matMoveBack = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      iOldX,iOldY,0,1 };

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMoveBack);	
*/
	glPopMatrix();
}

void NumberDraw::DrawProcentNumber(int iX, int iY, int iNumber, int iTexture)
{

	 char chNum[8];
	 int  iDraw = 0,i = 0;
//	 int  iOldX, iOldY;

	 memset(chNum,0,sizeof(chNum));

	sprintf(chNum, "%i", abs(iNumber));

//     pd3dDevice->SetTexture( 0, m_pTexture[iTexture] );
	glBindTexture(GL_TEXTURE_2D, m_pTexture[iTexture]);
	glEnable(GL_TEXTURE_2D);

//	 pd3dDevice->SetVertexShader(D3DFVF_QUADVERT);

	 
/*	 D3DMATRIX matMove = { 1,0,0,0,
						   0,1,0,0,
						   0,0,1,0,
						   iX,iY,0,1 };
     iOldX = -iX;
	 iOldY = -iY;

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMove);	

	 D3DMATRIX matNewChar = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      20,0,0,1 };
*/
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(iX,iY,0);
	while(chNum[i] != '\0')
	{
		switch(chNum[i])
		{
			case '0':
		    iDraw = 0;
			break;

			case '1':
		    iDraw = 1;
			break;

			case '2':
		    iDraw = 2;
			break;

			case '3':
		    iDraw = 3;
			break;

			case '4':
		    iDraw = 4;
			break;

			case '5':
		    iDraw = 5;
			break;

			case '6':
		    iDraw = 6;
			break;

			case '7':
		    iDraw = 7;
			break;

			case '8':
		    iDraw = 8;
			break;

			case '9':
		    iDraw = 9;
			break;

			default:
			iDraw = 0;
			break;
		}
	
//		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[iDraw], sizeof(QUADVERT));
		drawQuadVerts(m_pQuadVert[iDraw],0,1,4,2);
		i++;


//		pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matNewChar);	
//		iOldX -= 20;
		glTranslatef(20,0,0);

	}

	//Draw the % char
//	pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[12], sizeof(QUADVERT));
	drawQuadVerts(m_pQuadVert[12],0,1,4,2);
	
/*	D3DMATRIX matMoveBack = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      iOldX,iOldY,0,1 };

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMoveBack);	
*/
	glPopMatrix();
}
void NumberDraw::DrawProcentNumberEx(int iX, int iY, int iNumber, bool bAdd, int iAlpha, bool bShowProcent, int iTexture)
{
 char chNum[8];
	 int  iDraw = 0,i = 0;
//	 int  iOldX, iOldY;

	 //fix alpha
	 GLuint color = 0x00FFFFFF + (iAlpha << 24);

	 for(int i2 = 0; i2 < 16; i2++)
		 for(int i3 = 0; i3 < 6; i3++)
			m_pQuadVert[i2][i3].diffuse = color;


	 memset(chNum,0,sizeof(chNum));

	 sprintf(chNum, "%i", abs(iNumber));

//     pd3dDevice->SetTexture( 0, m_pTexture[iTexture] );
	glBindTexture(GL_TEXTURE_2D, m_pTexture[iTexture]);
	glEnable(GL_TEXTURE_2D);

//	 pd3dDevice->SetVertexShader(D3DFVF_QUADVERT);

	 
/*	 D3DMATRIX matMove = { 1,0,0,0,
						   0,1,0,0,
						   0,0,1,0,
						   iX,iY,0,1 };
     iOldX = -iX;
	 iOldY = -iY;

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMove);	

	 D3DMATRIX matNewChar = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      20,0,0,1 };
*/
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(iX,iY,0);

	 if(bAdd) {
		drawQuadVerts(m_pQuadVert[11],0,1,4,2);
//		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[11], sizeof(QUADVERT));
	 } else {
		drawQuadVerts(m_pQuadVert[10],0,1,4,2);
//		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[10], sizeof(QUADVERT));
	}


//	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matNewChar);	
	glTranslatef(20,0,0);
	
//	iOldX -= 20;

	while(chNum[i] != '\0')
	{
		switch(chNum[i])
		{
			case '0':
		    iDraw = 0;
			break;

			case '1':
		    iDraw = 1;
			break;

			case '2':
		    iDraw = 2;
			break;

			case '3':
		    iDraw = 3;
			break;

			case '4':
		    iDraw = 4;
			break;

			case '5':
		    iDraw = 5;
			break;

			case '6':
		    iDraw = 6;
			break;

			case '7':
		    iDraw = 7;
			break;

			case '8':
		    iDraw = 8;
			break;

			case '9':
		    iDraw = 9;
			break;

			default:
			iDraw = 0;
			break;
		}
	
//		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[iDraw], sizeof(QUADVERT));
		drawQuadVerts(m_pQuadVert[iDraw],0,1,4,2);
		i++;


//		pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matNewChar);	
//		iOldX -= 20;
		glTranslatef(20,0,0);

	}

	//Draw the % char
	if(bShowProcent) {
//		pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)m_pQuadVert[12], sizeof(QUADVERT));
		drawQuadVerts(m_pQuadVert[12],0,1,4,2);
	}
	
/*
	D3DMATRIX matMoveBack = { 1,0,0,0,
						      0,1,0,0,
						      0,0,1,0,
						      iOldX,iOldY,0,1 };

	pd3dDevice->MultiplyTransform(D3DTS_WORLDMATRIX(0),&matMoveBack);
*/
	glPopMatrix();

	 for(int i2 = 0; i2 < 16; i2++)
		 for(int i3 = 0; i3 < 6; i3++)
			m_pQuadVert[i2][i3].diffuse = 0xFFFFFFFF;
}
	

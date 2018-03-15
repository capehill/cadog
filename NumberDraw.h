#ifndef NUMBERDRAW_H
#define NUMBERDRAW_H



#include <stdio.h>
#include "Global.h"

#include "Player.h"



struct LNUM
{
	bool bShowProcent;
	bool bInUse;
	int  iFade;
	int  iNumber;
	float fY;
	float fX;
};



class NumberDraw
{

	public:
	
	GLuint			m_pTexture[3];
	QUADVERT				    m_pQuadVert[32][6];
	
	 NumberDraw();
	~NumberDraw();

	LNUM						m_fnum[32];


	void AddFadeNumber(int iNumber, bool bShowProect); //temp to show +/- numbers
	void UpdateFadeNumbers();
	void DrawFadeNumbers();

	void CleanUp();
	
	void LoadTextures();
	void ReleaseTextures();

	void DrawNumber(int iX, int iY, int iNumber, int iTexture);
	void DrawNumberSmall(int iX, int iY, int iNumber, int iTexture);

	void DrawProcentNumber(int iX, int iY, int iNumber,  int iTexture);
	void DrawProcentNumberEx(int iX, int iY, int iNumber, bool bAdd, int iAlpha, bool bShowProcent, int iTexture);
	
	void Clear();
	static NumberDraw* Instance() {return &I;}

	private:
	static NumberDraw I;
};

#define Number NumberDraw::Instance()

#endif

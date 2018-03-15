#ifndef MENU_H
#define MENU_H



#include <stdio.h>
#include "Global.h"

#define NOTHING				0
#define START_GAME			1
#define START_GAME_LEV5		2
#define START_GAME_LEV10	3
#define START_GAME_LEV15	4
#define EXIT_GAME			5



class CMenu
{

	public:
	GLuint			m_pTexture[7];

	int		m_iSelected;
	int		m_iSubSelected;
	bool	m_bNoButtonsPressed;
	int		m_iKeyDown;

	int     m_iAnimation[4];
	int     m_iSubAnimation[4];
	int		m_iShowBox;
	int		m_iState;
	int		m_iCheckPoint;

	 CMenu();
	~CMenu();

	void SetCheckpointLevels( int i ) { m_iCheckPoint = i; }; // 1 = level 5, 2 = level 10, 3 = level 15
	void Update();

	void MoveDown();
	void MoveUp();
	void Enter();

	void Escape();

	int  GetState();

	void Reset();

	void CleanUp();
	
	void LoadTextures();
	void ReleaseTextures();

	void Render();
	void RenderQuad(float x, float y, float w, float h, float u1, float v1, float u2, float v2, GLuint color  );

	static CMenu* Instance() {return &I;}

	private:
	static CMenu I;
};

#define Menu1 CMenu::Instance()

#endif

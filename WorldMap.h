#ifndef WORLDMAP_H
#define WORLDMAP_H



#include <stdio.h>
#include "Global.h"

struct MAP_ENEMY
{
	float fX;
	float fY;
	int iType;
};

struct MAP_ENEMY_EMITTER
{
	float fX;
	float fY;
	int iType;
};


class WorldMap
{

	protected:

 
	int *m_pLayer[3];
	int m_iEnemies;

	QUADVERT *m_pQuadVerts;
	
	GLubyte		*m_pVB[3];
	GLuint		m_pTexture[3];

	public:
	
	int	m_iWorldMapWidth;

	 WorldMap();
	~WorldMap();

	void CleanUp();
	

	void Setup3DStuff( bool bNight );
	void Release3DStuff();

	bool LoadMap(char *szFileName);
	bool RenderWorld(float fX, int iLayer);
	
	int	 GetTile( int iX, int iY );
	
	int GetAmountOfEnemies() { return m_iEnemies; }
	void PollEnemy(bool bRestart, MAP_ENEMY &me);
	void PollEnemyEmitter(bool bRestart, MAP_ENEMY_EMITTER &mee);

	static WorldMap* Instance() {return &I;}

	private:
	static WorldMap I;
};

#define World WorldMap::Instance()

#endif

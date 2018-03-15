#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H



#include <stdio.h>
#include <list>
#include "Global.h"
#include "Actor.h"

#include "NumberDraw.h"

#include "Enemy_Parrot.h"
#include "Enemy_Blob.h"
#include "Enemy_Blob_2.h"
#include "Enemy_Blob_Dumb.h"
#include "Enemy_Bill.h"
#include "Enemy_Sheppy.h"
#include "Enemy_Figaro.h"
#include "Enemy_Cocoa.h"
#include "Enemy_Spikey.h"

#include "Signs.h"
#include "Coin.h"

#define ENEMY_PARROT		19
#define ENEMY_BLOB			20
#define ENEMY_BLOB_DUMB		21
#define ENEMY_BLOB_2		22
#define ENEMY_BILL			23
#define ENEMY_SHEPPY		24
#define ENEMY_FIGARO		25
#define ENEMY_COCOA			26
#define ENEMY_SPIKEY		27

#define COIN_1			51
#define COIN_2			52
#define COIN_3			53
#define COIN_4			54

#define SIGN_EXIT		55
#define SIGN_CHECK		56


#define MAX_EMITTERS	32

struct EMITTER
{
	bool bInUse;
	bool bActive;
	float fX;
	float fY;
	int iType;
	int iRate;
	int iCounter;
};

typedef std::list<Actor*> ActorList;

class EnemyManager
{

	protected:

	int m_iEnemies;
	ActorList m_actors;
//	LPDIRECT3DTEXTURE8 m_pTexture[8];
	GLuint m_pTexture[8];
	EMITTER	m_emitter[MAX_EMITTERS]; //Won't support more then 8 now

	public:



	static EnemyManager* Instance() {return &I;}

	void ReleaseTextures();
	void LoadTextures();

	int HitEnemy(int iId, Actor *actor);

	void CleanUp();
	void Setup( );
	void Add( float fX, float fY, int iEnemyType );
	void AddEmitter( float fX, float fY, int iType );
	void UpdateEnemies( float fX );
	void AnimateEnemies();
	void RenderEnemies();
 
	int  CheckCollision(Actor *actor);

	private:
	static EnemyManager I;
};

#define Enemies EnemyManager::Instance()

#endif

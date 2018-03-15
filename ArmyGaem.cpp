//-----------------------------------------------------------------------------
// File: ArmyGaem.cpp
//
// Desc: DirectX window application created by the DirectX AppWizard
//-----------------------------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include "sdlapp.h"
#include "sdlgl.h"
#include "ArmyGaem.h"
#include "rwops.h"
#include <SDL/SDL_endian.h>


#include "WorldMap.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Menu.h"

#include "NumberDraw.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif


struct STORED_DATA
{
	int random1;
	int random2;
	int random3;
	int random4;
	int random5;

	int add_unit;
	int multiplier;
	
	int random6;
	int checkpoint;
	int random7;
	int random8;
	int add_unit2;
	int mutliplier2;

	int	random9;
	int hiscore;
	int random10;
};


#if !defined(HAVE_MUS_RW) || defined(USE_FMOD)
SOUND_TYPE *sndMusicStart = NULL;
SOUND_TYPE *sndMusic = NULL;
#else
Mix_Music *sndMusicStart = NULL;
Mix_Music *sndMusic = NULL;
#endif

SOUND_TYPE *sndJump = NULL;
SOUND_TYPE *sndEnemy = NULL;
SOUND_TYPE *sndCannon = NULL;
SOUND_TYPE *sndWater = NULL;
SOUND_TYPE *sndHit = NULL;
SOUND_TYPE *sndCoin = NULL;
SOUND_TYPE *sndCheck = NULL;
SOUND_TYPE *sndLC = NULL;

SOUND_TYPE *sndSelect = NULL;
SOUND_TYPE *sndBack = NULL;

SOUND_TYPE *sndLevelCompleted = NULL;
SOUND_TYPE *sndGameOver = NULL;

SOUND_TYPE *sndGameWon = NULL;

float pX = 0;
float pY = 0;

float tempX = 0;
bool  bLowRes = false;
bool  bDisallowExit = false;
bool  bC = false;


GLuint bg = 0;
GLuint life = 0;
GLuint texText = 0;
GLuint texVictory = 0;
GLuint texCheck = 0;


int iShowCheck = 0;

int iGameState = 0;
int iLevel = 1;

int iFadeOut = 255;
int iFadeIn = 255;

int iNextState = 0;

bool bChangeLevel = false;
int iExtraTimer = 0;

bool  bNoFade = false;
bool  bPause = false;

int	  iShowScreen = 0;
int   iShowScreenTimer = 0;

int			iHiScore = 0;
int			iCheckPoint = 0; //for now
STORED_DATA StoredData;

int iStopMusic = 0;
bool bPlayMusic = false;

int iMusicChannel = 0;

//Level settings (Music, day/night)

bool bLevelNight[] = { false, false, true, false, false, true, false, false, true, false,
					   false, true, false, true, false, false };

int iLevelMusic[] = { 1, 1, 3, 1, 1, 3, 2, 1, 3, 2, 1, 3, 1, 2, 1, 2};   

void ChangeMusic(int iMusic)
{
	static int iCurrentMusic = -1;

	if(iMusic == iCurrentMusic) return;


	iCurrentMusic = iMusic;

	Pack->OpenPackFile("Data/Music.pak");

	char *startName = NULL;
	char *musicName = NULL;

	if(iMusic == 0)
	{
		startName = NULL;
		musicName = "Menu.ogg";
	}
	else
	if(iMusic == 1)
	{
		startName = "MusicStart1.ogg";
		musicName = "Music1.ogg";
	}
	else
	if(iMusic == 2)
	{
		startName = "MusicStart2.ogg";
		musicName = "Music2.ogg";
	}
	else
	if(iMusic == 3)
	{
		startName = "MusicStart3.ogg";
		musicName = "Music3.ogg";
	}

#ifdef USE_FMOD
	FSOUND_Sample_Free(sndMusicStart);
	FSOUND_Sample_Free(sndMusic);
	if (startName!=NULL)
		sndMusicStart= FSOUND_Sample_Load(FSOUND_FREE, (const char*)Pack->GetDataByName(startName),FSOUND_LOADMEMORY,0,Pack->GetSizeByName(startName));
	else sndMusicStart = NULL;
	sndMusic = FSOUND_Sample_Load(FSOUND_FREE, (const char*)Pack->GetDataByName(musicName),FSOUND_LOADMEMORY | FSOUND_LOOP_NORMAL,0,Pack->GetSizeByName(musicName));
#else
#ifndef HAVE_MUS_RW
	Mix_FreeChunk(sndMusicStart);
	Mix_FreeChunk(sndMusic);
	if (startName!=NULL)
		sndMusicStart = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName(startName),Pack->GetSizeByName(startName),false),0);
	else sndMusicStart = NULL;
	sndMusic = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName(musicName),Pack->GetSizeByName(musicName),false),0);
#else
	Mix_FreeMusic(sndMusicStart);
	Mix_FreeMusic(sndMusic);
	if (startName!=NULL)
		sndMusicStart = Mix_LoadMUS_RW(rwFromSizedFP(Pack->GetFPByName(startName,false),Pack->GetSizeByName(startName),true));
	else sndMusicStart = NULL;
	sndMusic = Mix_LoadMUS_RW(rwFromSizedFP(Pack->GetFPByName(musicName,false),Pack->GetSizeByName(musicName),true));
#endif
#endif
	Pack->ClosePackFile();

}
void StartMusic()
{
	bPlayMusic = true;
#ifdef USE_FMOD
	iMusicChannel = FSOUND_PlaySound(FSOUND_FREE,sndMusicStart);
#else
#ifndef HAVE_MUS_RW
	iMusicChannel = Mix_PlayChannel(-1,sndMusicStart,0);
#else
	Mix_PlayMusic(sndMusicStart,0);
#endif
#endif
}

void StopMusic()
{
	iStopMusic = 255;
	
}

void UpdateMusic()
{
	//Check music!
	if(bPlayMusic)
	{
#ifdef USE_FMOD
		if(FSOUND_IsPlaying(iMusicChannel) == 0) //go to main music
			iMusicChannel = FSOUND_PlaySound(FSOUND_FREE,sndMusic);

		if (iStopMusic > 0) {
			FSOUND_SetVolume(iMusicChannel,iStopMusic);
			iStopMusic -= 8;
			if (iStopMusic <= 0) {
				FSOUND_StopSound(iMusicChannel);
				bPlayMusic = false;
			}
		}

#else
#ifndef HAVE_MUS_RW
		if (!Mix_Playing(iMusicChannel))
			iMusicChannel = Mix_PlayChannel(-1,sndMusic,0);

		if (iStopMusic > 0) {
			Mix_Volume(iMusicChannel,iStopMusic/4);
			iStopMusic-=8;
			if (iStopMusic <= 0) {
				Mix_HaltChannel(iMusicChannel);
				Mix_Volume(iMusicChannel,64);
				bPlayMusic = false;
			}
		}
#else
		if (!Mix_PlayingMusic()) {
			Mix_PlayMusic(sndMusic,0);
		}

		if (iStopMusic > 0) {
			Mix_VolumeMusic(iStopMusic/4);
			iStopMusic -= 8;
			if (iStopMusic <= 0) {
				Mix_HaltMusic();
				Mix_VolumeMusic(64);
				bPlayMusic = false;
			}
		}
#endif
#endif

	}
	
	 
}
	

#if defined(__APPLE__) && defined(__MACH__)
void getSupportPath(char *buffer) {
/*	NSString *dir = @"~/Library/Application Support/Cadog";
	dir = [dir stringByExpandingTildeInPath];

	NSFileManager *fileManager = [NSFileManager defaultManager];
	if ([fileManager fileExistsAtPath: dir] == NO)
		[fileManager createDirectoryAtPath: dir attributes: nil];

	const char *str = [dir UTF8String];
	strcpy(buffer, str);
	strcat(buffer, "/");
	[dir autorelease];
*/
	char *home = getenv("HOME");
	strcpy(buffer, home);
	strcat(buffer, "/Library/Application Support/Cadog");
	DIR *d = opendir(buffer);
	if (d==NULL)
		mkdir(buffer, 0755);
	else
		closedir(d);
	strcat(buffer, "/");
}

#endif

void LoadStoredData()
{

	const char *name = "Data.csd";
#if defined(__APPLE__) && defined(__MACH__)
	char buffer[1000];
	getSupportPath(buffer);
	strcat(buffer, name);
	name = buffer;
#endif
	
	FILE *f = fopen(name,"rb");
	if(f != NULL)
	{
		SDL_RWops *rw = SDL_RWFromFP(f,1);
		StoredData.random1 = SDL_ReadLE32(rw);
		StoredData.random2 = SDL_ReadLE32(rw);
		StoredData.random3 = SDL_ReadLE32(rw);
		StoredData.random4 = SDL_ReadLE32(rw);
		StoredData.random5 = SDL_ReadLE32(rw);
		StoredData.add_unit = SDL_ReadLE32(rw);
		StoredData.multiplier = SDL_ReadLE32(rw);
		StoredData.random6 = SDL_ReadLE32(rw);
		StoredData.checkpoint = SDL_ReadLE32(rw);
		StoredData.random7 = SDL_ReadLE32(rw);
		StoredData.random8 = SDL_ReadLE32(rw);
		StoredData.add_unit2 = SDL_ReadLE32(rw);
		StoredData.mutliplier2 = SDL_ReadLE32(rw);
		StoredData.random9 = SDL_ReadLE32(rw);
		StoredData.hiscore = SDL_ReadLE32(rw);
		StoredData.random10 = SDL_ReadLE32(rw);
		SDL_RWclose(rw);
//		fread(&StoredData,1,sizeof(StoredData),f);
//		fclose(f);

		iHiScore = (StoredData.hiscore / StoredData.mutliplier2) - StoredData.add_unit2;
		iCheckPoint = (StoredData.checkpoint / StoredData.multiplier) - StoredData.add_unit;
	}
}

void SaveStoredData()
{

	StoredData.random1 = rand() % 10;
	StoredData.random2 = rand() % 100000;
	StoredData.random3 = rand() % 10;
	StoredData.random4 = rand() % 100000;
	StoredData.random5 = rand() % 9000000;

	StoredData.add_unit = 3 + rand() % 15;
	StoredData.multiplier = 3 + rand() % 15;
	
	StoredData.random6 = rand() % 15;
	StoredData.checkpoint = (iCheckPoint+StoredData.add_unit) * StoredData.multiplier;
	StoredData.random7 = StoredData.random5 + StoredData.random1;
	StoredData.random8 = StoredData.random4 + StoredData.random1;
	StoredData.add_unit2 =  2 + rand() % 15;
	StoredData.mutliplier2 = 2 + rand() % 15;

	StoredData.random9 = rand() % 10;
	StoredData.hiscore = (iHiScore+StoredData.add_unit2) * StoredData.mutliplier2;
	StoredData.random10 = rand() % 50;


	const char *name = "Data.csd";
#if defined(__APPLE__) && defined(__MACH__)
        char buffer[1000];
        getSupportPath(buffer);
        strcat(buffer, name);
	name = buffer;
#endif
	
	FILE *f = fopen(name,"wb");
	
	if(f != NULL)
	{
		SDL_RWops *rw = SDL_RWFromFP(f,1);
		SDL_WriteLE32(rw,StoredData.random1);
		SDL_WriteLE32(rw,StoredData.random2);
		SDL_WriteLE32(rw,StoredData.random3);
		SDL_WriteLE32(rw,StoredData.random4);
		SDL_WriteLE32(rw,StoredData.random5);
		SDL_WriteLE32(rw,StoredData.add_unit);
		SDL_WriteLE32(rw,StoredData.multiplier);
		SDL_WriteLE32(rw,StoredData.random6);
		SDL_WriteLE32(rw,StoredData.checkpoint);
		SDL_WriteLE32(rw,StoredData.random7);
		SDL_WriteLE32(rw,StoredData.random8);
		SDL_WriteLE32(rw,StoredData.add_unit2);
		SDL_WriteLE32(rw,StoredData.mutliplier2);
		SDL_WriteLE32(rw,StoredData.random9);
		SDL_WriteLE32(rw,StoredData.hiscore);
		SDL_WriteLE32(rw,StoredData.random10);
		SDL_RWclose(rw);
//		fwrite(&StoredData,1,sizeof(StoredData),f);
//		fclose(f);
	}
}
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//	   message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	CMySDLApplication sdlApp(argc,argv);




	if( !sdlApp.Create()  )
	    return 1;





	LoadStoredData();

	Menu1->SetCheckpointLevels(0);

	return sdlApp.Run();
}




//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMySDLApplication::CMySDLApplication(int argc, char *argv[]) : CSDLApplication(argc,argv)
{
//	m_dwCreationWidth           = 200;
//	m_dwCreationHeight          = 16;
//	m_dwCreationWidth = 1024;
//	m_dwCreationHeight = 768;
	m_strWindowTitle            = "Cadog Adventures";

	// Create a D3D font using d3dfont.cpp
	m_bLoadingApp               = true;

	memset( &m_UserInput, 0, sizeof(m_UserInput) );
	m_fWorldRotX                = 0.0f;
	m_fWorldRotY                = 0.0f;


	iGameState = START;

	// Temp start
	pX = 820.0f;
	pY = 300.0f;
	
	iFadeIn = 255;
	iFadeOut = 256;
	Player1->SetAABB( 10,45, 22,45, 10,0, 22,0 ); 
	Player1->SetXY(pX,pY);

	LOG("\n Logging for AssemblyEntry started\n\n");
	


}






void CMySDLApplication::ReloadEnemies( int iLevel )
{

	Enemies->CleanUp();
	World->CleanUp();
	

	if(iLevel == 0 ) World->LoadMap("L0.clf");
	if(iLevel == 1 ) World->LoadMap("L1.clf");
	if(iLevel == 2 ) World->LoadMap("L2.clf");
	if(iLevel == 3 ) World->LoadMap("L3.clf");
	if(iLevel == 4 ) World->LoadMap("L4.clf");
	if(iLevel == 5 ) World->LoadMap("L5.clf");
	if(iLevel == 6 ) World->LoadMap("L6.clf");
	if(iLevel == 7 ) World->LoadMap("L7.clf");
	if(iLevel == 8 ) World->LoadMap("L8.clf");
	if(iLevel == 9 ) World->LoadMap("L9.clf");
	if(iLevel == 10 ) World->LoadMap("L10.clf");
	if(iLevel == 11 ) World->LoadMap("L11.clf");
	if(iLevel == 12 ) World->LoadMap("L12.clf");
	if(iLevel == 13 ) World->LoadMap("L13.clf");
	if(iLevel == 14 ) World->LoadMap("L14.clf");
	if(iLevel == 15 ) World->LoadMap("L15.clf");

	Enemies->Setup();
	
	bool restart = true;
	bool go = true;
	MAP_ENEMY me;
	while(go)
	{	
		 World->PollEnemy(restart,me);
		 restart = false;
		
		 if(me.iType != -1)
			Enemies->Add( me.fX, me.fY, me.iType);
		 else
			go = false;
	}

	go = true;
	restart = true;
	MAP_ENEMY_EMITTER mee;
	while(go)
	{	
		World->PollEnemyEmitter(restart,mee);
		restart = false;

		 if(mee.iType !=-1)
			Enemies->AddEmitter( mee.fX, mee.fY, mee.iType);
		 else
			go = false;
	}
}
void CMySDLApplication::LoadLevel( int iLevel )
{
	static bool bLevelLoaded = false;

	if(bLevelLoaded)
	{
		Enemies->CleanUp();
		World->CleanUp();
	}
	if(iLevel == 0 ) World->LoadMap("L0.clf");
	if(iLevel == 1 ) World->LoadMap("L1.clf");
	if(iLevel == 2 ) World->LoadMap("L2.clf");
	if(iLevel == 3 ) World->LoadMap("L3.clf");
	if(iLevel == 4 ) World->LoadMap("L4.clf");
	if(iLevel == 5 ) World->LoadMap("L5.clf");
	if(iLevel == 6 ) World->LoadMap("L6.clf");
	if(iLevel == 7 ) World->LoadMap("L7.clf");
	if(iLevel == 8 ) World->LoadMap("L8.clf");
	if(iLevel == 9 ) World->LoadMap("L9.clf");
	if(iLevel == 10 ) World->LoadMap("L10.clf");
	if(iLevel == 11 ) World->LoadMap("L11.clf");
	if(iLevel == 12 ) World->LoadMap("L12.clf");
	if(iLevel == 13 ) World->LoadMap("L13.clf");
	if(iLevel == 14 ) World->LoadMap("L14.clf");
	if(iLevel == 15 ) World->LoadMap("L15.clf");

	Enemies->Setup( );
	
	bool restart = true;
	bool go = true;
	MAP_ENEMY me;
	while(go)
	{	
		 World->PollEnemy(restart,me);
		 restart = false;
		
		 if(me.iType != -1)
			Enemies->Add( me.fX, me.fY, me.iType);
		 else
			go = false;
	}

	go = true;
	restart = true;
	MAP_ENEMY_EMITTER mee;
	while(go)
	{	
		World->PollEnemyEmitter(restart,mee);
		restart = false;

		 if(mee.iType !=-1)
			Enemies->AddEmitter( mee.fX, mee.fY, mee.iType);
		 else
			go = false;
	}

	Player1->Reset();


	bLevelLoaded = true;
}
//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//	   permanent initialization.
//-----------------------------------------------------------------------------
bool CMySDLApplication::OneTimeSceneInit()
{

	
	//audio
#ifdef USE_FMOD
	FSOUND_Init(44100, 32, 0);
#else
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(32);
	Mix_Volume(-1,64);
#endif

	ChangeMusic(0);
	



	
//	LoadLevel( iLevel );
 
/*	Pack->OpenPackFile("Data/Sound.pak");
	for (int i = 0; i < Pack->GetFileAmount(); i++) {
		char buffer[512];
		sprintf(buffer,"Data/unpacked/%s",Pack->GetNameByIndex(i));
		FILE *out = fopen(buffer,"wb");
		fwrite(Pack->GetDataByIndex(i),Pack->GetSizeByIndex(i),1,out);
		fclose(out);
	}
	Pack->ClosePackFile();
	Pack->OpenPackFile("Data/Music.pak");
	for (int i = 0; i < Pack->GetFileAmount(); i++) {
		char buffer[512];
		sprintf(buffer,"Data/unpacked/%s",Pack->GetNameByIndex(i));
		FILE *out = fopen(buffer,"wb");
		fwrite(Pack->GetDataByIndex(i),Pack->GetSizeByIndex(i),1,out);
		fclose(out);
	}
	Pack->ClosePackFile();
*/
/*	Pack->CleanUp();
	Pack->AddFile("sndBack.wav");
	Pack->AddFile("sndCannon.wav");
	Pack->AddFile("sndCheck.wav");
	Pack->AddFile("sndCoin.wav");
	Pack->AddFile("sndEnemy.wav");
	Pack->AddFile("sndGameOver.wav");
	Pack->AddFile("sndGameWon.wav");
	Pack->AddFile("sndHit.wav");
	Pack->AddFile("sndJump.wav");
	Pack->AddFile("sndLC.wav");
	Pack->AddFile("sndLevelCompleted.wav");
	Pack->AddFile("sndSelect.wav");
	Pack->AddFile("sndWater.wav");
	Pack->ProcessPackFile("Sound2.pak");
*/

	Pack->OpenPackFile("Data/Sound.pak");
	

#ifdef USE_FMOD
	sndJump = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndJump.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndJump.wav"));
	sndEnemy = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndEnemy.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndEnemy.wav"));
	sndCannon = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndCannon.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndCannon.wav"));
	sndWater = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndWater.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndWater.wav"));
	sndHit = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndHit.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndHit.wav"));
	sndCoin = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndCoin.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndCoin.wav"));
	sndCheck = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndCheck.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndCheck.wav"));
	
	sndLC = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndLC.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndLC.wav"));

	sndSelect = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndSelect.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndSelect.wav"));
	sndBack = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndBack.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndBack.wav"));

	sndLevelCompleted = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndLevelCompleted.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndLevelCompleted.wav"));
	sndGameOver = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndGameOver.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndGameOver.wav"));

	sndGameWon = FSOUND_Sample_Load(FSOUND_FREE,  (const char*)Pack->GetDataByName("sndGameWon.wav"),FSOUND_LOADMEMORY,0,Pack->GetSizeByName("sndGameWon.wav"));
#else
	sndJump = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndJump.wav"),Pack->GetSizeByName("sndJump.wav"),false),0);
	sndEnemy = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndEnemy.wav"),Pack->GetSizeByName("sndEnemy.wav"),false),0);
	sndCannon = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndCannon.wav"),Pack->GetSizeByName("sndCannon.wav"),false),0);
	sndWater = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndWater.wav"),Pack->GetSizeByName("sndWater.wav"),false),0);
	sndHit = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndHit.wav"),Pack->GetSizeByName("sndHit.wav"),false),0);
	sndCoin = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndCoin.wav"),Pack->GetSizeByName("sndCoin.wav"),false),0);
	sndCheck = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndCheck.wav"),Pack->GetSizeByName("sndCheck.wav"),false),0);

	sndLC = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndLC.wav"),Pack->GetSizeByName("sndLC.wav"),false),0);

	sndSelect = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndSelect.wav"),Pack->GetSizeByName("sndSelect.wav"),false),0);
	sndBack = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndBack.wav"),Pack->GetSizeByName("sndBack.wav"),false),0);

	sndLevelCompleted = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndLevelCompleted.wav"),Pack->GetSizeByName("sndLevelCompleted.wav"),false),0);
	sndGameOver = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndGameOver.wav"),Pack->GetSizeByName("sndGameOver.wav"),false),0);

	sndGameWon = Mix_LoadWAV_RW(rwFromSizedFP(Pack->GetFPByName("sndGameWon.wav"),Pack->GetSizeByName("sndGameWon.wav"),false),0);
#endif

	Pack->ClosePackFile();
	// TEMP DONE
	m_bLoadingApp = false;



	return true;
}
















//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
bool CMySDLApplication::InitDeviceObjects( )
{


	return true;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restores scene objects.
//-----------------------------------------------------------------------------
bool CMySDLApplication::RestoreDeviceObjects( bool bUseLowRes )
{
	// TODO: setup render states

	bLowRes = bUseLowRes;

	// Set up the textures
//	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	// Set miscellaneous render states
//	m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
//	m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
//	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	gluPerspective(45,viewport[2]/viewport[3],1,100);
	gluLookAt(0,0,-5,0,0,0,0,1,0);
	
	// Restore the font

	Pack->OpenPackFile("Data/Data5.pak");

 	if(!bLowRes)
	{
		life = loadTextureFromFP(Pack->GetFPByName("life.tga"), Pack->GetSizeByName("life.tga"), 0, 0);
		texText = loadTextureFromFP(Pack->GetFPByName("text.tga"), Pack->GetSizeByName("text.tga"), 0, 0);
		texVictory = loadTextureFromFP(Pack->GetFPByName("VictoryScreen.tga"), Pack->GetSizeByName("VictoryScreen.tga"), 0, 0);
		texCheck = loadTextureFromFP(Pack->GetFPByName("checkpoint.tga"), Pack->GetSizeByName("checkpoint.tga"), 0, 0);
	}
	else
	{
		life = loadTextureFromFP(Pack->GetFPByName("life.tga"), Pack->GetSizeByName("life.tga"), 64, 64);
		texText = loadTextureFromFP(Pack->GetFPByName("text.tga"), Pack->GetSizeByName("text.tga"), 256, 256);
		texVictory = loadTextureFromFP(Pack->GetFPByName("VictoryScreen.tga"), Pack->GetSizeByName("VictoryScreen.tga"), 0, 0);
		texCheck = loadTextureFromFP(Pack->GetFPByName("checkpoint.tga"), Pack->GetSizeByName("checkpoint.tga"), 256, 256);
	}

	Pack->ClosePackFile(); 

	Menu1->LoadTextures();
	if(iGameState == GAME)
		World->Setup3DStuff(bLevelNight[iLevel]);
	Enemies->LoadTextures();

	LoadBG( bLevelNight[iLevel]);

	if(bg != 0)
		LOG("DEBUG: BackGround Texture loaded OK\n");

	Player1->LoadTextures();
	
	Number->LoadTextures();


	//Here I loaded textures
	return true;
}




void CMySDLApplication::ChangeLevel()
{
	iLevel++;
	Player1->m_iLife++;

	iGameState = GAME;
 
	if(iLevel > 4)if(iCheckPoint == 0) iCheckPoint = 1;
	if(iLevel > 9) if(iCheckPoint == 1) iCheckPoint = 2;
	if(iLevel > 14) if(iCheckPoint == 2) iCheckPoint = 3;

	if(iLevel == 5 || iLevel == 10 || iLevel == 15) iShowCheck = 100;

	if(iLevel > 15) //Game Complete
	{
		bC = true;
		iLevel = 1;
		iFadeOut = 255;
		iFadeIn = 255;
	
		iGameState = MENU;
		//victory
	}

	ChangeMusic(iLevelMusic[iLevel]);
	StartMusic();

	World->Release3DStuff();
	ReleaseBG();
	LoadLevel(iLevel);

	World->Setup3DStuff(bLevelNight[iLevel]);
	LoadBG(bLevelNight[iLevel]);

	pX = 820.0f;
	pY = 300.0f;

	Player1->SetXY_CXCY(pX,pY);
	bPause = false;
}
void CMySDLApplication::StartGame()
{
	iGameState = GAME;
	 

	ChangeMusic(iLevelMusic[iLevel]);
	StartMusic();

 

	Number->Clear();
	World->Release3DStuff();
	ReleaseBG();
	LoadLevel(iLevel);

	World->Setup3DStuff(bLevelNight[iLevel]);
	LoadBG( bLevelNight[iLevel]);

	pX = 820.0f;
	pY = 300.0f;

	Player1->m_iScore = 0;
	Player1->m_iHealth = 100;
	Player1->m_iLife = 3;

	Player1->SetXY_CXCY(pX,pY);

	bPause = false;




}


void CMySDLApplication::Respawn()
{
	bPause = false;

	Player1->m_iHealth = 100;
	Player1->Reset();
	Player1->SetXY(Player1->m_fCX,Player1->m_fCY);

	
	for(int i = 0; i < 100; i++) //update player 2sec
		Player1->Update();



}
//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//	   the scene.
//-----------------------------------------------------------------------------
bool CMySDLApplication::FrameMove()
{
	  static float m_fFrameDelta = 0.0f;
	
	// TODO: update world
	
	// Update user input state
	UpdateInput( &m_UserInput );
	
	m_fFrameDelta += m_fElapsedTime;

/*	static int updates = 0;
	static int frames = 0;
	static float sinceLast = 0;
	sinceLast += m_fElapsedTime;
	if (sinceLast>3) {
		printf("%d updates in %f sec, %d fps\n",updates,sinceLast,int(updates/sinceLast));
		printf("%d frames in %f sec, %d fps\n",frames,sinceLast,int(frames/sinceLast));
		updates = 0;
		frames = 0;
		sinceLast = 0;
	}
	frames++;
*/
	if(m_fFrameDelta > 0.02f) // Okey, we're at 50FPS now
	{
//		updates++;
//		m_fFrameDelta = 0.0f;
		m_fFrameDelta -= 0.02f;
		if (m_fFrameDelta>0.02f) m_fFrameDelta = 0;

		//Allways update
		UpdateFade();
		UpdateMusic();
		Number->UpdateFadeNumbers();
		

		//Exit control (Allways in use!)
		if(iGameState == GAME)
		{
			if( m_UserInput.bEsc )
			{
				if(iNextState != 2)
				{
				SaveStoredData();
				StopMusic();
				Menu1->Reset();
				bPause = true;
				iNextState = 2; 
	            iFadeOut = 255;
				iFadeIn = 255;
				}
			}
		}
		else if(iGameState == MENU)
		{	
			if( m_UserInput.bEsc ) Menu1->Escape();
			if( Menu1->GetState() == EXIT_GAME )
			{
		
				SDL_Event event;
				event.type = SDL_QUIT;
				SDL_PushEvent(&event);
			}
		}

		


		//Now check gamestate
		if(!bPause)
		if(iGameState == GAME)
		{
			
			if(iShowCheck > 0) iShowCheck--;

				if( m_UserInput.bRotateRight )
				{
					Player1->PlayerAction(WALK_RIGHT);
					Player1->SetFacingRight(true);
				} 
				else if( m_UserInput.bRotateLeft )
				{
					Player1->PlayerAction(WALK_LEFT);
					Player1->SetFacingRight(false);
				} 
				
				if( m_UserInput.bRotateUp  )
				{
					Player1->PlayerAction(JUMP);
				} 
				else if ( m_UserInput.bRotateDown )
				{
					Player1->PlayerAction(DUCK);
				}

				if( m_UserInput.bRun ) Player1->PlayerAction(RUN);

				
						
			

				Player1->Update();

				//Fixx0r teh player verticles
			
				if(Player1->m_fX > 100 && Player1->m_fX < World->m_iWorldMapWidth * 32 - 500 )
				{
					if( -(Player1->m_fX - 600) < tempX) tempX = -(Player1->m_fX - 600.0f) ;
					if( -(Player1->m_fX - 480) > tempX) tempX = -(Player1->m_fX - 480.0f) ;
				}
					//tempX =  -(Player1->m_fX - 520.0f);


				Player1->Animation();

				//Enemies
				Enemies->UpdateEnemies( float(int(Player1->m_fX) >> 8 << 8));
				Enemies->AnimateEnemies();
				
				int iCollision = Enemies->CheckCollision( Player1 );

				if( iCollision == 10 && !Player1->m_iFlash) Player1->Flash();
				
				else if( iCollision == 5 )
				{
					Player1->ForceJump();
				}
		 


				if (Player1->m_fY < -64 || Player1->m_iHealth <= 0 ) //Dead
				{
					
					Player1->m_iLife--;

					
					Number->AddFadeNumber(-100,true);
					bPause = true;
					iFadeOut = 255;
					iFadeIn = 255;
					iNextState = 4;

					if(Player1->m_iLife < 0)
					{
						StopMusic();
						iNextState = 5; //Game Over!
						SaveStoredData();
				
						Menu1->Reset();
						bPause = true;
						iFadeOut = 255;
						iFadeIn = 255;
					}
					else
					{
						ReloadEnemies( iLevel );

					}
				}


				if (bChangeLevel)
				{
					StopMusic();
					bChangeLevel = false;
					bPause = true;
					iFadeOut = 255;
					iFadeIn = 255;

					if(iLevel == 15)
					{
						iNextState = 7; //game won!
						iExtraTimer = 300;
					}

					else iNextState = 6;

				/*	SaveStoredData();
					StopMusic();
					Menu1->Reset();
					bPause = true;
					iNextState = 2; 
					iFadeOut = 255;
					iFadeIn = 255;*/

				}

				//Update Stat
				if( Player1->m_iScore > iHiScore )
					iHiScore = Player1->m_iScore;

			
		}
		else if(iGameState == MENU)
		{
						
				if( m_UserInput.bRotateUp  )
				{
					Menu1->MoveUp();
					
				} 
				else if ( m_UserInput.bRotateDown )
				{
					Menu1->MoveDown();
				}
				
				if ( m_UserInput.bEnter ) Menu1->Enter();
			

				if( Menu1->GetState() == START_GAME )
				{			
					iLevel = 0;
					iNextState = 1; //start game
					iFadeOut = 255;
					iFadeIn = 255;
					bPause = true;
				}

				if( Menu1->GetState() == START_GAME_LEV5 )
				{	
					iLevel = 5;
					iNextState = 1; //start game
					iFadeOut = 255;
					iFadeIn = 255;
					bPause = true;
				}
			
				if( Menu1->GetState() == START_GAME_LEV10 )
				{	
					iLevel = 10;
					iNextState = 1; //start game
					iFadeOut = 255;
					iFadeIn = 255;
					bPause = true;
				}

				if( Menu1->GetState() == START_GAME_LEV15 )
				{		
					iLevel = 15;
					iNextState = 1; //start game
					iFadeOut = 255;
					iFadeIn = 255;
					bPause = true;
				}
			
			if( Menu1->GetState() == EXIT_GAME)
			{
				SDL_Event event;
				event.type = SDL_QUIT;
				SDL_PushEvent(&event);
			}

				Menu1->Update();
		}
		else if(iGameState == SHOW_SCREEN)
		{
			iShowScreenTimer--;

			if(iShowScreen == 4)
			{
				if(iShowScreenTimer < 100 && iShowScreenTimer > 50 && iExtraTimer > 0)
				{
					iShowScreenTimer++;
					iExtraTimer--;
				}
			}

			if(iShowScreenTimer < 0)
				if(iShowScreen == 2) //Next Level
				{
					bPause = true;
					iFadeOut = 0;
					iFadeIn = 255;

					iNextState = 3;
				}
				else if(iShowScreen == 3) //Gameover
				{

					SaveStoredData();
				
					Menu1->Reset();
					bPause = true;
					iFadeOut = 0;
					iFadeIn = 255;

					iNextState = 2;
				}
				else if(iShowScreen == 4) //Gameover
				{

					SaveStoredData();
				
					Menu1->Reset();
					bPause = true;
					iFadeOut = 0;
					iFadeIn = 255;

					iNextState = 2;
				}
		}
		else //We're not at the menu an not in the game, so we're just at startup
		{
/*			
			UserSelectNewDevice(); //Show Select Device Screen

			if(m_bCancelPressed) SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
			else 
			{
				StartMusic();
				UpdateMusic();
				Menu1->m_iCheckPoint = iCheckPoint;
				iGameState = MENU;
				if(bLowRes) LOG("DEBUG: Game Started using LowRes textures\n");
				else LOG("DEBUG: Game Started using HiRes textures\n");
			}
*/
			StartMusic();
			UpdateMusic();
			Menu1->m_iCheckPoint = iCheckPoint;
			iGameState = MENU;
			if(bLowRes) LOG("DEBUG: Game Started using LowRes textures\n");
			else LOG("DEBUG: Game Started using HiRes textures\n");
		}
	}

	return true;
}




//-----------------------------------------------------------------------------
// Name: UpdateInput()
// Desc: Update the user input.  Called once per frame 
//-----------------------------------------------------------------------------
void CMySDLApplication::UpdateInput( UserInput* pUserInput )
{

	// Get the input's device state, and put the state in dims

	Uint8 *keys = SDL_GetKeyState(NULL);
	
	pUserInput->bRotateLeft  = keys[SDLK_LEFT];
	pUserInput->bRotateRight = keys[SDLK_RIGHT];
	pUserInput->bRotateUp    = keys[SDLK_UP];
	pUserInput->bRotateDown  = keys[SDLK_DOWN];
	pUserInput->bEnter =	   keys[SDLK_RETURN];
	pUserInput->bRun =		   keys[SDLK_RSHIFT];
	pUserInput->bEsc =		   keys[SDLK_ESCAPE];

	if(bDisallowExit == true)
		bDisallowExit = pUserInput->bEsc;

	if(!pUserInput->bRun) pUserInput->bRun = keys[SDLK_LSHIFT];
}





void CMySDLApplication::LoadBG( bool bNight )
{

	Pack->OpenPackFile("Data/Data5.pak");


 
	if(!bLowRes)
	{
		if(bNight)
			bg = loadTextureFromFP(Pack->GetFPByName("background2.tga"), Pack->GetSizeByName("background2.tga"), 0, 0);
		else
			bg = loadTextureFromFP(Pack->GetFPByName("background1.tga"), Pack->GetSizeByName("background1.tga"), 0, 0);
	}
	else
	{
		if(bNight)
			bg = loadTextureFromFP(Pack->GetFPByName("background2.tga"), Pack->GetSizeByName("background2.tga"), 256, 256);
		else
			bg = loadTextureFromFP(Pack->GetFPByName("background1.tga"), Pack->GetSizeByName("background1.tga"), 256, 256);
	
	
		
	}

	Pack->ClosePackFile(); 

}

void CMySDLApplication::ReleaseBG()
{
	if (bg)
	{
		glDeleteTextures(1,&bg);
		bg=0;
	}

}


void CMySDLApplication::UpdateFade()
{
	if(iFadeOut < 256) iFadeOut-=8;
	if(iFadeOut < -20)
	{
		iFadeOut = 256;

		if(iNextState == 1)//Meaning we're starting the game
		{
			iGameState = GAME;
			StartGame();
		}

		if(iNextState == 2)
		{
			Menu1->m_iCheckPoint = iCheckPoint;
			ChangeMusic(0);
			StartMusic();
			iGameState = MENU;
			bPause = false;
		}

		if(iNextState == 3)
		{
			iGameState = GAME;
			ChangeLevel();
		}

		if(iNextState == 4)
		{
			iGameState = GAME;
			Respawn();
		}

		if(iNextState == 5)
		{
			playSound(sndGameOver);
			iGameState = SHOW_SCREEN;
			iShowScreen = 3;
			iShowScreenTimer = 150;
			bPause = false;
		}

		if(iNextState == 6)
		{
			playSound(sndLevelCompleted);
			iGameState = SHOW_SCREEN;
			iShowScreen = 2;
			iShowScreenTimer = 150;
			bPause = false;
		}

		if(iNextState == 7)
		{
			playSound(sndGameWon);
			iGameState = SHOW_SCREEN;
			iShowScreen = 4;
			iShowScreenTimer = 150;
			bPause = false;
		}

		iNextState = 0;

	}



	if(iFadeOut == 256)
	{
		if(iFadeIn < 256) iFadeIn-=8;
		if(iFadeIn <= 0) iFadeIn = 256;
	}

}
void CMySDLApplication::RenderFade()
{

	
	if(iFadeOut < 256)
	{
			
		int iTempFadeOut = iFadeOut;

		if(iTempFadeOut < 0) iTempFadeOut = 0;

		glDisable(GL_TEXTURE_2D);
		GLuint colorO = 0x00FFFFFF + ((255 - iTempFadeOut) << 24);
		RenderQuad(0,0,640,480,colorO);
	}
	else if(iFadeIn < 256)
	{
		glDisable(GL_TEXTURE_2D);
		GLuint colorI = 0x00FFFFFF + (iFadeIn << 24);
		RenderQuad(0,0,640,480,colorI);
			
	}
				
		
			
}
//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//	   rendering. This function sets up render states, clears the
//	   viewport, and renders the scene.
//-----------------------------------------------------------------------------
bool CMySDLApplication::Render()
{

	// Clear the viewport
	glClearColor(0,0,0,0);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,640,480,0,1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

//	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	
	
	if(iGameState == GAME)
	{

		//TEMP

		//Bg
		glBindTexture(GL_TEXTURE_2D, bg);
		glEnable(GL_TEXTURE_2D);
		RenderQuad(0,0,640,480,0xFFFFFFFF);

		//Bg World
		World->RenderWorld(tempX,0);

		//Render World
		World->RenderWorld(tempX,1);

		
		
		//player, world is not rotated right

		Enemies->RenderEnemies();
		Player1->Render();

		World->RenderWorld(tempX,2);



		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,640,480,0,1,100);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if(bLevelNight[iLevel])
		{
			glDisable(GL_TEXTURE_2D);
			RenderQuad(0,0,640,480,0x44000000);
		}
	
		// Render stats and help text  

		Number->DrawFadeNumbers();
		
		Number->DrawProcentNumber(545,4,Player1->m_iHealth,0);
		Number->DrawNumber(4,8,Player1->m_iScore,0);
		Number->DrawNumberSmall(8,38,iHiScore,2);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, life);
		for(int i = 0; i < Player1->m_iLife; i++)
			RenderQuad(590-((i%8)*24),32+(i/8)*16,48,48,0xFFFFFFFF);

		if(iShowCheck > 0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texCheck);
			RenderQuad(192,112,256,256,0xFFFFFFFF);
		}
	}
	else if( iGameState == MENU )
	{
		Menu1->Render();
	}
	else if( iGameState == SHOW_SCREEN)
	{
		float mX = 0.0f;

		glDisable(GL_TEXTURE_2D);
		RenderQuad(0,0,640,480,0xFFFFFFFF);

		if(iShowScreen == 2)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texText);

			if(iShowScreenTimer > 100)
			{
				mX = (576 / 50) * (iShowScreenTimer - 100);
				RenderQuadEx(64 + mX,112,512,256,0,0,1,0.5f,0xFFFFFFFF);
			}
			else if(iShowScreenTimer < 50)
			{
				mX = (576 / 50) * (50 - iShowScreenTimer);
				RenderQuadEx(64 - mX,112,512,256,0,0,1,0.5f,0xFFFFFFFF);
			}
			else RenderQuadEx(64,112,512,256,0,0,1,0.5f,0xFFFFFFFF);
		}
		else if(iShowScreen == 3)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texText);

			if(iShowScreenTimer > 100)
			{
				mX = (576 / 50) * (iShowScreenTimer - 100);
				RenderQuadEx(64 + mX,112,512,256,0,0.5f,1,1,0xFFFFFFFF);
			}
			else if(iShowScreenTimer < 50)
			{
				mX = (576 / 50) * (50 - iShowScreenTimer);
				RenderQuadEx(64 - mX,112,512,256,0,0.5f,1,1,0xFFFFFFFF);
			}
			else RenderQuadEx(64,112,512,256,0,0.5f,1,1,0xFFFFFFFF);
		}

		else if(iShowScreen == 4)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texVictory);

			if(iShowScreenTimer > 100)
			{
				mX = (560 / 50) * (iShowScreenTimer - 100);
				RenderQuadEx(80 + mX,0,480,480,0,0,1,1,0xFFFFFFFF);
			}
			else if(iShowScreenTimer < 50)
			{
				mX = (560 / 50) * (50 - iShowScreenTimer);
				RenderQuadEx(80 - mX,0,480,480,0,0,1,1,0xFFFFFFFF);
			}
			else RenderQuadEx(80,0,480,480,0,0,1,1,0xFFFFFFFF);
		}
	}

	//Allways draw

	RenderFade();

		
	return true;
}







//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Overrrides the main WndProc, so the sample can do custom message
//	   handling (e.g. processing mouse, keyboard, or menu commands).
//-----------------------------------------------------------------------------
void CMySDLApplication::handleEvent(const SDL_Event &event) {
/*	switch( msg )
	{
	    case WM_PAINT:
	    {
	        if( m_bLoadingApp )
	        {
				
				ShowWindow(hWnd, SW_HIDE);
	        }
	        break;
	    }

	}
*/
	return CSDLApplication::handleEvent(event);
}





//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Invalidates device objects.  
//-----------------------------------------------------------------------------
bool CMySDLApplication::InvalidateDeviceObjects()
{
	// TODO: Cleanup any objects created in RestoreDeviceObjects()
	
	//Here I released the textures
	ReleaseBG();

	if (texText)
	{
		glDeleteTextures(1,&texText);
		texText=0;
	}


	if (life)
	{
		glDeleteTextures(1,&life);
		life=0;
	}

	
	if (texCheck)
	{
		glDeleteTextures(1,&texCheck);
		texCheck=0;
	}


	
	if (texVictory)
	{
		glDeleteTextures(1,&texVictory);
		texVictory=0;
	}

	Menu1->ReleaseTextures();

	Player1->ReleaseTextures();

	World->Release3DStuff();
	Enemies->ReleaseTextures();
	return true;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//	   this function deletes any device dependent objects.  
//-----------------------------------------------------------------------------
bool CMySDLApplication::DeleteDeviceObjects()
{
	// TODO: Cleanup any objects created in InitDeviceObjects()

	ReleaseBG();

	if (texText)
	{
		glDeleteTextures(1,&texText);
		texText=0;
	}


	if (life)
	{
		glDeleteTextures(1,&life);
		life=0;
	}

	
	if (texCheck)
	{
		glDeleteTextures(1,&texCheck);
		texCheck=0;
	}


	
	if (texVictory)
	{
		glDeleteTextures(1,&texVictory);
		texVictory=0;
	}

	Menu1->ReleaseTextures();
	Player1->ReleaseTextures();

	World->Release3DStuff();
	Enemies->ReleaseTextures();
		
	Number->ReleaseTextures();

	return true;
}




//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//	   to cleanup after itself.
//-----------------------------------------------------------------------------
bool CMySDLApplication::FinalCleanup()
{

	World->CleanUp();
	Player1->CleanUp();	
	Enemies->CleanUp();

#ifdef USE_FMOD
	FSOUND_Sample_Free(sndMusicStart);
	FSOUND_Sample_Free(sndMusic);
	FSOUND_Sample_Free(sndJump);
	FSOUND_Sample_Free(sndEnemy);
	FSOUND_Sample_Free(sndCannon);
	FSOUND_Sample_Free(sndWater);
	FSOUND_Sample_Free(sndHit);
	FSOUND_Sample_Free(sndCoin);
	FSOUND_Sample_Free(sndCheck);
	FSOUND_Sample_Free(sndLC);

	FSOUND_Sample_Free(sndSelect);
	FSOUND_Sample_Free(sndBack);

	FSOUND_Sample_Free(sndLevelCompleted);
	FSOUND_Sample_Free(sndGameOver);

	FSOUND_Sample_Free(sndGameWon);

	FSOUND_Close();
#else
#ifndef HAVE_MUS_RW
	Mix_FreeChunk(sndMusicStart);
	Mix_FreeChunk(sndMusic);
#else
	Mix_FreeMusic(sndMusicStart);
	Mix_FreeMusic(sndMusic);
#endif
	Mix_FreeChunk(sndJump);
	Mix_FreeChunk(sndEnemy);
	Mix_FreeChunk(sndCannon);
	Mix_FreeChunk(sndWater);
	Mix_FreeChunk(sndHit);
	Mix_FreeChunk(sndCoin);
	Mix_FreeChunk(sndCheck);
	Mix_FreeChunk(sndLC);

	Mix_FreeChunk(sndSelect);
	Mix_FreeChunk(sndBack);

	Mix_FreeChunk(sndLevelCompleted);
	Mix_FreeChunk(sndGameOver);

	Mix_FreeChunk(sndGameWon);

	Mix_CloseAudio();
#endif


	// Write the settings to the registry

	return true;
}






void CMySDLApplication::RenderQuad(float x, float y, float w, float h, GLuint color)
{
	float z = -10;

	glColor4ub((color>>16)&0xFF,(color>>8)&0xFF,color&0xFF,(color>>24)&0xFF);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(x,y,z);
	glTexCoord2f(1,0); glVertex3f(x+w,y,z);
	glTexCoord2f(1,1); glVertex3f(x+w,y+h,z);
	glTexCoord2f(0,1); glVertex3f(x,y+h,z);
	glEnd();
}

void CMySDLApplication::RenderQuadEx(float x, float y, float w, float h, float u1, float v1, float u2, float v2, GLuint color)
{
	float z = -10;

	glColor4ub((color>>16)&0xFF,(color>>8)&0xFF,color&0xFF,(color>>24)&0xFF);
	glBegin(GL_QUADS);
	glTexCoord2f(u1,v1); glVertex3f(x,y,z);
	glTexCoord2f(u2,v1); glVertex3f(x+w,y,z);
	glTexCoord2f(u2,v2); glVertex3f(x+w,y+h,z);
	glTexCoord2f(u1,v2); glVertex3f(x,y+h,z);
	glEnd();
}

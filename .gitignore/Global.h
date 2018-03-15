#ifndef GLOBAL_H
#define GLOBAL_H

#include "SDL/SDL.h"
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
#include <math.h>

#ifndef M_PI
#define M_PI 3.141592654
#endif


#ifdef USE_FMOD
#include "fmod.h"
#else
#define USE_RWOPS
#include "SDL/SDL_mixer.h"
#endif


#include "Log.h"
#include "PackManager.h"

#define GAME		10
#define MENU		5
#define START		0
#define SHOW_SCREEN	15

#define SOLID 16
#define SEMI_SOLID 40
#define NOT_SOLID 64

#define WATER	54

#define RADIE (836-64) 

#define D3DFVF_QUADVERT (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE)

#define GRAVITY			1.0f
#define WATER_PULL_UP	0.7f

extern bool bLowRes;

struct QUADVERT
{
	// GL_T2F_C4UB_V3F
	GLfloat u,v;
	GLuint diffuse;
	GLfloat x,y,z;
};

#define drawColorQuadVert(q)	\
	glColor4ub(((q).diffuse>>16)&0xFF,((q).diffuse>>8)&0xFF,(q).diffuse&0xFF,((q).diffuse>>24)&0xFF);			\
	glTexCoord2f((q).u,(q).v);		\
	glVertex3f((q).x,(q).y,(q).z);

#define setQuadVertColor(q)	\
	glColor4ub(((q).diffuse>>16)&0xFF,((q).diffuse>>8)&0xFF,(q).diffuse&0xFF,((q).diffuse>>24)&0xFF);
	
#define drawQuadVert(q)		\
	glTexCoord2f((q).u,(q).v);		\
	glVertex3f((q).x,(q).y,(q).z);

#define drawQuadVerts(q,a,b,c,d) {	\
	setQuadVertColor((q)[a])	\
	glBegin(GL_QUADS);		\
	drawQuadVert((q)[a]);		\
	drawQuadVert((q)[b]);		\
	drawQuadVert((q)[c]);		\
	drawQuadVert((q)[d]);		\
	glEnd(); }

//Sound
#ifdef USE_FMOD
#define SOUND_TYPE FSOUND_SAMPLE
#define playSound(x) FSOUND_PlaySound(FSOUND_FREE,x)
#else
#define SOUND_TYPE Mix_Chunk
#define playSound(x) Mix_PlayChannel(-1,x,0)
#endif

extern SOUND_TYPE *sndJump;
extern SOUND_TYPE *sndEnemy;
extern SOUND_TYPE *sndCannon;
extern SOUND_TYPE *sndWater;
extern SOUND_TYPE *sndHit;
extern SOUND_TYPE *sndCoin;
extern SOUND_TYPE *sndCheck;
extern SOUND_TYPE *sndLC;
extern SOUND_TYPE *sndSelect;
extern SOUND_TYPE *sndBack;



extern bool bNight;
extern bool bChangeLevel;

#endif

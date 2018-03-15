#include <stdlib.h>
#include <string.h>
#include "sdlapp.h"
#include "Menu.h"
#include "NumberDraw.h"
#include "EnemyManager.h"
#include "PackManager.h"
#include "WorldMap.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/OpenGL.h>

extern "C" {
void showGui(int *width, int *height, int *bitdepth, int *lowres, int *windowed, int *multisamples);
}
#endif

void my_quit(void)
{
	Menu1->CleanUp();
	Number->CleanUp();
	Enemies->CleanUp();
	Pack->CleanUp();
	World->CleanUp();

	SDL_Quit();
}

CSDLApplication::CSDLApplication(int argc, char *argv[]) {
	m_bWindowed = false;
	m_dwCreationWidth = 1024;
	m_dwCreationHeight = 768;
	m_strWindowTitle = "SDL/GL Application";
	m_bLowRes = false;
	m_dwBitDepth = 0;
	m_dwMultisample = 0;

	m_fLastTime = float(SDL_GetTicks())/1000;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-s") && i+1<argc) {
			sscanf(argv[i+1],"%dx%dx%d",&m_dwCreationWidth,&m_dwCreationHeight,&m_dwBitDepth);
			i++;
		} else if (!strcmp(argv[i],"-m") && i+1<argc) {
			m_dwMultisample = atoi(argv[i+1]);
			i++;
		} else if (!strcmp(argv[i],"-l")) {
			m_bLowRes = true;
		} else if (!strcmp(argv[i],"-w")) {
			m_bWindowed = true;
		} else if (!strcmp(argv[i],"-h")) {
			printf("%s: [-h] [-l] [-w] [-s WxH[xD]] [-m n]\n",argv[0]);
			printf("\n");
			printf("\t-h\tshows this help\n");
			printf("\t-l\tuse lower resolution textures\n");
			printf("\t-w\trun windowed\n");
			printf("\t-s\tselect display resolution and bitdepth\n");
			printf("\t-m\tuse multisampling with n samples\n");
			printf("\n");

			if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE)<0) {
				fprintf(stderr,"%s\n",SDL_GetError());
				exit(1);
			}

			SDL_Rect **modes = NULL;
			modes = SDL_ListModes(NULL, SDL_OPENGL | SDL_FULLSCREEN);
			if (modes == (SDL_Rect**)0) {
				printf("No fullscreen modes available!\n");
			} else if (modes == (SDL_Rect**)-1) {
				// all resolutions available
			} else {
				printf("Available fullscreen resolutions:\n");
				for (int i = 0; modes[i]; i++) {
					if (i==0 || modes[i]->w!=modes[i-1]->w || modes[i]->h!=modes[i]->h)
						printf("\t%dx%d\n",modes[i]->w,modes[i]->h);
				}
			}
			SDL_Quit();
			exit(0);
		}

	}

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE)<0) {
		fprintf(stderr,"%s\n",SDL_GetError());
		exit(1);
	}
	if (atexit(my_quit)) {
		fprintf(stderr,"atexit(...)\n");
		SDL_Quit();
		exit(1);
	}

#ifdef SDLGUI
	int lowres, windowed;
	showGui(&m_dwCreationWidth, &m_dwCreationHeight, &m_dwBitDepth, &lowres, &windowed, &m_dwMultisample);
	m_bLowRes = lowres;
	m_bWindowed = windowed;
#else

	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	SDL_PixelFormat fmt = *(info->vfmt);
	SDL_Rect **modes = NULL;
	if (m_dwBitDepth>0) {
		fmt.BitsPerPixel = m_dwBitDepth;
		modes = SDL_ListModes(&fmt, SDL_OPENGL | (m_bWindowed?0:SDL_FULLSCREEN));
	} else {
		modes = SDL_ListModes(NULL, SDL_OPENGL | (m_bWindowed?0:SDL_FULLSCREEN));
	}
	if (modes == (SDL_Rect**)0) {
		fprintf(stderr,"No video modes available!\n");
		exit(0);
	} else if (modes == (SDL_Rect**)-1) {
		// all resolutions available
	} else {
		int best = 0;
		unsigned int bestDiff = (unsigned int)-1;
		for (int i = 0; modes[i]; i++) {
			unsigned int diff = abs(modes[i]->w-m_dwCreationWidth)+abs(modes[i]->h-m_dwCreationHeight);
			if (diff<bestDiff) {
				best = i;
				bestDiff = diff;
			}
		}
		m_dwCreationWidth = modes[best]->w;
		m_dwCreationHeight = modes[best]->h;
	}
#endif
}


bool CSDLApplication::Create() {
	OneTimeSceneInit();
	Initialize3DEnvironment();
	InitDeviceObjects();
	RestoreDeviceObjects(m_bLowRes);
	m_fLastTime = float(SDL_GetTicks())/1000;
	return true;
}

void CSDLApplication::handleEvent(const SDL_Event &event) {
}

void CSDLApplication::Initialize3DEnvironment() {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,0);
//	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
	if (m_dwMultisample>1) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,m_dwMultisample);
	}
	SDL_WM_SetCaption(m_strWindowTitle,m_strWindowTitle);
	m_screen = SDL_SetVideoMode(m_dwCreationWidth,m_dwCreationHeight,m_dwBitDepth,SDL_OPENGL|(m_bWindowed?SDL_RESIZABLE:SDL_FULLSCREEN));
#if defined(__APPLE__) && defined(__MACH__)
	long param = 1;
	CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &param);
#endif
	SDL_ShowCursor(SDL_DISABLE);
	glViewport(0,0,m_screen->w,m_screen->h);
	if (m_dwMultisample>1)
		glEnable(GL_MULTISAMPLE_ARB);
	glDepthFunc(GL_LEQUAL);
//	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glGetError(); // clear error flag
}

void CSDLApplication::Render3DEnvironment() {
	m_fTime = float(SDL_GetTicks())/1000;
	m_fElapsedTime = m_fTime - m_fLastTime;
	m_fLastTime = m_fTime;
	FrameMove();
	Render();
	SDL_GL_SwapBuffers();
#if !defined(__APPLE__) || !defined(__MACH__)
	SDL_Delay(1);
#endif
/*
	static float lastTime = 0;
	if (lastTime < 0.001) lastTime = m_fTime;
	static int frames = 0;
	frames++;
	if (m_fTime-lastTime > 1) {
		printf("%d fps\n", int(frames/(m_fTime-lastTime)));
		lastTime = m_fTime;
		frames = 0;
	}
*/
}

int CSDLApplication::Run() {
	while (true) {
		SDL_Event event;
		bool quit = false;
		while (SDL_PollEvent(&event)) {
			if (event.type==SDL_QUIT) {
				quit = true;
				break;
			} else if (event.type==SDL_VIDEORESIZE) {
				char name[50];
				SDL_VideoDriverName(name,sizeof(name));
				if (!strcmp(name,"x11") || !strcmp(name,"Quartz")) {
					m_screen = SDL_SetVideoMode(event.resize.w,event.resize.h,m_screen->format->BitsPerPixel,SDL_OPENGL|(m_bWindowed?SDL_RESIZABLE:SDL_FULLSCREEN));
					if (!strcmp(name,"Quartz")) {
						InvalidateDeviceObjects();
						RestoreDeviceObjects(m_bLowRes);
					}
				} else {
					m_screen->w = event.resize.w;
					m_screen->h = event.resize.h;
				}
				glViewport(0,0,m_screen->w,m_screen->h);
			} else {
				handleEvent(event);
			}
		}
		if (quit) break;
		Render3DEnvironment();
	}
	return 0;
}


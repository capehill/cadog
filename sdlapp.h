#ifndef __SDLAPP_H
#define __SDLAPP_H


#include <SDL/SDL.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif


class CSDLApplication {
protected:

	void Render3DEnvironment();
	void Initialize3DEnvironment();


	SDL_Surface *m_screen;

	bool	m_bWindowed;
	int	m_dwCreationWidth;
	int	m_dwCreationHeight;
	int	m_dwBitDepth;
	int	m_dwMultisample;
	char	*m_strWindowTitle;
	bool	m_bLowRes;
	float	m_fTime;
	float	m_fElapsedTime;
	float	m_fLastTime;

//	virtual bool ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT)	{ return true; }
	virtual bool OneTimeSceneInit()							{ return true; }
	virtual bool InitDeviceObjects( )						{ return true; }
	virtual bool RestoreDeviceObjects(bool bUseLowRes)		{ return true; }
	virtual bool FrameMove()				{ return true; }
	virtual bool Render()					{ return true; }
	virtual bool InvalidateDeviceObjects()	{ return true; }
	virtual bool DeleteDeviceObjects()		{ return true; }
	virtual bool FinalCleanup()		{ return true; }
	virtual void BeginNew()			{ return; }
	virtual bool DisableMusic()		{ return true; }
	virtual bool ToggleFPS()		{ return true; }
	virtual	void Cheat(int a)		{ return; }
public:


	 
	bool	m_bCancelPressed;
	// Functions to create, run, pause, and clean up the application
	virtual bool	Create();
	virtual int	Run();
//	virtual LRESULT	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void	handleEvent(const SDL_Event &event);
//	virtual void	Pause(bool bPause);

	bool UserSelectNewDevice();

	// Internal constructor
	CSDLApplication(int argc, char *argv[]);
};



#endif

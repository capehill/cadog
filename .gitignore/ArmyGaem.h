//-----------------------------------------------------------------------------
// File: ArmyGaem.h
//
// Desc: Header file ArmyGaem sample app
//-----------------------------------------------------------------------------
#ifndef AFX_ARMYGAEM_H__32E88440_10AC_4679_9DF7_F2CD29E9F8E9__INCLUDED_
#define AFX_ARMYGAEM_H__32E88440_10AC_4679_9DF7_F2CD29E9F8E9__INCLUDED_




// Struct to store the current input state
struct UserInput
{
//	Uint8 keys[256];   // DirectInput keyboard state buffer 

	// TODO: change as needed
	bool bRotateUp;
	bool bRotateDown;
	bool bRotateLeft;
	bool bRotateRight;
	bool bRun;
	bool bEnter;
	bool bEsc;
};




//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//	   generic functionality needed in all Direct3D samples. CMyD3DApplication 
//	   adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMySDLApplication : public CSDLApplication
{
	bool                    m_bLoadingApp;          // TRUE, if the app is loading

	UserInput               m_UserInput;            // Struct for storing user input 

	float                   m_fWorldRotX;           // World rotation state X-axis
	float                   m_fWorldRotY;           // World rotation state Y-axis

protected:
	bool OneTimeSceneInit();
	bool InitDeviceObjects();
	bool RestoreDeviceObjects( bool bUseLowRes );
	bool InvalidateDeviceObjects();
	bool DeleteDeviceObjects();
	bool Render();
	bool FrameMove();
	bool FinalCleanup();

	bool RenderText();

	void    UpdateInput( UserInput* pUserInput );
	void    CleanupDirectInput();

	void    ReadSettings();
	void    WriteSettings();
	
	

public:
	void handleEvent(const SDL_Event &event);
	CMySDLApplication(int argc, char *argv[]);

	void LoadBG( bool bNight );
	void ReleaseBG();

	void LoadLevel( int iLevel );

	void StartGame();
	void ChangeLevel();
	void Respawn();

	void RenderFade();
	void UpdateFade();

	void ReloadEnemies( int iLevel );
	void RenderQuad(float x, float y, float w, float h, GLuint color);
	void RenderQuadEx(float x, float y, float w, float h, float u1, float v1, float u2, float v2, GLuint color);
};


#endif // !defined(AFX_ARMYGAEM_H__32E88440_10AC_4679_9DF7_F2CD29E9F8E9__INCLUDED_)

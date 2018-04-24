////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "terrainshaderclass.h"
#include "lightclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"

///
#include "rendertextureclass.h"
#include "convolutionshaderclass.h"
#include "orthowindowclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();

	// Post Processing
	bool RenderToTexture();
	bool DownScaleTexture();
	bool ApplyPostProcessing();
	bool UpScaleTexture();
	bool RenderScene();		// Needs Removal
	bool RenderWindow();	// Needs Reworking

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	TerrainShaderClass* m_TerrainShader;
	LightClass* m_Light;
	ModelClass* m_Cube;
	TextureShaderClass* m_TextureShader;

	// Post Processing
	ConvolutionShaderClass* m_ConvolutionShader;

	OrthoWindowClass* m_SmallWindow, *m_BigWindow;
	RenderTextureClass* m_RenderTexture, *m_DownSampleTexture, *m_PostProcessTexture, *m_UpSampleTexture;

	float m_ScreenHeight = 0.0f;
	float m_ScreenWidth = 0.0f;
};

#endif
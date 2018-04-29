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

/// Post Prosessing
#include "basicshaderclass.h"
#include "convolutionshaderclass.h"
#include "rendertextureclass.h"
#include "orthowindowclass.h"
/// 

/// Skydome
#include "skydomeclass.h"
#include "skydomeshaderclass.h"
//#include "NoiseTextureClass.h"
///


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
	bool RenderSceneToTexture();
	bool DownScaleTexture();
	bool RenderConvolutionToTexture();
	bool UpScaleTexture();
	bool RenderScene();
	bool Render2DTextureScene();

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
	BasicShaderClass* m_BasicShader;
	ConvolutionShaderClass* m_ConvolutionShader;

	OrthoWindowClass* m_SmallWindow, *m_FullScreenWindow;
	RenderTextureClass* m_RenderTexture, *m_DownSampleTexture, *m_ConvolutionTexture, *m_UpSampleTexture;

	// A flag used in input processing to allow post processing to be "turned on and off."
	bool is_postProcessed = false;

	// Sky Dome
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	//NoiseTextureClass* m_NoiseTexture;
};

#endif
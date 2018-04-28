////////////////////////////////////////////////////////////////////////////////
// Filename: noisetextureclass.h
////////////////////////////////////////////////////////////////////////////////
// A class that generates a texture using Improved Perlin Noise.

#ifndef _NOISE_TEXTURE_CLASS_
#define _NOISE_TEXTURE_CLASS_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>

#include "improvednoise.h"

/////////////////
// DEFINITIONS //
/////////////////
#define _TEXTURE_WIDTH_ 256
#define _TEXTURE_HEIGHT_ 256

////////////////////////////////////////////////////////////////////////////////
// Class name: NoiseTextureClass
////////////////////////////////////////////////////////////////////////////////
class NoiseTextureClass
{
public:
	NoiseTextureClass();
	NoiseTextureClass(const NoiseTextureClass&);
	~NoiseTextureClass();

	bool Initialize(ID3D11Device*/*, WCHAR**/);
	void Shutdown();

	ID3D11ShaderResourceView* GetTextureVeiw();
	ID3D11Texture2D* GetTexture();

private:

	int m_texturedata[_TEXTURE_HEIGHT_][_TEXTURE_WIDTH_];
	ID3D11ShaderResourceView* m_textureveiw;
	ID3D11Texture2D *m_texture;
	improvednoise* m_improvednoise;
};

#endif // !_NOISE_TEXTURE_CLASS_




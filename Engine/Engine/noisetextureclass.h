#ifndef _NOISE_TEXTURE_CLASS_
#define _NOISE_TEXTURE_CLASS_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: NoiseTextureClass
////////////////////////////////////////////////////////////////////////////////
class noisetextureclass
{
public:
	noisetextureclass();
	noisetextureclass(const noisetextureclass&);
	~noisetextureclass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif // !_NOISE_TEXTURE_CLASS_




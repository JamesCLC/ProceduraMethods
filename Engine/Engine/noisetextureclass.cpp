/////////////////////////////////////
// Filename: NoiseTextureClass.cpp //
////////////////////////////////////

#include "noisetextureclass.h"

NoiseTextureClass::NoiseTextureClass()
{
	m_texture = NULL;
	m_textureveiw = 0;
	m_improvednoise = 0;
}

NoiseTextureClass::NoiseTextureClass(const NoiseTextureClass& other)
{
}

NoiseTextureClass::~NoiseTextureClass()
{
}

bool NoiseTextureClass::Initialize(ID3D11Device* device/*, WCHAR* filename*/)
{
	HRESULT result;
	
	// Create data for the array.
	for (int j = 0; j < _TEXTURE_HEIGHT_; j++)			 // Nested for loop. Oh boy, this is not an optimal solution.
	{
		for (int i = 0; i < _TEXTURE_WIDTH_; i++)
		{
			m_texturedata[j][i] = m_improvednoise->Sample((double)_TEXTURE_HEIGHT_, (double)_TEXTURE_WIDTH_, 0.0);
		}
	}

	int col = 0;


	// Create the Subresource date for the texture.
	D3D11_SUBRESOURCE_DATA m_subresourcedata;

	m_subresourcedata.pSysMem = &m_texturedata;
	m_subresourcedata.SysMemPitch = NULL;
	m_subresourcedata.SysMemSlicePitch = NULL;			// Only used for 3D Textures. Edit - May need it if I want to use a 2D array, but I may need to swap to a 1d array

	int bar = 0;

	// Initialise the texture description.
	D3D11_TEXTURE2D_DESC desc;							// Code based on [https://msdn.microsoft.com/en-us/library/windows/desktop/ff476903(v=vs.85).aspx]

	desc.Height = _TEXTURE_HEIGHT_;
	desc.Width = _TEXTURE_WIDTH_;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	
	int foo = 0;

	
	// Create the texture using the description and the perlin data.
	device->CreateTexture2D(&desc, &m_subresourcedata, &m_texture);
	

	return false;
}

void NoiseTextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	// Release the texture view resource.
	if (m_textureveiw)
	{
		m_textureveiw->Release();
		m_textureveiw = 0;
	}


	// Release the array containing texture data.
	if (m_texturedata)
	{
		// Delete this?
	}

	// Release the improved noise object.
	if (m_improvednoise)
	{
		m_improvednoise->Shutdown();
		//delete m_improvednoise;
		m_improvednoise = 0;
	}
	return;
}

ID3D11ShaderResourceView * NoiseTextureClass::GetTextureVeiw()
{
	return m_textureveiw;
}

ID3D11Texture2D * NoiseTextureClass::GetTexture()
{
	return m_texture;
}


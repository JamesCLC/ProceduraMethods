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
	
	float *buf = (float *)malloc(_TEXTURE_WIDTH_ * _TEXTURE_HEIGHT_ * 4 * sizeof(float));

	for (int i = 0; i < _TEXTURE_WIDTH_ * _TEXTURE_HEIGHT_ * 4; i++)
	{
		buf[i] = 1.0f;
	}


	// Create the Subresource date for the texture.
	D3D11_SUBRESOURCE_DATA m_subresourcedata;
	m_subresourcedata.pSysMem = (void *)buf;
	m_subresourcedata.SysMemPitch = _TEXTURE_WIDTH_ * 4 * sizeof(float);
	m_subresourcedata.SysMemSlicePitch = NULL;

	int bar = 0;

	// Initialise the texture description.
	D3D11_TEXTURE2D_DESC desc;							// Code based on [https://msdn.microsoft.com/en-us/library/windows/desktop/ff476903(v=vs.85).aspx]

	desc.Height = _TEXTURE_HEIGHT_;
	desc.Width = _TEXTURE_WIDTH_;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	
	int foo = 0;

	// Create the texture using the description and the perlin data.
	if (FAILED(device->CreateTexture2D(&desc, &m_subresourcedata, &m_texture)))
	{
		return false;
	}
	
	delete[] buf;

	return true;
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



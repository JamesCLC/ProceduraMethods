////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>
#include "improvednoise.h"
///
// Additional Input Information added based on: http://www.rastertek.com/dx11ter03.html
#include "textureclass.h"
///


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	    D3DXVECTOR3 normal;
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, char*);
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool GenerateHeightMap(ID3D11Device* device, bool keydown);
	int  GetIndexCount();

	//
	bool SmoothTerrain(ID3D11Device* device, bool keydown);
	bool FlattenPeaks(ID3D11Device* device, bool keydown);

	ID3D11ShaderResourceView* GetSandTexture();
	ID3D11ShaderResourceView* GetSlopeTexture();
	//


private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	void RandomHeightFeild();
	void ApplyPerlinNoise();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	bool m_terrainGeneratedToggle;
	///
	bool m_terrainSmoothedToggle;
	bool m_terrainFlattenPeaks;

	TextureClass *m_SandTexture;
	TextureClass *m_SlopeTexture;
	///
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
	improvednoise* perlinNoise;
};

#endif
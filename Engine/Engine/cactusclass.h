////////////////////////////////////////////////////////////////////////////////
// Filename: cactusclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CACTUSCLASS_H_
#define _CACTUSCLASS_H_

// Model Loading/Rendering Code based on - http://www.rastertek.com/dx11tut07.html
// Instancing Code based on - http://www.rastertek.com/dx11tut37.html

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include "LSystemClass.h"
#include <vector>
#include <stack>

using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

// Angle Definitions in Radians
#define _22_5_DEGREES_POS_ 0.3926991
#define _22_5_DEGREES_NEG_ 5.8904862
#define _25_DEGREES_POS_ 0.436332f
#define _25_DEGREES_NEG_ 5.84685f
#define _30_DEGREES_POS_ 0.523599f
#define _30_DEGREES_NEG_ 5.75959f
#define _45_DEGREES_POS_ 0.785398f
#define _45_DEGREES_NEG_ 5.49779f
#define _60_DEGREES_POS_ 1.0472f
#define _60_DEGREES_NEG_ 5.23599f
#define _90_DEGREES_POS_ 1.5708f
#define _90_DEGREES_NEG_ 4.71239f



////////////////////////////////////////////////////////////////////////////////
// Class name: CactusClass
////////////////////////////////////////////////////////////////////////////////
class CactusClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct InstanceType
	{
		D3DXMATRIX transform;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	CactusClass();
	CactusClass(const CactusClass&);
	~CactusClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	void ParseAxiom(InstanceType[], int);

private:
	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_instanceCount;
	TextureClass* m_Texture;
	ModelType* m_model;

	LSystemClass* LSystem;
	std::string axiom;

};

#endif
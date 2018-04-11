////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_instanceBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	LSystem = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data.
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialise the LSystem.
	LSystem = new LSystemClass;
	if (!LSystem)
	{
		return false;
	}

	// Generate the L-System.
	LSystem->Generate(2);

	// Get the generated string to be parsed here.
	axiom = LSystem->GetAxiom();

	// Initialize the vertex and instace buffer that hold the geometry for the model.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	// Shutdown the LSystem.
	if (LSystem)
	{
		LSystem->ShutDown();
		delete LSystem;
		LSystem = 0;
	}

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}


int ModelClass::GetInstanceCount()
{
	return m_instanceCount;
}


ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	InstanceType* instances;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(int i=0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		// Normals
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	// Set the number of instanes in the array.
	m_instanceCount = 10;

	// Create the instance array.
	instances = new InstanceType[m_instanceCount];
	if (!instances)
	{
		return false;
	}

	// Fill the instance buffer with transform information based on the L-System.
	ParseAxiom(instances, m_instanceCount);

	// Set up the description of the static instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the instance array now that we are done with it.
	delete [] instances;
	instances = 0;

	// ERROR?

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the instance buffer.
	if(m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];


	// Set strides for the Vertex and Instance Buffers.
	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);

	// Set offsets for the Vertex and Instance Buffers.
	offsets[0] = 0;
	offsets[1] = 0;

	// Set the array of pointers to the Vertex and Instance Buffers.
	bufferPointers[0] = m_vertexBuffer;
	bufferPointers[0] = m_instanceBuffer;	// Are these used?
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 2, &m_vertexBuffer, strides, offsets);


	////////////////////////////////////////////////////////////////
	// Migrating to the GPU may require me to change things here. //
	////////////////////////////////////////////////////////////////

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}


void ModelClass::ParseAxiom(InstanceType instances[], int m_instanceCount)
{
	//Initialise the Matricies we'll be using to manipulate our cactus.
	D3DXMATRIX m_translate_1, m_translate_2, m_rotate, m_transform;

	// Initialise matricies to the Indentiy Matrix.
	D3DXMatrixIdentity(&m_translate_1);
	D3DXMatrixIdentity(&m_translate_2);
	D3DXMatrixIdentity(&m_rotate);
	D3DXMatrixIdentity(&m_transform);

	// For Matrix Decomposition Debugging.
	D3DXVECTOR3    pOutScale;
	D3DXQUATERNION pOutRotation;
	D3DXVECTOR3    pOutTranslation;

	// Create our Matrix Stack (used for branching.) 
	std::stack<D3DXMATRIX> MatrixStack;

	// Define our translation matricies.
	D3DXMatrixTranslation(&m_translate_1, 0.0f, 1.0f, 0.0f);	// Translate - Ensure Rotation around the edge, not the centre.
	D3DXMatrixRotationX(&m_rotate, 0.174533f);					// Rotate 10 Degrees (In radians.)
	D3DXMatrixTranslation(&m_translate_2, 0.0f, 2.0f, 0.0f);	// Translate - One Cube's Width so the cubes move out and don't overlap.

	// Set a Root location (At the origin using an Identity Matrix.)
	D3DXMatrixIdentity(&m_transform);
	instances[0].transform = m_transform;
	MatrixStack.push(m_transform);

	int filledInstances = 0;
	int debugfoo = 0;

	for (int i = 0; i < axiom.length(); i++)
	{
		// Apply L-System Rules
		if (axiom.at(i) == 'F')
		{
			// Update the number of instances in the buffer.
			filledInstances++;

			// Translate out
			D3DXMatrixMultiply(&m_transform, &m_translate_2, &m_transform);

			// Define this transform relative to the previous instance.
			D3DXMatrixMultiply(&m_transform, &m_transform, &instances[filledInstances - 1].transform);		// Stack?

			// Update this model's instance buffer.
			instances[filledInstances].transform = m_transform;

			// Reset the transform matrix to make room for new translations/rotations.
			D3DXMatrixIdentity(&m_transform);

			// Debug
			D3DXMatrixDecompose(&pOutScale, &pOutRotation, &pOutTranslation, &m_transform);

		}
		else if (axiom.at(i) == '+')	// Rotate in the positive direction.
		{
			//D3DXMatrixMultiply(&m_transform, &m_transform, &m_rotate);
			D3DXMatrixMultiply(&m_transform, &m_rotate, &m_transform);
		}
		else if (axiom.at(i) == '-')	// Rotate in the negative direction.
		{
			// Placeholder
			//D3DXMatrixMultiply(&m_transform, &m_transform, &m_rotate);
			D3DXMatrixMultiply(&m_transform, &m_rotate, &m_transform);
		}
		else if (axiom.at(i) == '[')	// Begin a branch.
		{
			// Push the current matrix onto the stack.
			MatrixStack.push(m_transform);

			// scale down
				// TO DO
		}
		else if (axiom.at(i) == ']')	// End a branch.
		{
			// Pop the current matrix off the stack.
			MatrixStack.pop();

			// Set m_transform to be the matrix at the top of the stack.
			m_transform = MatrixStack.top();

			debugfoo = MatrixStack.size();

			

			// scale up
				// TO DO
		}

		// Stop itterating through the axiom once the instances have all been calculated.
		if (filledInstances == (m_instanceCount - 1))
		{
			return;
		}
	} // end for
}


/*
   // DA RULES
   if (axiom.at(i) == 'F')			// Move forward.
   {
   D3DXMatrixMultiply(&m_transform, &m_translate_2, &m_transform);
   }
   else if (axiom.at(i) == '+')	// Rotate in the positive direction.
   {
   // Rotate 0.523599 Radians (30 Degrees.)
   D3DXMatrixMultiply(&m_transform, &m_transform, &m_rotate);
   }
   else if (axiom.at(i) == '-')	// Rotate in the negative direction.
   {
   // TO DO
   }
   else if (axiom.at(i) == '[')	// Begin a branch.
   {
   // Push the current matrix onto the stack.
   MatrixStack.push(m_transform);
   }
   else if (axiom.at(i) == ']')	// End a branch.
   {
   // Pop the current matrix off the stack.
   MatrixStack.pop();
   // then top - set to the top value
   }

   // Erin's Advice - Decompose my marticies here to make sure they're ok.
   //D3DXMatrixDecompose(&pOutScale, &pOutRotation, &pOutTranslation, &m_transform);

*/
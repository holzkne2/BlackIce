#include "stdafx.h"
#include "Mesh.h"
#include "BlackIceEngine.h"
#include <vector>

using namespace BlackIceEngine;

Mesh::Mesh()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


Mesh::~Mesh()
{
}

void Mesh::Init()
{
	ID3D11Device* device = Engine::Get().GetDeviceResources()->GetD3DDevice();
	bool results = InitBuffers(device);
}

bool Mesh::InitBuffers(ID3D11Device* device)
{
	std::vector<VertexType> vertices;
	std::vector<unsigned long> indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	unsigned int latitudeBands = 20;
	unsigned int longitudeBands = 20;

	// Set the number of vertices in the vertex array.
	m_vertexCount = (latitudeBands + 1) * (longitudeBands + 1);

	// Set the number of indices in the index array.
	m_indexCount = latitudeBands * longitudeBands * 6;

	// Load the vertex array with data.
	//vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	for (unsigned int lat = 0; lat <= latitudeBands; ++lat)
	{
		float theta = lat * DirectX::XM_PI / latitudeBands;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (unsigned int lon = 0; lon <= longitudeBands; ++lon)
		{
			float phi = lon * 2 * DirectX::XM_PI / latitudeBands;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			float x = cosPhi * sinTheta;
			float y = cosTheta;
			float z = sinPhi * sinTheta;

			VertexType vertex;
			vertex.position = D3DXVECTOR3(x, y, z);
			vertex.color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

			vertices.push_back(vertex);
		}
	}

	// Load the index array with data.
	//indices[0] = 0;  // Bottom left.
	//indices[1] = 1;  // Top middle.
	//indices[2] = 2;  // Bottom right.

	for (unsigned int lat = 0; lat < latitudeBands; ++lat)
	{
		for (unsigned int lon = 0; lon < longitudeBands; ++lon)
		{
			float first = (lat * (longitudeBands + 1)) + lon;
			float second = first + longitudeBands + 1;
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Mesh::Shutdown()
{
	ShutdownBuffers();
}

void Mesh::ShutdownBuffers()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
}

void Mesh::Render()
{
	ID3D11DeviceContext* deviceContext = Engine::Get().GetDeviceResources()->GetD3DDeviceContext();
	RenderBuffers(deviceContext);
}

void Mesh::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
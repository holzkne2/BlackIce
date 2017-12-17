#include "stdafx.h"
#include "BlackIceEngine.h"
#include "pch.h"

using namespace BlackIceEngine;

Engine::Engine()
{
	m_deviceResources = std::make_unique<DeviceResources>();
	GetDefaultSize(m_screenWidth, m_screenHeight);
}


Engine::~Engine()
{
}

void Engine::Init(HWND window, int width, int height)
{
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();

	m_deviceResources->CreateWindowSizeDependentResources();

	m_mesh = std::make_unique<Mesh>();
	m_mesh->Init();

	m_material = std::make_unique<SimpleStandardMaterial>();
	m_material->Init();

	m_light = std::make_unique<Light>();
	m_light->SetDiffuseColor(1, 1, 1);
	m_light->SetDirection(0.25, -0.5, -1);
}

void Engine::Tick()
{
	Update();
	Render();
}

void Engine::Update()
{

}

void Engine::Render()
{
	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();
	// Add your rendering code here.
	context;

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);

	D3DXVECTOR3 up, position, lookAt;
	up = D3DXVECTOR3(0, 1, 0);
	position = D3DXVECTOR3(0, 0, -5);
	lookAt = D3DXVECTOR3(0, 0, 1);
	D3DXMatrixLookAtLH(&view, &position, &lookAt, &up);

	D3DXMATRIX projection;
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)m_screenWidth / (float)m_screenHeight;
	D3DXMatrixPerspectiveFovLH(&projection, fieldOfView, screenAspect, 0.1f, 1000);

	m_mesh->Render();
	m_material->Render(m_mesh->GetIndexCount(), world, view, projection, m_light.get());

	m_deviceResources->PIXEndEvent();

	// Show the new frame.
	m_deviceResources->Present();
}

void Engine::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::Gray);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}

void Engine::Shutdown()
{
	m_mesh->Shutdown();
	m_material->Shutdown();
}

void Engine::OnWindowSizeChanged(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	if (!m_deviceResources->WindowSizeChanged(m_screenWidth, m_screenHeight))
		return;
}

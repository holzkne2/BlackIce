#pragma once

#include <memory>
#include "DeviceResources.h"
#include "Mesh.h"
#include "SimpleStandardMaterial.h"
#include "Light.h"

namespace BlackIceEngine
{
	class Engine
	{
	public:
		Engine();
		virtual ~Engine();

		void Init(HWND window, int width, int height);
		void Shutdown();

		void Tick();

		void GetDefaultSize(int& width, int& height) const {
			width = 1080; height = 720;
		}

		void OnDeviceLost() {};
		void OnDeviceRestored() {};

		void OnActivated() {}
		void OnDeactivated() {}
		void OnSuspending() {}
		void OnResuming() {}
		void OnWindowSizeChanged(int width, int height);

		DeviceResources* GetDeviceResources() const { return m_deviceResources.get(); }

	private:
		void Update();
		void Render();

		void Clear();

	private:
		std::unique_ptr<DeviceResources> m_deviceResources;
		int m_screenWidth;
		int m_screenHeight;


		std::unique_ptr<Mesh> m_mesh;
		std::unique_ptr<SimpleStandardMaterial> m_material;
		std::unique_ptr<Light> m_light;

		// Singleton
	public:
		static Engine& Get()
		{
			static Engine instance;
			return instance;
		}
	};
}

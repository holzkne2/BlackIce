#pragma once

#include <memory>
#include "DeviceResources.h"

namespace BlackIceEngine
{
	class Engine
	{
	public:
		Engine();
		virtual ~Engine();

		void Init(HWND window, int width, int height);

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
		void OnWindowSizeChanged(int width, int height) {}

	private:
		void Update();
		void Render();

		void Clear();

	private:
		std::unique_ptr<DeviceResources> m_deviceResources;

		// Singleton
	public:
		static Engine& Get()
		{
			static Engine instance;
			return instance;
		}
	};
}

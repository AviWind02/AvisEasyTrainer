#pragma once
namespace controls {

	inline int g_MouseDeltaX = 0;
	inline int g_MouseDeltaY = 0;

	namespace inputhook {

		void Init(HWND hwnd);
		void Remove(HWND hwnd);
	}

}
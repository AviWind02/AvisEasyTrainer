#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <string>

//#include <dxgi.h>
//#include <d3d12.h>
//#pragma comment(lib, "d3d12.lib")
//
//
//#include <d3d12.h>
//#include <dxgi1_4.h>

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Api/PluginHandle.hpp>
#include <RED4ext/Api/Sdk.hpp>
#include <RED4ext/Api/Version.hpp>
#include <RED4ext/GameEngine.hpp>
#include <RED4ext/Relocation.hpp>

#include "Vendor/kiero/kiero.h"


namespace loghandler
{
    inline RED4ext::PluginHandle handle = 0;
    inline const RED4ext::v0::Sdk* sdk = nullptr;
}
//Hooks
#include "render/Hooks/hooks.h"
//Draw
#include "Render/Draw/draw.h"

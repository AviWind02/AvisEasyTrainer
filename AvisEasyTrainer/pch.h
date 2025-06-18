#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <queue>

//Placed into the only hook files that needs it
//#include <dxgi.h>
//#include <d3d12.h>
//#pragma comment(lib, "d3d12.lib")
//
//
//#include <d3d12.h>
//#include <dxgi1_4.h>

//Vendors
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
#include "Files/jsonhelper.h"
#include "Global/math.h"

//Hooks
#include "render/Hooks/hooks.h"
//Draw
#include "Render/UserInterface/Style.h"
#include "Render/Draw/draw.h"
#include "Render/Font/font.h"
#include "Controls/controls.h"
#include "Render/UserInterface/UserInterface.h"
#include "Render/UserInterface/submenu.h"
#include "Render/UserInterface/buttons.h"

#include "Render/UserInterface/Backend/NotificationSystem.h"
#include "Render/UserInterface/Backend/InfoBox.h"
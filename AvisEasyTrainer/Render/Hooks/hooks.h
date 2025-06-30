#pragma once
namespace Render::Hooks
{
    void Init(RED4ext::PluginHandle aHandle, const RED4ext::v0::Sdk* aSdk);
    void WaitForDX12AndInit(RED4ext::PluginHandle aHandle, const RED4ext::v0::Sdk* aSdk);
    void Shutdown();
}
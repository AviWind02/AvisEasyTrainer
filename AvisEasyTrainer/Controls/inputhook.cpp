#pragma once

#include "pch.h"
#include "controls.h"
#include <windows.h>
#include <imgui.h>

// Credit: Mouse trap logic Stolen from Cyber Engine Tweaks (CET)
// https://github.com/yamashi/CyberEngineTweaks/

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC oWndProc = nullptr;

namespace controls::inputhook
{
    static bool g_TrapEnabled = false;

    // RED4ext: input::InputSystemWin32Base::ForceCursor
    static RED4ext::UniversalRelocFunc<void (*)(RED4ext::CBaseEngine::UnkD0*, RED4ext::CName, bool)>
        forceCursor(2130646213UL); // Thanks CET!

    void SetMouseTrap(bool enable)
    {
        if (g_TrapEnabled == enable)
            return;

        g_TrapEnabled = enable;

        auto* engine = RED4ext::CGameEngine::Get();
        if (!engine || !engine->unkD0)
        {
            loghandler::sdk->logger->Error(loghandler::handle, "[InputHook] Engine or unkD0 is null");
            return;
        }

        RED4ext::CName reason("ImGui");
        forceCursor(engine->unkD0, reason, enable);

    }

    bool IsMouseTrapped()
    {
        return g_TrapEnabled;
    }

    static LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return TRUE;

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse || io.WantCaptureKeyboard)
        {
            if ((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) ||
                (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) ||
                (uMsg == WM_INPUT))
                return TRUE;
        }

        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    // Setup WndProc and mouse trap system
    void Init(HWND hwnd)
    {
        loghandler::sdk->logger->InfoF(loghandler::handle,
            "[InputHook::Init] Subclassing WndProc: hwnd=%p", hwnd);

        oWndProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));

        loghandler::sdk->logger->Info(loghandler::handle, "[InputHook::Init] Completed");
    }

    void Remove(HWND hwnd)
    {
        loghandler::sdk->logger->InfoF(loghandler::handle,
            "[InputHook::Remove] Unsubclassing WndProc: hwnd=%p", hwnd);

        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));

        loghandler::sdk->logger->Info(loghandler::handle, "[InputHook::Remove] Removed");
    }
}

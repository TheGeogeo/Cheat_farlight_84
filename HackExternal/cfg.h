#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include "singleton.h"
#include "imgui/imgui.h"

DWORD keys[] = { VK_LMENU, VK_MENU, VK_SHIFT, VK_RSHIFT, VK_CONTROL, VK_RCONTROL, VK_LBUTTON, VK_RBUTTON };
const char* keyItems[] = { "LAlt", "RAlt", "LShift", "RShift", "LControl", "RControl", "LMouse", "RMouse" };

inline namespace Configuration
{
	class Settings : public Singleton<Settings>
	{
	public:

		const char* BoxTypes[2] = { "Full Box","Cornered Box" };
		const char* LineTypes[3] = { "Bottom To Enemy","Top To Enemy","Crosshair To Enemy" };

		bool b_MenuShow = true;

		bool b_Visual = false;
		bool b_EspBox = false;
		bool b_EspSkeleton = false;
		bool b_EspLine = false;
		bool b_EspDistance = false;
		bool b_EspHealth = false;
		bool b_EspShield = false;
		bool b_EspName = false;
		bool b_EspTeam = false;

		bool b_Aimbot = false;
		bool b_AimbotFOV = false;
		bool b_AimbotSmooth = false;

		bool b_NoRecoil = false;
		bool b_NoSpread = false;
		bool b_FastReload = false;
		bool b_FastScope = false;

		ImColor VisibleColor = ImColor(1.f / 255, 1.f, 1.f);
		float fl_VisibleColor[3] = { 1.f,1.f / 255,1.f };  //

		ImColor FovColor = ImColor(255.f / 255, 0.f, 0.f);
		float fl_FovColor[3] = { 255.f / 255,0.f,0.f };  //

		int BoxType = 0;
		int LineType = 0;
		int tab_index = 0;
		int AimKey[2] = { 0, 0 };

		float Smoothing = 2.0f;
		float AimbotFOV = 100.0f;
		float max_distance = 500.0f;
	};
#define CFG Configuration::Settings::Get()
}

bool IsImGuiActive() {
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemActive();
}
bool IsAppWindowFocused() {
	HWND activeWindow = GetForegroundWindow();
	return activeWindow == GameVars.gameHWND;
}
bool GetAimKey()
{
	// Check if ImGui is active or if the application window isn't focused
	if (IsImGuiActive() || !IsAppWindowFocused())
		return false;

	// Check if only one of the two keys is pressed
	bool firstKeyActive = GetAsyncKeyState(keys[CFG.AimKey[0]]);
	bool secondKeyActive = GetAsyncKeyState(keys[CFG.AimKey[1]]);

	return (firstKeyActive || secondKeyActive); // XOR logic ensures only one key is active
}

#pragma once
#ifndef IMGUI_NOTIFY
#define IMGUI_NOTIFY

#pragma once
#include <vector>
#include <string>
#include <chrono>
#include "font_awesome_5.h"
#include "fa_solid_900.h"
#include "imgui.h"

namespace ImGui
{

	inline void MergeIconsWithLatestFont(float font_size, bool FontDataOwnedByAtlas = false)
	{
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.FontDataOwnedByAtlas = FontDataOwnedByAtlas;

		GetIO().Fonts->AddFontFromMemoryTTF((void*)fa_solid_900, sizeof(fa_solid_900), font_size, &icons_config, icons_ranges);
	}
}

#endif

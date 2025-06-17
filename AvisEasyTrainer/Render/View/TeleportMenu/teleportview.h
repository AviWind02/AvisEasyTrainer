#pragma once
namespace view::teleport {
	void TeleportMainView();
	inline render::ui::SubMenu teleportMenu{ "Teleport Menu", &TeleportMainView };

}
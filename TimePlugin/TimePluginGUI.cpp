#include "pch.h"
#include "TimePlugin.h"

void TimePlugin::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Name of the plugin to be shown on the f2 -> plugins list
std::string TimePlugin::GetPluginName() {
	return "Time plugin";
}

void TimePlugin::RenderSettings() {

	CVarWrapper enableCvar = cvarManager->getCvar("timeForOnlineGames");

	if (!enableCvar) { return; } //Wrapper Null Check

	bool enabled = enableCvar.getBoolValue();

	if (ImGui::Checkbox("Enable time For online games", &enabled)) {
		enableCvar.setValue(enabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Toggle time For online games");
	}

	//time color picker
	ImGui::Separator();
	ImGui::TextUnformatted("Change color of time");

	CVarWrapper timeColorVar = cvarManager->getCvar("timeColor");
	if (!timeColorVar) { return; }

	// converts from 0-255 color to 0.0-1.0 color
	LinearColor textColor = timeColorVar.getColorValue() / 255;
	if (ImGui::ColorEdit4("Text Color", &textColor.R)) {
		timeColorVar.setValue(textColor * 255);
	}
}

void TimePlugin::Render(CanvasWrapper canvas) {

	CVarWrapper textColorVar = cvarManager->getCvar("timeColor");
	if (!textColorVar) {
		return;
	}
	LinearColor textColor = textColorVar.getColorValue();
	canvas.SetColor(textColor);

	//toggle time for online games
	CVarWrapper isTimeWantedForOnlineGames = cvarManager->getCvar("timeForOnlineGames");

	bool enabled = isTimeWantedForOnlineGames.getBoolValue();
	
	if (gameWrapper->IsInOnlineGame()) {
		if (!enabled) { return;  }
	}

	// sets position to top left
	// x moves to the right
	// y moves down
	// bottom right would be 1920, 1080 for 1080p monitors
	canvas.SetPosition(Vector2F{ 900.0, 0.0 });

	// get time
	std::time_t t = std::time(NULL);
	std::tm now = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&now, "%T");
	std::string s = ss.str();

	// say Time
	// draws from the last set position
	// the two floats are text x and y scale
	// the false turns off the drop shadow
	canvas.DrawString(s, 2.0, 2.0, false);
}
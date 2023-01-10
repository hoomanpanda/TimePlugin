#include "pch.h"
#include "TimePlugin.h"

void TimePlugin::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Name of the plugin to be shown on the f2 -> plugins list
std::string TimePlugin::GetPluginName() {
	return "Time plugin";
}

bool inDragMode = false;

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

	ImGui::Separator();

	//time color picker
	
	ImGui::TextUnformatted("Change color of time");
	CVarWrapper timeColorVar = cvarManager->getCvar("timeColor");
	if (!timeColorVar) { return; }

	// converts from 0-255 color to 0.0-1.0 color
	LinearColor textColor = timeColorVar.getColorValue() / 255;
	if (ImGui::ColorEdit4("Text Color", &textColor.R)) {
		timeColorVar.setValue(textColor * 255);
	}

	ImGui::Separator();
	
	//time overlay location slider
	CVarWrapper xLocCvar = cvarManager->getCvar("time_x_location");
	if (!xLocCvar) { return; }
	float xLoc = xLocCvar.getFloatValue();
	if (ImGui::SliderFloat("X Location", &xLoc, 0.0, 5000.0)) {
		xLocCvar.setValue(xLoc);
	}
	CVarWrapper yLocCvar = cvarManager->getCvar("time_y_location");
	if (!yLocCvar) { return; }
	float yLoc = yLocCvar.getFloatValue();
	if (ImGui::SliderFloat("Y Location", &yLoc, 0.0, 5000.0)) {
		yLocCvar.setValue(yLoc);
	}

	//Drag Mode for time overlay 
	DragWidget(xLocCvar, yLocCvar);


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

	//render location of overlay
	CVarWrapper xLocCvar = cvarManager->getCvar("time_x_location");
	if (!xLocCvar) { return; }
	float xLoc = xLocCvar.getFloatValue();

	CVarWrapper yLocCvar = cvarManager->getCvar("time_y_location");
	if (!yLocCvar) { return; }
	float yLoc = yLocCvar.getFloatValue();

	canvas.SetPosition(Vector2F{ xLoc, yLoc });

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

void TimePlugin::DragWidget(CVarWrapper xLocCvar, CVarWrapper yLocCvar) {
	ImGui::Checkbox("Drag Mode", &inDragMode);

	if (inDragMode) {
		if (ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered()) {
			// doesn't do anything if any ImGui is hovered over
			return;
		}
		// drag cursor w/ arrows to N, E, S, W
		ImGui::SetMouseCursor(2);
		if (ImGui::IsMouseDown(0)) {
			// if holding left click, move
			// sets location to current mouse position
			ImVec2 mousePos = ImGui::GetMousePos();
			xLocCvar.setValue(mousePos.x);
			yLocCvar.setValue(mousePos.y);
		}
	}
}
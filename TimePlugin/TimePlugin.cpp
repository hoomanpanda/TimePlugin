#include "pch.h"
#include "TimePlugin.h"


BAKKESMOD_PLUGIN(TimePlugin, "Time Plugin", "1.0", 0)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
bool timeForOnlineGamesEnabled = false;

void TimePlugin::onLoad()
{
	_globalCvarManager = cvarManager;

	//get time
	std::time_t t = std::time(NULL);
	std::tm now = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&now, "%T");
	std::string s = ss.str();
	LOG(s);
	
	//time color var
	cvarManager->registerCvar("timeColor", "#FFFFFF", "color of time");

	//overlay location
	cvarManager->registerCvar("time_x_location", "0", "set x location of the overlay");
	cvarManager->registerCvar("time_y_location", "0", "set y location of the overlay");

	//overlay size
	cvarManager->registerCvar("time_x_scale", "2", "set width of the overlay");
	cvarManager->registerCvar("time_y_scale", "2", "set height of the overlay");

	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas) {
		Render(canvas);
		});

	cvarManager->registerCvar("timeForOnlineGames", "0", "Enable time for online games", true, true, 0, true, 1)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
			timeForOnlineGamesEnabled = cvar.getBoolValue();
			});


	// !! Enable debug logging by setting DEBUG_LOG = true in logging.h !!
	//DEBUGLOG("TimePlugin debug mode enabled");

	// LOG and DEBUGLOG use fmt format strings https://fmt.dev/latest/index.html
	//DEBUGLOG("1 = {}, 2 = {}, pi = {}, false != {}", "one", 2, 3.14, true);

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	LOG("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	LOG("the cvar with name: {} changed", cvarName);
	//	LOG("the new value is: {}", newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&TimePlugin::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&TimePlugin::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	LOG("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&TimePlugin::YourPluginMethod, this);
}



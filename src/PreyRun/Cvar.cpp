#include "../idLib/precompiled.h"
#pragma hdrstop

#include "Cvar.hpp"

#include "../idLib/Str.h"

namespace pr
{
	std::atomic<bool> preysplit_pipeopen { false };
	bool preysplit_mapchanged { false };

	std::string reload_latestsave {};
	bool reload_ready { false };

	bool oneClickLoad { false };

	bool runFinished { false };

	namespace Timer
	{
		prTimerMonitor inGame {}; // The actual in-game timer in a thread safe monitor
		prTimer RTA; // The RTA timer
		std::atomic<bool> running { false }; // This variable is set to true when we are timing a run

		prTimer demo;
		bool timedemo { false };
	}

#ifdef PR_DEBUG
	namespace dbg
	{
		double frametimer_value { 0 };
		idTimer frametimer;
		idTimer timer;
	}
#endif // PR_DEBUG

	namespace Cvar
	{
		// Information
	#define PR_VERSION_DISPLAY_STRING ENGINE_VERSION "/" PR_BUILD_CONFIGURATION "-" PR_GIT_SHA_SHORT
		idCVar version("PR_Version", PR_VERSION_DISPLAY_STRING, CVAR_GAME | CVAR_ROM, "PreyRun Version");
	#ifdef PR_DEBUG
		idCVar dbg_ghostingver("PR_dbg_GhostingVersion", PR_STR(PR_GHOSTING_DEMO_VERSION), CVAR_GAME | CVAR_ROM, "");
	#endif // PR_DEBUG

		idCVar autojump("PR_AutoJump", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Automatically jumps when holding your 'jump' key (_moveUp)");
		idCVar autopause("PR_AutoPause", "0", CVAR_GAME | CVAR_BOOL, "Automatically pauses the game after map load, set PR_Freeze to 0 to unfreeze the game");
		idCVar preysplit("PR_PreySplit", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle support for interaction with the LiveSplit component PreySplit");
		idCVar preysplit_update("PR_PreySplit_update", "41", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "The time in milliseconds PreyRun should wait before updating the game timer of PreySplit again, note that this is only for displaying the current game time the splits will always be accurate", 0.f, 1000.f, idCmdSystem::ArgCompletion_Integer<0, 1000>);
		idCVar mutejukeboxes("PR_MuteJukeBoxes", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Mutes all jukeboxes. This should make it easier to upload your runs directly to YouTube without changing the music in the bar, especially if you live in Germany ;)");
		idCVar freeze("PR_Freeze", "0", CVAR_GAME | CVAR_BOOL, "Completely freezes the game until PR_Freeze is set back to 0");
		idCVar autocmd_show("PR_AutoCmd_Show", "0", CVAR_GAME | CVAR_INTEGER, "Visually displays all autocommand zones");
		idCVar log("PR_Log", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Log PreyRun output to a file (PreyRun.log)");

		idCVar timer_autostart("PR_Timer_AutoStart", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Automatically start the hud timer at run begin. Check PR_Timer_Methode for information when this might be");
		idCVar timer_autostop("PR_Timer_AutoStop", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Automatically stop the hud timer. Check PR_Timer_Methode for information when this might be");
		idCVar timer_methode("PR_Timer_Methode", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Switch between the different timing methods\n0 - RTA - Real Time Attack, the whole game from start (the first frame you can control Tommy) to finish (when the sphere boss is killed) with game time (default)\n1 - Individual Level, Starts when a map or save game, is loaded and stops when the map has been completed", 0.f, 1.f);
		idCVar timer_backup("PR_Timer_Backup", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle support for backing up the game time to resume it after the game crashed. The time recovery happens automatically once you load the map you were on when the game crashed.");
		idCVar timer_backup_interval("PR_Timer_Backup_Interval", "250", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "The interval, in milliseconds, for updating the backup timer");

		idCVar oneclickload("PR_OneClickLoad", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "If set to 1 you only need to press your load quick save button once not twice");

		idCVar exec_runconfig("PR_ExecRunConfig", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Load run specific config file. RTArun.cfg when pr_timer_methode = 0 and ILrun.cfg when pr_timer_methode = 1");
		idCVar exec_mapconfig("PR_ExecMapConfig", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Load map specific config file located at MapScript/MapName.cfg. Example files are included in PreyRun.");

		// Ghosting
		idCVar gh_autorecord("PR_GH_AutoRecord", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Whether to automatically record a ghost demo when the autostart of the timer is triggered (in other words on run start)");
		idCVar gh_name("PR_GH_Name", "Player", CVAR_GAME | CVAR_ARCHIVE, "The name which will be displayed for your recorded ghosts");
		idCVar gh_color_r("PR_GH_Color_R", "0", CVAR_GAME | CVAR_ARCHIVE, "The amount of red in your ghosts trail", 0.0f, 255.0f, idCmdSystem::ArgCompletion_Integer<0, 255>);
		idCVar gh_color_g("PR_GH_Color_G", "0", CVAR_GAME | CVAR_ARCHIVE, "The amount of green in your ghosts trail", 0.0f, 255.0f, idCmdSystem::ArgCompletion_Integer<0, 255>);
		idCVar gh_color_b("PR_GH_Color_B", "255", CVAR_GAME | CVAR_ARCHIVE, "The amount of blue in your ghosts trail", 0.0f, 255.0f, idCmdSystem::ArgCompletion_Integer<0, 255>);

	#ifdef PR_DEVELOP
		idCVar autostrafe("PR_AutoStrafe", "1", CVAR_GAME | CVAR_BOOL, "Automatically strafe while running");
		bool autostrafe_right { false };
		int autostrafe_count { 0 };

		idCVar fixedseed("PR_FixedSeed", "0", CVAR_GAME | CVAR_BOOL, "Forces the RNG seed to PR_FixedSeed_Value");
		idCVar fixedseed_value("PR_FixedSeed_Value", "0", CVAR_GAME | CVAR_INTEGER, "The Value the RNG seed should be set to if pr_fixedseed is set to 1");
	#endif // PR_DEVELOP

	#ifdef PR_DEBUG
		namespace dbg
		{
			idCVar hud_drawtime("pr_dbg_hud_drawtime", "0", CVAR_GAME | CVAR_BOOL, "*Debug*: Shows the time in milliseconds it took to draw the hud");
			idCVar hud_frametime("pr_dbg_hud_frametime", "0", CVAR_GAME | CVAR_BOOL, "*Debug*: Shows the time in milliseconds it took to render this frame");

			idCVar ghosts("pr_dbg_ghosts", "0", CVAR_GAME | CVAR_BOOL, "*Debug*: Debug ghosting replays and recording");
		}
	#endif // PR_DEBUG

		idCVar hud("PR_hud", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle the display of all PreyRun hud elements, the timers of PreySplit will still work they just wont be visible anymore");

		// Speedometer
		idCVar hud_speedometer("PR_hud_Speedometer", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of the speedometer which displays your velocity");
		idCVar hud_speedometer_r("PR_hud_Speedometer_R", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Defines the red value of the speedometer", 0.f, 255.f);
		idCVar hud_speedometer_g("PR_hud_Speedometer_G", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Defines the green value of the speedometer", 0.f, 255.f);
		idCVar hud_speedometer_b("PR_hud_Speedometer_B", "63.75", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Defines the blue value of the speedometer", 0.f, 255.f);
		idCVar hud_speedometer_precision("PR_hud_Speedometer_Precision", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "the amount of numbers shown after the comma", 0.f, 6.f);
		idCVar hud_speedometer_x("PR_hud_Speedometer_X", "310", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Position of the speedometer x coordinate", 0.f, 639.f);
		idCVar hud_speedometer_y("PR_hud_Speedometer_Y", "460", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Position of the speedometer y coordinate", 0.f, 479.f);
		idCVar hud_speedometer_style("PR_hud_Speedometer_Style", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Controls the style of the speedometer\n0 - Normal constant display\n1- Blue color when gaining speed and red when losing speed");

		// Timer
		idCVar hud_timer("PR_hud_Timer", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of the hud timer, note that the timer will still work even if set to 0. It will just not be displayed on the hud");
		idCVar hud_timer_x("PR_hud_Timer_X", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud timer position", 0.f, 639.f);
		idCVar hud_timer_y("PR_hud_Timer_Y", "235", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud timer position", 0.f, 479.f);
		idCVar hud_timer_r("PR_hud_Timer_R", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud timer color", 0.f, 255.f);
		idCVar hud_timer_g("PR_hud_Timer_G", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud timer color", 0.f, 255.f);
		idCVar hud_timer_b("PR_hud_Timer_B", "63.75", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud timer color", 0.f, 255.f);
		idCVar hud_timer_precision("PR_hud_Timer_Precision", "2", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud timer precision", 0.f, 3.f);
		idCVar hud_timer_alldigits("PR_hud_Timer_AllDigits", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Display all digits or only the non zero ones");

		// RTA Timer
		idCVar hud_rtatimer("PR_hud_RTATimer", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of the RTA hud timer, note that the timer will still work even if hidden");
		idCVar hud_rtatimer_x("PR_hud_RTATimer_X", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud RTA timer position", 0.f, 639.f);
		idCVar hud_rtatimer_y("PR_hud_RTATimer_Y", "220", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud RTA timer position", 0.f, 479.f);
		idCVar hud_rtatimer_r("PR_hud_RTATimer_R", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud RTA timer color", 0.f, 255.f);
		idCVar hud_rtatimer_g("PR_hud_RTATimer_G", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud  RTA timer color", 0.f, 255.f);
		idCVar hud_rtatimer_b("PR_hud_RTATimer_B", "63.75", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud RTA timer color", 0.f, 255.f);
		idCVar hud_rtatimer_precision("PR_hud_RTATimer_Precision", "2", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud RTA timer precision", 0.f, 3.f);
		idCVar hud_rtatimer_alldigits("PR_hud_RTATimer_AllDigits", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Display all digits or only the non zero ones");

		// Run Finished
		idCVar hud_runfinished("PR_hud_RunFinished", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "If enabled timers will be displayed in a different color and display in full precision once a run is finished");
		idCVar hud_runfinished_r("PR_hud_RunFinished_R", "50", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Color for the timers when you finished a run", 0.f, 255.f);
		idCVar hud_runfinished_g("PR_hud_RunFinished_G", "150", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Color for the timers when you finished a run", 0.f, 255.f);
		idCVar hud_runfinished_b("PR_hud_RunFinished_B", "180", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Color for the timers when you finished a run", 0.f, 255.f);

		// JumpSpeed
		idCVar hud_jumpspeed("PR_hud_JumpSpeed", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of your jumpspeed, the speed you had before you jumped");
		idCVar hud_jumpspeed_x("PR_hud_JumpSpeed_X", "310", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Jumpspeed position", 0.f, 639.f);
		idCVar hud_jumpspeed_y("PR_hud_JumpSpeed_Y", "445", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Jumpspeed position", 0.f, 479.f);
		idCVar hud_jumpspeed_r("PR_hud_JumpSpeed_R", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Jumpspeed color", 0.f, 255.f);
		idCVar hud_jumpspeed_g("PR_hud_JumpSpeed_G", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Jumpspeed color", 0.f, 255.f);
		idCVar hud_jumpspeed_b("PR_hud_JumpSpeed_B", "63.75", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Jumpspeed color", 0.f, 255.f);
		idCVar hud_jumpspeed_precision("PR_hud_JumpSpeed_Precision", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Jumpspeed precision", 0.f, 6.f, idCmdSystem::ArgCompletion_Integer<0, 6>);
		idCVar hud_jumpspeed_style("PR_hud_JumpSpeed_Style", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "0 - Always display your jumpspeed\n1 - jumspeed fades out when on the ground\n2 - jumpspeed only visible while in the air", 0.f, 2.f, idCmdSystem::ArgCompletion_Integer<0, 2>);

		// Viewangles
		idCVar hud_viewangles("PR_hud_ViewAngles", "0", CVAR_GAME | CVAR_BOOL, "Toggle display of your current view angles");

		// Velocity
		idCVar hud_velocity("PR_hud_Velocity", "0", CVAR_GAME | CVAR_BOOL, "Toggle display of detailed information about your current velocity");

		// Location
		idCVar hud_location("PR_hud_Location", "0", CVAR_GAME | CVAR_BOOL, "Toggle display of your current position in the level");
		idCVar hud_location_methode("PR_hud_Location_Methode", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Switch the different location methods.\n0 - Display eye (viewpoint / camera) position (default)\n1 - Display feet position", 0, 1);

		// Entity Info
		idCVar hud_entityinfo("PR_hud_EntityInfo", "0", CVAR_GAME | CVAR_BOOL, "Shows infos about the entity aimed at. Which info will be displayed can be selected with entityinfo_*");
		idCVar hud_entityinfo_health("PR_hud_EntityInfo_Health", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Display the entities health when pr_hud_entityinfo is enabled");
		idCVar hud_entityinfo_name("PR_hud_EntityInfo_Name", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Display the entities name when pr_hud_entityinfo is enabled");
		idCVar hud_entityinfo_type("PR_hud_EntityInfo_Type", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Display the entities type when pr_hud_entityinfo is enabled");

		// Ammo
		idCVar hud_ammo("PR_hud_Ammo", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of your current ammo (White = loaded magazine, Yellow = empty magazine, Red = no ammo left)");
		idCVar hud_ammo_x("PR_hud_Ammo_X", "548", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "X Position of the ammo display");
		idCVar hud_ammo_y("PR_hud_Ammo_Y", "448", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Y Position of the ammo display");

		// Health
		idCVar hud_health("PR_hud_Health", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of your current health and maximum health, will be grayed out if god mode is enabled");
		idCVar hud_health_x("PR_hud_Health_X", "12", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "X Position of the health display");
		idCVar hud_health_y("PR_hud_Health_Y", "340", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Y Position of the health display");

		// Damage
		idCVar hud_damage("PR_hud_Damage", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, " Toggle display of the damage you receive");
		idCVar hud_damage_style("PR_hud_Damage_Style", "1", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "0 - Display the last amount of damage you received\n1 - Display the amount of damage you received in the last time", 0, 1, idCmdSystem::ArgCompletion_Integer<0, 1>);

		// SpiritPower
		idCVar hud_spiritpower("PR_hud_SpiritPower", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of your current spirit power / mana, will be grayed out when not having the ability to spirit walk");
		idCVar hud_spiritpower_x("PR_hud_SpiritPower_X", "12", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "X Position of the spirit power display");
		idCVar hud_spiritpower_y("PR_hud_SpiritPower_Y", "355", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Y Position of the spirit power display");

		// Distance
		idCVar hud_distance("PR_hud_Distance", "0", CVAR_GAME | CVAR_BOOL, "Toggle display of the distance between your eyes position (view position / camera) and the object or entity aimed at");

		// Custom hud element
		idCVar hud_custom("PR_hud_Custom", "0", CVAR_GAME | CVAR_BOOL, "Toggle display of the custom hud element, this will display any text given to PR_hud_Custom_Text");
		idCVar hud_custom_x("PR_hud_Custom_X", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "X position of the custom hud element", 0.f, 639.f);
		idCVar hud_custom_y("PR_hud_Custom_Y", "250", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Y position of the custom hud element", 0.f, 479.f);
		idCVar hud_custom_r("PR_hud_Custom_R", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Custom hud element color", 0.f, 255.f);
		idCVar hud_custom_g("PR_hud_Custom_G", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Custom hud element color", 0.f, 255.f);
		idCVar hud_custom_b("PR_hud_Custom_B", "63.75", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Custom hud element color", 0.f, 255.f);
		idCVar hud_custom_text("PR_hud_Custom_Text", "", CVAR_GAME, "The text to display");

		// Keys
		idCVar hud_keys("PR_hud_Keys", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Toggle display of showing the pressed movement keys");
		idCVar hud_keys_methode("PR_hud_Keys_Style", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Select the style in which the pressed movement keys are displayed\n0 - Under the crosshair, only pressed keys are shown\n1 - Under the crosshair, pressed keys are white and not pressed keys are gray\n2 - Around the crosshair, only pressed keys are shown\n3 - Around the crosshair, pressed keys are white and not pressed keys are gray\n4 - At the right side of the screen, only pressed keys are displayed\n5 - At the right side of the screen, pressed keys are white and not pressed keys are gray", 0.f, 5.f, idCmdSystem::ArgCompletion_Integer<0, 5>);
	} // End of namespace: Cvar
} // End of namespace: pr

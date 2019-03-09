#pragma once

// PreyRun BEGIN
#include "GameTimer.hpp"
#include "Monitor.hpp"

#include <atomic>

class idStr;

namespace pr
{
	extern std::atomic<bool> preysplit_pipeopen;
	extern bool preysplit_mapchanged;

	extern idStr reload_latestsave;
	extern bool reload_ready;

	extern bool oneClickLoad;

	extern bool runFinished;

	namespace Timer
	{
		extern prTimerMonitor inGame;
		extern prTimer RTA;
		extern std::atomic<bool> running;

		extern prTimer demo;
		extern bool timedemo;
	}

#ifdef PR_DEBUG
	namespace dbg
	{
		extern idTimer timer;
		extern idTimer frametimer;
		extern double frametimer_value;
	}
#endif // PR_DEBUG

	namespace Cvar
	{
		extern idCVar autojump;
		extern idCVar autopause;
		extern idCVar preysplit;
		extern idCVar preysplit_update;
		extern idCVar mutejukeboxes;
		extern idCVar freeze;
		extern idCVar autocmd_show;
		extern idCVar log;

		extern idCVar timer_autostart;
		extern idCVar timer_autostop;
		extern idCVar timer_methode;
		extern idCVar timer_backup;
		extern idCVar timer_backup_interval;

		extern idCVar oneclickload;

		extern idCVar exec_runconfig;
		extern idCVar exec_mapconfig;

		// Ghosting
		extern idCVar gh_autorecord;
		extern idCVar gh_name;
		extern idCVar gh_color_r;
		extern idCVar gh_color_g;
		extern idCVar gh_color_b;

	#ifdef PR_DEVELOP
		extern idCVar autostrafe;
		extern bool autostrafe_right;
		extern int autostrafe_count;

		extern idCVar fixedseed;
		extern idCVar fixedseed_value;
	#endif // PR_DEVELOP

	#ifdef PR_DEBUG
		namespace dbg
		{
			extern idCVar hud_drawtime;
			extern idCVar hud_frametime;
			extern idCVar ghosts;
		} // namespace: dbg
	#endif // PR_DEBUG

		extern idCVar hud;
		// Speedometer
		extern idCVar hud_speedometer;
		extern idCVar hud_speedometer_r;
		extern idCVar hud_speedometer_g;
		extern idCVar hud_speedometer_b;
		extern idCVar hud_speedometer_precision;
		extern idCVar hud_speedometer_x;
		extern idCVar hud_speedometer_y;
		extern idCVar hud_speedometer_style;
		// Timer
		extern idCVar hud_timer;
		extern idCVar hud_timer_x;
		extern idCVar hud_timer_y;
		extern idCVar hud_timer_r;
		extern idCVar hud_timer_g;
		extern idCVar hud_timer_b;
		extern idCVar hud_timer_precision;
		extern idCVar hud_timer_alldigits;
		// RTA Timer
		extern idCVar hud_rtatimer;
		extern idCVar hud_rtatimer_x;
		extern idCVar hud_rtatimer_y;
		extern idCVar hud_rtatimer_r;
		extern idCVar hud_rtatimer_g;
		extern idCVar hud_rtatimer_b;
		extern idCVar hud_rtatimer_precision;
		extern idCVar hud_rtatimer_alldigits;
		// Run Finished
		extern idCVar hud_runfinished;
		extern idCVar hud_runfinished_r;
		extern idCVar hud_runfinished_g;
		extern idCVar hud_runfinished_b;
		// JumpSpeed
		extern idCVar hud_jumpspeed;
		extern idCVar hud_jumpspeed_x;
		extern idCVar hud_jumpspeed_y;
		extern idCVar hud_jumpspeed_r;
		extern idCVar hud_jumpspeed_g;
		extern idCVar hud_jumpspeed_b;
		extern idCVar hud_jumpspeed_precision;
		extern idCVar hud_jumpspeed_style;
		// Viewangles
		extern idCVar hud_viewangles;
		// Velocity
		extern idCVar hud_velocity;
		// Location
		extern idCVar hud_location;
		extern idCVar hud_location_methode;
		// Entity Info
		extern idCVar hud_entityinfo;
		extern idCVar hud_entityinfo_health;
		extern idCVar hud_entityinfo_name;
		extern idCVar hud_entityinfo_type;
		// Ammo
		extern idCVar hud_ammo;
		extern idCVar hud_ammo_x;
		extern idCVar hud_ammo_y;
		// Health
		extern idCVar hud_health;
		extern idCVar hud_health_x;
		extern idCVar hud_health_y;
		// Damage
		extern idCVar hud_damage;
		extern idCVar hud_damage_style;
		extern idCVar hud_damage_x;
		extern idCVar hud_damage_y;
		// SpiritPower
		extern idCVar hud_spiritpower;
		extern idCVar hud_spiritpower_x;
		extern idCVar hud_spiritpower_y;
		// Distance
		extern idCVar hud_distance;
		// Custom Hud Element
		extern idCVar hud_custom;
		extern idCVar hud_custom_text;
		extern idCVar hud_custom_x;
		extern idCVar hud_custom_y;
		extern idCVar hud_custom_r;
		extern idCVar hud_custom_g;
		extern idCVar hud_custom_b;
		// Keys
		extern idCVar hud_keys;
		extern idCVar hud_keys_methode;
	} // namespace: Cvar
} // namespace: pr

  // PreyRun END

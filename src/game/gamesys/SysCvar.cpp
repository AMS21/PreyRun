// Copyright (C) 2004 Id Software, Inc.
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "../../framework/BuildVersion.h" // HUMANHEAD mdl

#if defined( _DEBUG )
	#define	BUILD_DEBUG	"-debug"
#else
	#define	BUILD_DEBUG "-release"
#endif

/*

All game cvars should be defined here.

*/

// PreyRun BEGIN
idCVar pr_autojump("PR_AutoJump", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "automaticly jumps when holding spacebar");
idCVar pr_autopause("PR_AutoPause", "0", CVAR_GAME | CVAR_BOOL, "Automaticly pauses after map load");
idCVar pr_preysplit("PR_PreySplit", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Enables support for interaction with PreySplit a LiveSplit component");

// HUD BEGIN

// Speedometer
idCVar pr_hud_speedometer("PR_hud_Speedometer", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Shows current verlocity");
idCVar pr_hud_speedometer_r("PR_hud_Speedometer_R", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Defines the red value of the speedometer 0-255",0,255);
idCVar pr_hud_speedometer_g("PR_hud_Speedometer_G", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Defines the green value of the speedometer 0-255",0,255);
idCVar pr_hud_speedometer_b("PR_hud_Speedometer_B", "63.75", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Defines the blue value of the speedometer 0-255",0,255);
idCVar pr_hud_speedometer_precision("PR_hud_Speedometer_Precision", "6", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "the amount of number shown after the comma 0-6",0,6);
idCVar pr_hud_speedometer_x("PR_hud_Speedometer_x", "310", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "position of the speedometer x coordinate 0-639",0,639);
idCVar pr_hud_speedometer_y("PR_hud_Speedometer_y", "460", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "posiont of the speedometer y coordinate 0-479",0,479);

// Viewangles
idCVar pr_hud_viewangles("PR_hud_ViewAngles", "0", CVAR_GAME | CVAR_BOOL , "Shows viewangles");

// Velocity
idCVar pr_hud_velocity("PR_hud_Velocity", "0", CVAR_GAME | CVAR_BOOL, "Shows information about the users Velocity");

// Timer
idTimer pr_Timer; // The actual timer
bool pr_timer_running = false;

idCVar pr_autostart("PR_Timer_AutoStart", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Automaticly start the hud timer at run begin");
idCVar pr_autostop("PR_Timer_AutoStop", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Automaticly stop the hud timer when the sphere boss dies");

idCVar pr_hud_timer("PR_hud_Timer", "1", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Shows the timer");
idCVar pr_hud_timer_x("PR_hud_Timer_x", "0", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud timer position 0-639",0,639);
idCVar pr_hud_timer_y("PR_hud_Timer_y", "235", CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "Hud timer position 0-479",0,479);
idCVar pr_hud_timer_r("PR_hud_Timer_r", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud timer color 0-255",0,255);
idCVar pr_hud_timer_g("PR_hud_Timer_g", "255", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud timer color 0-255",0,255);
idCVar pr_hud_timer_b("PR_hud_Timer_b", "63.75", CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "Hud timer color 0-255",0,255);

// Location
idCVar pr_hud_location("PR_hud_Location", "0", CVAR_GAME | CVAR_BOOL, "Shows the current position");

// Entity Info
idCVar pr_hud_entityinfo("PR_hud_EntityInfo", "0", CVAR_GAME | CVAR_BOOL, "Shows info about the entity aimed at");

// Ammo
idCVar pr_hud_ammo("PR_hud_Ammo", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Shows ammo in numbers");

// Health
idCVar pr_hud_health("PR_hud_Health", "0", CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "Shows health in numbers");

// HUD END
// PreyRun END

#ifdef HUMANHEAD	// HUMANHEAD
idCVar g_tips(						"g_tips",						"1",			CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE,	"allow hud tips to display" );
idCVar g_jawflap(					"g_jawflap",					"1",			CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE,	"controls jawflapping" );
idCVar g_wicked(					"g_wicked",						"0",			CVAR_GAME | CVAR_BOOL,					"if wicked mode is active" );
idCVar g_casino(					"g_casino",						"0",			CVAR_GAME | CVAR_BOOL,					"if casino mode is active" );
idCVar g_roadhouseCompleted(		"g_roadhouseCompleted",			"0",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL,	"if roadhouse map has been completed once" );
idCVar g_precache(					"com_precache",					"1",			CVAR_BOOL | CVAR_SYSTEM | CVAR_NOCHEAT, "if on, precaches needed resources" );	// HUMANHEAD pdm: game side version
idCVar g_debugProjections(			"g_debugProjections",			"0",			CVAR_GAME | CVAR_BOOL,			"Shows projection windings" );
idCVar g_showProjectileLaunchPoint(	"g_showProjectileLaunchPoint",	"0",			CVAR_GAME | CVAR_BOOL,			"Shows position of projectile origin" );
idCVar p_tripwireDebug(				"p_tripwireDebug",				"0",			CVAR_GAME | CVAR_BOOL,			"Show tripwire trace" );
idCVar p_playerPhysicsDebug(		"p_playerPhysicsDebug",			"0",			CVAR_GAME | CVAR_INTEGER,		"Show player physics gravity alignment info" );
idCVar p_camRotRateScale(			"p_camRotRateScale",			"1.3",			CVAR_GAME | CVAR_FLOAT,			"Used to scale delta time thats passed into camera interpolator" );
idCVar p_camInterpDebug(			"p_camInterpDebug",				"0",			CVAR_GAME | CVAR_INTEGER,		"1: Shows lerp info, 2: Shows slerp info" );
idCVar p_iterRotMoveNumIterations(	"p_iterRotMoveNumIterations",	"5",			CVAR_GAME | CVAR_INTEGER,		"Per frame, the number of attempts to align the players bbox if unaligned to gravity" );
idCVar p_iterRotMoveTransDist(		"p_iterRotMoveTransDist",		"20",			CVAR_GAME | CVAR_FLOAT,			"Distance to translate per player bbox alignment attempt" );
idCVar p_disableCamInterp(			"p_disableCamInterp",			"0",			CVAR_GAME | CVAR_BOOL,			"Disable camera interpolator" );
idCVar p_mountedGunDebug(			"p_mountedGunDebug",			"0",			CVAR_GAME | CVAR_INTEGER,		"Shows debug info for mountedgun" );
idCVar g_mbNumBlurs(				"g_mbNumBlurs",					"5",			CVAR_GAME | CVAR_INTEGER,		"Number of images used in motion blur effect" );
idCVar g_mbFrameSpan(				"g_mbFrameSpan",				"10",			CVAR_GAME | CVAR_FLOAT,			"Motion blur: time each image spans" );
idCVar g_postEventsDebug(			"g_postEventsDebug",			"0",			CVAR_GAME | CVAR_BOOL,			"for showing all posted events" );
idCVar g_debugger(					"g_debugger",					"0",			CVAR_GAME | CVAR_INTEGER,		"In-game debugger (1=on, 2=interactive)" );
idCVar g_nodormant(					"g_nodormant",					"0",			CVAR_GAME | CVAR_BOOL,			"Disallow any dormant logic" );
idCVar g_robustDormantAll(			"g_robustDormantAll",			"0",			CVAR_GAME | CVAR_BOOL,			"All dormant checks check all areas" );
idCVar g_dormanttests(				"g_dormanttests",				"0",			CVAR_GAME | CVAR_BOOL,			"" );
idCVar pm_wallwalkstepsize(			"pm_wallwalkstepsize",			"8",			CVAR_GAME | CVAR_FLOAT,			"Step size while wallwalking" );
idCVar g_vehicleDebug(				"g_vehicleDebug",				"0",			CVAR_GAME | CVAR_INTEGER,		"print out vehicle physics debug info" );
idCVar sys_SavedPosition(			"sys_savedPosition",			"",				CVAR_GAME | CVAR_ARCHIVE,		"saved position used by getpos/putpos" );
idCVar g_crosshair(					"g_crosshair",					"1",			CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE,	"Which crosshair to use (0=off)", 0, 8 );
idCVar g_springConstant(			"g_springConstant",				"200000",		CVAR_GAME | CVAR_FLOAT,			"Modify factor for bind controller tension" );
idCVar ai_debugBrain(				"ai_debugBrain",				"0",			CVAR_GAME | CVAR_INTEGER,		"" );
idCVar ai_debugActions(				"ai_debugAction",				"0",			CVAR_GAME | CVAR_INTEGER,		"" );
idCVar ai_talonAttack(				"ai_talonAttack",				"1",			CVAR_GAME | CVAR_INTEGER,		"Enable Talon's attacks" );
idCVar ai_debugPath(				"ai_debugPath",					"0",			CVAR_GAME | CVAR_INTEGER,		"Draw general-purpose pathfinding debug info" );
idCVar ai_hideSkipThink(			"ai_hideSkipThink",				"1",			CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE,	"Hidden monsters don't think at all" );
idCVar g_debugAFs(					"g_debugAFs",					"0",			CVAR_GAME | CVAR_INTEGER,		"print out info on what the ragdolls are doing" );
idCVar g_debugFX(					"g_debugFX",					"0",			CVAR_GAME | CVAR_BOOL,			"" );
idCVar g_showDormant(				"g_showDormant",				"0",			CVAR_GAME | CVAR_BOOL,			"1= Prints out msgs when an entity goes dormant" );
idCVar ai_showNoAAS(				"ai_showNoAAS",					"1",			CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE,	"1= Show when monsters do not have AAS available (shows a ?)" );
idCVar ai_printSpeech(				"ai_printSpeech",				"0",			CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE,	"1= Draw hunter speech sounds" );
idCVar ai_skipSpeech(				"ai_skipSpeech",				"0",			CVAR_GAME | CVAR_BOOL,			"1= Do not use the AI speech system" );
idCVar ai_skipThink(				"ai_skipThink",					"0",			CVAR_GAME | CVAR_BOOL,			"1= Do not execute hhAI::Think() fxn" );
idCVar g_useDDA(					"g_useDDA",						"1",			CVAR_GAME | CVAR_BOOL,			"Whether to use dda system" );
idCVar g_debugMatter(				"g_debugmatter",				"0",			CVAR_GAME | CVAR_INTEGER,		"turns on matter based debugging, 2=include wallwalk traces" );
idCVar g_debugImpulse(				"g_debugimpulse",				"0",			CVAR_GAME | CVAR_BOOL,			"turns on impulse debugging" );
idCVar sys_forceCache(				"sys_forceCache",				"0",			CVAR_BOOL | CVAR_SYSTEM,		"force resource caching even when developer is on" ); //HUMANHEAD mdc - added for developer cache skipping
idCVar g_showGamePortals(			"g_showGamePortals",			"0",			CVAR_GAME | CVAR_INTEGER,		"draw game portal connections" );
idCVar g_showValidSoundAreas(		"g_showValidSoundAreas",		"0",			CVAR_GAME | CVAR_INTEGER,		"draw valid sound areas" );
idCVar g_testModelPitch(			"g_testModelPitch",				"0",			CVAR_GAME | CVAR_FLOAT, "pitch of angle of test model" );
idCVar g_printDDA(					"g_printDDA",					"0",			CVAR_GAME | CVAR_BOOL,			"Prints DDA information to the console" );
#if GOLD
idCVar g_trackDDA(					"g_trackDDA",					"0",			CVAR_GAME | CVAR_BOOL,			"Whether to track detailed statistics as the player goes through a level" );
idCVar g_dumpDDA(					"g_dumpDDA",					"0",			CVAR_GAME | CVAR_BOOL,			"Whether to automatically dump DDA tracking statistics on level change" );
idCVar g_maxEntitiesWarning(		"g_maxEntitiesWarning",			"0",			CVAR_GAME | CVAR_INTEGER,		"max number of entities before warning is displayed" );
#else
idCVar g_trackDDA(					"g_trackDDA",					"1",			CVAR_GAME | CVAR_BOOL,			"Whether to track detailed statistics as the player goes through a level" );
idCVar g_dumpDDA(					"g_dumpDDA",					"0",			CVAR_GAME | CVAR_BOOL,			"Whether to automatically dump DDA tracking statistics on level change" );
idCVar g_maxEntitiesWarning(		"g_maxEntitiesWarning",			"3596",			CVAR_GAME | CVAR_INTEGER,		"max number of entities before warning is displayed" );
#endif
idCVar g_showEntityCount(			"g_showEntityCount",			"0",			CVAR_GAME | CVAR_BOOL,			"1=show entity count" );
idCVar g_expensiveMS(				"g_expensiveMS",				"0.1",			CVAR_GAME | CVAR_FLOAT,			"ms of think to be considered expensive by debugger" );
idCVar g_runMapCycle(				"g_runMapCycle",				"1",			CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE,			"whether to run the map cycling script (g_mapcycle)" );
idCVar g_forceSingleSmokeView(		"g_forceSingleSmokeView",		"0",			CVAR_GAME | CVAR_BOOL,			"do not consider multiple views for smoke system" );

#if GERMAN_VERSION
idCVar g_nogore(					"g_nogore",						"1",			CVAR_GAME | CVAR_BOOL | CVAR_ROM,			"Disables gore");
#else
idCVar g_nogore(					"g_nogore",						"0",			CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE,			"Disables gore");
#endif
#endif	//HUMANHEAD END

//HUMANHEAD rww - only supported types
//const char *si_gameTypeArgs[]		= { "singleplayer", "deathmatch", "Tourney", "Team DM", "Last Man", "cooperative", NULL };
const char *si_gameTypeArgs[]		= { "singleplayer", "deathmatch", "Team DM", NULL };
//HUMANHEAD END
const char *si_readyArgs[]			= { "Not Ready", "Ready", NULL }; 
const char *si_spectateArgs[]		= { "Play", "Spectate", NULL };

// HUMANHEAD pdm: removed
//const char *ui_skinArgs[]			= { "skins/characters/player/marine_mp", "skins/characters/player/marine_mp_red", "skins/characters/player/marine_mp_blue", "skins/characters/player/marine_mp_green", "skins/characters/player/marine_mp_yellow", NULL };
const char *ui_teamArgs[]			= { "Red", "Blue", NULL }; 

struct gameVersion_s {
	gameVersion_s( void ) { sprintf( string, "%s 1.0.%d%s%s %s %s %s", GAME_NAME, BUILD_NUMBER, BUILD_DEBUG, ID_VERSIONTAG, BUILD_STRING, __DATE__, __TIME__ ); }
	char	string[256];
} gameVersion;

idCVar g_version(					"g_version",				gameVersion.string,	CVAR_GAME | CVAR_ROM, "game version" );

// noset vars
idCVar gamename(					"gamename",					GAME_VERSION,	CVAR_GAME | CVAR_SERVERINFO | CVAR_ROM, "" );
idCVar gamedate(					"gamedate",					__DATE__,		CVAR_GAME | CVAR_ROM, "" );

// server info
//HUMANHEAD rww - changed defaults for si_name, si_map, si_maxPlayers
idCVar si_name(						"si_name",					"Prey Server",	CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE, "name of the server" );
//HUMANHEAD rww - removed unsupported game types from description
idCVar si_gameType(					"si_gameType",				si_gameTypeArgs[ 0 ],	CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE, "game type - singleplayer, deathmatch, or Team DM", si_gameTypeArgs, idCmdSystem::ArgCompletion_String<si_gameTypeArgs> );
idCVar si_map(						"si_map",					"dmshuttle2",CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE, "map to be played next on server", idCmdSystem::ArgCompletion_MapName );
idCVar si_maxPlayers(				"si_maxPlayers",			"8",			CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_INTEGER, "max number of players allowed on the server", 1, 8 );
idCVar si_fragLimit(				"si_fragLimit",				"10",			CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_INTEGER, "frag limit", 1, MP_PLAYER_MAXFRAGS );
idCVar si_timeLimit(				"si_timeLimit",				"10",			CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_INTEGER, "time limit in minutes", 0, 60 );
idCVar si_teamDamage(				"si_teamDamage",			"0",			CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_BOOL, "enable team damage" );
idCVar si_warmup(					"si_warmup",				"0",			CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_BOOL, "do pre-game warmup" );
idCVar si_usePass(					"si_usePass",				"0",			CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_BOOL, "enable client password checking" );
idCVar si_pure(						"si_pure",					"1",			CVAR_GAME | CVAR_SERVERINFO | CVAR_BOOL, "server is pure and does not allow modified data" );
idCVar si_spectators(				"si_spectators",			"1",			CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_BOOL, "allow spectators or require all clients to play" );
idCVar si_serverURL(				"si_serverURL",				"",				CVAR_GAME | CVAR_SERVERINFO | CVAR_ARCHIVE, "where to reach the server admins and get information about the server" );

// user info
// HUMANHEAD pdm - changed defaults for ui_name
idCVar ui_name(						"ui_name",					"Tommy",		CVAR_GAME | CVAR_USERINFO | CVAR_ARCHIVE, "player name" );
// HUMANHEAD pdm: removed
//idCVar ui_skin(						"ui_skin",				ui_skinArgs[ 0 ],	CVAR_GAME | CVAR_USERINFO | CVAR_ARCHIVE, "player skin", ui_skinArgs, idCmdSystem::ArgCompletion_String<ui_skinArgs> );
idCVar ui_team(						"ui_team",				ui_teamArgs[ 0 ],	CVAR_GAME | CVAR_USERINFO | CVAR_ARCHIVE, "player team", ui_teamArgs, idCmdSystem::ArgCompletion_String<ui_teamArgs> ); 
idCVar ui_autoSwitch(				"ui_autoSwitch",			"1",			CVAR_GAME | CVAR_USERINFO | CVAR_ARCHIVE | CVAR_BOOL, "auto switch weapon" );
idCVar ui_autoReload(				"ui_autoReload",			"1",			CVAR_GAME | CVAR_USERINFO | CVAR_ARCHIVE | CVAR_BOOL, "auto reload weapon" );
idCVar ui_showGun(					"ui_showGun",				"1",			CVAR_GAME | CVAR_USERINFO | CVAR_ARCHIVE | CVAR_BOOL, "show gun" );
idCVar ui_ready(					"ui_ready",				si_readyArgs[ 0 ],	CVAR_GAME | CVAR_USERINFO, "player is ready to start playing", idCmdSystem::ArgCompletion_String<si_readyArgs> );
idCVar ui_spectate(					"ui_spectate",		si_spectateArgs[ 0 ],	CVAR_GAME | CVAR_USERINFO, "play or spectate", idCmdSystem::ArgCompletion_String<si_spectateArgs> );
idCVar ui_chat(						"ui_chat",					"0",			CVAR_GAME | CVAR_USERINFO | CVAR_BOOL | CVAR_ROM | CVAR_CHEAT, "player is chatting" );
//HUMANHEAD rww
idCVar ui_modelNum(					"ui_modelNum",				"0",			CVAR_GAME | CVAR_USERINFO | CVAR_ARCHIVE, "player model" );
//HUMANHEAD END

// change anytime vars
idCVar developer(					"developer",				"0",			CVAR_GAME | CVAR_BOOL, "" );

idCVar r_aspectRatio( 				"r_aspectRatio",			"0",			CVAR_RENDERER | CVAR_INTEGER | CVAR_ARCHIVE, "aspect ratio of view:\n0 = 4:3\n1 = 16:9\n2 = 16:10", 0, 2 );

idCVar g_cinematic(					"g_cinematic",				"1",			CVAR_GAME | CVAR_BOOL, "skips updating entities that aren't marked 'cinematic' '1' during cinematics" );
idCVar g_cinematicMaxSkipTime(		"g_cinematicMaxSkipTime",	"600",			CVAR_GAME | CVAR_FLOAT, "# of seconds to allow game to run when skipping cinematic.  prevents lock-up when cinematic doesn't end.", 0, 3600 );

idCVar g_muzzleFlash(				"g_muzzleFlash",			"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "show muzzle flashes" );
idCVar g_projectileLights(			"g_projectileLights",		"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "show dynamic lights on projectiles" );
idCVar g_bloodEffects(				"g_bloodEffects",			"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "show blood splats, sprays and gibs" );
idCVar g_doubleVision(				"g_doubleVision",			"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "show double vision when taking damage" );
idCVar g_monsters(					"g_monsters",				"1",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_decals(					"g_decals",					"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "show decals such as bullet holes" );
idCVar g_knockback(					"g_knockback",				"1000",			CVAR_GAME | CVAR_INTEGER, "" );
//idCVar g_skill(						"g_skill",					"1",			CVAR_GAME | CVAR_INTEGER, "" );	// HUMANHEAD pdm: skill not used
idCVar g_nightmare(					"g_nightmare",				"0",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "if nightmare mode is allowed" );
idCVar g_gravity(					"g_gravity",		DEFAULT_GRAVITY_STRING, CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_skipFX(					"g_skipFX",					"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_skipParticles(				"g_skipParticles",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_ragdollDecals(				"g_ragdollDecals",			"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "ragdolls leave blood splats" );

idCVar g_disasm(					"g_disasm",					"0",			CVAR_GAME | CVAR_BOOL, "disassemble script into base/script/disasm.txt on the local drive when script is compiled" );
idCVar g_debugBounds(				"g_debugBounds",			"0",			CVAR_GAME | CVAR_BOOL, "checks for models with bounds > 2048" );
idCVar g_debugAnim(					"g_debugAnim",				"-1",			CVAR_GAME | CVAR_INTEGER, "displays information on which animations are playing on the specified entity number.  set to -1 to disable." );
idCVar g_debugMove(					"g_debugMove",				"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_debugDamage(				"g_debugDamage",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_debugWeapon(				"g_debugWeapon",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_debugScript(				"g_debugScript",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_debugMover(				"g_debugMover",				"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_debugTriggers(				"g_debugTriggers",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_debugCinematic(			"g_debugCinematic",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_stopTime(					"g_stopTime",				"0",			CVAR_GAME | CVAR_BOOL, "" );
/* HUMANHEAD pdm: not used
idCVar g_damageScale(				"g_damageScale",			"1",			CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "scale final damage on player by this factor" );
idCVar g_armorProtection(			"g_armorProtection",		"0.3",			CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "armor takes this percentage of damage" );
idCVar g_armorProtectionMP(			"g_armorProtectionMP",		"0.6",			CVAR_GAME | CVAR_FLOAT | CVAR_ARCHIVE, "armor takes this percentage of damage in mp" );
idCVar g_useDynamicProtection(		"g_useDynamicProtection",	"1",			CVAR_GAME | CVAR_BOOL | CVAR_ARCHIVE, "scale damage and armor dynamically to keep the player alive more often" );
idCVar g_healthTakeTime(			"g_healthTakeTime",			"5",			CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "how often to take health in nightmare mode" );
idCVar g_healthTakeAmt(				"g_healthTakeAmt",			"5",			CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "how much health to take in nightmare mode" );
idCVar g_healthTakeLimit(			"g_healthTakeLimit",		"25",			CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "how low can health get taken in nightmare mode" );
*/


idCVar g_showPVS(					"g_showPVS",				"0",			CVAR_GAME | CVAR_INTEGER, "", 0, 2 );
idCVar g_showTargets(				"g_showTargets",			"0",			CVAR_GAME | CVAR_BOOL, "draws entities and thier targets.  hidden entities are drawn grey." );
idCVar g_showTriggers(				"g_showTriggers",			"0",			CVAR_GAME | CVAR_BOOL, "draws trigger entities (orange) and thier targets (green).  disabled triggers are drawn grey." );
idCVar g_showCollisionWorld(		"g_showCollisionWorld",		"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_showCollisionModels(		"g_showCollisionModels",	"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_showCollisionTraces(		"g_showCollisionTraces",	"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_maxShowDistance(			"g_maxShowDistance",		"128",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_showEntityInfo(			"g_showEntityInfo",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_showviewpos(				"g_showviewpos",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_showcamerainfo(			"g_showcamerainfo",			"0",			CVAR_GAME | CVAR_ARCHIVE, "displays the current frame # for the camera when playing cinematics" );
idCVar g_showTestModelFrame(		"g_showTestModelFrame",		"0",			CVAR_GAME | CVAR_BOOL, "displays the current animation and frame # for testmodels" );
idCVar g_showActiveEntities(		"g_showActiveEntities",		"0",			CVAR_GAME | CVAR_BOOL, "draws boxes around thinking entities.  dormant entities (outside of pvs) are drawn yellow.  non-dormant are green." );
idCVar g_showEnemies(				"g_showEnemies",			"0",			CVAR_GAME | CVAR_BOOL, "draws boxes around monsters that have targeted the the player" );

idCVar g_artificialPlayerCount(		"g_artificialPlayerCount",	"0",			CVAR_GAME | CVAR_INTEGER, "spawns artificial players" ); //HUMANHEAD rww

idCVar g_frametime(					"g_frametime",				"0",			CVAR_GAME | CVAR_BOOL, "displays timing information for each game frame" );
idCVar g_timeentities(				"g_timeEntities",			"0",			CVAR_GAME | CVAR_FLOAT, "when non-zero, shows entities whose think functions exceeded the # of milliseconds specified" );
	
idCVar ai_debugScript(				"ai_debugScript",			"-1",			CVAR_GAME | CVAR_INTEGER, "displays script calls for the specified monster entity number" );
idCVar ai_debugMove(				"ai_debugMove",				"0",			CVAR_GAME | CVAR_BOOL, "draws movement information for monsters" );
idCVar ai_debugTrajectory(			"ai_debugTrajectory",		"0",			CVAR_GAME | CVAR_BOOL, "draws trajectory tests for monsters" );
idCVar ai_testPredictPath(			"ai_testPredictPath",		"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar ai_showCombatNodes(			"ai_showCombatNodes",		"0",			CVAR_GAME | CVAR_BOOL, "draws attack cones for monsters" );
idCVar ai_showPaths(				"ai_showPaths",				"0",			CVAR_GAME | CVAR_BOOL, "draws path_* entities" );
idCVar ai_showObstacleAvoidance(	"ai_showObstacleAvoidance",	"0",			CVAR_GAME | CVAR_INTEGER, "draws obstacle avoidance information for monsters.  if 2, draws obstacles for player, as well", 0, 2, idCmdSystem::ArgCompletion_Integer<0,2> );
idCVar ai_blockedFailSafe(			"ai_blockedFailSafe",		"1",			CVAR_GAME | CVAR_BOOL, "enable blocked fail safe handling" );
	
idCVar g_dvTime(					"g_dvTime",					"1",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_dvAmplitude(				"g_dvAmplitude",			"0.001",		CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_dvFrequency(				"g_dvFrequency",			"0.5",			CVAR_GAME | CVAR_FLOAT, "" );

idCVar g_kickTime(					"g_kickTime",				"1",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_kickAmplitude(				"g_kickAmplitude",			"0.0001",		CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_blobTime(					"g_blobTime",				"1",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_blobSize(					"g_blobSize",				"1",			CVAR_GAME | CVAR_FLOAT, "" );

idCVar g_testHealthVision(			"g_testHealthVision",		"0",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_editEntityMode(			"g_editEntityMode",			"0",			CVAR_GAME | CVAR_INTEGER,	"0 = off\n"
																											"1 = lights\n"
																											"2 = sounds\n"
																											"3 = articulated figures\n"
																											"4 = particle systems\n"
																											"5 = monsters\n"
																											"6 = entity names\n"
																											"7 = entity models", 0, 7, idCmdSystem::ArgCompletion_Integer<0,7> );
idCVar g_dragEntity(				"g_dragEntity",				"0",			CVAR_GAME | CVAR_BOOL, "allows dragging physics objects around by placing the crosshair over them and holding the fire button" );
idCVar g_dragDamping(				"g_dragDamping",			"0.5",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_dragShowSelection(			"g_dragShowSelection",		"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_dropItemRotation(			"g_dropItemRotation",		"",				CVAR_GAME, "" );

idCVar g_vehicleVelocity(			"g_vehicleVelocity",		"1000",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_vehicleForce(				"g_vehicleForce",			"50000",		CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_vehicleSuspensionUp(		"g_vehicleSuspensionUp",	"32",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_vehicleSuspensionDown(		"g_vehicleSuspensionDown",	"20",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_vehicleSuspensionKCompress("g_vehicleSuspensionKCompress","200",		CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_vehicleSuspensionDamping(	"g_vehicleSuspensionDamping","400",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_vehicleTireFriction(		"g_vehicleTireFriction",	"0.8",			CVAR_GAME | CVAR_FLOAT, "" );

idCVar ik_enable(					"ik_enable",				"1",			CVAR_GAME | CVAR_BOOL, "enable IK" );
idCVar ik_debug(					"ik_debug",					"0",			CVAR_GAME | CVAR_BOOL, "show IK debug lines" );

idCVar af_useLinearTime(			"af_useLinearTime",			"1",			CVAR_GAME | CVAR_BOOL, "use linear time algorithm for tree-like structures" );
idCVar af_useImpulseFriction(		"af_useImpulseFriction",	"0",			CVAR_GAME | CVAR_BOOL, "use impulse based contact friction" );
idCVar af_useJointImpulseFriction(	"af_useJointImpulseFriction","0",			CVAR_GAME | CVAR_BOOL, "use impulse based joint friction" );
idCVar af_useSymmetry(				"af_useSymmetry",			"1",			CVAR_GAME | CVAR_BOOL, "use constraint matrix symmetry" );
idCVar af_skipSelfCollision(		"af_skipSelfCollision",		"0",			CVAR_GAME | CVAR_BOOL, "skip self collision detection" );
idCVar af_skipLimits(				"af_skipLimits",			"0",			CVAR_GAME | CVAR_BOOL, "skip joint limits" );
idCVar af_skipFriction(				"af_skipFriction",			"0",			CVAR_GAME | CVAR_BOOL, "skip friction" );
idCVar af_forceFriction(			"af_forceFriction",			"-1",			CVAR_GAME | CVAR_FLOAT, "force the given friction value" );
idCVar af_maxLinearVelocity(		"af_maxLinearVelocity",		"128",			CVAR_GAME | CVAR_FLOAT, "maximum linear velocity" );
idCVar af_maxAngularVelocity(		"af_maxAngularVelocity",	"1.57",			CVAR_GAME | CVAR_FLOAT, "maximum angular velocity" );
idCVar af_timeScale(				"af_timeScale",				"1",			CVAR_GAME | CVAR_FLOAT, "scales the time" );
idCVar af_jointFrictionScale(		"af_jointFrictionScale",	"0",			CVAR_GAME | CVAR_FLOAT, "scales the joint friction" );
idCVar af_contactFrictionScale(		"af_contactFrictionScale",	"0",			CVAR_GAME | CVAR_FLOAT, "scales the contact friction" );
idCVar af_highlightBody(			"af_highlightBody",			"",				CVAR_GAME, "name of the body to highlight" );
idCVar af_highlightConstraint(		"af_highlightConstraint",	"",				CVAR_GAME, "name of the constraint to highlight" );
idCVar af_showTimings(				"af_showTimings",			"0",			CVAR_GAME | CVAR_BOOL, "show articulated figure cpu usage" );
idCVar af_showConstraints(			"af_showConstraints",		"0",			CVAR_GAME | CVAR_BOOL, "show constraints" );
idCVar af_showConstraintNames(		"af_showConstraintNames",	"0",			CVAR_GAME | CVAR_BOOL, "show constraint names" );
idCVar af_showConstrainedBodies(	"af_showConstrainedBodies",	"0",			CVAR_GAME | CVAR_BOOL, "show the two bodies contrained by the highlighted constraint" );
idCVar af_showPrimaryOnly(			"af_showPrimaryOnly",		"0",			CVAR_GAME | CVAR_BOOL, "show primary constraints only" );
idCVar af_showTrees(				"af_showTrees",				"0",			CVAR_GAME | CVAR_BOOL, "show tree-like structures" );
idCVar af_showLimits(				"af_showLimits",			"0",			CVAR_GAME | CVAR_BOOL, "show joint limits" );
idCVar af_showBodies(				"af_showBodies",			"0",			CVAR_GAME | CVAR_BOOL, "show bodies" );
idCVar af_showBodyNames(			"af_showBodyNames",			"0",			CVAR_GAME | CVAR_BOOL, "show body names" );
idCVar af_showMass(					"af_showMass",				"0",			CVAR_GAME | CVAR_BOOL, "show the mass of each body" );
idCVar af_showTotalMass(			"af_showTotalMass",			"0",			CVAR_GAME | CVAR_BOOL, "show the total mass of each articulated figure" );
idCVar af_showInertia(				"af_showInertia",			"0",			CVAR_GAME | CVAR_BOOL, "show the inertia tensor of each body" );
idCVar af_showVelocity(				"af_showVelocity",			"0",			CVAR_GAME | CVAR_BOOL, "show the velocity of each body" );
idCVar af_showActive(				"af_showActive",			"0",			CVAR_GAME | CVAR_BOOL, "show tree-like structures of articulated figures not at rest" );
idCVar af_testSolid(				"af_testSolid",				"1",			CVAR_GAME | CVAR_BOOL, "test for bodies initially stuck in solid" );

idCVar rb_showTimings(				"rb_showTimings",			"0",			CVAR_GAME | CVAR_BOOL, "show rigid body cpu usage" );
idCVar rb_showBodies(				"rb_showBodies",			"0",			CVAR_GAME | CVAR_BOOL, "show rigid bodies" );
idCVar rb_showMass(					"rb_showMass",				"0",			CVAR_GAME | CVAR_BOOL, "show the mass of each rigid body" );
idCVar rb_showInertia(				"rb_showInertia",			"0",			CVAR_GAME | CVAR_BOOL, "show the inertia tensor of each rigid body" );
idCVar rb_showVelocity(				"rb_showVelocity",			"0",			CVAR_GAME | CVAR_BOOL, "show the velocity of each rigid body" );
idCVar rb_showActive(				"rb_showActive",			"0",			CVAR_GAME | CVAR_BOOL, "show rigid bodies that are not at rest" );

// The default values for player movement cvars are set in def/player.def
idCVar pm_jumpheight(				"pm_jumpheight",			"48",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "approximate hieght the player can jump" );
idCVar pm_stepsize(					"pm_stepsize",				"16",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "maximum height the player can step up without jumping" );
idCVar pm_crouchspeed(				"pm_crouchspeed",			"80",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "speed the player can move while crouched" );
idCVar pm_walkspeed(				"pm_walkspeed",				"140",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "speed the player can move while walking" );
idCVar pm_runspeed(					"pm_runspeed",				"220",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "speed the player can move while running" );
idCVar pm_noclipspeed(				"pm_noclipspeed",			"200",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "speed the player can move while in noclip" );
idCVar pm_spectatespeed(			"pm_spectatespeed",			"450",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "speed the player can move while spectating" );
idCVar pm_spectatebbox(				"pm_spectatebbox",			"32",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "size of the spectator bounding box" );
idCVar pm_usecylinder(				"pm_usecylinder",			"0",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_BOOL, "use a cylinder approximation instead of a bounding box for player collision detection" );
idCVar pm_minviewpitch(				"pm_minviewpitch",			"-89",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "amount player's view can look up (negative values are up)" );
idCVar pm_maxviewpitch(				"pm_maxviewpitch",			"89",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "amount player's view can look down" );
idCVar pm_stamina(					"pm_stamina",				"24",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "length of time player can run" );
idCVar pm_staminathreshold(			"pm_staminathreshold",		"45",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "when stamina drops below this value, player gradually slows to a walk" );
idCVar pm_staminarate(				"pm_staminarate",			"0.75",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "rate that player regains stamina. divide pm_stamina by this value to determine how long it takes to fully recharge." );
idCVar pm_crouchheight(				"pm_crouchheight",			"38",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "height of player's bounding box while crouched" );
idCVar pm_crouchviewheight(			"pm_crouchviewheight",		"32",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "height of player's view while crouched" );
idCVar pm_normalheight(				"pm_normalheight",			"74",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "height of player's bounding box while standing" );
idCVar pm_normalviewheight(			"pm_normalviewheight",		"68",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "height of player's view while standing" );
idCVar pm_deadheight(				"pm_deadheight",			"20",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "height of player's bounding box while dead" );
idCVar pm_deadviewheight(			"pm_deadviewheight",		"10",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "height of player's view while dead" );
idCVar pm_crouchrate(				"pm_crouchrate",			"0.87",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "time it takes for player's view to change from standing to crouching" );
idCVar pm_bboxwidth(				"pm_bboxwidth",				"32",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "x/y size of player's bounding box" );
idCVar pm_crouchbob(				"pm_crouchbob",				"0.5",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "bob much faster when crouched" );
idCVar pm_walkbob(					"pm_walkbob",				"0.3",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "bob slowly when walking" );
idCVar pm_runbob(					"pm_runbob",				"0.4",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "bob faster when running" );
idCVar pm_runpitch(					"pm_runpitch",				"0.002",		CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "" );
idCVar pm_runroll(					"pm_runroll",				"0.005",		CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "" );
idCVar pm_bobup(					"pm_bobup",					"0.002",		CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "" );	// HUMANHEAD: was 0.005
idCVar pm_bobpitch(					"pm_bobpitch",				"0.002",		CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "" );
idCVar pm_bobroll(					"pm_bobroll",				"0.002",		CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "" );
idCVar pm_thirdPersonRange(			"pm_thirdPersonRange",		"80",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "camera distance from player in 3rd person" );
idCVar pm_thirdPersonHeight(		"pm_thirdPersonHeight",		"0",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "height of camera from normal view height in 3rd person" );
idCVar pm_thirdPersonAngle(			"pm_thirdPersonAngle",		"0",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_FLOAT, "direction of camera from player in 3rd person in degrees (0 = behind player, 180 = in front)" );
idCVar pm_thirdPersonClip(			"pm_thirdPersonClip",		"1",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_BOOL, "clip third person view into world space" );
idCVar pm_thirdPerson(				"pm_thirdPerson",			"0",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_BOOL, "enables third person view" );
idCVar pm_thirdPersonDeath(			"pm_thirdPersonDeath",		"0",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_BOOL, "enables third person view when player dies" );
idCVar pm_modelView(				"pm_modelView",				"0",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_INTEGER, "draws camera from POV of player model (1 = always, 2 = when dead)", 0, 2, idCmdSystem::ArgCompletion_Integer<0,2> );
idCVar pm_airTics(					"pm_air",					"1800",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_INTEGER, "how long in milliseconds the player can go without air before he starts taking damage" );
//HUMANHEAD rww
idCVar pm_thirdPersonDeathMP(		"pm_thirdPersonDeathMP",	"1",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_BOOL, "enables third person view when player dies in mp" );
//HUMANHEAD END

//HUMANHEAD rww
idCVar g_showAimHealth(				"g_showAimHealth",			"0",			CVAR_GAME | CVAR_BOOL, "health display of target (gameplay testing)" );
//HUMANHEAD END
idCVar g_showPlayerShadow(			"g_showPlayerShadow",		"0",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "enables shadow of player model" );
idCVar g_showHud(					"g_showHud",				"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "" );
idCVar g_showProjectilePct(			"g_showProjectilePct",		"0",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "enables display of player hit percentage" );
idCVar g_showBrass(					"g_showBrass",				"1",			CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "enables ejected shells from weapon" );
idCVar g_gun_x(						"g_gunX",					"0",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_gun_y(						"g_gunY",					"0",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_gun_z(						"g_gunZ",					"0",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_viewNodalX(				"g_viewNodalX",				"0",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_viewNodalZ(				"g_viewNodalZ",				"0",			CVAR_GAME | CVAR_FLOAT, "" );
idCVar g_fov(						"g_fov",					"90",			CVAR_GAME | CVAR_INTEGER | CVAR_NOCHEAT, "" );
idCVar g_skipViewEffects(			"g_skipViewEffects",		"0",			CVAR_GAME | CVAR_BOOL, "skip damage and other view effects" );
idCVar g_mpWeaponAngleScale(		"g_mpWeaponAngleScale",		"0",			CVAR_GAME | CVAR_FLOAT, "Control the weapon sway in MP" );

idCVar g_testParticle(				"g_testParticle",			"0",			CVAR_GAME | CVAR_INTEGER, "test particle visualation, set by the particle editor" );
idCVar g_testParticleName(			"g_testParticleName",		"",				CVAR_GAME, "name of the particle being tested by the particle editor" );
idCVar g_testModelRotate(			"g_testModelRotate",		"0",			CVAR_GAME, "test model rotation speed" );
idCVar g_testPostProcess(			"g_testPostProcess",		"",				CVAR_GAME, "name of material to draw over screen" );
idCVar g_testModelAnimate(			"g_testModelAnimate",		"0",			CVAR_GAME | CVAR_INTEGER, "test model animation,\n"
																							"0 = cycle anim with origin reset\n"
																							"1 = cycle anim with fixed origin\n"
																							"2 = cycle anim with continuous origin\n"
																							"3 = frame by frame with continuous origin\n"
																							"4 = play anim once", 0, 4, idCmdSystem::ArgCompletion_Integer<0,4> );
idCVar g_testModelBlend(			"g_testModelBlend",			"0",			CVAR_GAME | CVAR_INTEGER, "number of frames to blend" );
idCVar g_testDeath(					"g_testDeath",				"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar g_exportMask(				"g_exportMask",				"",				CVAR_GAME | CVAR_ARCHIVE, "" );	// HUMANHEAD: made archive
idCVar g_flushSave(					"g_flushSave",				"0",			CVAR_GAME | CVAR_BOOL, "1 = don't buffer file writing for save games." );

idCVar aas_test(					"aas_test",					"0",			CVAR_GAME | CVAR_INTEGER, "" );
idCVar aas_showAreas(				"aas_showAreas",			"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar aas_showPath(				"aas_showPath",				"0",			CVAR_GAME | CVAR_INTEGER, "" );
idCVar aas_showFlyPath(				"aas_showFlyPath",			"0",			CVAR_GAME | CVAR_INTEGER, "" );
idCVar aas_showWallEdges(			"aas_showWallEdges",		"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar aas_showHideArea(			"aas_showHideArea",			"0",			CVAR_GAME | CVAR_INTEGER, "" );
idCVar aas_pullPlayer(				"aas_pullPlayer",			"0",			CVAR_GAME | CVAR_INTEGER, "" );
idCVar aas_randomPullPlayer(		"aas_randomPullPlayer",		"0",			CVAR_GAME | CVAR_BOOL, "" );
idCVar aas_goalArea(				"aas_goalArea",				"0",			CVAR_GAME | CVAR_INTEGER, "" );
idCVar aas_showPushIntoArea(		"aas_showPushIntoArea",		"0",			CVAR_GAME | CVAR_BOOL, "" );

idCVar g_password(					"g_password",				"",				CVAR_GAME | CVAR_ARCHIVE, "game password" );
idCVar password(					"password",					"",				CVAR_GAME | CVAR_NOCHEAT, "client password used when connecting" );

idCVar g_countDown(					"g_countDown",				"10",			CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "pregame countdown in seconds", 4, 3600 );
idCVar g_gameReviewPause(			"g_gameReviewPause",		"10",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_INTEGER | CVAR_ARCHIVE, "scores review time in seconds (at end game)", 2, 3600 );
idCVar g_TDMArrows(					"g_TDMArrows",				"1",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_BOOL, "draw arrows over teammates in team deathmatch" );
idCVar g_balanceTDM(				"g_balanceTDM",				"1",			CVAR_GAME | CVAR_BOOL, "maintain even teams" );

idCVar net_clientPredictGUI(		"net_clientPredictGUI",		"1",			CVAR_GAME | CVAR_BOOL, "test guis in networking without prediction" );

idCVar g_voteFlags(					"g_voteFlags",				"0",			CVAR_GAME | CVAR_NETWORKSYNC | CVAR_INTEGER | CVAR_ARCHIVE, "vote flags. bit mask of votes not allowed on this server\n"
																					"bit 0 (+1)   restart now\n"
																					"bit 1 (+2)   time limit\n"
																					"bit 2 (+4)   frag limit\n"
																					"bit 3 (+8)   game type\n"
																					"bit 4 (+16)  kick player\n"
																					"bit 5 (+32)  change map\n"
																					"bit 6 (+64)  spectators\n"
																					"bit 7 (+128) next map" );
idCVar g_mapCycle(					"g_mapCycle",				"mapcycle",		CVAR_GAME | CVAR_ARCHIVE, "map cycling script for multiplayer games - see mapcycle.scriptcfg" );

// HUMANHEAD pdm: removed
//idCVar mod_validSkins(				"mod_validSkins",			"skins/characters/player/marine_mp;skins/characters/player/marine_mp_green;skins/characters/player/marine_mp_blue;skins/characters/player/marine_mp_red;skins/characters/player/marine_mp_yellow",		CVAR_GAME | CVAR_ARCHIVE, "valid skins for the game" );

idCVar net_serverDownload(			"net_serverDownload",		"0",			CVAR_GAME | CVAR_INTEGER | CVAR_ARCHIVE, "enable server download redirects. 0: off 1: redirect to si_serverURL 2: use builtin download. see net_serverDl cvars for configuration" );
idCVar net_serverDlBaseURL(			"net_serverDlBaseURL",		"",				CVAR_GAME | CVAR_ARCHIVE, "base URL for the download redirection" );
idCVar net_serverDlTable(			"net_serverDlTable",		"",				CVAR_GAME | CVAR_ARCHIVE, "pak names for which download is provided, seperated by ;" );

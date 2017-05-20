﻿// Copyright (C) 2004 Id Software, Inc.
//

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "TypeInfo.h"

///////////////////
// PreyRun BEGIN //
///////////////////

// Includes
#include "../../PreyRun/Hooking.hpp"
#include "../../PreyRun/GameTimer.hpp"

/*
==================
Cmd_PR_timedemo_f
==================
*/
void Cmd_PR_timedemo_f(const idCmdArgs &args)
{
	pr::timeDemoInit();

	cmdSystem->BufferCommandText(CMD_EXEC_NOW, va("playDemo %s\n", args.Argv(1)));

	pr::Timer::demo = true;
}

/*
==================
Cmd_PR_reload_f
==================
*/
void Cmd_PR_reload_f(const idCmdArgs &args)
{
	if (pr::reload_ready)
	{
		cmdSystem->BufferCommandText(CMD_EXEC_NOW, va("loadgame %s\n", pr::reload_latestsave.Mid(10, pr::reload_latestsave.Length()).c_str()));

		pr::ConsoleWrite("Loading save %s", pr::reload_latestsave.c_str());
	}
	else
	{
		pr::ConsoleWrite("There is no most recent save, sorry!");
	}
}

/*
==================
Cmd_PR_preysplit_split_f
==================
*/
void Cmd_PR_preysplit_split_f(const idCmdArgs &args)
{
	if (pr::Cvar::preysplit.GetBool() && pr::Timer::inGame.IsRunning())
	{
		pr::WriteCustomSplit(pr::GetTime());

		pr::ConsoleWrite("Send custom split to PreySplit");
	}
}

/*
==================
Cmd_PR_ch_sethealth_f
==================
*/
void Cmd_PR_ch_sethealth_f(const idCmdArgs &args)
{
	auto localPlayer = gameLocal.GetLocalPlayer();
	if (!localPlayer || !gameLocal.CheatsOk(true)) { return; }

	localPlayer->SetHealth(atoi(args.Argv(1)));
}

/*
==================
Cmd_PR_ch_setspiritpower_f
==================
*/
void Cmd_PR_ch_setspiritpower_f(const idCmdArgs &args)
{
	auto localPlayer = gameLocal.GetLocalPlayer();
	if (!localPlayer || !gameLocal.CheatsOk(true)) { return; }

	if (localPlayer->inventory.requirements.bCanSpiritWalk)
	{
		ammo_t ammo_spiritpower = idWeapon::GetAmmoNumForName("ammo_spiritpower");
		localPlayer->inventory.ammo[ammo_spiritpower] = atoi(args.Argv(1));
		if (localPlayer->inventory.ammo[ammo_spiritpower] > localPlayer->inventory.maxSpirit)
		{
			localPlayer->inventory.ammo[ammo_spiritpower] = localPlayer->inventory.maxSpirit;
		}
	}
	else
	{
		pr::ConsoleWrite("You do not have the spirit walk ability");
	}
}

/*
==================
Cmd_PR_ch_setpos_f
==================
*/
void Cmd_PR_ch_setpos_f(const idCmdArgs &args)
{
	if (args.Argc() != 4)
	{
		pr::ConsoleWrite("Usage: PR_CH_SetPos <X (Float)> <Y (Float)> <Z (Float)>");
		return;
	}

	auto localPlayer = gameLocal.GetLocalPlayer();
	if (!localPlayer || !gameLocal.CheatsOk(true)) { return; }

	localPlayer->SetOrigin(idVec3(atof(args.Argv(1)), atof(args.Argv(2)), atof(args.Argv(3))));
}

/*
==================
Cmd_PR_ch_setpos_offset_f
==================
*/
void Cmd_PR_ch_setpos_offset_f(const idCmdArgs &args)
{
	if (args.Argc() != 4)
	{
		pr::ConsoleWrite("Usage: PR_CH_SetPos_Offset <X (Float)> <Y (Float)> <Z (Float)>");
		return;
	}

	auto localPlayer = gameLocal.GetLocalPlayer();
	if (!localPlayer || !gameLocal.CheatsOk(true)) { return; }

	auto origin = localPlayer->GetOrigin();

	origin.x += atof(args.Argv(1));
	origin.y += atof(args.Argv(2));
	origin.z += atof(args.Argv(3));

	localPlayer->SetOrigin(origin);
}

///////////
// Timer //
///////////

/*
==================
Cmd_PR_timer_start_f
==================
*/
void Cmd_PR_timer_start_f(const idCmdArgs &args)
{
	if (!pr::Cvar::freeze.GetBool() && !pr::Timer::inGame.IsRunning())
	{
		pr::Timer::inGame.Start();
		pr::Timer::RTA.Start();
	}

	pr::Timer::running = true;

	pr::ConsoleWrite("Starting in-game and RTA timer");
}

/*
==================
Cmd_PR_timer_stop_f
==================
*/
void Cmd_PR_timer_stop_f(const idCmdArgs &args)
{
	if (pr::Timer::inGame.IsRunning() || (pr::Cvar::freeze.GetBool() && pr::Timer::running))
	{
		pr::Timer::inGame.Stop();
		pr::Timer::RTA.Stop();

		pr::Timer::running = false;

		pr::ConsoleWrite("Stopping in-game and RTA timer");
	}
}

/*
==================
Cmd_PR_timer_reset_f
==================
*/
void Cmd_PR_timer_reset_f(const idCmdArgs &args)
{
	pr::Timer::inGame.Stop();
	pr::Timer::RTA.Stop();

	pr::Timer::inGame.Clear();
	pr::Timer::RTA.Clear();

	pr::Timer::running = false;

	pr::WriteTimerReset(pr::GetTime());

	pr::ConsoleWrite("Resetting in-game and RTA timer");
}

/*
==================
Cmd_PR_timer_now_f
==================
*/
void Cmd_PR_timer_now_f(const idCmdArgs &args)
{
	auto times = PR_ms2time(pr::Timer::inGame.Milliseconds());
	auto rtatimes = PR_ms2time(pr::Timer::RTA.Milliseconds());

	pr::ConsoleWrite("In-game timer: %02d:%02d:%02d.%03d", times.hours, times.minutes, times.seconds, times.milliseconds);
	pr::ConsoleWrite("RTA timer: %02d:%02d:%02d.%03d", rtatimes.hours, rtatimes.minutes, rtatimes.seconds, rtatimes.milliseconds);
}

// Autocmdzones

/*
==================
Cmd_PR_autocmd_add_f
==================
*/
void Cmd_PR_autocmd_add_f(const idCmdArgs &args)
{
	if (args.Argc() != 8)
	{
		pr::ConsoleWrite("Usage: Pr_AutoCmd_Add <Start X (Float)> <Start Y (Float)> <Start 1 Z (Float)> <End 2 X (Float)> <End 2 Y (Float)> <End 2 Z (Float)> <Command (String)>");
		return;
	}

	pr::AutocmdzoneHandler::getInstance().Add(
		idVec3(atof(args.Argv(1)), atof(args.Argv(2)), atof(args.Argv(3))) // Starting Point
		, idVec3(atof(args.Argv(4)), atof(args.Argv(5)), atof(args.Argv(6))) // End Points
		, static_cast<idStr>(args.Argv(7)) // commands
	);

	pr::ConsoleWrite("Successfully added autocmdzone number %d", pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
}

/*
==================
Cmd_PR_autocmd_clear_f
==================
*/
void Cmd_PR_autocmd_clear_f(const idCmdArgs &args)
{
	pr::ConsoleWrite("Cleared %d autocmdzones", pr::AutocmdzoneHandler::getInstance().NumOfZones());

	pr::AutocmdzoneHandler::getInstance().Clear();
}

/*
==================
Cmd_PR_autocmd_edit_f
==================
*/
void Cmd_PR_autocmd_edit_f(const idCmdArgs &args)
{
	if (args.Argc() != 9)
	{
		if (pr::AutocmdzoneHandler::getInstance().NumOfZones() > 1)
		{
			pr::ConsoleWrite("Usage: PR_AutoCmd_Edit <Index (0-%d)> <Start X (Float)> <Start Y (Float)> <Start 1 Z (Float)> <End 2 X (Float)> <End 2 Y (Float)> <End 2 Z (Float)> <Command (String)>", pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
		}
		else
		{
			// If there are actually 0 autocmdzones this here will still be printed but i guess thats okay
			pr::ConsoleWrite("Usage: Pr_AutoCmd_Edit <Index (0)> <Start X (Float)> <Start Y (Float)> <Start 1 Z (Float)> <End 2 X (Float)> <End 2 Y (Float)> <End 2 Z (Float)> <Command (String)>");
		}
		return;
	}

	// The index must be between 0 < Index > and NumOfZones
	if (atoi(args.Argv(1)) > pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1 || atoi(args.Argv(1)) < 0)
	{
		if (pr::AutocmdzoneHandler::getInstance().NumOfZones() == 0)
		{
			pr::ConsoleWrite("There are no autocmds you need to create one first using PR_AutoCmd_Add");
		}
		else
		{
			pr::ConsoleWrite("Out of bounds: %d max: %d", atoi(args.Argv(1)), pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
		}
		return;
	}

	pr::AutocmdzoneHandler::getInstance().Edit(
		atoi(args.Argv(1)) // Index
		, idVec3(atof(args.Argv(2)), atof(args.Argv(3)), atof(args.Argv(4))) // Start Position
		, idVec3(atof(args.Argv(5)), atof(args.Argv(6)), atof(args.Argv(7))) // End Position
		, static_cast<idStr>(args.Argv(8)) // Commands
	);
}

/*
==================
Cmd_PR_autocmd_list_f
==================
*/
void Cmd_PR_autocmd_list_f(const idCmdArgs &args)
{
	auto& aczHandler = pr::AutocmdzoneHandler::getInstance();
	auto num { 0 };

	pr::ConsoleWrite("Index: StartX StartY StartZ  EndX EndY EndZ  Command to execute");

	for (auto && e : aczHandler)
	{
		idVec3 pos1 = e.GetPos1();
		idVec3 pos2 = e.GetPos2();
		auto cmds = e.GetCmds();

		pr::ConsoleWrite("%d: %.2f %.2f %.2f %.2f %.2f %.2f %s", num, pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, cmds.c_str());

		num++;
	}
}

/*
==================
Cmd_PR_autocmd_remove_f
==================
*/
void Cmd_PR_autocmd_remove_f(const idCmdArgs &args)
{
	if (args.Argc() != 2)
	{
		if (pr::AutocmdzoneHandler::getInstance().NumOfZones() > 1)
		{
			pr::ConsoleWrite("Usage: PR_AutoCmd_Remove <Index (0-%d)>", pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
		}
		else
		{
			// If there are actually 0 autocmdzones this here will still be printed but i guess thats okay
			pr::ConsoleWrite("Usage: PR_AutoCmd_Remove <Index (0)>");
		}

		return;
	}

	// The index must be between 0 < Index > and NumOfZones
	if (atoi(args.Argv(1)) > pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1 || atoi(args.Argv(1)) < 0)
	{
		if (pr::AutocmdzoneHandler::getInstance().NumOfZones() == 0)
		{
			pr::ConsoleWrite("There are no autocmds you need to create one first using PR_AutoCmd_Add");
		}
		else
		{
			pr::ConsoleWrite("Out of bounds: %d max: %d", atoi(args.Argv(1)), pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
		}

		return;
	}

	pr::AutocmdzoneHandler::getInstance().Remove(atoi(args.Argv(1)));

	pr::ConsoleWrite("Succesfully removed autocmdzone number %d", atoi(args.Argv(1)));
}

#ifdef PR_DEBUG
void Cmd_PR_test_f(const idCmdArgs &args)
{

}

void Cmd_PR_crash_f(const idCmdArgs &args)
{
	idStr text { "Crash" };

	// Trying to print an idStr will cause the game to crash
	gameLocal.Printf("%s", text);
}

void Cmd_PR_dbg_timer_f(const idCmdArgs &args)
{
	auto time = PR_ms2time(pr::Timer::inGame.Milliseconds());
	auto rtatime = PR_ms2time(pr::Timer::inGame.Milliseconds());

	pr::ConsoleWrite("Timer is running: %s\nTimer shoud be on: %s\nIn-game timer:\nTime: %02d:%02d:%02d.%03d\nMilliseconds: %f ms\nClockTicks: %f %s\nRTA timer:\nTime: %02d:%02d:%02d.%03d\nMilliseconds: %f ms\nClockTicks: %f %s", pr::Timer::inGame.IsRunning() ? "True" : "False", pr::Timer::running ? "True" : "False", time.hours, time.minutes, time.seconds, time.milliseconds, pr::Timer::inGame.Milliseconds(), pr::Timer::inGame.ClockTicks(), pr::Timer::pr_gametimer_clocktick_postfix, rtatime.hours, rtatime.minutes, rtatime.seconds, rtatime.milliseconds, pr::Timer::RTA.Milliseconds(), pr::Timer::RTA.ClockTicks(), pr::Timer::pr_gametimer_clocktick_postfix);
}

void Cmd_PR_dbg_timer_set_f(const idCmdArgs &args)
{
	if (args.Argc() != 2)
	{
		pr::ConsoleWrite("Usage: PR_dbg_timer_set <Microseconds (Float)>");
		return;
	}

	pr::Timer::inGame.SetCT(atof(args.Argv(1)));
	pr::Timer::RTA.SetCT(atof(args.Argv(1)));
}

void Cmd_PR_dbg_reload_f(const idCmdArgs &args)
{
	if (pr::reload_ready)
	{
		pr::ConsoleWrite("ready: %s, filename: %s", pr::reload_ready ? "True" : "False", pr::reload_latestsave.c_str());
	}
	else
	{
		pr::ConsoleWrite("pr_reload is not ready cant print info");
	}
}

void Cmd_PR_dbg_backup_f(const idCmdArgs &args)
{
	// Open the file
	auto file = fileSystem->OpenFileRead("backuptmr");

	if (file == nullptr)
	{
		pr::ConsoleWrite("backuptmr can't be opend");
		return;
	}

	bool isValid;
	file->ReadBool(isValid);
	if (!isValid)
	{
		pr::ConsoleWrite("backuptmr is not valid!");
		return;
	}

	float ms;
	idStr mapName;

	file->ReadFloat(ms);
	file->ReadString(mapName);

	static_cast<double>(ms);

	if (mapName.IsEmpty())
	{
		pr::ConsoleWrite("backuptmr is empty");
		return;
	}

	// Close the opened file
	fileSystem->CloseFile(file);

	auto correctMap = static_cast<idStr>(gameLocal.GetMapName()) == mapName;

	pr::ConsoleWrite("Backup Timer: time: %f, mapName: $s CorrectMap: %s", ms, mapName.c_str(), correctMap ? "True" : "False");
}

void Cmd_PR_dbg_rng(const idCmdArgs &args)
{
	auto rng = gameLocal.random;

	pr::ConsoleWrite("Seed: %d, Int: %d, Int: %d, Float: %f, Float: %f", rng.GetSeed(), rng.RandomInt(), rng.RandomInt(), rng.RandomFloat(), rng.RandomFloat());
}

void Cmd_PR_dbg_randseed_f(const idCmdArgs &args)
{
	pr::ConsoleWrite("Seed: %d", gameLocal.random.GetSeed());
}

void Cmd_PR_dbg_rngint_f(const idCmdArgs &args)
{
	pr::ConsoleWrite("Int: %d", gameLocal.random.RandomInt());
}

void Cmd_PR_dbg_rngfloat_f(const idCmdArgs &args)
{
	pr::ConsoleWrite("Float: %f", gameLocal.random.RandomFloat());
}

void Cmd_PR_dbg_autocmd_f(const idCmdArgs &args)
{
	if (args.Argc() != 2)
	{
		pr::ConsoleWrite("Usage: PR_dbg_autocmd <Index (0-%d)>", pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
		return;
	}

	if (atoi(args.Argv(1)) > pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1 || atoi(args.Argv(1)) < 0)
	{
		pr::ConsoleWrite("Out of bounds: %d max: %d", atoi(args.Argv(1)), pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
		return;
	}

	auto acz = pr::AutocmdzoneHandler::getInstance().at(atoi(args.Argv(1)));

	pr::ConsoleWrite("activated: %s", acz.GetActivated() ? "True" : "False");
}

void Cmd_PR_dbg_autocmd_trigger_f(const idCmdArgs &args)
{
	if (args.Argc() != 2)
	{
		pr::ConsoleWrite("Usage: PR_dbg_autocmd_trigger <Index (0-%d)>", pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
	}

	if (atoi(args.Argv(1)) > pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1)
	{
		pr::ConsoleWrite("Out of bounds: %d max: %d", atoi(args.Argv(1)), pr::AutocmdzoneHandler::getInstance().NumOfZones() - 1);
		return;
	}

	auto acz = pr::AutocmdzoneHandler::getInstance().at(atoi(args.Argv(1)));

	acz.Run();
}
#endif // PR_DEBUG

/////////////////
// PreyRun END //
/////////////////

/*
==================
Cmd_GetFloatArg
==================
*/
float Cmd_GetFloatArg(const idCmdArgs &args, int &argNum)
{
	const char *value;

	value = args.Argv(argNum++);
	return atof(value);
}

//HUMANHEAD rww - make generally available so we can use it to spew an entlist on too many ents
void Cmd_EntityList(const idStr &match)
{
	int			e;
	idEntity	*check;
	int			count;
	size_t		size;

	count = 0;
	size = 0;

	gameLocal.Printf("%-4s  %-20s %-20s %s\n", " Num", "EntityDef", "Class", "Name");
	gameLocal.Printf("--------------------------------------------------------------------\n");
	for (e = 0; e < MAX_GENTITIES; ++e)
	{
		check = gameLocal.entities[e];

		if (!check) { continue; }

		if (!check->name.Filter(match, true)) { continue; }

		gameLocal.Printf("%4i: %-20s %-20s %s\n", e,
			check->GetEntityDefName(), check->GetClassname(), check->name.c_str());

		++count;
		size += check->spawnArgs.Allocated();
	}

	gameLocal.Printf("...%d entities\n...%d bytes of spawnargs\n", count, size);
}
//HUMANHEAD END

/*
===================
Cmd_EntityList_f
===================
*/
void Cmd_EntityList_f(const idCmdArgs &args)
{
	idStr		match;

	if (args.Argc() > 1)
	{
		match = args.Args();
		match.Replace(" ", "");
	}
	else { match = ""; }

	Cmd_EntityList(match);
}

/*
===================
Cmd_ActiveEntityList_f
===================
*/
void Cmd_ActiveEntityList_f(const idCmdArgs &args)
{
	idEntity	*check;
	int			count;

	count = 0;

	gameLocal.Printf("%-4s  %-20s %-20s %s\n", " Num", "EntityDef", "Class", "Name");
	gameLocal.Printf("--------------------------------------------------------------------\n");
	for (check = gameLocal.activeEntities.Next(); check != NULL; check = check->activeNode.Next())
	{
		char	dormant = check->fl.isDormant ? '-' : ' ';
		gameLocal.Printf("%4i:%c%-20s %-20s %s\n", check->entityNumber, dormant, check->GetEntityDefName(), check->GetClassname(), check->name.c_str());
		++count;
	}

	gameLocal.Printf("...%d active entities\n", count);
}

/*
===================
Cmd_ListSpawnArgs_f
===================
*/
void Cmd_ListSpawnArgs_f(const idCmdArgs &args)
{
	int i;
	idEntity *ent;

	ent = gameLocal.FindEntity(args.Argv(1));
	if (!ent)
	{
		gameLocal.Printf("entity not found\n");
		return;
	}

	for (i = 0; i < ent->spawnArgs.GetNumKeyVals(); ++i)
	{
		const idKeyValue *kv = ent->spawnArgs.GetKeyVal(i);
		gameLocal.Printf("\"%s\"  " S_COLOR_WHITE "\"%s\"\n", kv->GetKey().c_str(), kv->GetValue().c_str());
	}
}

/*
===================
Cmd_ReloadScript_f
===================
*/
void Cmd_ReloadScript_f(const idCmdArgs &args)
{
	// shutdown the map because entities may point to script objects
	gameLocal.MapShutdown();

	// recompile the scripts
	gameLocal.program.Startup(SCRIPT_DEFAULT);

	// error out so that the user can rerun the scripts
	gameLocal.Error("Exiting map to reload scripts");
}

/*
===================
Cmd_Script_f
===================
*/
void Cmd_Script_f(const idCmdArgs &args)
{
	const char *	script;
	idStr			text;
	idStr			funcname;
	static int		funccount = 0;
	idThread *		thread;
	const function_t *func;
	idEntity		*ent;

	if (!gameLocal.CheatsOk()) { return; }

	sprintf(funcname, "ConsoleFunction_%d", funccount++);

	script = args.Args();
	sprintf(text, "void %s() {%s;}\n", funcname.c_str(), script);
	if (gameLocal.program.CompileText("console", text, true))
	{
		func = gameLocal.program.FindFunction(funcname);
		if (func)
		{
			// set all the entity names in case the user named one in the script that wasn't referenced in the default script
			for (ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = ent->spawnNode.Next())
			{
				gameLocal.program.SetEntity(ent->name, ent);
			}

			thread = new idThread(func);
			thread->Start();
		}
	}
}

/*
==================
KillEntities

Kills all the entities of the given class in a level.
==================
*/
void KillEntities(const idCmdArgs &args, const idTypeInfo &superClass)
{
	idEntity	*ent;
	idStrList	ignore;
	const char *name;
	int			i;

	if (!gameLocal.GetLocalPlayer() || !gameLocal.CheatsOk(false)) { return; }

	for (i = 1; i < args.Argc(); ++i)
	{
		name = args.Argv(i);
		ignore.Append(name);
	}

	for (ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = ent->spawnNode.Next())
	{
		if (ent->IsType(superClass))
		{
			for (i = 0; i < ignore.Num(); ++i)
			{
				if (ignore[i] == ent->name) { break; }
			}

			if (i >= ignore.Num()) { ent->PostEventMS(&EV_Remove, 0); }
		}
	}
}

/*
==================
Cmd_KillMonsters_f

Kills all the monsters in a level.
==================
*/
void Cmd_KillMonsters_f(const idCmdArgs &args)
{
	//HUMANHEAD jsh dont kill monsters with "no_kill_monsters" set
	//KillEntities( args, idAI::Type );
	idEntity	*ent;
	idStrList	ignore;
	const char *name;
	int			i;

	// HUMANHEAD bg: Kill only active monsters, using Damage() so they trigger their targets.
	if (idStr::Icmp(args.Argv(1), "active") == 0)
	{
		hhPlayer *player = static_cast<hhPlayer*>(gameLocal.GetLocalPlayer());
		if (!player || !gameLocal.CheatsOk()) { return; }
		for (ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = ent->spawnNode.Next())
		{
			if (ent->IsType(idAI::Type) && !ent->fl.isDormant)
			{
				ent->Damage(player, player, vec3_zero, "damage_instantdeath", 9999.0f, 0);
			}
		}
		KillEntities(args, idProjectile::Type);
		return;
	}
	// HUMANHEAD END

	if (!gameLocal.GetLocalPlayer() || !gameLocal.CheatsOk(false)) { return; }

	for (i = 1; i < args.Argc(); ++i)
	{
		name = args.Argv(i);
		ignore.Append(name);
	}

	for (ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = ent->spawnNode.Next())
	{
		if (ent->IsType(idAI::Type) && !ent->spawnArgs.GetBool("no_kill_monsters", "0"))
		{
			for (i = 0; i < ignore.Num(); ++i)
			{
				if (ignore[i] == ent->name) { break; }
			}
			if (i >= ignore.Num()) { ent->PostEventMS(&EV_Remove, 0); }
		}
	}
	//END HUMANHEAD

	// kill any projectiles as well since they have pointers to the monster that created them
	KillEntities(args, idProjectile::Type);
}

/*
==================
Cmd_KillMovables_f

Kills all the moveables in a level.
==================
*/
void Cmd_KillMovables_f(const idCmdArgs &args)
{
	if (!gameLocal.GetLocalPlayer() || !gameLocal.CheatsOk(false)) { return; }
	KillEntities(args, idMoveable::Type);
}

/*
==================
Cmd_KillRagdolls_f

Kills all the ragdolls in a level.
==================
*/
void Cmd_KillRagdolls_f(const idCmdArgs &args)
{
	if (!gameLocal.GetLocalPlayer() || !gameLocal.CheatsOk(false)) { return; }
	KillEntities(args, idAFEntity_Generic::Type);
	KillEntities(args, idAFEntity_WithAttachedHead::Type);
}

/*
==================
Cmd_Give_f

Give items to a client
==================
*/
void Cmd_Give_f(const idCmdArgs &args)
{
	const char *name;
	int			i;
	bool		give_all;
	idPlayer	*player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	//HUMANHEAD PCF rww 05/16/06
	bool wasInside = declManager->GetInsideLevelLoad();
	declManager->SetInsideLevelLoad(true);
	//HUMANHEAD END

	name = args.Argv(1);

	if (idStr::Icmp(name, "all") == 0) { give_all = true; }
	else { give_all = false; }

	if (give_all || (idStr::Cmpn(name, "weapon", 6) == 0))
	{
		if (gameLocal.world->spawnArgs.GetBool("no_Weapons"))
		{
			gameLocal.world->spawnArgs.SetBool("no_Weapons", false);
			for (i = 0; i < gameLocal.numClients; ++i)
			{
				if (gameLocal.entities[i])
				{
					gameLocal.entities[i]->PostEventSec(&EV_Player_SelectWeapon, 0.5f, gameLocal.entities[i]->spawnArgs.GetString("def_weapon1"));
				}
			}
		}
	}

	if ((idStr::Cmpn(name, "weapon_", 7) == 0) || (idStr::Cmpn(name, "item_", 5) == 0) || (idStr::Cmpn(name, "ammo_", 5) == 0))
	{
		player->GiveItem(name);
		//HUMANHEAD PCF rww 05/16/06
		declManager->SetInsideLevelLoad(wasInside);
		//HUMANHEAD END
		return;
	}

	if (give_all || idStr::Icmp(name, "health") == 0)
	{
		player->health = player->inventory.maxHealth;
		player->healthPulse = true;	// HUMANHEAD pdm
		if (!give_all)
		{
			//HUMANHEAD PCF rww 05/16/06
			declManager->SetInsideLevelLoad(wasInside);
			//HUMANHEAD END
			return;
		}
	}

	if (give_all || idStr::Icmp(name, "weapons") == 0)
	{
		player->inventory.weapons = BIT(MAX_WEAPONS) - 1;
		player->CacheWeapons();

		if (!give_all)
		{
			//HUMANHEAD PCF rww 05/16/06
			declManager->SetInsideLevelLoad(wasInside);
			//HUMANHEAD END
			return;
		}

		static_cast<hhPlayer *>(player)->UnlockWeapon(-1);	// HUMANHEAD mdl:  Unlock all weapons for give all
	}

#ifdef HUMANHEAD	// HUMANHEAD pdm
	if (give_all || idStr::Icmp(name, "items") == 0)
	{
		player->GiveItem("item_lighter");
		player->GiveItem("item_talon");
		player->GiveItem("item_spiritwalk");
		player->GiveItem("item_wallwalkboots");
		player->GiveItem("item_hand1");
		if (!give_all)
		{
			//HUMANHEAD PCF rww 05/16/06
			declManager->SetInsideLevelLoad(wasInside);
			//HUMANHEAD END
			return;
		}
	}
#endif

	if (give_all || idStr::Icmp(name, "ammo") == 0)
	{
		player->spiritPulse = true;
		for (i = 0; i < AMMO_NUMTYPES; ++i)
		{
			player->inventory.ammo[i] = player->inventory.MaxAmmoForAmmoClass(player, idWeapon::GetAmmoNameForNum(static_cast<ammo_t>(i)));
		}
		if (!give_all)
		{
			//HUMANHEAD PCF rww 05/16/06
			declManager->SetInsideLevelLoad(wasInside);
			//HUMANHEAD END
			return;
		}
	}

#ifndef HUMANHEAD	// HUMANHEAD pdm: not used
	/*
		if ( give_all || idStr::Icmp( name, "armor" ) == 0 ) {
			player->inventory.armor = player->inventory.maxarmor;
			if ( !give_all ) {
				return;
			}
		}

		if ( idStr::Icmp( name, "berserk" ) == 0 ) {
			player->GivePowerUp( BERSERK, SEC2MS( 30.0f ) );
			return;
		}

		if ( idStr::Icmp( name, "invis" ) == 0 ) {
			player->GivePowerUp( INVISIBILITY, SEC2MS( 30.0f ) );
			return;
		}

		if ( idStr::Icmp( name, "pda" ) == 0 ) {
			player->GivePDA( args.Argv(2), NULL );
			return;
		}

		if ( idStr::Icmp( name, "video" ) == 0 ) {
			player->GiveVideo( args.Argv(2), NULL );
			return;
		}
	*/
#endif

	if (!give_all && !player->Give(args.Argv(1), args.Argv(2)))
	{
		gameLocal.Printf("unknown item\n");
	}

	//HUMANHEAD PCF rww 05/16/06
	declManager->SetInsideLevelLoad(wasInside);
	//HUMANHEAD END
}

/*
==================
Cmd_CenterView_f

Centers the players pitch
==================
*/
void Cmd_CenterView_f(const idCmdArgs &args)
{
	idPlayer	*player;
	idAngles	ang;

	player = gameLocal.GetLocalPlayer();
	if (!player) { return; }

	ang = player->viewAngles;
	ang.pitch = 0.0f;
	player->SetViewAngles(ang);
}

/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
void Cmd_God_f(const idCmdArgs &args)
{
	char		*msg;
	idPlayer	*player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (player->godmode)
	{
		player->godmode = false;
		msg = "godmode OFF\n";
	}
	else
	{
		player->godmode = true;
		msg = "godmode ON\n";
	}

	gameLocal.Printf("%s", msg);
}

/*
==================
Cmd_Notarget_f

Sets client to notarget

argv(0) notarget
==================
*/
void Cmd_Notarget_f(const idCmdArgs &args)
{
	char		*msg;
	idPlayer	*player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (player->fl.notarget)
	{
		player->fl.notarget = false;
		msg = "notarget OFF\n";
	}
	else
	{
		player->fl.notarget = true;
		msg = "notarget ON\n";
	}

	gameLocal.Printf("%s", msg);
}

/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
void Cmd_Noclip_f(const idCmdArgs &args)
{
	char		*msg;
	idPlayer	*player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (player->noclip) { msg = "noclip OFF\n"; }
	else { msg = "noclip ON\n"; }
	player->noclip = !player->noclip;

	gameLocal.Printf("%s", msg);
}

/*
=================
Cmd_Kill_f
=================
*/
void Cmd_Kill_f(const idCmdArgs &args)
{
	idPlayer	*player;

	if (gameLocal.isMultiplayer)
	{
		if (gameLocal.isClient)
		{
			idBitMsg	outMsg;
			byte		msgBuf[MAX_GAME_MESSAGE_SIZE];
			outMsg.Init(msgBuf, sizeof(msgBuf));
			outMsg.WriteByte(GAME_RELIABLE_MESSAGE_KILL);
			networkSystem->ClientSendReliableMessage(outMsg);
		}
		else
		{
			player = gameLocal.GetClientByCmdArgs(args);
			if (!player)
			{
				common->Printf("kill <client nickname> or kill <client index>\n");
				return;
			}
			player->Kill(false, false);
			cmdSystem->BufferCommandText(CMD_EXEC_NOW, va("say killed client %d '%s^0'\n", player->entityNumber, gameLocal.userInfo[player->entityNumber].GetString("ui_name")));
		}
	}
	else
	{
		player = gameLocal.GetLocalPlayer();
		if (!player) { return; }
		player->Kill(false, false);
	}
}

/*
=================
Cmd_PlayerModel_f
=================
*/
void Cmd_PlayerModel_f(const idCmdArgs &args)
{
	idPlayer	*player;
	const char *name;
	idVec3		pos;
	idAngles	ang;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (args.Argc() < 2)
	{
		gameLocal.Printf("usage: playerModel <modelname>\n");
		return;
	}

	name = args.Argv(1);
	player->spawnArgs.Set("model", name);

	pos = player->GetPhysics()->GetOrigin();
	ang = player->viewAngles;
	player->SpawnToPoint(pos, ang);
}

/*
==================
Cmd_Say
==================
*/
static void Cmd_Say(bool team, const idCmdArgs &args)
{
#if HUMANHEAD	// HUMANHEAD pdm
	idStr name;
#else
	const char *name;
#endif
	idStr text;
	const char *cmd = team ? "sayTeam" : "say";

	if (!gameLocal.isMultiplayer)
	{
		gameLocal.Printf("%s can only be used in a multiplayer game\n", cmd);
		return;
	}

	if (args.Argc() < 2)
	{
		gameLocal.Printf("usage: %s <text>\n", cmd);
		return;
	}

	text = args.Args();
	if (text.Length() == 0) { return; }

	if (text[text.Length() - 1] == '\n') { text[text.Length() - 1] = '\0'; }
	name = "player";

	idPlayer *	player;

	// here we need to special case a listen server to use the real client name instead of "server"
	// "server" will only appear on a dedicated server
	if (gameLocal.isClient || cvarSystem->GetCVarInteger("net_serverDedicated") == 0)
	{
		player = gameLocal.localClientNum >= 0 ? static_cast<idPlayer *>(gameLocal.entities[gameLocal.localClientNum]) : NULL;
		if (player)
		{
			name = player->GetUserInfo()->GetString("ui_name", "player");
#if HUMANHEAD	// HUMANHEAD pdm
			// Doctor up name and text with appropriate colors
			name.Insert(S_COLOR_YELLOW, 0);
			text.Insert(S_COLOR_YELLOW, 0);
#endif
		}

	}
	else { name = "server"; }

	if (gameLocal.isClient)
	{
		idBitMsg	outMsg;
		byte		msgBuf[256];
		outMsg.Init(msgBuf, sizeof(msgBuf));
		outMsg.WriteByte(team ? GAME_RELIABLE_MESSAGE_TCHAT : GAME_RELIABLE_MESSAGE_CHAT);
		outMsg.WriteString(name);
		outMsg.WriteString(text, -1, false);
		networkSystem->ClientSendReliableMessage(outMsg);
	}
	else
	{
		gameLocal.mpGame.ProcessChatMessage(gameLocal.localClientNum, team, name, text, NULL);
	}
}

/*
==================
Cmd_Say_f
==================
*/
static void Cmd_Say_f(const idCmdArgs &args)
{
	Cmd_Say(false, args);
}

/*
==================
Cmd_SayTeam_f
==================
*/
static void Cmd_SayTeam_f(const idCmdArgs &args)
{
	//HUMANHEAD rww - never use teamchat in dm
	bool teamSay = true;
	if (gameLocal.gameType != GAME_TDM) { teamSay = false; }
	//HUMANHEAD END
	Cmd_Say(teamSay, args);
}

/*
==================
Cmd_AddChatLine_f
==================
*/
static void Cmd_AddChatLine_f(const idCmdArgs &args)
{
	gameLocal.mpGame.AddChatLine(args.Argv(1));
}

/*
==================
Cmd_Kick_f
==================
*/
static void Cmd_Kick_f(const idCmdArgs &args)
{
	idPlayer *player;

	if (!gameLocal.isMultiplayer)
	{
		gameLocal.Printf("kick can only be used in a multiplayer game\n");
		return;
	}

	if (gameLocal.isClient)
	{
		gameLocal.Printf("You have no such power. This is a server command\n");
		return;
	}

	player = gameLocal.GetClientByCmdArgs(args);
	if (!player)
	{
		gameLocal.Printf("usage: kick <client nickname> or kick <client index>\n");
		return;
	}
	cmdSystem->BufferCommandText(CMD_EXEC_NOW, va("say kicking out client %d '%s^0'\n", player->entityNumber, gameLocal.userInfo[player->entityNumber].GetString("ui_name")));
	cmdSystem->BufferCommandText(CMD_EXEC_NOW, va("kick %d\n", player->entityNumber));
}

/*
==================
Cmd_GetViewpos_f
==================
*/
void Cmd_GetViewpos_f(const idCmdArgs &args)
{
	idPlayer	*player;
	idVec3		origin;
	idMat3		axis;

	player = gameLocal.GetLocalPlayer();
	if (!player) { return; }

	const renderView_t *view = player->GetRenderView();
	if (view)
	{
		gameLocal.Printf("(%s) %.1f\n", view->vieworg.ToString(), view->viewaxis[0].ToYaw());
	}
	else
	{
		player->GetViewPos(origin, axis);
		gameLocal.Printf("(%s) %.1f\n", origin.ToString(), axis[0].ToYaw());
	}
}

/*
=================
Cmd_SetViewpos_f
=================
*/
void Cmd_SetViewpos_f(const idCmdArgs &args)
{
	idVec3		origin;
	idAngles	angles;
	int			i;
	idPlayer	*player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if ((args.Argc() != 4) && (args.Argc() != 5))
	{
		gameLocal.Printf("usage: setviewpos <x> <y> <z> <yaw>\n");
		return;
	}

	angles.Zero();
	if (args.Argc() == 5)
	{
		angles.yaw = atof(args.Argv(4));
	}

	for (i = 0; i < 3; ++i)
	{
		origin[i] = atof(args.Argv(i + 1));
	}
	origin.z -= pm_normalviewheight.GetFloat() - 0.25f;

	player->Teleport(origin, angles, NULL);
}

/*
=================
Cmd_Teleport_f
=================
*/
void Cmd_Teleport_f(const idCmdArgs &args)
{
	idVec3		origin;
	idAngles	angles;
	idPlayer	*player;
	idEntity	*ent;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (args.Argc() != 2)
	{
		gameLocal.Printf("usage: teleport <name of entity to teleport to>\n");
		return;
	}

	ent = gameLocal.FindEntity(args.Argv(1));
	if (!ent)
	{
		gameLocal.Printf("entity not found\n");
		return;
	}

	angles.Zero();
	angles.yaw = ent->GetPhysics()->GetAxis()[0].ToYaw();
	origin = ent->GetPhysics()->GetOrigin();

	player->Teleport(origin, angles, ent);
}

/*
=================
Cmd_Trigger_f
=================
*/
void Cmd_Trigger_f(const idCmdArgs &args)
{
	idVec3		origin;
	idAngles	angles;
	idPlayer	*player;
	idEntity	*ent;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (args.Argc() != 2)
	{
		gameLocal.Printf("usage: trigger <name of entity to trigger>\n");
		return;
	}

	ent = gameLocal.FindEntity(args.Argv(1));
	if (!ent)
	{
		gameLocal.Printf("entity not found\n");
		return;
	}

	ent->Signal(SIG_TRIGGER);
	ent->ProcessEvent(&EV_Activate, player);
	ent->TriggerGuis();
}

/*
===================
Cmd_Spawn_f
===================
*/
void Cmd_Spawn_f(const idCmdArgs &args)
{
	const char *key, *value;
	int			i;
	float		yaw;
	idVec3		org;
	idPlayer	*player;
	idDict		dict;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk(false)) { return; }

	if (args.Argc() & 1)
	{	// must always have an even number of arguments
		gameLocal.Printf("usage: spawn classname [key/value pairs]\n");
		return;
	}

	yaw = player->viewAngles.yaw;

	value = args.Argv(1);
	dict.Set("classname", value);
	dict.Set("angle", va("%f", yaw + 180));

	org = player->GetPhysics()->GetOrigin() + idAngles(0, yaw, 0).ToForward() * 180 + idVec3(0, 0, 1);
	dict.Set("origin", org.ToString());

	for (i = 2; i < args.Argc() - 1; i += 2)
	{

		key = args.Argv(i);
		value = args.Argv(i + 1);

		dict.Set(key, value);
	}

	//HUMANHEAD PCF rww 05/16/06
	bool wasInside = declManager->GetInsideLevelLoad();
	declManager->SetInsideLevelLoad(true);
	//HUMANHEAD END

	gameLocal.SpawnEntityDef(dict);

	//HUMANHEAD PCF rww 05/16/06
	declManager->SetInsideLevelLoad(wasInside);
	//HUMANHEAD END
}

//HUMANHEAD rww
/*
===================
Cmd_SpawnArtificialPlayer_f
===================
*/
void Cmd_SpawnArtificialPlayer_f(const idCmdArgs &args)
{
	if (!gameLocal.isMultiplayer || !gameLocal.isServer)
	{
		gameLocal.Printf("Artificial players must be spawned in multiplayer as the server.\n");
		return;
	}

	if (!gameLocal.CheatsOk(false)) { return; }

	gameLocal.SpawnArtificialPlayer();
}

/*
===================
Cmd_PlayerShadowToggle_f
===================
*/
#if !GOLD
void Cmd_PlayerShadowToggle_f(const idCmdArgs &args)
{
	bool setShadows;

	if (!gameLocal.CheatsOk(false)) { return; }
	if (!gameLocal.entities[0] || !gameLocal.entities[0]->IsType(hhPlayer::Type)) { return; }

	//set everyone to opposite of whatever first client is
	setShadows = !gameLocal.entities[0]->GetRenderEntity()->noShadow;

	const int	maxClients = gameLocal.serverInfo.GetInt("si_maxPlayers");
	for (int i = 0; i < maxClients; ++i)
	{
		if (!gameLocal.entities[i]) { break; }
		if (gameLocal.entities[i]->IsType(hhPlayer::Type))
		{
			gameLocal.entities[i]->GetRenderEntity()->noShadow = setShadows;
		}
	}
}
#endif
//HUMANHEAD END

/*
==================
Cmd_Damage_f

Damages the specified entity
==================
*/
void Cmd_Damage_f(const idCmdArgs &args)
{
	if (!gameLocal.GetLocalPlayer() || !gameLocal.CheatsOk(false)) { return; }
	if (args.Argc() != 3)
	{
		gameLocal.Printf("usage: damage <name of entity to damage> <damage>\n");
		return;
	}

	idEntity *ent = gameLocal.FindEntity(args.Argv(1));
	if (!ent)
	{
		gameLocal.Printf("entity not found\n");
		return;
	}

	ent->Damage(gameLocal.world, gameLocal.world, idVec3(0, 0, 1), "damage_moverCrush", atoi(args.Argv(2)), INVALID_JOINT);
}


/*
==================
Cmd_Remove_f

Removes the specified entity
==================
*/
void Cmd_Remove_f(const idCmdArgs &args)
{
	if (!gameLocal.GetLocalPlayer() || !gameLocal.CheatsOk(false)) { return; }
	if (args.Argc() != 2)
	{
		gameLocal.Printf("usage: remove <name of entity to remove>\n");
		return;
	}

	idEntity *ent = gameLocal.FindEntity(args.Argv(1));
	if (!ent)
	{
		gameLocal.Printf("entity not found\n");
		return;
	}

	delete ent;
}

/*
===================
Cmd_TestLight_f
===================
*/
void Cmd_TestLight_f(const idCmdArgs &args)
{
	int			i;
	idStr		filename;
	const char *key, *value, *name;
	idPlayer *	player;
	idDict		dict;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk(false)) { return; }

	renderView_t	*rv = player->GetRenderView();

	float fov = tan(idMath::M_DEG2RAD * rv->fov_x / 2);


	dict.SetMatrix("rotation", mat3_default);
	dict.SetVector("origin", rv->vieworg);
	dict.SetVector("light_target", rv->viewaxis[0]);
	dict.SetVector("light_right", rv->viewaxis[1] * -fov);
	dict.SetVector("light_up", rv->viewaxis[2] * fov);
	dict.SetVector("light_start", rv->viewaxis[0] * 16);
	dict.SetVector("light_end", rv->viewaxis[0] * 1000);

	if (args.Argc() >= 2)
	{
		value = args.Argv(1);
		filename = args.Argv(1);
		filename.DefaultFileExtension(".tga");
		dict.Set("texture", filename);
	}

	dict.Set("classname", "light");
	for (i = 2; i < args.Argc() - 1; i += 2)
	{

		key = args.Argv(i);
		value = args.Argv(i + 1);

		dict.Set(key, value);
	}

	for (i = 0; i < MAX_GENTITIES; ++i)
	{
		name = va("spawned_light_%d", i);		// not just light_, or it might pick up a prelight shadow
		if (!gameLocal.FindEntity(name)) { break; }
	}
	dict.Set("name", name);

	gameLocal.SpawnEntityDef(dict);

	gameLocal.Printf("Created new light\n");
}

/*
===================
Cmd_TestPointLight_f
===================
*/
void Cmd_TestPointLight_f(const idCmdArgs &args)
{
	const char *key, *value, *name;
	int			i;
	idPlayer	*player;
	idDict		dict;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk(false)) { return; }

	dict.SetVector("origin", player->GetRenderView()->vieworg);

	if (args.Argc() >= 2)
	{
		value = args.Argv(1);
		dict.Set("light", value);
	}
	else { dict.Set("light", "300"); }

	dict.Set("classname", "light");
	for (i = 2; i < args.Argc() - 1; i += 2)
	{

		key = args.Argv(i);
		value = args.Argv(i + 1);

		dict.Set(key, value);
	}

	for (i = 0; i < MAX_GENTITIES; ++i)
	{
		name = va("light_%d", i);
		if (!gameLocal.FindEntity(name)) { break; }
	}
	dict.Set("name", name);

	gameLocal.SpawnEntityDef(dict);

	gameLocal.Printf("Created new point light\n");
}

/*
==================
Cmd_PopLight_f
==================
*/
void Cmd_PopLight_f(const idCmdArgs &args)
{
	idEntity	*ent;
	idMapEntity *mapEnt;
	idMapFile	*mapFile = gameLocal.GetLevelMap();
	idLight		*lastLight;
	int			last;

	if (!gameLocal.CheatsOk()) { return; }

	bool removeFromMap = (args.Argc() > 1);

	lastLight = NULL;
	last = -1;
	for (ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = ent->spawnNode.Next())
	{
		if (!ent->IsType(idLight::Type)) { continue; }

		// HUMANHEAD pdm: don't allow popping of our generated lights (like headlights)
		if (ent->IsBound()) { continue; }
		// HUMANHEAD END

		if (gameLocal.spawnIds[ent->entityNumber] > last)
		{
			last = gameLocal.spawnIds[ent->entityNumber];
			lastLight = static_cast<idLight*>(ent);
		}
	}

	if (lastLight)
	{
		// find map file entity
		mapEnt = mapFile->FindEntity(lastLight->name);

		if (removeFromMap && mapEnt)
		{
			mapFile->RemoveEntity(mapEnt);
		}
		gameLocal.Printf("Removing light %i\n", lastLight->GetLightDefHandle());
		delete lastLight;
	}
	else { gameLocal.Printf("No lights to clear.\n"); }

}

/*
====================
Cmd_ClearLights_f
====================
*/
void Cmd_ClearLights_f(const idCmdArgs &args)
{
	idEntity *ent;
	idEntity *next;
	idLight *light;
	idMapEntity *mapEnt;
	idMapFile *mapFile = gameLocal.GetLevelMap();

	bool removeFromMap = (args.Argc() > 1);

	gameLocal.Printf("Clearing all lights.\n");
	for (ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = next)
	{
		next = ent->spawnNode.Next();
		if (!ent->IsType(idLight::Type)) { continue; }

		light = static_cast<idLight*>(ent);
		mapEnt = mapFile->FindEntity(light->name);

		if (removeFromMap && mapEnt) { mapFile->RemoveEntity(mapEnt); }
		// HUMANHEAD pdm: don't allow popping of our generated lights (like headlights)
		else if (ent->IsBound()) { continue; }
		// HUMANHEAD END

		delete light;
	}
}

/*
==================
Cmd_TestFx_f
==================
*/
void Cmd_TestFx_f(const idCmdArgs &args)
{
	idVec3		offset;
	const char *name;
	idPlayer *	player;
	idDict		dict;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	// delete the testModel if active
	if (gameLocal.testFx)
	{
		delete gameLocal.testFx;
		gameLocal.testFx = NULL;
	}

	if (args.Argc() < 2) { return; }

	name = args.Argv(1);

	offset = player->GetPhysics()->GetOrigin() + player->viewAngles.ToForward() * 100.0f;

	dict.Set("origin", offset.ToString());
	dict.Set("test", "1");
	dict.Set("fx", name);
	//HUMANHEAD rww - hhEntityFx
	gameLocal.testFx = static_cast<hhEntityFx *>(gameLocal.SpawnEntityType(hhEntityFx::Type, &dict));
}

#define MAX_DEBUGLINES	128

typedef struct
{
	bool used;
	idVec3 start, end;
	int color;
	bool blink;
	bool arrow;
} gameDebugLine_t;

gameDebugLine_t debugLines[MAX_DEBUGLINES];

/*
==================
Cmd_AddDebugLine_f
==================
*/
static void Cmd_AddDebugLine_f(const idCmdArgs &args)
{
	int i, argNum;
	const char *value;

	if (!gameLocal.CheatsOk()) { return; }

	if (args.Argc() < 7)
	{
		gameLocal.Printf("usage: addline <x y z> <x y z> <color>\n");
		return;
	}
	for (i = 0; i < MAX_DEBUGLINES; ++i)
	{
		if (!debugLines[i].used) { break; }
	}
	if (i >= MAX_DEBUGLINES)
	{
		gameLocal.Printf("no free debug lines\n");
		return;
	}
	value = args.Argv(0);
	if (!idStr::Icmp(value, "addarrow")) { debugLines[i].arrow = true; }
	else { debugLines[i].arrow = false; }
	debugLines[i].used = true;
	debugLines[i].blink = false;
	argNum = 1;
	debugLines[i].start.x = Cmd_GetFloatArg(args, argNum);
	debugLines[i].start.y = Cmd_GetFloatArg(args, argNum);
	debugLines[i].start.z = Cmd_GetFloatArg(args, argNum);
	debugLines[i].end.x = Cmd_GetFloatArg(args, argNum);
	debugLines[i].end.y = Cmd_GetFloatArg(args, argNum);
	debugLines[i].end.z = Cmd_GetFloatArg(args, argNum);
	debugLines[i].color = Cmd_GetFloatArg(args, argNum);
}

/*
HUMANHEAD rww
==================
Debug_ClearDebugLines
==================
*/
void Debug_ClearDebugLines(void)
{
	for (int i = 0; i < MAX_DEBUGLINES; ++i)
	{
		debugLines[i].used = false;
	}
}

/*
HUMANHEAD rww
==================
Debug_AddDebugLinesForTri
==================
*/
void Debug_AddDebugLinesForTri(srfTriangles_t *tri)
{
	int i;
	int j = 0;
	for (i = 0; i < MAX_DEBUGLINES; ++i)
	{
		if (!debugLines[i].used)
		{
			break;
		}
	}
	while (i < MAX_DEBUGLINES && j < tri->numVerts)
	{
		if (debugLines[i].used)
		{
			++i;
			continue;
		}
		debugLines[i].used = true;
		debugLines[i].blink = false;
		debugLines[i].start.x = 0;
		debugLines[i].start.y = 0;
		debugLines[i].start.z = 0;
		debugLines[i].end.x = tri->verts[j].xyz[0];
		debugLines[i].end.y = tri->verts[j].xyz[1];
		debugLines[i].end.z = tri->verts[j].xyz[2];
		debugLines[i].color = 1;

		++j;
		++i;
	}
}

/*
HUMANHEAD rww
==================
Debug_AddDebugLine
==================
*/
void Debug_AddDebugLine(idVec3 &start, idVec3 &end, int color)
{
	int i;
	for (i = 0; i < MAX_DEBUGLINES; ++i)
	{
		if (!debugLines[i].used)
		{
			break;
		}
	}
	if (i == MAX_DEBUGLINES)
	{ //we're full
		return;
	}

	debugLines[i].used = true;
	debugLines[i].blink = false;
	debugLines[i].start.x = start[0];
	debugLines[i].start.y = start[1];
	debugLines[i].start.z = start[2];
	debugLines[i].end.x = end[0];
	debugLines[i].end.y = end[1];
	debugLines[i].end.z = end[2];
	debugLines[i].color = color;
}

/*
==================
Cmd_RemoveDebugLine_f
==================
*/
static void Cmd_RemoveDebugLine_f(const idCmdArgs &args)
{
	int i, num;
	const char *value;

	if (!gameLocal.CheatsOk()) { return; }

	if (args.Argc() < 2)
	{
		gameLocal.Printf("usage: removeline <num>\n");
		return;
	}
	value = args.Argv(1);
	num = atoi(value);
	for (i = 0; i < MAX_DEBUGLINES; ++i)
	{
		if (debugLines[i].used)
		{
			if (--num < 0) { break; }
		}
	}
	if (i >= MAX_DEBUGLINES)
	{
		gameLocal.Printf("line not found\n");
		return;
	}
	debugLines[i].used = false;
}

/*
==================
Cmd_BlinkDebugLine_f
==================
*/
static void Cmd_BlinkDebugLine_f(const idCmdArgs &args)
{
	int i, num;
	const char *value;

	if (!gameLocal.CheatsOk()) { return; }

	if (args.Argc() < 2)
	{
		gameLocal.Printf("usage: blinkline <num>\n");
		return;
	}
	value = args.Argv(1);
	num = atoi(value);
	for (i = 0; i < MAX_DEBUGLINES; ++i)
	{
		if (debugLines[i].used)
		{
			if (--num < 0) { break; }
		}
	}
	if (i >= MAX_DEBUGLINES)
	{
		gameLocal.Printf("line not found\n");
		return;
	}
	debugLines[i].blink = !debugLines[i].blink;
}

/*
==================
PrintFloat
==================
*/
static void PrintFloat(float f)
{
	char buf[128], i;

	for (i = sprintf(buf, "%3.2f", f); i < 7; ++i) { buf[i] = ' '; }
	buf[i] = '\0';
	gameLocal.Printf(buf);
}

/*
==================
Cmd_ListDebugLines_f
==================
*/
static void Cmd_ListDebugLines_f(const idCmdArgs &args)
{
	int i, num;

	if (!gameLocal.CheatsOk()) { return; }

	num = 0;
	gameLocal.Printf("line num: x1     y1     z1     x2     y2     z2     c  b  a\n");
	for (i = 0; i < MAX_DEBUGLINES; ++i)
	{
		if (debugLines[i].used)
		{
			gameLocal.Printf("line %3d: ", num);
			PrintFloat(debugLines[i].start.x);
			PrintFloat(debugLines[i].start.y);
			PrintFloat(debugLines[i].start.z);
			PrintFloat(debugLines[i].end.x);
			PrintFloat(debugLines[i].end.y);
			PrintFloat(debugLines[i].end.z);
			gameLocal.Printf("%d  %d  %d\n", debugLines[i].color, debugLines[i].blink, debugLines[i].arrow);
			++num;
		}
	}
	if (!num) { gameLocal.Printf("no debug lines\n"); }
}

/*
==================
D_DrawDebugLines
==================
*/
void D_DrawDebugLines(void)
{
	int i;
	idVec3 forward, right, up, p1, p2;
	idVec4 color;
	float l;

	for (i = 0; i < MAX_DEBUGLINES; ++i)
	{
		if (debugLines[i].used)
		{
			if (!debugLines[i].blink || (gameLocal.time & (1 << 9)))
			{
				color = idVec4(debugLines[i].color & 1, (debugLines[i].color >> 1) & 1, (debugLines[i].color >> 2) & 1, 1);
				gameRenderWorld->DebugLine(color, debugLines[i].start, debugLines[i].end);
				//
				if (debugLines[i].arrow)
				{
					// draw a nice arrow
					forward = debugLines[i].end - debugLines[i].start;
					l = forward.Normalize() * 0.2f;
					forward.NormalVectors(right, up);

					if (l > 3.0f) { l = 3.0f; }
					p1 = debugLines[i].end - l * forward + (l * 0.4f) * right;
					p2 = debugLines[i].end - l * forward - (l * 0.4f) * right;
					gameRenderWorld->DebugLine(color, debugLines[i].end, p1);
					gameRenderWorld->DebugLine(color, debugLines[i].end, p2);
					gameRenderWorld->DebugLine(color, p1, p2);
				}
			}
		}
	}
}

/*
==================
Cmd_ListCollisionModels_f
==================
*/
static void Cmd_ListCollisionModels_f(const idCmdArgs &args)
{
	if (!gameLocal.CheatsOk()) { return; }

	collisionModelManager->ListModels();
}

/*
==================
Cmd_CollisionModelInfo_f
==================
*/
static void Cmd_CollisionModelInfo_f(const idCmdArgs &args)
{
	const char *value;

	if (!gameLocal.CheatsOk()) { return; }

	if (args.Argc() < 2)
	{
		gameLocal.Printf("usage: collisionModelInfo <modelNum>\n"
			"use 'all' instead of the model number for accumulated info\n");
		return;
	}

	value = args.Argv(1);
	if (!idStr::Icmp(value, "all")) { collisionModelManager->ModelInfo(-1); }
	else { collisionModelManager->ModelInfo(atoi(value)); }
}

/*
==================
Cmd_ExportModels_f
==================
*/
static void Cmd_ExportModels_f(const idCmdArgs &args)
{
	idModelExport	exporter;
	idStr			name;

	// don't allow exporting models when cheats are disabled,
	// but if we're not in the game, it's ok
	if (gameLocal.GetLocalPlayer() && !gameLocal.CheatsOk(false)) { return; }

	if (args.Argc() < 2) { exporter.ExportModels("def", ".def"); }
	else
	{
		name = args.Argv(1);
		name = "def/" + name;
		name.DefaultFileExtension(".def");
		exporter.ExportDefFile(name);
	}
}

/*
==================
Cmd_ReexportModels_f
==================
*/
static void Cmd_ReexportModels_f(const idCmdArgs &args)
{
	idModelExport	exporter;
	idStr			name;

	// don't allow exporting models when cheats are disabled,
	// but if we're not in the game, it's ok
	if (gameLocal.GetLocalPlayer() && !gameLocal.CheatsOk(false)) { return; }

	idAnimManager::forceExport = true;
	if (args.Argc() < 2) { exporter.ExportModels("def", ".def"); }
	else
	{
		name = args.Argv(1);
		name = "def/" + name;
		name.DefaultFileExtension(".def");
		exporter.ExportDefFile(name);
	}
	idAnimManager::forceExport = false;
}

/*
==================
Cmd_ReloadAnims_f
==================
*/
static void Cmd_ReloadAnims_f(const idCmdArgs &args)
{
	// don't allow reloading anims when cheats are disabled,
	// but if we're not in the game, it's ok
	if (gameLocal.GetLocalPlayer() && !gameLocal.CheatsOk(false)) { return; }

	animationLib.ReloadAnims();
}

/*
==================
Cmd_ListAnims_f
==================
*/
static void Cmd_ListAnims_f(const idCmdArgs &args)
{
	idEntity *		ent;
	int				num;
	size_t			size;
	size_t			alloced;
	idAnimator *	animator;
	const char *	classname;
	const idDict *	dict;
	int				i;

	if (args.Argc() > 1)
	{
		idAnimator animator;

		classname = args.Argv(1);

		dict = gameLocal.FindEntityDefDict(classname, false);
		if (!dict)
		{
			gameLocal.Printf("Entitydef '%s' not found\n", classname);
			return;
		}
		animator.SetModel(dict->GetString("model"));

		gameLocal.Printf("----------------\n");
		num = animator.NumAnims();
		for (i = 0; i < num; ++i) { gameLocal.Printf("%s\n", animator.AnimFullName(i)); }
		gameLocal.Printf("%d anims\n", num);
	}
	else
	{
		animationLib.ListAnims();

		size = 0;
		num = 0;
		for (ent = gameLocal.spawnedEntities.Next(); ent != NULL; ent = ent->spawnNode.Next())
		{
			animator = ent->GetAnimator();
			if (animator)
			{
				alloced = animator->Allocated();
				size += alloced;
				++num;
			}
		}

		gameLocal.Printf("%d memory used in %d entity animators\n", size, num);
	}
}

/*
==================
Cmd_AASStats_f
==================
*/
static void Cmd_AASStats_f(const idCmdArgs &args)
{
	int aasNum;

	if (!gameLocal.CheatsOk()) { return; }

	aasNum = aas_test.GetInteger();
	idAAS *aas = gameLocal.GetAAS(aasNum);
	if (!aas) { gameLocal.Printf("No aas #%d loaded\n", aasNum); }
	else { aas->Stats(); }
}

/*
==================
Cmd_TestDamage_f
==================
*/
static void Cmd_TestDamage_f(const idCmdArgs &args)
{
	idPlayer *player;
	const char *damageDefName;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (args.Argc() < 2 || args.Argc() > 3)
	{
		gameLocal.Printf("usage: testDamage <damageDefName> [angle]\n");
		return;
	}

	damageDefName = args.Argv(1);

	idVec3	dir;
	if (args.Argc() == 3)
	{
		float angle = atof(args.Argv(2));

		idMath::SinCos(DEG2RAD(angle), dir[1], dir[0]);
		dir[2] = 0;
	}
	else { dir.Zero(); }

	// give the player full health before and after
	// running the damage
	player->health = player->inventory.maxHealth;
	player->Damage(NULL, NULL, dir, damageDefName, 1.0f, INVALID_JOINT);
	player->health = player->inventory.maxHealth;
}

/*
==================
Cmd_TestBoneFx_f
==================
*/
static void Cmd_TestBoneFx_f(const idCmdArgs &args)
{
	idPlayer *player;
	const char *bone, *fx;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (args.Argc() < 3 || args.Argc() > 4)
	{
		gameLocal.Printf("usage: testBoneFx <fxName> <boneName>\n");
		return;
	}

	fx = args.Argv(1);
	bone = args.Argv(2);

	player->StartFxOnBone(fx, bone);
}

/*
==================
Cmd_TestDamage_f
==================
*/
static void Cmd_TestDeath_f(const idCmdArgs &args)
{
	idPlayer *player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	idVec3 dir;
	idMath::SinCos(DEG2RAD(45.0f), dir[1], dir[0]);
	dir[2] = 0;

	g_testDeath.SetBool(1);
	player->Damage(NULL, NULL, dir, "damage_triggerhurt_1000", 1.0f, INVALID_JOINT);
	if (args.Argc() >= 2) { player->SpawnGibs(dir, "damage_triggerhurt_1000"); }

}

/*
==================
Cmd_WeaponSplat_f
==================
*/
static void Cmd_WeaponSplat_f(const idCmdArgs &args)
{
	idPlayer *player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	//HUMANHEAD: aob - we don't have BloodSplat anymore
	//player->weapon.GetEntity()->BloodSplat( 2.0f );
}

/*
==================
Cmd_SaveSelected_f
==================
*/
static void Cmd_SaveSelected_f(const idCmdArgs &args)
{
	int i;
	idPlayer *player;
	idEntity *s;
	idMapEntity *mapEnt;
	idMapFile *mapFile = gameLocal.GetLevelMap();
	idDict dict;
	idStr mapName;
	const char *name;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	s = player->dragEntity.GetSelected();
	if (!s)
	{
		gameLocal.Printf("no entity selected, set g_dragShowSelection 1 to show the current selection\n");
		return;
	}

	if (args.Argc() > 1)
	{
		mapName = args.Argv(1);
		mapName = "maps/" + mapName;
	}
	else { mapName = mapFile->GetName(); }

	// find map file entity
	mapEnt = mapFile->FindEntity(s->name);
	// create new map file entity if there isn't one for this articulated figure
	if (!mapEnt)
	{
		mapEnt = new idMapEntity();
		mapFile->AddEntity(mapEnt);
		for (i = 0; i < 9999; ++i)
		{
			name = va("%s_%d", s->GetEntityDefName(), i);
			if (!gameLocal.FindEntity(name)) { break; }
		}
		s->name = name;
		mapEnt->epairs.Set("classname", s->GetEntityDefName());
		mapEnt->epairs.Set("name", s->name);
	}

	if (s->IsType(idMoveable::Type))
	{
		// save the moveable state
		mapEnt->epairs.Set("origin", s->GetPhysics()->GetOrigin().ToString(8));
		mapEnt->epairs.Set("rotation", s->GetPhysics()->GetAxis().ToString(8));
	}
	else if (s->IsType(idAFEntity_Generic::Type) || s->IsType(idAFEntity_WithAttachedHead::Type))
	{
		// save the articulated figure state
		dict.Clear();
		static_cast<idAFEntity_Base *>(s)->SaveState(dict);
		mapEnt->epairs.Copy(dict);
	}

	// write out the map file
	mapFile->Write(mapName, ".map");
}

/*
==================
Cmd_DeleteSelected_f
==================
*/
static void Cmd_DeleteSelected_f(const idCmdArgs &args)
{
	idPlayer *player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (player) { player->dragEntity.DeleteSelected(); }
}

/*
==================
Cmd_SaveMoveables_f
==================
*/
static void Cmd_SaveMoveables_f(const idCmdArgs &args)
{
	int e, i;
	idMoveable *m;
	idMapEntity *mapEnt;
	idMapFile *mapFile = gameLocal.GetLevelMap();
	idStr mapName;
	const char *name;

	if (!gameLocal.CheatsOk()) { return; }

	for (e = 0; e < MAX_GENTITIES; ++e)
	{
		m = static_cast<idMoveable *>(gameLocal.entities[e]);

		if (!m || !m->IsType(idMoveable::Type)) { continue; }

		if (m->IsBound()) { continue; }

		if (!m->IsAtRest()) { break; }
	}

	if (e < MAX_GENTITIES)
	{
		gameLocal.Warning("map not saved because the moveable entity %s is not at rest", gameLocal.entities[e]->name.c_str());
		return;
	}

	if (args.Argc() > 1)
	{
		mapName = args.Argv(1);
		mapName = "maps/" + mapName;
	}
	else { mapName = mapFile->GetName(); }

	for (e = 0; e < MAX_GENTITIES; ++e)
	{
		m = static_cast<idMoveable *>(gameLocal.entities[e]);

		if (!m || !m->IsType(idMoveable::Type)) { continue; }

#if HUMANHEAD	// HUMANHEAD pdm: skip all deathwalk entities
		if (!idStr::Icmpn(m->name, "dw_", 3)) { continue; }
#endif

		if (m->IsBound()) { continue; }

		// find map file entity
		mapEnt = mapFile->FindEntity(m->name);
		// create new map file entity if there isn't one for this articulated figure
		if (!mapEnt)
		{
			mapEnt = new idMapEntity();
			mapFile->AddEntity(mapEnt);
			for (i = 0; i < 9999; ++i)
			{
				name = va("%s_%d", m->GetEntityDefName(), i);
				if (!gameLocal.FindEntity(name)) { break; }
			}
			m->name = name;
			mapEnt->epairs.Set("classname", m->GetEntityDefName());
			mapEnt->epairs.Set("name", m->name);
		}
		// save the moveable state
		mapEnt->epairs.Set("origin", m->GetPhysics()->GetOrigin().ToString(8));
		mapEnt->epairs.Set("rotation", m->GetPhysics()->GetAxis().ToString(8));
	}

	// write out the map file
	mapFile->Write(mapName, ".map");
}

/*
==================
Cmd_SaveRagdolls_f
==================
*/
static void Cmd_SaveRagdolls_f(const idCmdArgs &args)
{
	int e, i;
	idAFEntity_Base *af;
	idMapEntity *mapEnt;
	idMapFile *mapFile = gameLocal.GetLevelMap();
	idDict dict;
	idStr mapName;
	const char *name;

	if (!gameLocal.CheatsOk()) { return; }

	if (args.Argc() > 1)
	{
		mapName = args.Argv(1);
		mapName = "maps/" + mapName;
	}
	else { mapName = mapFile->GetName(); }

	for (e = 0; e < MAX_GENTITIES; ++e)
	{
		af = static_cast<idAFEntity_Base *>(gameLocal.entities[e]);

		if (!af) { continue; }

		if (!af->IsType(idAFEntity_WithAttachedHead::Type) && !af->IsType(idAFEntity_Generic::Type)) { continue; }

#if HUMANHEAD	// HUMANHEAD pdm: skip all deathwalk entities
		if (!idStr::Icmpn(af->name, "dw_", 3)) { continue; }
#endif

		if (af->IsBound()) { continue; }

		if (!af->IsAtRest())
		{
			gameLocal.Warning("the articulated figure for entity %s is not at rest", gameLocal.entities[e]->name.c_str());
		}

		dict.Clear();
		af->SaveState(dict);

		// find map file entity
		mapEnt = mapFile->FindEntity(af->name);
		// create new map file entity if there isn't one for this articulated figure
		if (!mapEnt)
		{
			mapEnt = new idMapEntity();
			mapFile->AddEntity(mapEnt);
			for (i = 0; i < 9999; ++i)
			{
				name = va("%s_%d", af->GetEntityDefName(), i);
				if (!gameLocal.FindEntity(name)) { break; }
			}
			af->name = name;
			mapEnt->epairs.Set("classname", af->GetEntityDefName());
			mapEnt->epairs.Set("name", af->name);
		}
		// save the articulated figure state
		mapEnt->epairs.Copy(dict);
	}

	// write out the map file
	mapFile->Write(mapName, ".map");
}

/*
==================
Cmd_BindRagdoll_f
==================
*/
static void Cmd_BindRagdoll_f(const idCmdArgs &args)
{
	idPlayer *player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (player) { player->dragEntity.BindSelected(); }
}

/*
==================
Cmd_UnbindRagdoll_f
==================
*/
static void Cmd_UnbindRagdoll_f(const idCmdArgs &args)
{
	idPlayer *player;

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk()) { return; }

	if (player) { player->dragEntity.UnbindSelected(); }
}

/*
==================
Cmd_GameError_f
==================
*/
static void Cmd_GameError_f(const idCmdArgs &args)
{
	gameLocal.Error("game error");
}

/*
==================
Cmd_SaveLights_f
==================
*/
static void Cmd_SaveLights_f(const idCmdArgs &args)
{
	int e, i;
	idLight *light;
	idMapEntity *mapEnt;
	idMapFile *mapFile = gameLocal.GetLevelMap();
	idDict dict;
	idStr mapName;
	const char *name;

	if (!gameLocal.CheatsOk()) { return; }

	if (args.Argc() > 1)
	{
		mapName = args.Argv(1);
		mapName = "maps/" + mapName;
	}
	else { mapName = mapFile->GetName(); }

	for (e = 0; e < MAX_GENTITIES; ++e)
	{
		light = static_cast<idLight*>(gameLocal.entities[e]);

		if (!light || !light->IsType(idLight::Type)) { continue; }

#if HUMANHEAD	// HUMANHEAD pdm: skip all deathwalk entities
		if (!idStr::Icmpn(light->name, "dw_", 3)) { continue; }
#endif

		dict.Clear();
		light->SaveState(&dict);

		// find map file entity
		mapEnt = mapFile->FindEntity(light->name);
		// create new map file entity if there isn't one for this light
		if (!mapEnt)
		{
			mapEnt = new idMapEntity();
			mapFile->AddEntity(mapEnt);
			for (i = 0; i < 9999; ++i)
			{
				name = va("%s_%d", light->GetEntityDefName(), i);
				if (!gameLocal.FindEntity(name)) { break; }
			}
			light->name = name;
			mapEnt->epairs.Set("classname", light->GetEntityDefName());
			mapEnt->epairs.Set("name", light->name);
		}
		// save the light state
		mapEnt->epairs.Copy(dict);
	}

	// write out the map file
	mapFile->Write(mapName, ".map");
}


/*
==================
Cmd_SaveParticles_f
==================
*/
static void Cmd_SaveParticles_f(const idCmdArgs &args)
{
	int e;
	idEntity *ent;
	idMapEntity *mapEnt;
	idMapFile *mapFile = gameLocal.GetLevelMap();
	idDict dict;
	idStr mapName, strModel;

	if (!gameLocal.CheatsOk()) { return; }

	if (args.Argc() > 1)
	{
		mapName = args.Argv(1);
		mapName = "maps/" + mapName;
	}
	else { mapName = mapFile->GetName(); }

	for (e = 0; e < MAX_GENTITIES; e++)
	{

		ent = static_cast<idStaticEntity*> (gameLocal.entities[e]);

		if (!ent) { continue; }

#if HUMANHEAD	// HUMANHEAD pdm: skip all deathwalk entities
		if (!idStr::Icmpn(ent->name, "dw_", 3))
		{
			continue;
		}
#endif

		strModel = ent->spawnArgs.GetString("model");
		if (strModel.Length() && strModel.Find(".prt") > 0)
		{
			dict.Clear();
			dict.Set("model", ent->spawnArgs.GetString("model"));
			dict.SetVector("origin", ent->GetPhysics()->GetOrigin());

			// find map file entity
			mapEnt = mapFile->FindEntity(ent->name);
			// create new map file entity if there isn't one for this entity
			if (!mapEnt) { continue; }
			// save the particle state
			mapEnt->epairs.Copy(dict);
		}
	}

	// write out the map file
	mapFile->Write(mapName, ".map");
}


/*
==================
Cmd_DisasmScript_f
==================
*/
static void Cmd_DisasmScript_f(const idCmdArgs &args)
{
	gameLocal.program.Disassemble();
}

/*
==================
Cmd_TestSave_f
==================
*/
static void Cmd_TestSave_f(const idCmdArgs &args)
{
	idFile *f;

	f = fileSystem->OpenFileWrite("test.sav");
	gameLocal.SaveGame(f);
	fileSystem->CloseFile(f);
}

/*
==================
Cmd_RecordViewNotes_f
==================
*/
static void Cmd_RecordViewNotes_f(const idCmdArgs &args)
{
	idPlayer *player;
	idVec3 origin;
	idMat3 axis;

	if (args.Argc() <= 3) { return; }

	player = gameLocal.GetLocalPlayer();
	if (!player) { return; }

	player->GetViewPos(origin, axis);

	// Argv(1) = filename for map (viewnotes/mapname/person)
	// Argv(2) = note number (person0001)
	// Argv(3) = comments

	// HUMANHEAD pdm: prepended p:/base/
	idStr str = "p:/base/";
	str += args.Argv(1);
	str.SetFileExtension(".txt");
	// HUMANHEAD pdm: made explicit so we can write to P drive
	idFile *file = fileSystem->OpenExplicitFileAppend(str);
	if (file)
	{
		file->WriteFloatString("\"view\"\t( %s )\t( %s )\r\n", origin.ToString(), axis.ToString());
		file->WriteFloatString("\"comments\"\t\"%s: %s\"\r\n\r\n", args.Argv(2), args.Argv(3));
		fileSystem->CloseFile(file);
	}

	idStr viewComments = args.Argv(1);
	viewComments.StripLeading("viewnotes/");
	viewComments += " -- Loc: ";
	viewComments += origin.ToString();
	viewComments += "\n";
	viewComments += args.Argv(3);
	player->hud->SetStateString("viewcomments", viewComments);
	player->hud->HandleNamedEvent("showViewComments");
}

/*
==================
Cmd_CloseViewNotes_f
==================
*/
static void Cmd_CloseViewNotes_f(const idCmdArgs &args)
{
	idPlayer *player = gameLocal.GetLocalPlayer();

	if (!player) { return; }

	player->hud->SetStateString("viewcomments", "");
	player->hud->HandleNamedEvent("hideViewComments");
}

/*
==================
Cmd_ShowViewNotes_f
==================
*/
static void Cmd_ShowViewNotes_f(const idCmdArgs &args)
{
	static idLexer parser(LEXFL_ALLOWPATHNAMES | LEXFL_NOSTRINGESCAPECHARS | LEXFL_NOSTRINGCONCAT | LEXFL_NOFATALERRORS);
	idToken	token;
	idPlayer *player;
	idVec3 origin;
	idMat3 axis;

	player = gameLocal.GetLocalPlayer();

	if (!player) { return; }

	if (!parser.IsLoaded())
	{
		idStr str = "p:\\base\\viewnotes\\";	// HUMANHEAD pdm: added p:\base and changed to backslashes
		str += gameLocal.GetMapName();
		str.StripFileExtension();
		str += "\\";	// HUMANHEAD pdm: changed to backslashes
		if (args.Argc() > 1) { str += args.Argv(1); }
		else { str += "comments"; }
		str.SetFileExtension(".txt");
		if (!parser.LoadFile(str, true))
		{	// HUMANHEAD pdm: added 'true' to treat as OS path
			gameLocal.Printf("No view notes for %s\n", gameLocal.GetMapName());
			return;
		}
	}

	if (parser.ExpectTokenString("view") && parser.Parse1DMatrix(3, origin.ToFloatPtr()) &&
		parser.Parse1DMatrix(9, axis.ToFloatPtr()) && parser.ExpectTokenString("comments") && parser.ReadToken(&token))
	{
		player->hud->SetStateString("viewcomments", token);
		player->hud->HandleNamedEvent("showViewComments");
		player->Teleport(origin, axis.ToAngles(), NULL);
	}
	else
	{
		parser.FreeSource();
		player->hud->HandleNamedEvent("hideViewComments");
		player->hud->SetStateString("viewcomments", ""); // HUMANHEAD mdl:  So we don't erase when nothing is displayed
		return;
	}
}

// HUMANHEAD mdl:  Added EraseViewNote
/*
==================
Cmd_EraseViewNote_f
==================
*/
static void Cmd_EraseViewNote_f(const idCmdArgs &args)
{
	idLexer parser(LEXFL_ALLOWPATHNAMES | LEXFL_NOSTRINGESCAPECHARS | LEXFL_NOSTRINGCONCAT | LEXFL_NOFATALERRORS);
	idToken	token;
	idPlayer *player;
	idVec3 origin;
	idMat3 axis;

	player = gameLocal.GetLocalPlayer();

	if (!player)
	{
		return;
	}

	const char *comment = player->hud->State().GetString("viewcomments");
	if (!comment)
	{
		gameLocal.Warning("No currently displayed comment.\n");
		return;
	}

	idStr str = "p:\\base\\viewnotes\\";	// HUMANHEAD pdm: added p:\base and changed to backslashes
	str += gameLocal.GetMapName();
	str.StripFileExtension();
	str += "\\";	// HUMANHEAD pdm: changed to backslashes
	if (args.Argc() > 1)
	{
		str += args.Argv(1);
	}
	else
	{
		str += "comments";
	}
	str.SetFileExtension(".txt");
	if (!parser.LoadFile(str, true))
	{	// HUMANHEAD pdm: added 'true' to treat as OS path
		gameLocal.Printf("No view notes for %s\n", gameLocal.GetMapName());
		return;
	}

	idFile *file = fileSystem->OpenExplicitFileWrite(str);
	if (!file)
	{
		gameLocal.Warning("Failed to open view notes file for writing.\n");
		return;
	}

	bool found = false;
	while (parser.ExpectTokenString("view") && parser.Parse1DMatrix(3, origin.ToFloatPtr()) &&
		parser.Parse1DMatrix(9, axis.ToFloatPtr()) && parser.ExpectTokenString("comments") && parser.ReadToken(&token))
	{

		if (idStr::Cmp(comment, token) == 0)
		{

			if (found) { gameLocal.Warning("Found note more than once...\n"); }
			found = true;
			continue;
		}

		file->WriteFloatString("\"view\"\t( %s )\t( %s )\r\n", origin.ToString(), axis.ToString());
		file->WriteFloatString("\"comments\"\t\"%s\"\r\n\r\n", static_cast<const char *>(token));
	}

	if (found) { gameLocal.Printf("Removed note.\n"); }
	else { gameLocal.Printf("Couldn't find note.\n"); }

#if !GOLD
	bool removeFile = (file->Length() == 0);
#endif

	fileSystem->CloseFile(file);
	player->hud->HandleNamedEvent("hideViewComments");

#if !GOLD
	if (removeFile)
	{
		remove(str);
}
#endif
}
// HUMANHEAD END

/*
=================
FindEntityGUIs

helper function for Cmd_NextGUI_f.  Checks the passed entity to determine if it
has any valid gui surfaces.
=================
*/
bool FindEntityGUIs(idEntity *ent, const modelSurface_t ** surfaces, int maxSurfs, int &guiSurfaces)
{
	renderEntity_t			*renderEnt;
	idRenderModel			*renderModel;
	const modelSurface_t	*surf;
	const idMaterial		*shader;
	int						i;

	assert(surfaces != NULL);
	assert(ent != NULL);

	memset(surfaces, 0x00, sizeof(modelSurface_t *) * maxSurfs);
	guiSurfaces = 0;

	renderEnt = ent->GetRenderEntity();
	renderModel = renderEnt->hModel;
	if (renderModel == NULL)
	{
		return false;
	}

	for (i = 0; i < renderModel->NumSurfaces(); ++i)
	{
		surf = renderModel->Surface(i);
		if (surf == NULL)
		{
			continue;
		}
		shader = surf->shader;
		if (shader == NULL)
		{
			continue;
		}
		if (shader->GetEntityGui() > 0)
		{
			surfaces[guiSurfaces++] = surf;
		}
	}

	return (guiSurfaces != 0);
}

/*
=================
Cmd_NextGUI_f
=================
*/
void Cmd_NextGUI_f(const idCmdArgs &args)
{
	idVec3					origin;
	idAngles				angles;
	idPlayer				*player;
	idEntity				*ent;
	int						guiSurfaces;
	bool					newEnt;
	renderEntity_t			*renderEnt;
	int						surfIndex;
	srfTriangles_t			*geom;
	idMat4					modelMatrix;
	idVec3					normal;
	idVec3					center;
	const modelSurface_t	*surfaces[MAX_RENDERENTITY_GUI];

	player = gameLocal.GetLocalPlayer();
	if (!player || !gameLocal.CheatsOk())
	{
		return;
	}

	if (args.Argc() != 1)
	{
		gameLocal.Printf("usage: nextgui\n");
		return;
	}

	// start at the last entity
	ent = gameLocal.lastGUIEnt.GetEntity();

	// see if we have any gui surfaces left to go to on the current entity.
	guiSurfaces = 0;
	newEnt = false;
	if (ent == NULL)
	{
		newEnt = true;
	}
	else if (FindEntityGUIs(ent, surfaces, MAX_RENDERENTITY_GUI, guiSurfaces) == true)
	{
		if (gameLocal.lastGUI >= guiSurfaces)
		{
			newEnt = true;
		}
	}
	else
	{
		// no actual gui surfaces on this ent, so skip it
		newEnt = true;
	}

	if (newEnt == true)
	{
		// go ahead and skip to the next entity with a gui...
		if (ent == NULL)
		{
			ent = gameLocal.spawnedEntities.Next();
		}
		else
		{
			ent = ent->spawnNode.Next();
		}

		for (; ent != NULL; ent = ent->spawnNode.Next())
		{
			// HUMANHEAD pdm: added exclusions for animated entities since they can't be teleported to
			if (!ent->IsType(hhAnimatedEntity::Type) && ent->spawnArgs.GetString("gui", NULL) != NULL) { break; }

			if (!ent->IsType(hhAnimatedEntity::Type) && ent->spawnArgs.GetString("gui2", NULL) != NULL) { break; }

			if (!ent->IsType(hhAnimatedEntity::Type) && ent->spawnArgs.GetString("gui3", NULL) != NULL) { break; }

			// try the next entity
			gameLocal.lastGUIEnt = ent;
		}

		gameLocal.lastGUIEnt = ent;
		gameLocal.lastGUI = 0;

		if (!ent)
		{
			gameLocal.Printf("No more gui entities. Starting over...\n");
			return;
		}
	}

	if (FindEntityGUIs(ent, surfaces, MAX_RENDERENTITY_GUI, guiSurfaces) == false)
	{
		gameLocal.Printf("Entity \"%s\" has gui properties but no gui surfaces.\n", ent->name.c_str());
	}

	if (guiSurfaces == 0)
	{
		gameLocal.Printf("Entity \"%s\" has gui properties but no gui surfaces!\n", ent->name.c_str());
		return;
	}

	gameLocal.Printf("Teleporting to gui entity \"%s\", gui #%d.\n", ent->name.c_str(), gameLocal.lastGUI);

	renderEnt = ent->GetRenderEntity();
	surfIndex = gameLocal.lastGUI++;
	geom = surfaces[surfIndex]->geometry;
	if (geom == NULL)
	{
		gameLocal.Printf("Entity \"%s\" has gui surface %d without geometry!\n", ent->name.c_str(), surfIndex);
		return;
	}

	assert(geom->facePlanes != NULL);

	modelMatrix = idMat4(renderEnt->axis, renderEnt->origin);
	normal = geom->facePlanes[0].Normal() * renderEnt->axis;
	center = geom->bounds.GetCenter() * modelMatrix;

	origin = center + (normal * 32.0f);
	origin.z -= player->EyeHeight();
	normal *= -1.0f;
	angles = normal.ToAngles();

	//	make sure the player is in noclip
	player->noclip = true;
	player->Teleport(origin, angles, NULL);
}

static void ArgCompletion_DefFile(const idCmdArgs &args, void(*callback)(const char *s))
{
	cmdSystem->ArgCompletion_FolderExtension(args, callback, "def/", true, ".def", NULL);
}

/*
===============
Cmd_TestId_f
outputs a string from the string table for the specified id
===============
*/
void Cmd_TestId_f(const idCmdArgs &args)
{
	idStr	id;
	int		i;
	if (args.Argc() == 1)
	{
		common->Printf("usage: testid <string id>\n");
		return;
	}

	for (i = 1; i < args.Argc(); i++)
	{
		id += args.Argv(i);
	}
	if (idStr::Cmpn(id, STRTABLE_ID, STRTABLE_ID_LENGTH) != 0)
	{
		id = STRTABLE_ID + id;
	}
	gameLocal.mpGame.AddChatLine(common->GetLanguageDict()->GetString(id), "<nothing>", "<nothing>", "<nothing>");
}

//HUMANHEAD rww
#if !GOLD
void Cmd_CharSet_f(const idCmdArgs &args)
{
	int ix, jx;

	common->Printf("   ");
	for (ix = 0; ix < 16; ix++)
	{
		common->Printf("%x", ix);
	}
	common->Printf("\n");

	for (ix = 0; ix < 16; ix++)
	{
		common->Printf("%x ", ix);
		for (jx = 0; jx < 16; jx++)
		{
			if (ix == 0 && (jx == 0 || jx == 9 || jx == 10 || jx == 13))
			{
				common->Printf(" ");
			}
			else
			{
				common->Printf("%c", (char) (ix * 16 + jx));
			}
		}
		common->Printf("\n");
	}
}

/*
===============
Cmd_PrintTypeName_f
===============
*/
void Cmd_PrintTypeName_f(const idCmdArgs &args)
{
	if (args.Argc() == 1)
	{
		common->Printf("usage: printTypeName <type num>\n");
		return;
	}

	int typeNum = atoi(args.Argv(1));
	idTypeInfo *type = idClass::GetType(typeNum);
	if (type)
	{
		common->Printf("Type '%i' is '%s'\n", typeNum, type->classname);
	}
	else
	{
		common->Printf("Invalid typenum.\n");
	}
}
#endif
/*
===============
Cmd_PlayTime_f
HUMANHEAD mdl
===============
*/
void Cmd_PlayTime_f(const idCmdArgs &args)
{
	unsigned int seconds, minutes, hours, time;
	time = MS2SEC(gameLocal.GetTimePlayed());
	seconds = time % 60;
	minutes = (time / 60) % 60;
	hours = time / 3600;
	common->Printf("%u:%.2u:%.2u\n", hours, minutes, seconds);
}
//HUMANHEAD END

/*
=================
idGameLocal::InitConsoleCommands

Let the system know about all of our commands
so it can perform tab completion
=================
*/
void idGameLocal::InitConsoleCommands(void)
{
	// PreyRun BEGIN
#ifdef PR_DEVELOP
	cmdSystem->AddCommand("PR_TimeDemo", Cmd_PR_timedemo_f, CMD_FL_GAME, "*Unfinished* PreyRun cmd: Displays plays a demo and tells you how long it took");
#endif // PR_DEVELOP
	cmdSystem->AddCommand("PR_Reload", Cmd_PR_reload_f, CMD_FL_GAME, "PreyRun cmd: Load the latest savegame");

	cmdSystem->AddCommand("PR_PreySplit_Split", Cmd_PR_preysplit_split_f, CMD_FL_GAME, "PreyRun cmd: Tells PreySplit to do an custom split");

	// Hud-Timer Commands
	cmdSystem->AddCommand("PR_Timer_Start", Cmd_PR_timer_start_f, CMD_FL_GAME, "PreyRun cmd: Starts the in-game timer. pr_hud_timer must be enabled for the time display to appear on the screen");
	cmdSystem->AddCommand("PR_Timer_Stop", Cmd_PR_timer_stop_f, CMD_FL_GAME, "PreyRun cmd: Stops the in-game timer");
	cmdSystem->AddCommand("PR_Timer_Reset", Cmd_PR_timer_reset_f, CMD_FL_GAME, "PreyRun cmd: Stops and resets the in-game timer");
	cmdSystem->AddCommand("PR_Timer_Now", Cmd_PR_timer_now_f, CMD_FL_GAME, "PreyRun cmd: Displays the current time of the in-game timer");

	// Autocmdzone Commands
	cmdSystem->AddCommand("PR_AutoCmd_Add", Cmd_PR_autocmd_add_f, CMD_FL_GAME, "PreyRun cmd: Add a autocmdzone");
	cmdSystem->AddCommand("PR_AutoCmd_Clear", Cmd_PR_autocmd_clear_f, CMD_FL_GAME, "PreyRun cmd: Remove all autocmdzones");
	cmdSystem->AddCommand("PR_AutoCmd_Edit", Cmd_PR_autocmd_edit_f, CMD_FL_GAME, "PreyRun cmd: Edit a autocmdzone by index retrieved from PR_AutoCmd_List");
	cmdSystem->AddCommand("PR_AutoCmd_List", Cmd_PR_autocmd_list_f, CMD_FL_GAME, "PreyRun cmd: List all autocmdzones with Index");
	cmdSystem->AddCommand("PR_AutoCmd_Remove", Cmd_PR_autocmd_remove_f, CMD_FL_GAME, "PreyRun cmd: Removes a autocmdzone by index retrieved from PR_AutoCmd_List");

	// Cheat Commands
	cmdSystem->AddCommand("PR_CH_SetHealth", Cmd_PR_ch_sethealth_f, CMD_FL_GAME | CMD_FL_CHEAT, "*Cheat* PreyRun cmd: Set your health to the specified value");
	cmdSystem->AddCommand("PR_CH_SetPos", Cmd_PR_ch_setpos_f, CMD_FL_GAME | CMD_FL_CHEAT, "*Cheat* PreyRun cmd: Set your X, Y and Z coordinate");
	cmdSystem->AddCommand("PR_CH_SetPos_Offset", Cmd_PR_ch_setpos_offset_f, CMD_FL_GAME | CMD_FL_CHEAT, "*Cheat* PreyRun cmd: Offset your X, Y and Z coordiante by the given values");
	cmdSystem->AddCommand("PR_CH_SetSpiritPower", Cmd_PR_ch_setspiritpower_f, CMD_FL_GAME | CMD_FL_CHEAT, "*Cheat* PreyRun cmd: Set your spiritpower to the specified value");

	// DEBUG COMMANDS
#ifdef PR_DEBUG
	cmdSystem->AddCommand("PR_Test", Cmd_PR_test_f, CMD_FL_GAME, "PreyRun Debug cmd: For testing purpose");
	cmdSystem->AddCommand("Crash", Cmd_PR_crash_f, CMD_FL_GAME, "Crashes the game");

	cmdSystem->AddCommand("PR_dbg_timer", Cmd_PR_dbg_timer_f, CMD_FL_GAME, "PreyRun Debug cmd: Prints timer stats");
	cmdSystem->AddCommand("PR_dbg_timer_set", Cmd_PR_dbg_timer_set_f, CMD_FL_GAME, "PreyRun Debug cmd: Set timer");

	cmdSystem->AddCommand("PR_dbg_reload", Cmd_PR_dbg_reload_f, CMD_FL_GAME, "PreyRun Debug cmd: Prints info about the pr_reload command");
	cmdSystem->AddCommand("PR_dbg_backup", Cmd_PR_dbg_backup_f, CMD_FL_GAME, "PreyRun Debug cmd: Prints info about the backup timer");

	// Randomness
	cmdSystem->AddCommand("PR_dbg_rng", Cmd_PR_dbg_rng, CMD_FL_GAME, "PreyRun Debug cmd: Prints infos about the rng device.");
	cmdSystem->AddCommand("PR_dbg_randseed", Cmd_PR_dbg_randseed_f, CMD_FL_GAME, "PreyRun Debug cmd: Prints infos about the random seed");
	cmdSystem->AddCommand("PR_dbg_rngint", Cmd_PR_dbg_rngint_f, CMD_FL_GAME, "PreyRun Debug cmd: Prints an rng interger");
	cmdSystem->AddCommand("PR_dbg_rngfloat", Cmd_PR_dbg_rngfloat_f, CMD_FL_GAME, "PreyRun Debug cmd: Prints an rng float");

	// Autocmdzones
	cmdSystem->AddCommand("PR_dbg_autocmd", Cmd_PR_dbg_autocmd_f, CMD_FL_GAME, "PreyRun Debug cmd: Prints debug info about an indexed autocmdzone");
	cmdSystem->AddCommand("PR_dbg_autocmd_trigger", Cmd_PR_dbg_autocmd_trigger_f, CMD_FL_GAME, "PreyRun Debug cmd: Triggers an autocmdzone");
#endif // PR_DEBUG

	// PreyRun END

	cmdSystem->AddCommand("listTypeInfo", ListTypeInfo_f, CMD_FL_GAME, "list type info");
	cmdSystem->AddCommand("writeGameState", WriteGameState_f, CMD_FL_GAME, "write game state");
	cmdSystem->AddCommand("testSaveGame", TestSaveGame_f, CMD_FL_GAME | CMD_FL_CHEAT, "test a save game for a level");
	cmdSystem->AddCommand("game_memory", idClass::DisplayInfo_f, CMD_FL_GAME, "displays game class info");
	cmdSystem->AddCommand("listClasses", idClass::ListClasses_f, CMD_FL_GAME, "lists game classes");
	cmdSystem->AddCommand("listThreads", idThread::ListThreads_f, CMD_FL_GAME | CMD_FL_CHEAT, "lists script threads");
	cmdSystem->AddCommand("listEntities", Cmd_EntityList_f, CMD_FL_GAME | CMD_FL_CHEAT, "lists game entities");
	cmdSystem->AddCommand("listActiveEntities", Cmd_ActiveEntityList_f, CMD_FL_GAME | CMD_FL_CHEAT, "lists active game entities");
	cmdSystem->AddCommand("listMonsters", idAI::List_f, CMD_FL_GAME | CMD_FL_CHEAT, "lists monsters");
	cmdSystem->AddCommand("listSpawnArgs", Cmd_ListSpawnArgs_f, CMD_FL_GAME | CMD_FL_CHEAT, "list the spawn args of an entity", idGameLocal::ArgCompletion_EntityName);
	cmdSystem->AddCommand("say", Cmd_Say_f, CMD_FL_GAME, "text chat");
	cmdSystem->AddCommand("sayTeam", Cmd_SayTeam_f, CMD_FL_GAME, "team text chat");
	cmdSystem->AddCommand("addChatLine", Cmd_AddChatLine_f, CMD_FL_GAME, "internal use - core to game chat lines");
	cmdSystem->AddCommand("gameKick", Cmd_Kick_f, CMD_FL_GAME, "same as kick, but recognizes player names");
	cmdSystem->AddCommand("give", Cmd_Give_f, CMD_FL_GAME | CMD_FL_CHEAT, "gives one or more items");
	cmdSystem->AddCommand("centerview", Cmd_CenterView_f, CMD_FL_GAME, "centers the view");
	cmdSystem->AddCommand("god", Cmd_God_f, CMD_FL_GAME | CMD_FL_CHEAT, "enables god mode");
	cmdSystem->AddCommand("notarget", Cmd_Notarget_f, CMD_FL_GAME | CMD_FL_CHEAT, "disables the player as a target");
	cmdSystem->AddCommand("noclip", Cmd_Noclip_f, CMD_FL_GAME | CMD_FL_CHEAT, "disables collision detection for the player");
	cmdSystem->AddCommand("kill", Cmd_Kill_f, CMD_FL_GAME, "kills the player");
	cmdSystem->AddCommand("where", Cmd_GetViewpos_f, CMD_FL_GAME | CMD_FL_CHEAT, "prints the current view position");
	cmdSystem->AddCommand("getviewpos", Cmd_GetViewpos_f, CMD_FL_GAME | CMD_FL_CHEAT, "prints the current view position");
	cmdSystem->AddCommand("setviewpos", Cmd_SetViewpos_f, CMD_FL_GAME | CMD_FL_CHEAT, "sets the current view position");
	cmdSystem->AddCommand("teleport", Cmd_Teleport_f, CMD_FL_GAME | CMD_FL_CHEAT, "teleports the player to an entity location", idGameLocal::ArgCompletion_EntityName);
	//	cmdSystem->AddCommand( "trigger",				Cmd_Trigger_f,				CMD_FL_GAME|CMD_FL_CHEAT,	"triggers an entity", idGameLocal::ArgCompletion_EntityName );// HUMANHEAD CJRPERSISTENTMERGE:  our version of this is better
	cmdSystem->AddCommand("spawn", Cmd_Spawn_f, CMD_FL_GAME | CMD_FL_CHEAT, "spawns a game entity", idCmdSystem::ArgCompletion_Decl<DECL_ENTITYDEF>);
	//HUMANHEAD rww
	cmdSystem->AddCommand("spawnArtificialPlayer", Cmd_SpawnArtificialPlayer_f, CMD_FL_GAME | CMD_FL_CHEAT, "spawns an artificial player entity");
#if !GOLD
	cmdSystem->AddCommand("playerShadowToggle", Cmd_PlayerShadowToggle_f, CMD_FL_GAME | CMD_FL_CHEAT, "debugging - toggles shadows on current spawned players");
#endif
	//HUMANHEAD END
	cmdSystem->AddCommand("damage", Cmd_Damage_f, CMD_FL_GAME | CMD_FL_CHEAT, "apply damage to an entity", idGameLocal::ArgCompletion_EntityName);
	//	cmdSystem->AddCommand( "remove",				Cmd_Remove_f,				CMD_FL_GAME|CMD_FL_CHEAT,	"removes an entity", idGameLocal::ArgCompletion_EntityName );// HUMANHEAD CJRPERSISTENTMERGE:  our version of this is better
	cmdSystem->AddCommand("killMonsters", Cmd_KillMonsters_f, CMD_FL_GAME | CMD_FL_CHEAT, "removes all monsters");
	cmdSystem->AddCommand("killMoveables", Cmd_KillMovables_f, CMD_FL_GAME | CMD_FL_CHEAT, "removes all moveables");
	cmdSystem->AddCommand("killRagdolls", Cmd_KillRagdolls_f, CMD_FL_GAME | CMD_FL_CHEAT, "removes all ragdolls");
	cmdSystem->AddCommand("addline", Cmd_AddDebugLine_f, CMD_FL_GAME | CMD_FL_CHEAT, "adds a debug line");
	cmdSystem->AddCommand("addarrow", Cmd_AddDebugLine_f, CMD_FL_GAME | CMD_FL_CHEAT, "adds a debug arrow");
	cmdSystem->AddCommand("removeline", Cmd_RemoveDebugLine_f, CMD_FL_GAME | CMD_FL_CHEAT, "removes a debug line");
	cmdSystem->AddCommand("blinkline", Cmd_BlinkDebugLine_f, CMD_FL_GAME | CMD_FL_CHEAT, "blinks a debug line");
	cmdSystem->AddCommand("listLines", Cmd_ListDebugLines_f, CMD_FL_GAME | CMD_FL_CHEAT, "lists all debug lines");
	cmdSystem->AddCommand("playerModel", Cmd_PlayerModel_f, CMD_FL_GAME | CMD_FL_CHEAT, "sets the given model on the player", idCmdSystem::ArgCompletion_Decl<DECL_MODELDEF>);
	cmdSystem->AddCommand("testFx", Cmd_TestFx_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests an FX system", idCmdSystem::ArgCompletion_Decl<DECL_FX>);
	cmdSystem->AddCommand("testBoneFx", Cmd_TestBoneFx_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests an FX system bound to a joint", idCmdSystem::ArgCompletion_Decl<DECL_FX>);
	cmdSystem->AddCommand("testLight", Cmd_TestLight_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests a light");
	cmdSystem->AddCommand("testPointLight", Cmd_TestPointLight_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests a point light");
	cmdSystem->AddCommand("popLight", Cmd_PopLight_f, CMD_FL_GAME | CMD_FL_CHEAT, "removes the last created light");
	cmdSystem->AddCommand("testDeath", Cmd_TestDeath_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests death");
	cmdSystem->AddCommand("testSave", Cmd_TestSave_f, CMD_FL_GAME | CMD_FL_CHEAT, "writes out a test savegame");
	cmdSystem->AddCommand("testModel", idTestModel::TestModel_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests a model", idTestModel::ArgCompletion_TestModel);
	cmdSystem->AddCommand("testSkin", idTestModel::TestSkin_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests a skin on an existing testModel", idCmdSystem::ArgCompletion_Decl<DECL_SKIN>);
	cmdSystem->AddCommand("testShaderParm", idTestModel::TestShaderParm_f, CMD_FL_GAME | CMD_FL_CHEAT, "sets a shaderParm on an existing testModel");
	cmdSystem->AddCommand("keepTestModel", idTestModel::KeepTestModel_f, CMD_FL_GAME | CMD_FL_CHEAT, "keeps the last test model in the game");
	cmdSystem->AddCommand("testAnim", idTestModel::TestAnim_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests an animation", idTestModel::ArgCompletion_TestAnim);
	cmdSystem->AddCommand("testParticleStopTime", idTestModel::TestParticleStopTime_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests particle stop time on a test model");
	cmdSystem->AddCommand("nextAnim", idTestModel::TestModelNextAnim_f, CMD_FL_GAME | CMD_FL_CHEAT, "shows next animation on test model");
	cmdSystem->AddCommand("prevAnim", idTestModel::TestModelPrevAnim_f, CMD_FL_GAME | CMD_FL_CHEAT, "shows previous animation on test model");
	cmdSystem->AddCommand("nextFrame", idTestModel::TestModelNextFrame_f, CMD_FL_GAME | CMD_FL_CHEAT, "shows next animation frame on test model");
	cmdSystem->AddCommand("prevFrame", idTestModel::TestModelPrevFrame_f, CMD_FL_GAME | CMD_FL_CHEAT, "shows previous animation frame on test model");
	cmdSystem->AddCommand("testBlend", idTestModel::TestBlend_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests animation blending");
	cmdSystem->AddCommand("reloadScript", Cmd_ReloadScript_f, CMD_FL_GAME | CMD_FL_CHEAT, "reloads scripts");
	cmdSystem->AddCommand("script", Cmd_Script_f, CMD_FL_GAME | CMD_FL_CHEAT, "executes a line of script");
	cmdSystem->AddCommand("listCollisionModels", Cmd_ListCollisionModels_f, CMD_FL_GAME, "lists collision models");
	cmdSystem->AddCommand("collisionModelInfo", Cmd_CollisionModelInfo_f, CMD_FL_GAME, "shows collision model info");
	cmdSystem->AddCommand("reexportmodels", Cmd_ReexportModels_f, CMD_FL_GAME | CMD_FL_CHEAT, "reexports models", ArgCompletion_DefFile);
	cmdSystem->AddCommand("reloadanims", Cmd_ReloadAnims_f, CMD_FL_GAME | CMD_FL_CHEAT, "reloads animations");
	cmdSystem->AddCommand("listAnims", Cmd_ListAnims_f, CMD_FL_GAME, "lists all animations");
	cmdSystem->AddCommand("aasStats", Cmd_AASStats_f, CMD_FL_GAME, "shows AAS stats");
	cmdSystem->AddCommand("testDamage", Cmd_TestDamage_f, CMD_FL_GAME | CMD_FL_CHEAT, "tests a damage def", idCmdSystem::ArgCompletion_Decl<DECL_ENTITYDEF>);
	cmdSystem->AddCommand("weaponSplat", Cmd_WeaponSplat_f, CMD_FL_GAME | CMD_FL_CHEAT, "projects a blood splat on the player weapon");
	cmdSystem->AddCommand("saveSelected", Cmd_SaveSelected_f, CMD_FL_GAME | CMD_FL_CHEAT, "saves the selected entity to the .map file");
	cmdSystem->AddCommand("deleteSelected", Cmd_DeleteSelected_f, CMD_FL_GAME | CMD_FL_CHEAT, "deletes selected entity");
	cmdSystem->AddCommand("saveMoveables", Cmd_SaveMoveables_f, CMD_FL_GAME | CMD_FL_CHEAT, "save all moveables to the .map file");
	cmdSystem->AddCommand("saveRagdolls", Cmd_SaveRagdolls_f, CMD_FL_GAME | CMD_FL_CHEAT, "save all ragdoll poses to the .map file");
	cmdSystem->AddCommand("bindRagdoll", Cmd_BindRagdoll_f, CMD_FL_GAME | CMD_FL_CHEAT, "binds ragdoll at the current drag position");
	cmdSystem->AddCommand("unbindRagdoll", Cmd_UnbindRagdoll_f, CMD_FL_GAME | CMD_FL_CHEAT, "unbinds the selected ragdoll");
	cmdSystem->AddCommand("saveLights", Cmd_SaveLights_f, CMD_FL_GAME | CMD_FL_CHEAT, "saves all lights to the .map file");
	cmdSystem->AddCommand("saveParticles", Cmd_SaveParticles_f, CMD_FL_GAME | CMD_FL_CHEAT, "saves all lights to the .map file");
	cmdSystem->AddCommand("clearLights", Cmd_ClearLights_f, CMD_FL_GAME | CMD_FL_CHEAT, "clears all lights");
	cmdSystem->AddCommand("gameError", Cmd_GameError_f, CMD_FL_GAME | CMD_FL_CHEAT, "causes a game error");

#ifndef	ID_DEMO_BUILD
	cmdSystem->AddCommand("disasmScript", Cmd_DisasmScript_f, CMD_FL_GAME | CMD_FL_CHEAT, "disassembles script");
	cmdSystem->AddCommand("recordViewNotes", Cmd_RecordViewNotes_f, CMD_FL_GAME | CMD_FL_CHEAT, "record the current view position with notes");
	cmdSystem->AddCommand("showViewNotes", Cmd_ShowViewNotes_f, CMD_FL_GAME | CMD_FL_CHEAT, "show any view notes for the current map, successive calls will cycle to the next note");
	cmdSystem->AddCommand("closeViewNotes", Cmd_CloseViewNotes_f, CMD_FL_GAME | CMD_FL_CHEAT, "close the view showing any notes for this map");
	cmdSystem->AddCommand("exportmodels", Cmd_ExportModels_f, CMD_FL_GAME | CMD_FL_CHEAT, "exports models", ArgCompletion_DefFile);
	cmdSystem->AddCommand("eraseViewNote", Cmd_EraseViewNote_f, CMD_FL_GAME | CMD_FL_CHEAT, "erase the currently displayed note"); // HUMANHEAD mdl
#endif

	//HUMANHEAD jsh PCF 5/1/06 allow multiplayer stuff for demo
	// multiplayer client commands ( replaces old impulses stuff )
	cmdSystem->AddCommand("clientDropWeapon", idMultiplayerGame::DropWeapon_f, CMD_FL_GAME, "drop current weapon");
	cmdSystem->AddCommand("clientMessageMode", idMultiplayerGame::MessageMode_f, CMD_FL_GAME, "ingame gui message mode");
	// FIXME: implement
//	cmdSystem->AddCommand( "clientVote",			idMultiplayerGame::Vote_f,	CMD_FL_GAME,				"cast your vote: clientVote yes | no" );
//	cmdSystem->AddCommand( "clientCallVote",		idMultiplayerGame::CallVote_f,	CMD_FL_GAME,			"call a vote: clientCallVote si_.. proposed_value" );
	cmdSystem->AddCommand("clientVoiceChat", idMultiplayerGame::VoiceChat_f, CMD_FL_GAME, "voice chats: clientVoiceChat <sound shader>");
	cmdSystem->AddCommand("clientVoiceChatTeam", idMultiplayerGame::VoiceChatTeam_f, CMD_FL_GAME, "team voice chats: clientVoiceChat <sound shader>");

	// multiplayer server commands
	cmdSystem->AddCommand("serverMapRestart", idGameLocal::MapRestart_f, CMD_FL_GAME, "restart the current game");
	cmdSystem->AddCommand("serverForceReady", idMultiplayerGame::ForceReady_f, CMD_FL_GAME, "force all players ready");
	cmdSystem->AddCommand("serverNextMap", idGameLocal::NextMap_f, CMD_FL_GAME, "change to the next map");

	// localization help commands
	cmdSystem->AddCommand("nextGUI", Cmd_NextGUI_f, CMD_FL_GAME | CMD_FL_CHEAT, "teleport the player to the next func_static with a gui");
	cmdSystem->AddCommand("testid", Cmd_TestId_f, CMD_FL_GAME | CMD_FL_CHEAT, "output the string for the specified id.");

#if !GOLD //HUMANHEAD rww
	cmdSystem->AddCommand("charset", Cmd_CharSet_f, CMD_FL_SYSTEM, "print console character set");
	cmdSystem->AddCommand("testsnap", idClass::TestSnap_f, CMD_FL_GAME | CMD_FL_CHEAT, "quick check to make sure entity snapshots line up");
	cmdSystem->AddCommand("printTypeName", Cmd_PrintTypeName_f, CMD_FL_SYSTEM | CMD_FL_GAME, "prints type name for type num");
#endif //HUMANHEAD END

	cmdSystem->AddCommand("playTime", Cmd_PlayTime_f, CMD_FL_GAME, "prints current playtime"); //HUMANHEAD mdl

}

/*
=================
idGameLocal::ShutdownConsoleCommands
=================
*/
void idGameLocal::ShutdownConsoleCommands(void)
{
	cmdSystem->RemoveFlaggedCommands(CMD_FL_GAME);
}

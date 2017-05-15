#pragma once
//#include "../game/game_local.h"

// Enums
enum class PR_timer_methode : int
{
	RealTimeAttack,
	IndividualLevel
};

enum class PR_weapons : int
{
	None,
	Wrench,
	Rifle,
	Crawler,
	Leecher,
	Minigun,
	Shotgun,
	RocketLauncher
};

enum class PR_location_methode : int
{
	EyePos,
	FeetPos
};

enum class PR_keys_style : int
{
	Normal,
	NormalGrey,
	Reflex,
	ReflexGrey,
	Momentum,
	MomentumGrey
};

enum class PR_jumpspeed_style : int
{
	Normal,
	Fading,
	OnlyInAir
};

enum class PR_damage_style : int
{
	Last,
	Adding
};

// From Doom 3 SDK - UsecmdcmdGen.h 132 - 186
enum PR_usercmdButton_t : int
{
	UB_NONE,

	UB_MOVEUP,
	UB_MOVEDOWN,
	UB_LOOKLEFT,
	UB_LOOKRIGHT,
	UB_MOVEFORWARD,
	UB_MOVEBACK,
	UB_LOOKUP,
	UB_LOOKDOWN,
	UB_MOVELEFT,
	UB_MOVERIGHT,

	UB_ATTACK,
	UB_SPEED,
	UB_ZOOM,
	UB_SHOWSCORES,
	UB_USE,

	UB_IMPULSE0,
	UB_IMPULSE1,
	UB_IMPULSE2,
	UB_IMPULSE3,
	UB_IMPULSE4,
	UB_IMPULSE5,
	UB_IMPULSE6,
	UB_IMPULSE7,
	UB_IMPULSE8,
	UB_IMPULSE9,
	UB_IMPULSE10,
	UB_IMPULSE11,
	UB_IMPULSE12,
	UB_IMPULSE13,
	UB_IMPULSE14,
	UB_IMPULSE15,
	UB_IMPULSE16,
	UB_IMPULSE17,
	UB_IMPULSE18,
	UB_IMPULSE19,
	UB_IMPULSE20,
	UB_IMPULSE21,
	UB_IMPULSE22,
	UB_IMPULSE23,
	UB_IMPULSE24,
	UB_IMPULSE25,
	UB_IMPULSE26,
	UB_IMPULSE27,
	UB_IMPULSE28,
	UB_IMPULSE29,
	UB_IMPULSE30,
	UB_IMPULSE31,

	UB_MAX_BUTTONS
};

#pragma once

namespace pr
{
	// Enums
	enum class TimerMethode : int
	{
		RealTimeAttack,
		IndividualLevel
	};

	enum class Weapons : int
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

	enum class LocationMethode : int
	{
		EyePos,
		FeetPos
	};

	enum class KeysStyle : int
	{
		Normal,
		NormalGrey,
		Reflex,
		ReflexGrey,
		Momentum,
		MomentumGrey
	};

	enum class JumpspeedStyle : int
	{
		Normal,
		Fading,
		OnlyInAir
	};

	enum class DamageStyle : int
	{
		Last,
		Adding
	};

	enum class SpeedometerStyle : int
	{
		Normal,
		Colored
	};

	enum GhostDemoInfo : unsigned char
	{
		Position = 0x00,
		MapChange = 0x01,
		DemoEnd = 0x02
	};
} // End of namespace: pr

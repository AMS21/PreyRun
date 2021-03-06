﻿// Copyright (C) 2004 Id Software, Inc.
//
#pragma once

#include "../game/gamesys/SysCvar.h" // pr_fixedseed,pr_fixedseed_value
#include <new>
#include "../sys/sys_public.h"

#include "PreyRun.hpp"

// My own Version of the Random Number Generator from Random.h

/*
===============================================================================

Random number generator

===============================================================================
*/

class prRandom
{
public:
	prRandom(int seed = 0);

	operator idRandom() const;
	operator idRandom&();

	void				SetSeed(int seed);
	int					GetSeed() const;

	int					RandomInt();			// random integer in the range [0, MAX_RAND]
	int					RandomInt(int max);		// random integer in the range [0, max]
	float				RandomFloat();		// random number in the range [0.0f, 1.0f]
	float				CRandomFloat();		// random number in the range [-1.0f, 1.0f]

	static const int	MAX_RAND = 0x7fff;

private:
	class idRandomHolder
	{
	public:
		enum class idRandomHolderCreationFlag
		{
			dontCreate,
			create
		};

		struct idRandomInfo
		{
			explicit idRandomInfo(int seed_ = 0, bool keepSeed_ = false)
				: seed { seed_ }, keepSeed { keepSeed_ } {}

			int seed;
			bool keepSeed;
		};

		idRandomHolder(idRandomHolderCreationFlag flag, idRandomInfo info = idRandomInfo {})
			: hasObject { flag == idRandomHolderCreationFlag::create }
		{
			if (hasObject)
			{
				new (&idRandom_) idRandom { info.seed, info.keepSeed };
			}
		}

		~idRandomHolder()
		{
			if (hasObject)
			{
				idRandom_.~idRandom();
			}
		}

		idRandomHolder& operator=(idRandomInfo info)
		{
			if (hasObject)
			{
				idRandom_.~idRandom();
			}

			new (&idRandom_) idRandom(info.seed, info.keepSeed);
			hasObject = true;

			return *this;
		}

		idRandom* get()
		{
			if (!hasObject)
			{
				return nullptr;
			}

			return &idRandom_;
		}
	private:
		bool hasObject;

		union
		{
			idRandom idRandom_;
		};
	};

	int	seed;

	idRandomHolder holder;
};

PR_FINLINE prRandom::prRandom(int seed)
	: holder { idRandomHolder::idRandomHolderCreationFlag::dontCreate }
{
	this->seed = seed;
}

PR_FINLINE prRandom::operator idRandom() const
{
	if (pr_fixedseed.GetBool())
	{
		return idRandom(pr_fixedseed_value.GetInteger(), true);
	}
	return idRandom(this->seed);
}

PR_FINLINE prRandom::operator idRandom&()
{
	if (pr_fixedseed.GetBool())
	{
		holder = idRandomHolder::idRandomInfo { pr_fixedseed_value.GetInteger(),true };
		auto p = holder.get();
		return *p;
	}
	holder = idRandomHolder::idRandomInfo { this->seed,false };
	auto p = holder.get();
	return *p;
}

PR_FINLINE void prRandom::SetSeed(int seed)
{
	if (pr_fixedseed.GetBool())
	{
		this->seed = pr_fixedseed_value.GetInteger();
	}
	else
	{
		this->seed = seed;
	}
}

PR_FINLINE int prRandom::GetSeed(void) const
{
	if (pr_fixedseed.GetBool())
	{
		return pr_fixedseed_value.GetInteger();
	}
	return seed;
}

PR_FINLINE int prRandom::RandomInt(void)
{
	if (pr_fixedseed.GetBool())
	{
		return ((69069 * pr_fixedseed_value.GetInteger() + 1) & prRandom::MAX_RAND);
	}

	seed = 69069 * seed + 1;
	return (seed & prRandom::MAX_RAND);
}

PR_FINLINE int prRandom::RandomInt(int max)
{
	if (max == 0)
	{
		return 0;			// avoid divide by zero error
	}
	return RandomInt() % max;
}

PR_FINLINE float prRandom::RandomFloat(void)
{
	return (RandomInt() / (float) (prRandom::MAX_RAND + 1));
}

PR_FINLINE float prRandom::CRandomFloat(void)
{
	return (2.0f * (RandomFloat() - 0.5f));
}

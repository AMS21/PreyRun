#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "GhostManager.hpp"

#include "../Logging.hpp"
#include "../Utility.hpp"

#include <algorithm>

namespace pr
{
	void GhostManager::LoadGhostFile(const idStr& filePath)
	{
		if (mRunning)
		{
			pr::ConsoleWrite("Already displaying ghosts!");
			return;
		}

		for (auto&& x : mGhosts)
		{
			if (x.SameFile(filePath))
			{
				pr::ConsoleWrite("File is already loaded!");
				return;
			}
		}

		mGhosts.emplace_back(filePath.c_str());

		ConsoleWrite("Successfully loaded: %s", filePath.c_str());
	}

	void GhostManager::ClearAllGhosts() noexcept
	{
		mGhosts.clear();

		mFrameTime = 0;
		mRunning = false;
	}

	void GhostManager::Render() const
	{
		for (auto&& x : mGhosts)
		{
			x.Draw();
		}
	}

	void GhostManager::Update(const int FTdelta)
	{
		if (mGhosts.size() > 0)
		{
			mRunning = true;
			mFrameTime += FTdelta;

			// Remove all ghosts which set their finished flag, which happens once the end of the demo is reached
			mGhosts.erase(std::remove_if(std::begin(mGhosts), std::end(mGhosts), []
			(const auto &e)
			{
				return e.IsFinished();
			}),
						  std::end(mGhosts));

			if (mGhosts.size() == 0)
			{
				mFrameTime = 0;
				mRunning = false;

				DebugLog("All ghosts have finished!");
			}
			else
			{
				for (auto&& x : mGhosts)
				{
					x.Update(mFrameTime);

				#ifdef PR_DEBUG
					if (x.FrameTime() != mFrameTime && !x.IsFinished())
					{
						pr::DebugWarning("Ghost desynced! %d != %d", mFrameTime, x.FrameTime());
						pr::DebugLog("String: %s", x.Debug().c_str());
					}
				#endif // PR_DEBUG
				}
			}
		}
	}

	void GhostManager::ClearLines() noexcept
	{
		for (auto&& x : mGhosts)
		{
			x.ClearLine();
		}
	}

	std::vector<idStr> GhostManager::Debug() const
	{
		std::vector<idStr> ret;

		idStr header;
		sprintf(header, "Ghosts:%s, %d, %llu", mRunning ? "True" : "False", mGhosts.size(), mFrameTime);

		ret.push_back(header);

		for (auto&& x : mGhosts)
		{
			ret.emplace_back(x.Debug());
		}

		return ret;
	}

	GhostManager ghostManager {};
} // End of namespace: pr

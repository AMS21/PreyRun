#include "../idLib/precompiled.h"
#pragma hdrstop

#include "AutoCmd.hpp"

#include "../game/Game_local.h"

namespace pr
{
	template <class CharType>
	auto split(std::basic_string<CharType> const &str,
			   std::basic_string<CharType> const &delims)
	{
		using StringType = std::basic_string<CharType>;
		using container_type = std::vector<StringType>;

		container_type result {};
		result.reserve(7U);

		auto const lambda
		{
			[&](auto c)
		{
			return std::any_of(std::begin(delims), std::end(delims), [c](auto ch)
			{
				return ch == c;
			});
		}
		};

		auto const begin = std::begin(str);
		auto const end = std::end(str);
		auto the_next_one = begin;

		for (auto pos = begin; pos != end; pos = the_next_one + 1)
		{
			the_next_one = std::find_if(pos, end, lambda);
			result.emplace_back(pos, the_next_one);
			if (the_next_one == end) break;
		}

		result.erase(std::remove_if(std::begin(result), std::end(result),
									[](auto &&e)
		{
			return e.empty();
		}), std::end(result));

		result.shrink_to_fit();
		return result;
	}

	// Autocmdzone
	void AutocmdzoneHandler::Autocmdzone::Run()
	{
		if (!mActivated)
		{
			static std::string const delims { ";" };

			auto f = [](auto &&e)
			{
				return split(std::forward<decltype(e)>(e), delims);
			};

			std::vector<std::vector<std::string>> v {};
			v.push_back(f(std::string(mCommands.c_str())));

			for (auto const &vec : v)
			{
				for (auto const &str : vec)
				{
				#ifdef PR_DBG_AUTOCMD
					pr::FunctionLog(__FUNCTION__, "Executing '%s'", str.c_str());
				#endif // PR_DBG_AUTOCMD
					cmdSystem->BufferCommandText(CMD_EXEC_NOW, str.c_str());
				}
			}

			mActivated = true;
		}
	}

	void AutocmdzoneHandler::Autocmdzone::Draw() const
	{
		if (gameLocal.GetLocalPlayer())
		{
			const auto axis { gameLocal.GetLocalPlayer()->viewAngles.ToMat3() };

			const idBounds bounds(mBeginPoint, mEndPoint);
			idStr string;

			if (mActivated) { gameRenderWorld->DebugBounds(colorRed, bounds); }
			else { gameRenderWorld->DebugBounds(colorBlue, bounds); }

			sprintf(string, "Autocmdzone:\n%s", mCommands.c_str());

			gameRenderWorld->DrawText(string.c_str(), bounds.GetCenter(), 0.1f, colorWhite, axis);
		}
	}

	//AutocmdzoneHandler
	AutocmdzoneHandler& AutocmdzoneHandler::getInstance() noexcept
	{
		static AutocmdzoneHandler instance;

		return instance;
	}

	AutocmdzoneHandler::AutocmdzoneHandler() noexcept
	{}

	// Checks if any of the autocommandzones should be triggered and triggers them
	void AutocmdzoneHandler::UpdateZones()
	{
		if (gameLocal.GetLocalPlayer())
		{
			const auto playerbounds = gameLocal.GetLocalPlayer()->GetPhysics()->GetAbsBounds();

			for (auto& e : this->mZones)
			{
				const idBounds bounds { e.GetBeginPoint(),e.GetEndPoint() };

				// Do we intersect with the Player?
				if (bounds.IntersectsBounds(playerbounds)) { e.Run(); }
				else { e.SetActivated(false); }
			}
		}
	}

	void AutocmdzoneHandler::Draw() const
	{
		for (auto const &e : mZones)
		{
			e.Draw();
		}
	}

	void AutocmdzoneHandler::Add(const idVec3 &pos1, const idVec3 &pos2, const idStr &cmds)
	{
	#ifdef PR_DBG_AUTOCMD
		pr::FunctionLog(__FUNCTION__, "Adding autocmdzone: %f %f %f %f %f %f %s", pos1_.x, pos1_.y, pos1_.z, pos2_.x, pos2_.y, pos2_.z, cmds_.c_str());
	#endif // PR_DBG_AUTOCMD
		this->mZones.emplace_back(pos1, pos2, cmds);
	}

	// !Under the premise that const int ´num´ will always be > 0 and < mZones.size()!
	void AutocmdzoneHandler::Edit(const int num, const idVec3 &pos1, const idVec3 &pos2, const idStr &cmds)
	{
	#ifdef PR_DBG_AUTOCMD
		pr::FunctionLog(__FUNCTION__, "Editing number: %d", num);
	#endif // PR_DBG_AUTOCMD

	#ifdef PR_DEBUG
		assert(num >= 0 && "Faulty parameter passed to num");
		assert(num < mZones.size() && "Faulty parameter passed to num");

		mZones.at(num).SetBeginPoint(pos1);
		mZones.at(num).SetEndPoint(pos2);
		mZones.at(num).SetCommands(cmds);
	#else
		// Unchecked access
		mZones[num].SetBeginPoint(pos1);
		mZones[num].SetEndPoint(pos2);
		mZones[num].SetCommands(cmds);
	#endif // PR_DEBUG
	}
} // End of namespace: pr

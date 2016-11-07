#include "../idLib/precompiled.h"
#pragma hdrstop

#include "PreyRun.hpp"
#include "AutoCmd.hpp"

namespace pr
{
	template <class CharType>
	auto split(std::basic_string<CharType> const &str,
		std::basic_string<CharType> const &delims)
		-> std::vector<std::basic_string<CharType>> {
		using StringType = std::basic_string<CharType>;
		using container_type = std::vector<StringType>;
		container_type result{};
		result.reserve(7U);
		auto lambda{
			[&](auto c) {
			return std::any_of(std::begin(delims), std::end(delims), [c](auto ch) {
				return ch == c;
			});
		}
		};

		auto const begin = std::begin(str);
		auto const end = std::end(str);
		auto the_next_one = begin;

		for (auto pos = begin; pos != end; pos = the_next_one + 1) {
			the_next_one = std::find_if(pos, end, lambda);
			result.emplace_back(pos, the_next_one);
			if (the_next_one == end) break;
		}

		result.erase(std::remove_if(std::begin(result), std::end(result),
			[](auto &&e) {
			return e.empty();
		}), std::end(result));

		result.shrink_to_fit();
		return result;
	}

	// Autocmdzone
	AutocmdzoneHandler::Autocmdzone::Autocmdzone()
	{
		this->pos1 = idVec3(0, 0, 0);
		this->pos2 = idVec3(0, 0, 0);

		this->cmds = "";
		this->activated = false;
	}

	void AutocmdzoneHandler::Autocmdzone::Run()
	{
		if (!activated)
		{
			static std::string const delims{ ";" };

			auto f = [](auto &&e)
			{
				return split(std::forward<decltype(e)>(e), delims);
			};

			std::vector<std::vector<std::string>> v{};
			v.push_back(f(std::string(cmds.c_str())));

			for (auto const &vec : v)
			{
				for (auto const &str : vec)
				{
#ifdef PR_DBG_AUTOCMD
					gameLocal.Printf("Autocmdzone::Run(): executing '%s'\n",str.c_str());
#endif // PR_DBG_AUTOCMD
					cmdSystem->BufferCommandText(CMD_EXEC_NOW, str.c_str());
				}
			}

			activated = true;
		}
	}

	void AutocmdzoneHandler::Autocmdzone::Draw() const
	{
		if (gameLocal.GetLocalPlayer())
		{
			auto axis{ gameLocal.GetLocalPlayer()->viewAngles.ToMat3() };

			idBounds bounds(pos1, pos2);
			idStr string;

			if (activated) { gameRenderWorld->DebugBounds(colorRed, bounds); }
			else { gameRenderWorld->DebugBounds(colorBlue, bounds); }

			sprintf(string, "Autocmdzone:\n%s", cmds.c_str());

			gameRenderWorld->DrawText(string.c_str(), bounds.GetCenter(), 0.1f, colorWhite, axis, 1);
		}
	}

	//AutocmdzoneHandler
	AutocmdzoneHandler& AutocmdzoneHandler::getInstance()
	{
		static AutocmdzoneHandler instance;

		return instance;
	}

	AutocmdzoneHandler::AutocmdzoneHandler()
	{
	}

	void AutocmdzoneHandler::Trigger()
	{
		if (gameLocal.GetLocalPlayer())
		{
			auto playerbounds = gameLocal.GetLocalPlayer()->GetPhysics()->GetAbsBounds();

			for (auto it = acz.begin(); it != acz.end(); ++it)
			{
				idBounds bounds{ it->GetPos1(),it->GetPos2() };

				// Do we intersect with the Player?
				if (bounds.IntersectsBounds(playerbounds))
				{
					it->Run();
				}
				else
				{
					it->SetActivated(false);
				}
			}
		}
	}

	void AutocmdzoneHandler::Draw()
	{
		for (auto it = acz.begin(); it != acz.end(); ++it)
		{
			it->Draw();
		}
	}

	void AutocmdzoneHandler::Add(idVec3 pos1_, idVec3 pos2_, cmdType cmds_)
	{
#ifdef PR_DBG_AUTOCMD
		gameLocal.Printf("AutocmdzoneHandler::Add() Adding autocmdzone: %f %f %f %f %f %f %s\n", pos1_.x, pos1_.y, pos1_.z, pos2_.x, pos2_.y, pos2_.z, cmds_.c_str());
#endif // PR_DBG_AUTOCMD
		this->acz.push_back(Autocmdzone(pos1_, pos2_, cmds_));
	}

	void AutocmdzoneHandler::Edit(int num, idVec3 pos1_, idVec3 pos2_, cmdType cmds_)
	{
#ifdef PR_DBG_AUTOCMD
		gameLocal.Printf("AutocmdzoneHandler::Edit() editing number: %d\n",num);
#endif // PR_DBG_AUTOCMD
		acz.at(num).SetPos1(pos1_);
		acz.at(num).SetPos2(pos2_);
		acz.at(num).SetCmds(cmds_);
	}
}

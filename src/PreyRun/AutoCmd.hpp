#pragma once
#include "../game/game_local.h"
#include "StdLib.hpp" // std::vector
#include <regex>

typedef idStr cmdType;

namespace pr
{
	// AutocmdzoneHandler
	class AutocmdzoneHandler
	{
	public:
		AutocmdzoneHandler(AutocmdzoneHandler const&) = delete;
		AutocmdzoneHandler& operator=(AutocmdzoneHandler const&) = delete;

		// Iterator stuff
		auto begin() -> decltype(auto) { return acz.begin(); }
		auto begin() const -> decltype(auto) { return acz.begin(); }
		auto cbegin() const -> decltype(auto) { return acz.cbegin(); }

		auto end() -> decltype(auto) { return acz.end(); }
		auto end() const -> decltype(auto) { return acz.end(); }
		auto cend() const -> decltype(auto) { return acz.cend(); }

		// Singleton
		static AutocmdzoneHandler& getInstance();

		void Draw() const;

		// Function to handle the triggering of the autocmdzones
		void CheckForTriggering();

		void Add(const idVec3 &pos1_, const idVec3 &pos2_, const cmdType &cmds_);
		void Remove(const int &index) { acz.erase(acz.begin() + index); };
		void Clear() { this->acz.clear(); };
		void Edit(const int &num, const idVec3 &pos1_, const idVec3 &pos2_, const cmdType &cmds_);

		int NumOfZones() const { return acz.size(); };
	private:
		AutocmdzoneHandler();

		class Autocmdzone
		{
		public:
			Autocmdzone() : pos1(idVec3(0, 0, 0)), pos2(idVec3(0, 0, 0)), cmds(""), activated(false) {};
			Autocmdzone(idVec3 pos1_, idVec3 pos2_, cmdType cmds_)
				: pos1 { pos1_ }, pos2 { pos2_ }, cmds { cmds_ }, activated(false) {}

			// Actually run the cmd
			void Run();

			// Draw the zone
			void Draw() const;

			idVec3 GetPos1() const { return pos1; };
			idVec3 GetPos2() const { return pos2; };
			cmdType GetCmds() const { return cmds; };
			bool GetActivated() const { return activated; };

			void SetPos1(idVec3 val) { this->pos1 = val; };
			void SetPos2(idVec3 val) { this->pos2 = val; };
			void SetCmds(cmdType val) { this->cmds = val; };
			void SetActivated(bool val = true) { this->activated = val; };

		private:
			idVec3 pos1; // Begin point
			idVec3 pos2; // End point

			cmdType cmds;

			bool activated;
		};

		std::vector<Autocmdzone> acz;
	public:
		using iterator = decltype(acz)::iterator;
		using const_iterator = decltype(acz)::const_iterator;

		Autocmdzone at(int index) { return acz.at(index); }
	};
}

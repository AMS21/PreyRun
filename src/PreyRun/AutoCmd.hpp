#pragma once
#include "../game/game_local.h"
#include "StdLib.hpp" // std::vector

typedef idStr cmdType;

namespace pr
{
	// AutocmdzoneHandler
	class AutocmdzoneHandler
	{
	public:
		AutocmdzoneHandler(AutocmdzoneHandler const &) = delete;
		AutocmdzoneHandler & operator= (AutocmdzoneHandler const &) = delete;

		// Iterator stuff
		auto begin() -> decltype(auto) { return acz.begin(); }
		auto begin() const -> decltype(auto) { return acz.begin(); }
		auto cbegin() const -> decltype(auto) { return acz.cbegin(); }

		auto end() -> decltype(auto) { return acz.end(); }
		auto end() const -> decltype(auto) { return acz.end(); }
		auto cend() const -> decltype(auto) { return acz.cend(); }

		// Sigleton
		static AutocmdzoneHandler& getInstance();

		void Draw();

		// Function to handle the triggering of the autocmdzones
		void Trigger();

		void Add(idVec3 pos1_, idVec3 pos2_, cmdType cmds_);
		inline void Remove(int index) { acz.erase(acz.begin() + index); };
		inline void Clear() { this->acz.clear(); };
		void Edit(int num, idVec3 pos1_, idVec3 pos2_, cmdType cmds_);

		inline int NumOfZones() { return acz.size(); };
	private:
		AutocmdzoneHandler();

		class Autocmdzone
		{
		public:
			Autocmdzone();
			Autocmdzone(idVec3 pos1_, idVec3 pos2_, cmdType cmds_)
				: pos1{ pos1_ }, pos2{ pos2_ }, cmds{ cmds_ } {};

			// Actually run the cmd
			void Run();

			// Draw the zone
			void Draw();

			inline idVec3 GetPos1() { return pos1; };
			inline idVec3 GetPos2() { return pos2; };
			inline cmdType GetCmds() { return cmds; };
			inline bool GetActivated() { return activated; };

			inline void SetPos1(idVec3 val) { this->pos1 = val; };
			inline void SetPos2(idVec3 val) { this->pos2 = val; };
			inline void SetCmds(cmdType val) { this->cmds = val; };
			inline void SetActivated(bool val) { this->activated = val; };

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
	};
}
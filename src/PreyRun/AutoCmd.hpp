#pragma once

#include <regex>

class idStr;

namespace pr
{
	// AutocmdzoneHandler
	class AutocmdzoneHandler
	{
	public:
		AutocmdzoneHandler(AutocmdzoneHandler const&) = delete;
		AutocmdzoneHandler& operator=(AutocmdzoneHandler const&) = delete;

		// Iterator stuff
		auto begin() noexcept -> decltype(auto) { return mZones.begin(); }
		auto begin() const noexcept -> decltype(auto) { return mZones.begin(); }
		auto cbegin() const noexcept -> decltype(auto) { return mZones.cbegin(); }

		auto end() noexcept -> decltype(auto) { return mZones.end(); }
		auto end() const noexcept -> decltype(auto) { return mZones.end(); }
		auto cend() const noexcept -> decltype(auto) { return mZones.cend(); }

		// Singleton
		static AutocmdzoneHandler& getInstance() noexcept;

		void Draw() const;

		// Function to handle the triggering of the autocmdzones
		void UpdateZones();

		void Add(const idVec3 &pos1, const idVec3 &pos2, const idStr &cmds);
		void Remove(const int index) { mZones.erase(mZones.begin() + index); };
		void Clear() noexcept { this->mZones.clear(); };
		void Edit(const int num, const idVec3 &pos1, const idVec3 &pos2, const idStr &cmds);

		int NumOfZones() const noexcept { return static_cast<int>(mZones.size()); };
	private:
		AutocmdzoneHandler() noexcept;

		class Autocmdzone
		{
		public:
			Autocmdzone() noexcept
				: mBeginPoint(idVec3(0, 0, 0)), mEndPoint(idVec3(0, 0, 0)), mCommands(""), mActivated(false)
			{}
			Autocmdzone(const idVec3& pos1_, const idVec3& pos2_, const idStr& cmds_) noexcept
				: mBeginPoint { pos1_ }, mEndPoint { pos2_ }, mCommands { cmds_ }, mActivated(false)
			{}

			// Actually run the command
			void Run();

			// Draw the zone
			void Draw() const;

			idVec3 GetBeginPoint() const noexcept { return mBeginPoint; };
			idVec3 GetEndPoint() const noexcept { return mEndPoint; };
			idStr GetCommands() const noexcept { return mCommands; };
			bool GetActivated() const noexcept { return mActivated; };

			void SetBeginPoint(const idVec3& val) { this->mBeginPoint = val; };
			void SetEndPoint(const idVec3& val) { this->mEndPoint = val; };
			void SetCommands(const idStr& val) { this->mCommands = val; };
			void SetActivated(const bool val = true) noexcept { this->mActivated = val; };

		private:
			idVec3 mBeginPoint;
			idVec3 mEndPoint;

			idStr mCommands;

			bool mActivated;
		};

		std::vector<Autocmdzone> mZones;
	public:
		using iterator = decltype(mZones)::iterator;
		using const_iterator = decltype(mZones)::const_iterator;

		Autocmdzone at(const int index) const
		{
		#ifdef PR_DEBUG
			return mZones.at(index);
		#else
			return mZones[index];
		#endif // PR_DEBUG
		}
	};
} // End of namespace: pr

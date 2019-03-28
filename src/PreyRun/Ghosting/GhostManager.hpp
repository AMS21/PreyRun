#pragma once

#include "GhostEntity.hpp"

#include <vector>

namespace pr
{
	class GhostManager
	{
	public:
		GhostManager() noexcept;

		void LoadGhostFile(const idStr& filePath);

		void ClearAllGhosts() noexcept;

		void Render() const;

		void Update(const int ftime);

		void ClearLines() noexcept;

		std::vector<idStr> Debug() const;

	private:
		std::vector<GhostEntity> mGhosts;
		bool mRunning;
		uint64_t mFrameTime;
	};

	extern GhostManager ghostManager;
} // End of Namespace: pr

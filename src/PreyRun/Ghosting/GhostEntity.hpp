#pragma once

#include <deque>

class idStr;

namespace pr
{
	class GhostEntity
	{
	public:
		explicit GhostEntity(const char* filePath);

		void Update(const int cframetime);

		void Draw() const;

		idStr Debug() const;

		void ClearLine() noexcept;

		bool SameFile(const idStr& otherFile) const;

		bool IsFinished() const noexcept
		{ return mFinished; }

		uint64_t FrameTime() const noexcept
		{ return mFrameTime; }

	private:
		bool mFinished;
		uint64_t mFrameTime = 0;
		idFile* mFile;
		idVec3 mColor; // Only RGB values
		idVec3 mPosition;
		idStr mName;
		idStr mMapName;
		std::deque<idVec3> mLinePoints;
	};
} // End of namespace: pr

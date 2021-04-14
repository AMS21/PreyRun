#include "../../idLib/precompiled.h"
#pragma hdrstop

#include "GhostEntity.hpp"

#include "../Constants.hpp"
#include "../Logging.hpp"
#include "../Utility.hpp"

#include "../../game/Game_local.h"
#include <stdexcept>

namespace pr
{
	constexpr unsigned MaxLineLenght = 20;

	GhostEntity::GhostEntity(const char* filePath)
		: mFinished(false), mPosition(vec3_origin)
	{
		mFile = fileSystem->OpenFileRead(filePath);

		if (mFile == nullptr)
		{
			pr::Warning("Failed to open: %s", filePath);

			throw std::runtime_error("Failed to open file.");
		}

		// Read header
		unsigned demoVersion;
		mFile->ReadUnsignedInt(demoVersion);

		if (demoVersion != GhostingDemoVersion)
		{
			pr::Warning("Demo versions don't match! Client: %d, Demo: %d", GhostingDemoVersion, demoVersion);

			throw std::runtime_error("File version doesn't match client version.");
		}

		mFile->ReadString(mName);
		mFile->ReadString(mMapName);
		mFile->ReadVec3(mColor);
	}

	void GhostEntity::Update(const int cframetime)
	{
		// Only update if frame time is newer
		if (cframetime > mFrameTime)
		{
			unsigned char eventType;
			mFile->ReadUnsignedChar(eventType);

			switch (eventType)
			{
				case GhostDemoInfo::MapChange:
				{
					// Read new map name
					mFile->ReadString(mMapName);

					// Read the next char which will always be the position, so don't bother checking
					mFile->ReadUnsignedChar(eventType);

					// Clear line points so we don't get some weird artifacts
					mLinePoints.clear();

					pr::DebugLog("Ghosting: Updated map to: %s", mMapName.c_str());
				}

				[[fallthrough]];
				case GhostDemoInfo::Position:
				{
					// Read frame delta
					unsigned frameDelta;
					mFile->ReadUnsignedInt(frameDelta);

					mFrameTime += frameDelta;

					// Read position delta
					idVec3 posDelta;
					mFile->ReadVec3(posDelta);
					mPosition += posDelta;

					// Delete oldest point if at max size
					if (mLinePoints.size() >= MaxLineLenght)
					{
						mLinePoints.pop_back();
					}

					// Add latest point in the front
					mLinePoints.push_front(mPosition);

					break;
				}

				case GhostDemoInfo::DemoEnd:
				{
					pr::DebugLog("Ghosting: Demo end reached!");

					mFinished = true;

					break;
				}
			}
		}
	}

	void GhostEntity::Draw() const
	{
		if (gameLocal.GetMapName() == mMapName && !mFinished)
		{
			for (auto i { 1u }; i < mLinePoints.size(); ++i)
			{
			#ifdef PR_DEBUG
				gameRenderWorld->DebugLine(idVec4(pr::hexToFloat(mColor.x), pr::hexToFloat(mColor.y), pr::hexToFloat(mColor.z), 1.0f), mLinePoints.at(i - 1), mLinePoints.at(i));
			#else
				gameRenderWorld->DebugLine(idVec4(pr::hexToFloat(mColor.x), pr::hexToFloat(mColor.y), pr::hexToFloat(mColor.z), 1.0f), mLinePoints[i - 1], mLinePoints[i]);
			#endif // PR_DEBUG
			}

			// Name position
			auto namePos = mPosition;
			namePos.z += 16.f;

			gameRenderWorld->DrawText(mName.c_str(), namePos, 0.2f, idVec4(1.0f, 1.0f, 1.0f, 1.0f), gameLocal.GetLocalPlayer()->viewAngles.ToMat3());
		}
	}

	idStr GhostEntity::Debug() const
	{
		idStr ret;
		idStr map;

		mMapName.ExtractFileName(map);

		sprintf(ret, "%llu:%s %s %.0f %.0f %.0f", mFrameTime, mName.c_str(), map.c_str(), mPosition.x, mPosition.y, mPosition.z);

		return ret;
	}

	void GhostEntity::ClearLine() noexcept
	{
		mLinePoints.clear();
	}

	bool GhostEntity::SameFile(const idStr& otherFile) const
	{
		return otherFile.Cmp(mFile->GetName()) == 0;
	}
} // End of namespace: pr

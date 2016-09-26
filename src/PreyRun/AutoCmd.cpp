#include "../idLib/precompiled.h"
#pragma hdrstop

#include "AutoCmd.hpp"

namespace pr
{
	// Autocmdzone
	AutocmdzoneHandler::Autocmdzone::Autocmdzone()
	{
#ifdef PR_DEBUG
		gameLocal.Printf("constructed Autocmdzone with default constructor.\n");
#endif // PR_DEBUG
		this->pos1 = idVec3(0, 0, 0);
		this->pos2 = idVec3(0, 0, 0);

		this->cmds = "";
		this->activated = false;
	}

	void AutocmdzoneHandler::Autocmdzone::Run()
	{
		if (!activated)
		{
			cmdSystem->BufferCommandText(CMD_EXEC_NOW, cmds.c_str());
			activated = true;
		}
	}

	void AutocmdzoneHandler::Autocmdzone::Draw()
	{
		if (gameLocal.GetLocalPlayer())
		{
			idMat3 axis{ gameLocal.GetLocalPlayer()->viewAngles.ToMat3() };

			idBounds bounds(pos1, pos2);

			if (activated)
			{
				gameRenderWorld->DebugBounds(colorRed, bounds);
			}
			else
			{
				gameRenderWorld->DebugBounds(colorBlue, bounds);
			}

			idStr string;

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
		//this->acz.resize(5); // I gues u usualy wont have more than 5 zones
	}

	void AutocmdzoneHandler::Trigger()
	{
		if (gameLocal.GetLocalPlayer())
		{
			for (std::vector<Autocmdzone>::iterator it = acz.begin(); it != acz.end(); ++it)
			{
				idBounds bounds{ it->GetPos1(),it->GetPos2() };

				// Do we intersect with the Player?
				if (bounds.IntersectsBounds(gameLocal.GetLocalPlayer()->GetPhysics()->GetAbsBounds()))
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
		for (std::vector<Autocmdzone>::iterator it = acz.begin(); it != acz.end(); ++it)
		{
			it->Draw();
		}
	}

	void AutocmdzoneHandler::Add(idVec3 pos1_, idVec3 pos2_, cmdType cmds_)
	{
#ifdef PR_DEBUG
		gameLocal.Printf("AutocmdzoneHandler.Add() Adding autocmdzone: %f %f %f %f %f %f %s\n", pos1_.x, pos1_.y, pos1_.z, pos2_.x, pos2_.y, pos2_.z, cmds_.c_str());
#endif // PR_DEBUG
		this->acz.push_back(AutocmdzoneHandler::Autocmdzone(pos1_, pos2_, cmds_));
	}

	void AutocmdzoneHandler::Edit(int num, idVec3 pos1_, idVec3 pos2_, cmdType cmds_)
	{
		acz.at(num).SetPos1(pos1_);
		acz.at(num).SetPos2(pos2_);
		acz.at(num).SetCmds(cmds_);
	}
}
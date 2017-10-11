/*
* A Command & Conquer: Renegade SSGM Plugin, containing logic for evacuating n amount of soldiers
* Copyright(C) 2017  Neijwiert
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "GK_Evac_Base.h"

class GK_Evac_Person : public GK_Evac_Base
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Destroyed(GameObject *obj);
		virtual void Killed(GameObject *obj, GameObject *killer);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
		virtual void Animation_Complete(GameObject *obj, const char *animation_name);

		void GotoEvacLocation();
		void CancelEvac();
		void RestoreState();
		void FinalizeEvac();

		bool shouldUnregister;
		bool movingToEvacLocation;
		int troopBoneObjId;
		bool previousHibernatingState;
		bool previousInnateState;
};
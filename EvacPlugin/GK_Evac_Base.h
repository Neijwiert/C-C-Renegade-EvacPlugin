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

#include <scripts.h>

#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)
#define MONITOR_OBJ_ID_PARAMETER_NAME "MonitorObjId"
#define MONITOR_OBJ_ID_PARAMETER PPCAT(MONITOR_OBJ_ID_PARAMETER_NAME, ":int")

class GK_Evac_Base : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	protected:
		virtual void Created(GameObject *obj);

		inline int GetMonitorObjId()
		{
			return this->monitorObjId;
		}

		inline GameObject *GetMonitorObj()
		{
			return Commands->Find_Object(this->monitorObjId);
		}

		inline void SendMonitorCustom(int type, int param, float delay = 0.0f)
		{
			GameObject *monitorObj = GetMonitorObj();
			Commands->Send_Custom_Event(Owner(), monitorObj, type, param, delay);
		}

		inline Vector3 GetMonitorPos()
		{
			GameObject *monitorObj = GetMonitorObj();

			return Commands->Get_Position(monitorObj);
		}

	private:

		int monitorObjId;
};
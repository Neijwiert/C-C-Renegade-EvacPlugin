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

#include "General.h"
#include "GK_Evac_Troop_Bone.h"

#include "GK_Evac_Monitor.h"

#ifdef _DEBUG
#include <engine_common.h>
#endif

void GK_Evac_Troop_Bone::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, GK_EVAC_TROOP_BONE_ANIM))
	{
#ifdef _DEBUG
		Console_Output("Troop bone done with animation, destroying\n");
#endif

		int evacPersonId = Get_Int_Parameter("EvacPersonId");
		GameObject *evacPersonObj = Commands->Find_Object(evacPersonId);
		Commands->Send_Custom_Event(obj, evacPersonObj, GK_CUSTOM_EVENT_TROOP_BONE_FINISHED_ANIMATION, 0, 0.0f);

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<GK_Evac_Troop_Bone> GK_Evac_Troop_BoneRegistrant("GK_Evac_Troop_Bone", "EvacPersonId:int");
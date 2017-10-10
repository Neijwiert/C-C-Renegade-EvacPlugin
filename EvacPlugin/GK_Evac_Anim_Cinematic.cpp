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
#include "GK_Evac_Anim_Cinematic.h"
#include "GK_Evac_Monitor.h"

#include <engine_string.h>

void GK_Evac_Anim_Cinematic::Register_Auto_Save_Variables()
{
	GK_Evac_Base::Register_Auto_Save_Variables();

	Auto_Save_Variable(&this->wayPathObjId, sizeof(this->wayPathObjId), 1);
	Auto_Save_Variable(&this->ropeObjId, sizeof(this->ropeObjId), 2);
}

void GK_Evac_Anim_Cinematic::Created(GameObject *obj)
{
	GK_Evac_Base::Created(obj);

	Commands->Enable_Hibernation(obj, false);

	GameObject *wayPathObj = CreateWayPathObj();
	CreateChopperObj(wayPathObj);
	CreateRopeObj();

	Commands->Start_Timer(obj, this, 240 / 30.0f, 0);
}

void GK_Evac_Anim_Cinematic::Timer_Expired(GameObject *obj, int number)
{
	if (number == 0)
	{
		GameObject *wayPathObj = Commands->Find_Object(this->wayPathObjId);
		Commands->Set_Animation(wayPathObj, "XG_EV5_Path.XG_EV5_PathL", true, NULL, 0.0f, -1.0f, false);

		GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);
		Commands->Set_Animation(ropeObj, "XG_EV5_rope.XG_EV5_ropeL", true, NULL, 0.0f, -1.0f, false);

		Commands->Destroy_Object(obj);
	}
}

GameObject *GK_Evac_Anim_Cinematic::CreateCinematicObject(const char *model)
{
	Vector3 pos = Commands->Get_Position(Owner());
	GameObject *cinObj = Commands->Create_Object("Generic_Cinematic", pos);

	Commands->Enable_Hibernation(cinObj, false);
	if (cinObj)
	{
		Commands->Add_To_Dirty_Cull_List(cinObj);
		Commands->Set_Model(cinObj, model);
		Commands->Set_Facing(cinObj, Commands->Get_Facing(Owner()));
	}

	return cinObj;
}

GameObject *GK_Evac_Anim_Cinematic::CreateRealObject(const char *preset)
{
	Vector3 pos = Commands->Get_Position(Owner());
	GameObject *realObj = Commands->Create_Object(preset, pos);
	Commands->Set_Facing(realObj, Commands->Get_Facing(Owner()));

	if (realObj)
	{
		Commands->Enable_Engine(realObj, true);
		Commands->Add_To_Dirty_Cull_List(realObj);
	}

	return realObj;
}

GameObject *GK_Evac_Anim_Cinematic::CreateWayPathObj()
{
	GameObject *wayPathObj = CreateCinematicObject("XG_EV5_Path");
	Commands->Attach_Script(wayPathObj, "GK_Evac_Generic_Cinematic_Item", StringClass::getFormattedString("%d,%d,%s", GetMonitorObjId(), GK_EVAC_CINEMATIC_ITEM_TYPE_WAYPATH, "XG_EV5_Path.XG_EV5_PathZ"));
	Commands->Set_Animation(wayPathObj, "XG_EV5_Path.XG_EV5_PathA", true, NULL, 0.0f, -1.0f, false);

	this->wayPathObjId = Commands->Get_ID(wayPathObj);

	return wayPathObj;
}

void GK_Evac_Anim_Cinematic::CreateChopperObj(GameObject *wayPathObj)
{
	const char *transportPreset = Get_Parameter("TransportPreset");
	GameObject *chopperObj = CreateRealObject(transportPreset);
	Commands->Attach_Script(chopperObj, "GK_Evac_Generic_Cinematic_Item", StringClass::getFormattedString("%d,%d,%s", GetMonitorObjId(), GK_EVAC_CINEMATIC_ITEM_TYPE_CHOPPER, "v_GDI_trnspt.XG_EV5_trnsz"));
	
	const char *canChopperBeSeen = Get_Parameter("CanChopperBeSeen");
	const char *canChopperBeDamaged = Get_Parameter("CanChopperBeDamaged");
	Commands->Attach_Script(chopperObj, "GK_Evac_Chopper_Cinematic_Item", StringClass::getFormattedString("%d,%s,%s", GetMonitorObjId(), canChopperBeSeen, canChopperBeDamaged));

	Commands->Attach_To_Object_Bone(chopperObj, wayPathObj, "BN_Trajectory");
}

void GK_Evac_Anim_Cinematic::CreateRopeObj()
{
	GameObject *ropeObj = CreateCinematicObject("XG_EV5_rope");
	Commands->Attach_Script(ropeObj, "GK_Evac_Generic_Cinematic_Item", StringClass::getFormattedString("%d,%d,%s", GetMonitorObjId(), GK_EVAC_CINEMATIC_ITEM_TYPE_ROPE, "XG_EV5_rope.XG_EV5_ropeZ"));
	Commands->Set_Animation(ropeObj, "XG_EV5_rope.XG_EV5_ropeA", true, NULL, 0.0f, -1.0f, false);

	this->ropeObjId = Commands->Get_ID(ropeObj);
}

ScriptRegistrant<GK_Evac_Anim_Cinematic> GK_Evac_Anim_CinematicRegistrant("GK_Evac_Anim_Cinematic", PPCAT(MONITOR_OBJ_ID_PARAMETER, ", TransportPreset:string, CanChopperBeSeen:int, CanChopperBeDamaged:int"));
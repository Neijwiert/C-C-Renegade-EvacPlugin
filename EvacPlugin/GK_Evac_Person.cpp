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
#include "GK_Evac_Person.h"
#include "GK_Evac_Monitor.h"

#include <SoldierGameObj.h>

void GK_Evac_Person::Register_Auto_Save_Variables()
{
	GK_Evac_Base::Register_Auto_Save_Variables();

	Auto_Save_Variable(&this->shouldUnregister, sizeof(this->shouldUnregister), 1);
	Auto_Save_Variable(&this->movingToEvacLocation, sizeof(this->movingToEvacLocation), 2);
	Auto_Save_Variable(&this->troopBoneObjId, sizeof(this->troopBoneObjId), 3);
	Auto_Save_Variable(&this->previousHibernatingState, sizeof(this->previousHibernatingState), 4);
	Auto_Save_Variable(&this->previousInnateState, sizeof(this->previousInnateState), 5);
}

void GK_Evac_Person::Created(GameObject *obj)
{
	GK_Evac_Base::Created(obj);

	if (!obj->As_SoldierGameObj())
	{
		Destroy_Script();
	}

	this->shouldUnregister = true;
	this->movingToEvacLocation = false;
	this->troopBoneObjId = 0;
	this->previousHibernatingState = obj->Is_Hibernating();
	this->previousInnateState = obj->As_SoldierGameObj()->Is_Innate_Enabled();

	SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_REGISTER_EVAC_PERSON, 0);
}

void GK_Evac_Person::Destroyed(GameObject *obj)
{
	if (this->shouldUnregister)
	{
		SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_UNREGISTER_EVAC_PERSON, 0);
	}
}

void GK_Evac_Person::Killed(GameObject *obj, GameObject *killer)
{
	this->shouldUnregister = false;

	SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_PERSON_KILLED, 0);
}

void GK_Evac_Person::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED)
	{
		GotoEvacLocation();
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_PERSON_BEING_EVACUATED)
	{
		this->troopBoneObjId = param;
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC)
	{
		CancelEvac();
	}
}

void GK_Evac_Person::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 4001)
	{
		this->movingToEvacLocation = false;

		if (complete_reason == ACTION_COMPLETE_NORMAL)
		{
			SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_PERSON_ARRIVED_AT_EVAC_LOCATION, 0);
		}
		else
		{
			SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION, 0);
		}
	}
}

void GK_Evac_Person::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, GK_EVAC_PERSON_EXIT_ANIM))
	{
		this->shouldUnregister = false;

		SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_PERSON_EVACUATED, 0);

		Commands->Destroy_Object(obj);
	}
}

void GK_Evac_Person::GotoEvacLocation()
{
	this->movingToEvacLocation = true;

	Commands->Enable_Hibernation(Owner(), false);
	Commands->Innate_Disable(Owner());

	Vector3 monitorPos = GetMonitorPos();

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 4001);
	params.Set_Movement(monitorPos, 0.8f, 1.0f);

	Commands->Action_Goto(Owner(), params);
}

void GK_Evac_Person::CancelEvac()
{
	if (this->movingToEvacLocation)
	{
		Commands->Action_Reset(Owner(), WWMATH_FLOAT_MAX);
	}

	if (this->troopBoneObjId != 0)
	{
		GameObject *troopBoneObj = Commands->Find_Object(this->troopBoneObjId);
		Commands->Destroy_Object(troopBoneObj);

		Commands->Set_Animation(Owner(), NULL, false, NULL, 0.0f, -1.0f, false);

		this->troopBoneObjId = 0;
	}

	RestoreState();

	Destroy_Script();
}

void GK_Evac_Person::RestoreState()
{
	Commands->Enable_Hibernation(Owner(), this->previousHibernatingState);

	if (this->previousInnateState)
	{
		Commands->Innate_Enable(Owner());
	}
}

ScriptRegistrant<GK_Evac_Person> GK_Evac_PersonRegistrant("GK_Evac_Person", MONITOR_OBJ_ID_PARAMETER);
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
#include "GK_Evac_Generic_Cinematic_Item.h"
#include "GK_Evac_Monitor.h"

void GK_Evac_Generic_Cinematic_Item::Register_Auto_Save_Variables()
{
	GK_Evac_Base::Register_Auto_Save_Variables();

	Auto_Save_Variable(&this->chopperArrived, sizeof(this->chopperArrived), 1);
}

void GK_Evac_Generic_Cinematic_Item::Created(GameObject *obj)
{
	GK_Evac_Base::Created(obj);

	this->chopperArrived = false;

	Commands->Enable_Hibernation(obj, false);

	int cinematicItemType = Get_Int_Parameter("CinematicItemType");
	SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_REGISTER_CINEMATIC_ITEM, cinematicItemType);
}

void GK_Evac_Generic_Cinematic_Item::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED)
	{
		this->chopperArrived = true;
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_COMPLETE)
	{
		GotoExitAnim();
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC)
	{
		CancelEvac();
	}
}

void GK_Evac_Generic_Cinematic_Item::Animation_Complete(GameObject *obj, const char *animation_name)
{
	const char *exitAnimation = Get_Parameter("ExitAnimation");
	if (!_stricmp(animation_name, exitAnimation))
	{
		Commands->Destroy_Object(obj);
	}
}

void GK_Evac_Generic_Cinematic_Item::GotoExitAnim()
{
	const char *exitAnimation = Get_Parameter("ExitAnimation");
	Commands->Set_Animation(Owner(), exitAnimation, false, NULL, 0.0f, -1.0f, false);
}

void GK_Evac_Generic_Cinematic_Item::CancelEvac()
{
	if (this->chopperArrived)
	{
		GotoExitAnim();
	}
	else
	{
		Commands->Destroy_Object(Owner());
	}
}

ScriptRegistrant<GK_Evac_Generic_Cinematic_Item> GK_Evac_Generic_Cinematic_ItemRegistrant("GK_Evac_Generic_Cinematic_Item", PPCAT(MONITOR_OBJ_ID_PARAMETER,", CinematicItemType:int, ExitAnimation:string"));
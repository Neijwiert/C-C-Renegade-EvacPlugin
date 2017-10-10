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
#include "GK_Evac_Chopper_Cinematic_Item.h"
#include "GK_Evac_Monitor.h"

void GK_Evac_Chopper_Cinematic_Item::Register_Auto_Save_Variables()
{
	GK_Evac_Base::Register_Auto_Save_Variables();

	Auto_Save_Variable(&this->canBeDamaged, sizeof(this->canBeDamaged), 1);
}

void GK_Evac_Chopper_Cinematic_Item::Created(GameObject *obj)
{
	GK_Evac_Base::Created(obj);

	this->canBeDamaged = (Get_Int_Parameter("CanBeDamaged") > 0);

	if (!Get_Int_Parameter("CanBeSeen"))
	{
		Commands->Set_Is_Visible(obj, false);
	}

	if (!this->canBeDamaged)
	{
		Commands->Set_Shield_Type(obj, "Blamo");
	}

	Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsA", false, NULL, 0.0f, -1.0f, false);
}

void GK_Evac_Chopper_Cinematic_Item::Killed(GameObject *obj, GameObject *killer)
{
	SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_CHOPPER_KILLED, 0);
}

void GK_Evac_Chopper_Cinematic_Item::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!this->canBeDamaged)
	{
		float maxHealth = Commands->Get_Max_Health(obj);
		float maxShield = Commands->Get_Max_Shield_Strength(obj);

		Commands->Set_Health(obj, maxHealth);
		Commands->Set_Shield_Strength(obj, maxShield);
	}
}

void GK_Evac_Chopper_Cinematic_Item::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV5_trnsA"))
	{
		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsL", true, NULL, 0.0f, -1.0f, false);

		SendMonitorCustom(GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED, 0);
	}
}

ScriptRegistrant<GK_Evac_Chopper_Cinematic_Item> GK_Evac_Chopper_Cinematic_ItemRegistrant("GK_Evac_Chopper_Cinematic_Item", PPCAT(MONITOR_OBJ_ID_PARAMETER, ", CanBeSeen:int, CanBeDamaged:int"));
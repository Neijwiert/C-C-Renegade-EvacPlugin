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
#include "GK_Evac_Monitor.h"
#include "GK_Evac_Person.h"
#include "GK_Evac_Troop_Bone.h"

#include <engine_string.h>

void GK_Evac_Monitor::Save(ScriptSaver &saver)
{
	Commands->Begin_Chunk(saver, 1000);

	Commands->Save_Data(saver, 1, sizeof(this->maxEvacPersonDeaths), &this->maxEvacPersonDeaths);
	Commands->Save_Data(saver, 2, sizeof(this->currentEvacPersonDeaths), &this->currentEvacPersonDeaths);
	Commands->Save_Data(saver, 3, sizeof(this->notifyObjId), &this->notifyObjId);
	Commands->Save_Data(saver, 4, sizeof(this->evacInitiated), &this->evacInitiated);
	Commands->Save_Data(saver, 5, sizeof(this->chopperObjId), &this->chopperObjId);
	Commands->Save_Data(saver, 6, sizeof(this->ropeObjId), &this->ropeObjId);
	Commands->Save_Data(saver, 7, sizeof(this->waypathObjId), &this->waypathObjId);
	Commands->Save_Data(saver, 8, sizeof(this->evacuatedPersonsCount), &this->evacuatedPersonsCount);
	Commands->Save_Data(saver, 9, sizeof(this->personBeingEvacuatedId), &this->personBeingEvacuatedId);

	Commands->End_Chunk(saver);

	Commands->Begin_Chunk(saver, 1001);

	int evacPersonIdsCount = this->evacPersonIds.Count();
	Commands->Save_Data(saver, 9, sizeof(evacPersonIdsCount), &evacPersonIdsCount);
	Commands->Save_Data(saver, 10, sizeof(int) * evacPersonIdsCount, this->evacPersonIds.Peek());

	Commands->End_Chunk(saver);

	Commands->Begin_Chunk(saver, 1002);

	int waitingForEvacPersonIdsCount = this->waitingForEvacPersonIds.Count();
	Commands->Save_Data(saver, 11, sizeof(waitingForEvacPersonIdsCount), &waitingForEvacPersonIdsCount);
	Commands->Save_Data(saver, 12, sizeof(int) * waitingForEvacPersonIdsCount, this->waitingForEvacPersonIds.Peek());

	Commands->End_Chunk(saver);
}

void GK_Evac_Monitor::Load(ScriptLoader &loader)
{
	unsigned int chunkType;
	while (Commands->Open_Chunk(loader, &chunkType))
	{
		if (chunkType == 1000)
		{
			int variableId;
			while (Commands->Load_Begin(loader, &variableId))
			{
				switch (variableId)
				{
					case 1:
						Commands->Load_Data(loader, sizeof(this->maxEvacPersonDeaths), &this->maxEvacPersonDeaths);

						break;
					case 2:
						Commands->Load_Data(loader, sizeof(this->currentEvacPersonDeaths), &this->currentEvacPersonDeaths);

						break;
					case 3:
						Commands->Load_Data(loader, sizeof(this->notifyObjId), &this->notifyObjId);

						break;
					case 4:
						Commands->Load_Data(loader, sizeof(this->evacInitiated), &this->evacInitiated);

						break;
					case 5:
						Commands->Load_Data(loader, sizeof(this->chopperObjId), &this->chopperObjId);

						break;
					case 6:
						Commands->Load_Data(loader, sizeof(this->ropeObjId), &this->ropeObjId);

						break;
					case 7:
						Commands->Load_Data(loader, sizeof(this->waypathObjId), &this->waypathObjId);

						break;
					case 8:
						Commands->Load_Data(loader, sizeof(this->evacuatedPersonsCount), &this->evacuatedPersonsCount);

						break;
					case 9:
						Commands->Load_Data(loader, sizeof(this->personBeingEvacuatedId), &this->personBeingEvacuatedId);

						break;
				}

				Commands->Load_End(loader);
			}
		}
		else if (chunkType == 1001)
		{
			int variableId;
			int evacPersonIdsCount = 0;
			while (Commands->Load_Begin(loader, &variableId))
			{
				if (variableId == 9)
				{
					Commands->Load_Data(loader, sizeof(evacPersonIdsCount), &evacPersonIdsCount);
				}
				else if (variableId == 10)
				{
					int *data = new int[evacPersonIdsCount];
					Commands->Load_Data(loader, sizeof(int) * evacPersonIdsCount, data);

					this->evacPersonIds.Delete_All();
					this->evacPersonIds.Resize(evacPersonIdsCount);

					for (int x = 0; x < evacPersonIdsCount; x++)
					{
						this->evacPersonIds.Add(data[x]);
					}

					delete[] data;
				}

				Commands->Load_End(loader);
			}
		}
		else if (chunkType == 1002)
		{
			int variableId;
			int waitingForEvacPersonIdsCount = 0;
			while (Commands->Load_Begin(loader, &variableId))
			{
				if (variableId == 11)
				{
					Commands->Load_Data(loader, sizeof(waitingForEvacPersonIdsCount), &waitingForEvacPersonIdsCount);
				}
				else if (variableId == 12)
				{
					int *data = new int[waitingForEvacPersonIdsCount];
					Commands->Load_Data(loader, sizeof(int) * waitingForEvacPersonIdsCount, data);

					this->waitingForEvacPersonIds.Delete_All();
					this->waitingForEvacPersonIds.Resize(waitingForEvacPersonIdsCount);

					for (int x = 0; x < waitingForEvacPersonIdsCount; x++)
					{
						this->waitingForEvacPersonIds.Add(data[x]);
					}

					delete[] data;
				}

				Commands->Load_End(loader);
			}
		}

		Commands->Close_Chunk(loader);
	}
}

void GK_Evac_Monitor::Created(GameObject *obj)
{
	this->personBeingEvacuatedId = 0;
	this->maxEvacPersonDeaths = Get_Int_Parameter("MaxEvacPersonDeaths");
	this->currentEvacPersonDeaths = 0;
	this->notifyObjId = Get_Int_Parameter("NotifyObjId");
	this->evacInitiated = false;
	this->chopperObjId = 0;
	this->ropeObjId = 0;
	this->waypathObjId = 0;
	this->evacuatedPersonsCount = 0;
}

void GK_Evac_Monitor::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == GK_CUSTOM_EVENT_EVAC_REGISTER_EVAC_PERSON)
	{
		RegisterEvacPerson(sender);
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_UNREGISTER_EVAC_PERSON)
	{
		UnregisterEvacPerson(sender);
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_PERSON_KILLED)
	{
		EvacPersonKilled(sender);
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_INITIATE_EVAC)
	{
		InitiateEvac();
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC)
	{
#ifdef _DEBUG
		Console_Output("Evac failed: CANCELED\n");
#endif

		GameObject *notifyObj = Commands->Find_Object(this->notifyObjId);
		Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_FAILED, GK_EVAC_FAILED_REASON_CANCELED, 0.0f);

		CancelEvac();
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_CHOPPER_KILLED)
	{
		ChopperKilled();
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_REGISTER_CINEMATIC_ITEM)
	{
		RegisterCinematicItem(sender, param);
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED)
	{
		ChopperArrived();
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_PERSON_ARRIVED_AT_EVAC_LOCATION)
	{
		EvacPersonReadyForEvac(sender);
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION)
	{
		FailedToMoveToEvac(sender);
	}
	else if (type == GK_CUSTOM_EVENT_EVAC_PERSON_EVACUATED)
	{
		PersonEvacuated(sender);
	}
}

void GK_Evac_Monitor::RegisterEvacPerson(GameObject *evacPerson)
{
	if (!evacPerson || this->evacInitiated)
	{
		return;
	}

	int evacPersonId = Commands->Get_ID(evacPerson);;
	for (int x = 0; x < this->evacPersonIds.Count(); x++)
	{
		if (this->evacPersonIds[x] == evacPersonId)
		{
			return;
		}
	}

	this->evacPersonIds.Add(evacPersonId);
}

void GK_Evac_Monitor::UnregisterEvacPerson(GameObject *evacPerson)
{
	if (!evacPerson)
	{
		return;
	}

	int evacPersonId = Commands->Get_ID(evacPerson);;

	int index = this->evacPersonIds.Find_Index(evacPersonId);
	if (index != -1)
	{
		if (evacPersonId == this->personBeingEvacuatedId)
		{
			this->personBeingEvacuatedId = 0;

			EvacuateNextPerson();
		}

		this->evacPersonIds.Delete_Range(index, 1);
	}
}

void GK_Evac_Monitor::EvacPersonKilled(GameObject *evacPerson)
{
	if (!evacPerson)
	{
		return;
	}

	int evacPersonId = Commands->Get_ID(evacPerson);
	int index = this->evacPersonIds.Find_Index(evacPersonId);
	if (index != -1)
	{
		this->evacPersonIds.Delete_Range(index, 1);
		this->currentEvacPersonDeaths++;

		GameObject *notifyObj = Commands->Find_Object(this->notifyObjId);
		Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_PERSON_KILLED, evacPersonId, 0.0f);

		if (this->currentEvacPersonDeaths >= this->maxEvacPersonDeaths)
		{
#ifdef _DEBUG
			Console_Output("Evac failed: TOO MANY DEATHS\n");
#endif
			Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_FAILED, GK_EVAC_FAILED_REASON_TOO_MANY_EVAC_PERSON_DEATHS, 0.0f);

			CancelEvac();
		}
		else
		{
			if (evacPersonId == this->personBeingEvacuatedId)
			{
				this->personBeingEvacuatedId = 0;

				EvacuateNextPerson();
			}
		}
	}
}

void GK_Evac_Monitor::InitiateEvac()
{
	if (this->evacInitiated)
	{
		return;
	}

	this->evacInitiated = true;

	Vector3 pos = Commands->Get_Position(Owner());
	GameObject *invisObj = Commands->Create_Object("Invisible_Object", pos);

	int objId = Commands->Get_ID(Owner());
	const char *transportPreset = Get_Parameter("TransportPreset");
	const char *canChopperBeSeen = Get_Parameter("CanChopperBeSeen");
	const char *canChopperBeDamaged = Get_Parameter("CanChopperBeDamaged");
	Commands->Attach_Script(invisObj, "GK_Evac_Anim_Cinematic", StringClass::getFormattedString("%d,%s,%s,%s", objId, transportPreset, canChopperBeSeen, canChopperBeDamaged));
}

void GK_Evac_Monitor::CancelEvac()
{
	if (this->evacInitiated)
	{
		for (int x = 0; x < this->evacPersonIds.Count(); x++)
		{
			GameObject *evacPerson = Commands->Find_Object(this->evacPersonIds[x]);
			Commands->Send_Custom_Event(Owner(), evacPerson, GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC, 0, 0.0f);
		}

		GameObject *chopperObj = Commands->Find_Object(this->chopperObjId);
		Commands->Send_Custom_Event(Owner(), chopperObj, GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC, 0, 0.0f);

		GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);
		Commands->Send_Custom_Event(Owner(), ropeObj, GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC, 0, 0.0f);

		GameObject *waypathObj = Commands->Find_Object(this->waypathObjId);
		Commands->Send_Custom_Event(Owner(), waypathObj, GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC, 0, 0.0f);
	}

	Destroy_Script();
}

void GK_Evac_Monitor::RegisterCinematicItem(GameObject *cinematicItem, int cinematicItemType)
{
	if (!this->evacInitiated)
	{
		return;
	}

	switch (cinematicItemType)
	{
		case GK_EVAC_CINEMATIC_ITEM_TYPE_CHOPPER:
			this->chopperObjId = Commands->Get_ID(cinematicItem);

			break;
		case GK_EVAC_CINEMATIC_ITEM_TYPE_ROPE:
			this->ropeObjId = Commands->Get_ID(cinematicItem);

			break;
		case GK_EVAC_CINEMATIC_ITEM_TYPE_WAYPATH:
			this->waypathObjId = Commands->Get_ID(cinematicItem);

			break;
	}
}

void GK_Evac_Monitor::ChopperArrived()
{
	if (!this->evacInitiated)
	{
		return;
	}

	for (int x = 0; x < this->evacPersonIds.Count(); x++)
	{
		GameObject *evacPerson = Commands->Find_Object(this->evacPersonIds[x]);
		Commands->Send_Custom_Event(Owner(), evacPerson, GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED, 0, 0.0f);
	}

	GameObject *chopperObj = Commands->Find_Object(this->chopperObjId);
	Commands->Send_Custom_Event(Owner(), chopperObj, GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED, 0, 0.0f);

	GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);
	Commands->Send_Custom_Event(Owner(), ropeObj, GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED, 0, 0.0f);

	GameObject *waypathObj = Commands->Find_Object(this->waypathObjId);
	Commands->Send_Custom_Event(Owner(), waypathObj, GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED, 0, 0.0f);
}

void GK_Evac_Monitor::FailedToMoveToEvac(GameObject *evacPerson)
{
	if (!evacPerson || !this->evacInitiated)
	{
		return;
	}

	int evacPersonId = Commands->Get_ID(evacPerson);
	int index = this->evacPersonIds.Find_Index(evacPersonId);
	if (index != -1)
	{
#ifdef _DEBUG
		Console_Output("Evac failed: FAILED TO MOVE TO EVAC LOC\n");
#endif

		GameObject *notifyObj = Commands->Find_Object(this->notifyObjId);
		Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION, evacPersonId, 0.0f);
		Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_FAILED, GK_EVAC_FAILED_REASON_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION, 0.0f);

		CancelEvac();
	}
}

void GK_Evac_Monitor::EvacPersonReadyForEvac(GameObject *evacPerson)
{
	if (!evacPerson || !this->evacInitiated)
	{
		return;
	}

	int evacPersonId = Commands->Get_ID(evacPerson);
	int index = this->evacPersonIds.Find_Index(evacPersonId);
	if (index != -1)
	{
		if (this->personBeingEvacuatedId != 0)
		{
			this->waitingForEvacPersonIds.Add(evacPersonId);
		}
		else
		{
			EvacuatePerson(evacPerson);
		}
	}
}

void GK_Evac_Monitor::PersonEvacuated(GameObject *evacPerson)
{
	if (!evacPerson || !this->evacInitiated)
	{
		return;
	}

	int evacPersonId = Commands->Get_ID(evacPerson);
	int index = this->evacPersonIds.Find_Index(evacPersonId);
	if (index != -1)
	{
		this->personBeingEvacuatedId = 0;
		this->evacuatedPersonsCount++;

		GameObject *notifyObj = Commands->Find_Object(this->notifyObjId);
		Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_PERSON_EVACUATED, evacPersonId, 0.0f);

		if (this->evacuatedPersonsCount >= this->evacPersonIds.Count())
		{
			Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_COMPLETE, 0, 0.0f);

			GameObject *chopperObj = Commands->Find_Object(this->chopperObjId);
			Commands->Send_Custom_Event(Owner(), chopperObj, GK_CUSTOM_EVENT_EVAC_COMPLETE, 0, 0.0f);

			GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);
			Commands->Send_Custom_Event(Owner(), ropeObj, GK_CUSTOM_EVENT_EVAC_COMPLETE, 0, 0.0f);

			GameObject *waypathObj = Commands->Find_Object(this->waypathObjId);
			Commands->Send_Custom_Event(Owner(), waypathObj, GK_CUSTOM_EVENT_EVAC_COMPLETE, 0, 0.0f);

			Destroy_Script();
		}
		else
		{
			EvacuateNextPerson();
		}
	}
}

void GK_Evac_Monitor::ChopperKilled()
{
	if (!this->evacInitiated)
	{
		return;
	}

#ifdef _DEBUG
	Console_Output("Evac failed: CHOPPER KILLED\n");
#endif

	GameObject *notifyObj = Commands->Find_Object(this->notifyObjId);
	Commands->Send_Custom_Event(Owner(), notifyObj, GK_CUSTOM_EVENT_EVAC_FAILED, GK_EVAC_FAILED_REASON_CHOPPER_KILLED, 0.0f);

	CancelEvac();
}

void GK_Evac_Monitor::EvacuatePerson(GameObject *evacPerson)
{
	this->personBeingEvacuatedId = Commands->Get_ID(evacPerson);

	Vector3 pos = Commands->Get_Position(Owner());
	GameObject *troopBoneObj = Commands->Create_Object("Invisible_Object", pos);

	int troopBoneObjId = Commands->Get_ID(troopBoneObj);
	Commands->Send_Custom_Event(Owner(), evacPerson, GK_CUSTOM_EVENT_EVAC_PERSON_BEING_EVACUATED, troopBoneObjId, 0.0f);

	Commands->Set_Model(troopBoneObj, "XG_EV5_troopBN");
	Commands->Attach_Script(troopBoneObj, "GK_Evac_Troop_Bone", StringClass::getFormattedString("%d", this->personBeingEvacuatedId));
	Commands->Attach_To_Object_Bone(evacPerson, troopBoneObj, "Troop_L");

	Commands->Set_Animation(troopBoneObj, GK_EVAC_TROOP_BONE_ANIM, false, NULL, 0.0f, -1.0f, false);

	Commands->Set_Animation(evacPerson, "S_A_Human.XG_EV5_troop", false, NULL, 0.0f, -1.0f, false);
}

void GK_Evac_Monitor::EvacuateNextPerson()
{
	while (this->waitingForEvacPersonIds.Count() > 0)
	{
		int currentEvacPersonId = this->waitingForEvacPersonIds[0];
		this->waitingForEvacPersonIds.Delete_Range(0, 1);

		if (this->evacPersonIds.Find_Index(currentEvacPersonId) != -1)
		{
			GameObject *nextEvacPerson = Commands->Find_Object(currentEvacPersonId);
			if (nextEvacPerson)
			{
				EvacuatePerson(nextEvacPerson);

				return;
			}
		}
	}
}

ScriptRegistrant<GK_Evac_Monitor> GK_Evac_MonitorRegistrant("GK_Evac_Monitor", "NotifyObjId:int, MaxEvacPersonDeaths:int, TransportPreset:string, CanChopperBeSeen:int, CanChopperBeDamaged:int");
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
#include <engine_vector.h>

enum
{
	GK_CUSTOM_EVENT_EVAC_REGISTER_EVAC_PERSON = 10000000,
	GK_CUSTOM_EVENT_EVAC_UNREGISTER_EVAC_PERSON,
	GK_CUSTOM_EVENT_EVAC_PERSON_KILLED,
	GK_CUSTOM_EVENT_EVAC_FAILED,
	GK_CUSTOM_EVENT_EVAC_INITIATE_EVAC,
	GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC,
	GK_CUSTOM_EVENT_EVAC_REGISTER_CINEMATIC_ITEM,
	GK_CUSTOM_EVENT_EVAC_CHOPPER_ARRIVED,
	GK_CUSTOM_EVENT_EVAC_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION,
	GK_CUSTOM_EVENT_EVAC_PERSON_ARRIVED_AT_EVAC_LOCATION,
	GK_CUSTOM_EVENT_EVAC_PERSON_EVACUATED,
	GK_CUSTOM_EVENT_EVAC_PERSON_BEING_EVACUATED,
	GK_CUSTOM_EVENT_EVAC_COMPLETE,
	GK_CUSTOM_EVENT_EVAC_CHOPPER_KILLED
};

enum
{
	GK_EVAC_FAILED_REASON_CANCELED,
	GK_EVAC_FAILED_REASON_TOO_MANY_EVAC_PERSON_DEATHS,
	GK_EVAC_FAILED_REASON_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION,
	GK_EVAC_FAILED_REASON_CHOPPER_KILLED
};

enum
{
	GK_EVAC_CINEMATIC_ITEM_TYPE_CHOPPER,
	GK_EVAC_CINEMATIC_ITEM_TYPE_ROPE,
	GK_EVAC_CINEMATIC_ITEM_TYPE_WAYPATH
};

#define GK_EVAC_PERSON_EXIT_ANIM "S_A_Human.XG_EV5_troop"

class GK_Evac_Monitor : public ScriptImpClass
{
	private:
		virtual void Save(ScriptSaver &saver);
		virtual void Load(ScriptLoader &loader);
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);

		void RegisterEvacPerson(GameObject *evacPerson);
		void UnregisterEvacPerson(GameObject *evacPerson);
		void EvacPersonKilled(GameObject *evacPerson);
		void InitiateEvac();
		void CancelEvac();
		void RegisterCinematicItem(GameObject *cinematicItem, int cinematicItemType);
		void ChopperArrived();
		void FailedToMoveToEvac(GameObject *evacPerson);
		void EvacPersonReadyForEvac(GameObject *evacPerson);
		void PersonEvacuated(GameObject *evacPerson);
		void ChopperKilled();
		void EvacuatePerson(GameObject *evacPerson);

		SimpleDynVecClass<int> evacPersonIds;
		SimpleDynVecClass<int> waitingForEvacPersonIds;
		bool evacuatingAPerson;
		int maxEvacPersonDeaths;
		int currentEvacPersonDeaths;
		int notifyObjId;
		bool evacInitiated;
		int chopperObjId;
		int ropeObjId;
		int waypathObjId;
		int evacuatedPersonsCount;
};
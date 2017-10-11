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
#include "EvacPlugin.h"
#include "engine_tt.h"
#include "engine_io.h"
#include "gmgame.h"

#ifdef CHAT_TEST
#include "GK_Evac_Monitor.h"
#endif

EvacPlugin::EvacPlugin()
{
#ifdef CHAT_TEST
	RegisterEvent(EVENT_CHAT_HOOK, this);
#endif
}

EvacPlugin::~EvacPlugin()
{
#ifdef CHAT_TEST
	UnregisterEvent(EVENT_CHAT_HOOK, this);
#endif
}

#ifdef CHAT_TEST
int evacMonitorId = 0;

bool EvacPlugin::OnChat(int PlayerID, TextMessageEnum Type, const wchar_t *Message, int recieverID)
{
	GameObject *playerObj = Get_GameObj(PlayerID);
	Vector3 playerPos = Commands->Get_Position(playerObj);

	if (wcscmp(Message, L"!test1") == 0)
	{
		GameObject *evacMonitorObj = Commands->Create_Object("Invisible_Object", playerPos);
		evacMonitorId = Commands->Get_ID(evacMonitorObj); 
		Commands->Attach_Script(evacMonitorObj, "GK_Evac_Monitor", "0,1,GDI_Transport_Helicopter_Flyover,1,0");
	}
	else if (wcscmp(Message, L"!test2") == 0)
	{
		playerPos.Y += 5;
		playerPos.Z += 1;
		GameObject *evacPerson1 = Commands->Create_Object("GDI_MiniGunner_0", playerPos);
		Commands->Attach_Script(evacPerson1, "GK_Evac_Person", StringClass::getFormattedString("%d", evacMonitorId));
	}
	else if (wcscmp(Message, L"!test3") == 0)
	{
		GameObject *evacMonitorObj = Commands->Find_Object(evacMonitorId);
		Commands->Send_Custom_Event(NULL, evacMonitorObj, GK_CUSTOM_EVENT_EVAC_INITIATE_EVAC, 0, 0.0f);
	}
	else if (wcscmp(Message, L"!test4") == 0)
	{
		GameObject *evacMonitorObj = Commands->Find_Object(evacMonitorId);
		Commands->Send_Custom_Event(NULL, evacMonitorObj, GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC, 0, 0.0f);
	}

	return true;
}
#endif

extern "C" __declspec(dllexport) Plugin *Plugin_Init()
{
	static EvacPlugin evacPlugin;

	return &evacPlugin;
}

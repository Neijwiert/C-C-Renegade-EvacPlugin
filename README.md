# C-C-Renegade-EvacPlugin

This plugin is used to evacuate n amount of soldiers (they will be destroyed once they are evacuated)

It contains the following scripts:
- GK_Evac_Monitor
    Script parameters:
      - NotifyObjId: The ID of an object which receives some customs to react upon what is happening with the evac
          GK_CUSTOM_EVENT_EVAC_FAILED: when the evac failed. The param specifies the reason 
            Param:
              GK_EVAC_FAILED_REASON_CANCELED: when the evac is canceled via custom
              GK_EVAC_FAILED_REASON_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION: when an evac person failed to move to the evac location                     GK_EVAC_FAILED_REASON_CHOPPER_KILLED: when the evac chopper is killed
              GK_EVAC_FAILED_REASON_TOO_MANY_EVAC_PERSON_DEATHS: when too many evac persons died
          GK_CUSTOM_EVENT_EVAC_PERSON_KILLED: when an evac person is killed. The param is the person's id.
          GK_CUSTOM_EVENT_EVAC_PERSON_FAILED_TO_MOVE_TO_EVAC_LOCATION: when an evac person failed to move to the evac location. The param                                                                        is the person's id.
          GK_CUSTOM_EVENT_EVAC_PERSON_EVACUATED: when an evac person is evacuated. The param is the person's id.
      - MaxEvacPersonDeaths: the maximum of evac persons that can die before it is considered failed
      - TransportPreset: the preset name for the evac chopper
      - CanChopperBeSeen: If objects with Enable_Enemy_Seen can see the chopper
      - CanChopperBeDamaged: Can the chopper be damaged at all
- GK_Evac_Person
    Script parameters:
      - MonitorObjId: The id of the monitor object (GK_Evac_Monitor)
      
Below scripts are also included, but they are used internally and don't need to be manually attached. I will not explain their script parameters.
  - GK_Evac_Anim_Cinematic
  - GK_Evac_Chopper_Cinematic_Item
  - GK_Evac_Generic_Cinematic_Item
  - GK_Evac_Troop_Bone

Example:

GameObject *evacMonitor = Commands->Create_Object("Invisible_Object", Vector3());
int evacMonitorId = Commands->Get_ID(evacMonitor);
Commands->Attach_Script(evacMonitor, "GK_Evac_Monitor", "0, 1, GDI_Transport_Helicopter_Flyover, 1, 0");

GameObject *evacPerson1 = Commands->Create_Object("GDI_MiniGunner_0", Vector3())
Commands->Attach_Script(evacPerson1, "GK_Evac_Person", StringClass::getFormattedString("%d", evacMonitorId));

GameObject *evacPerson2 = Commands->Create_Object("GDI_MiniGunner_0", Vector3())
Commands->Attach_Script(evacPerson2, "GK_Evac_Person", StringClass::getFormattedString("%d", evacMonitorId));

Then all you need to do is send a custom to the evac monitor GK_CUSTOM_EVENT_EVAC_INITIATE_EVAC. The evac will take place at the location of the monitor.

I based this on the logic of the evacuations in M01 and I improved it. The scripts should be singleplayer compatible. (Save/Loading working). You also have the ability to cancel an ongoing evacuation with the custom GK_CUSTOM_EVENT_EVAC_CANCEL_EVAC. 

#include <plugin.h> // Plugin-SDK version 1004 from 2026-04-18 13:03:53
#include <CMessages.h>
#include <CPedGroups.h>
#include <CGenericGameStorage.h>
#include <CStats.h>
#include <string>

using namespace plugin;

struct Main
{
    bool WasOnMission;
    std::string LastMissionPassedname;

    Main()
    {
        // register event callbacks
        Events::processScriptsEvent += []{ gInstance.OnGameProcess(); };
        Events::initGameEvent += [] {gInstance.Init(); };
    }

    void Init()
    {
        WasOnMission = CPedGroups::ms_bIsPlayerOnAMission;
        LastMissionPassedname = CStats::LastMissionPassedName;
        return;
    }

    void OnGameProcess()
    {
        if (WasOnMission && !CPedGroups::ms_bIsPlayerOnAMission)
        {
            if (!std::strcmp(LastMissionPassedname.c_str(), CStats::LastMissionPassedName))
            {
                return;
            }
            if (CGenericGameStorage::GenericSave(1)) //i have no clue about the meaning of this param maybe some docs are in order?
            {
                CMessages::AddMessageJumpQ("Autosaving...", 500, 0);
            }
            else {
                CMessages::AddMessageJumpQ("Failed to autosave...", 500, 0);
            }
            LastMissionPassedname = CStats::LastMissionPassedName;
        }

        WasOnMission = CPedGroups::ms_bIsPlayerOnAMission;
    }
} gInstance;

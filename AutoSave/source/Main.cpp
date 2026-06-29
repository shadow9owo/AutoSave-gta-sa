#include <plugin.h> // Plugin-SDK version 1004 from 2026-04-18 13:03:53
#include <CMessages.h>
#include <CPedGroups.h>
#include <CStats.h>
#include <C_PcSave.h>
#include <Windows.h>

using namespace plugin;

struct Main
{
    bool WasOnMission;
    int LastMissionProgress;

    Main()
    {
        // register event callbacks
        Events::processScriptsEvent += []{ gInstance.OnGameProcess(); };
        Events::initGameEvent += [] {gInstance.Init(); };
    }

    int GetCurrentSavePercent()
    {
        return (int)std::round(CStats::GetPercentageProgress() * 100.0f);
    }

    bool savenow()
    {
        PcSaveHelper.DeleteSlot(7);
        return !PcSaveHelper.SaveSlot(7); //its inverted i dont know why
    }

    void Init()
    {
        WasOnMission = CPedGroups::ms_bIsPlayerOnAMission;
        LastMissionProgress = GetCurrentSavePercent();
        return;
    }

    void OnGameProcess()
    {
        if (WasOnMission && !CPedGroups::ms_bIsPlayerOnAMission)
        {
            if (LastMissionProgress == GetCurrentSavePercent())
            {
                LastMissionProgress = GetCurrentSavePercent();
                WasOnMission = CPedGroups::ms_bIsPlayerOnAMission;
                return;
            }
            if (savenow())
            {
                CMessages::AddMessageJumpQ("Autosaving...", 3000, 0);
            }
            else {
                CMessages::AddMessageJumpQ("Failed to autosave...", 3000, 0);
            }
            LastMissionProgress = GetCurrentSavePercent();
        }

        WasOnMission = CPedGroups::ms_bIsPlayerOnAMission;
    }
} gInstance;

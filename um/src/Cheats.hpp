#pragma once
#include "Vector.hpp"
#include "Render.hpp"
#include "offsets.hpp"
#include "client.dll.hpp"
#include "Global.hpp"
#include <Windows.h>
#include <TlHelp32.h>
namespace driver {
    namespace codes {
        constexpr ULONG attach = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        constexpr ULONG read = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        constexpr ULONG write = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
    }
    struct Request {
        HANDLE process_id;

        PVOID target;
        PVOID buffer;

        SIZE_T size;
        SIZE_T return_size;
    };

    bool attach_to_process(HANDLE driver_handle, const DWORD pid) {
        Request r = {};
        r.process_id = reinterpret_cast<HANDLE>(pid);
        return DeviceIoControl(driver_handle, codes::attach, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
    }
    template <class T>
    T read_memory(HANDLE driver_handle, const std::uintptr_t addr) {
        T temp = {};
        Request r;
        r.target = reinterpret_cast<PVOID>(addr);
        r.buffer = &temp;
        r.size = sizeof(T);
        DeviceIoControl(driver_handle, codes::read, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);

        return temp;
    }
    template <class T>
    void write_memory(HANDLE driver_handle, const std::uintptr_t addr, const T& value) {
        Request r;
        r.target = reinterpret_cast<PVOID>(addr);
        r.buffer = (PVOID)&value;
        r.size = sizeof(T);

        DeviceIoControl(driver_handle, codes::write, &r, sizeof(r), &r, sizeof(r), nullptr, nullptr);
    }
}
namespace Cheats
{
    void espLoop(const int ProcessId, uintptr_t Client, uintptr_t Engine)
    {
        const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        const DWORD pid = ProcessId;
        const std::uintptr_t client = Client;
        const std::uintptr_t engine = Engine;

        uintptr_t LocalPlayerPawn = driver::read_memory<uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
        view_matrix_t viewMatrix = driver::read_memory<view_matrix_t>(driver, client + client_dll::dwViewMatrix);

        int myHealth = driver::read_memory<int>(driver, LocalPlayerPawn + C_BaseEntity::m_iHealth);
        int myTeam = driver::read_memory<int>(driver, LocalPlayerPawn + C_BaseEntity::m_iTeamNum);

        uintptr_t Entity = driver::read_memory<uintptr_t>(driver, client + client_dll::dwEntityList);

        for (int i = 1; i < 64; i++)
        {
            uintptr_t listEntity = driver::read_memory<uintptr_t>(driver, Entity + ((8 * (i & 0x7FFF) >> 9) + 16));
            if (listEntity == 0)
                continue;

            uintptr_t entityController = driver::read_memory<uintptr_t>(driver, listEntity + (120) * (i & 0x1FF));
            if (entityController == 0)
                continue;

            uintptr_t entityControllerPawn = driver::read_memory<uintptr_t>(driver, entityController + CCSPlayerController::m_hPlayerPawn);
            if (entityControllerPawn == 0)
                continue;

            listEntity = driver::read_memory<uintptr_t>(driver, Entity + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
            if (listEntity == 0)
                continue;

            uintptr_t entityPawn = driver::read_memory<uintptr_t>(driver, listEntity + (120) * (entityControllerPawn & 0x1FF));
            if (entityPawn == 0)
                continue;

            //Entity Values
            int playerTeam = driver::read_memory<int>(driver, entityPawn + C_BaseEntity::m_iTeamNum);
            int playerHealth = driver::read_memory<int>(driver, entityPawn + C_BaseEntity::m_iHealth);
            int playerArmor = driver::read_memory<int>(driver, entityPawn + C_CSPlayerPawnBase::m_ArmorValue);

            if (playerHealth <= 1 || playerHealth > 100)
                continue;

            if (entityPawn == LocalPlayerPawn)
                continue;

            //Rendering

            //Entity Feet
            Vector3 origin = driver::read_memory<Vector3>(driver, entityPawn + CGameSceneNode::m_vecOrigin);

            //Bones Pointer
            uint64_t gameScene = driver::read_memory<uint64_t>(driver, entityPawn + C_BaseEntity::m_pGameSceneNode);

            //Bones
            uint64_t boneArray = driver::read_memory<uint64_t>(driver, gameScene + CSkeletonInstance::m_modelState + 0x80);
            Vector3 playerHead = driver::read_memory<Vector3>(driver, boneArray + 6 * 32);
            Vector3 playerNeck = driver::read_memory<Vector3>(driver, boneArray + 5 * 32);
            Vector3 playerShoulderL = driver::read_memory<Vector3>(driver, boneArray + 13 * 32);
            Vector3 playerShoulderR = driver::read_memory<Vector3>(driver, boneArray + 8 * 32);
            Vector3 playerForeL = driver::read_memory<Vector3>(driver, boneArray + 14 * 32);
            Vector3 playerForeR = driver::read_memory<Vector3>(driver, boneArray + 9 * 32);
            Vector3 playerHandL = driver::read_memory<Vector3>(driver, boneArray + 16 * 32);
            Vector3 playerHandR = driver::read_memory<Vector3>(driver, boneArray + 11 * 32);
            Vector3 playerWaist = driver::read_memory<Vector3>(driver, boneArray + 0 * 32);
            Vector3 playerKneeL = driver::read_memory<Vector3>(driver, boneArray + 26 * 32);
            Vector3 playerKneeR = driver::read_memory<Vector3>(driver, boneArray + 23 * 32);
            Vector3 playerFeetL = driver::read_memory<Vector3>(driver, boneArray + 27 * 32);
            Vector3 playerFeetR = driver::read_memory<Vector3>(driver, boneArray + 24 * 32);


            //ScreenHead
            Vector3 head = { origin.x, origin.y, origin.z + 75.f };

            //WorldToScreen Calls
            Vector3 screenPos = origin.WorldtoScreen(viewMatrix);
            Vector3 screenHead = head.WorldtoScreen(viewMatrix);
            Vector3 boneHead = playerHead.WorldtoScreen(viewMatrix);
            Vector3 boneNeck = playerNeck.WorldtoScreen(viewMatrix);
            Vector3 boneShoulderL = playerShoulderL.WorldtoScreen(viewMatrix);
            Vector3 boneShoulderR = playerShoulderR.WorldtoScreen(viewMatrix);
            Vector3 boneForeL = playerForeL.WorldtoScreen(viewMatrix);
            Vector3 boneForeR = playerForeR.WorldtoScreen(viewMatrix);
            Vector3 boneHandL = playerHandL.WorldtoScreen(viewMatrix);
            Vector3 boneHandR = playerHandR.WorldtoScreen(viewMatrix);
            Vector3 boneWaist = playerWaist.WorldtoScreen(viewMatrix);
            Vector3 boneKneeL = playerKneeL.WorldtoScreen(viewMatrix);
            Vector3 boneKneeR = playerKneeR.WorldtoScreen(viewMatrix);
            Vector3 boneFeetL = playerFeetL.WorldtoScreen(viewMatrix);
            Vector3 boneFeetR = playerFeetR.WorldtoScreen(viewMatrix);

            //Define
            float height = abs(screenPos.y - screenHead.y);
            float width = height / 2.4f;

            //Esp boxes

            //2d
            if (playerTeam == myTeam && espBoxT == true && TeamActiveCombo == 0)
                Render::DrawRect(screenPos.x - width / 2, screenHead.y, width, height, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5);
            else if (playerTeam != myTeam && espBoxE == true && EnemyActiveCombo == 0)
                Render::DrawRect(screenPos.x - width / 2, screenHead.y, width, height, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5);

            //3d


            //Corners
            if (playerTeam == myTeam && espBoxT == true && TeamActiveCombo == 2)
            {
                //Top Top
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenHead.y, screenHead.x - width / 6, screenHead.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);
                //Right
                Render::DrawLine(screenPos.x + width / 6, screenHead.y, screenPos.x + width / 3, screenHead.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);

                //Bottom Bottom
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenPos.y, screenHead.x - width / 6, screenPos.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);
                //Right
                Render::DrawLine(screenPos.x + width / 6, screenPos.y, screenPos.x + width / 3, screenPos.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);

                //Top Bottom
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenHead.y + height / 10, screenHead.x - width / 3, screenHead.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);
                //Right
                Render::DrawLine(screenPos.x + width / 3, screenHead.y + height / 10, screenPos.x + width / 3, screenHead.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);

                //Bottom Top
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenPos.y - height / 10, screenHead.x - width / 3, screenPos.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);

                Render::DrawLine(screenPos.x + width / 3, screenPos.y - height / 10, screenPos.x + width / 3, screenPos.y, ImColor(TeamFrameColor[0], TeamFrameColor[1], TeamFrameColor[2]), 1.5f);
            }
            else if (playerTeam != myTeam && espBoxE == true && EnemyActiveCombo == 2)
            {
                //Top Top
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenHead.y, screenHead.x - width / 6, screenHead.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);
                //Right
                Render::DrawLine(screenPos.x + width / 6, screenHead.y, screenPos.x + width / 3, screenHead.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);

                //Bottom Bottom
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenPos.y, screenHead.x - width / 6, screenPos.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);
                //Right
                Render::DrawLine(screenPos.x + width / 6, screenPos.y, screenPos.x + width / 3, screenPos.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);

                //Top Bottom
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenHead.y + height / 10, screenHead.x - width / 3, screenHead.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);
                //Right
                Render::DrawLine(screenPos.x + width / 3, screenHead.y + height / 10, screenPos.x + width / 3, screenHead.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);

                //Bottom Top
                //Left
                Render::DrawLine(screenHead.x - width / 3, screenPos.y - height / 10, screenHead.x - width / 3, screenPos.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);
                //Right
                Render::DrawLine(screenPos.x + width / 3, screenPos.y - height / 10, screenPos.x + width / 3, screenPos.y, ImColor(EnemyFrameColor[0], EnemyFrameColor[1], EnemyFrameColor[2]), 1.5f);
            }

            //Esp Head Circle
            if (playerTeam == myTeam && espHeadT == true)
                Render::DrawHeadCircle(boneHead.x, boneHead.y, width / 7.f, ImColor(1.f, 1.f, 1.f), 1.5f);
            else if (playerTeam != myTeam && espHeadE == true)
                Render::DrawHeadCircle(boneHead.x, boneHead.y, width / 7.f, ImColor(1.f, 1.f, 1.f), 1.5f);

            //Esp Skelton
            if (playerTeam == myTeam && espSkeltonT == true)
            {
                Render::DrawLine(boneNeck.x, boneNeck.y, boneHead.x, boneHead.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneNeck.x, boneNeck.y, boneShoulderR.x, boneShoulderR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneNeck.x, boneNeck.y, boneShoulderL.x, boneShoulderL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneShoulderL.x, boneShoulderL.y, boneForeL.x, boneForeL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneShoulderR.x, boneShoulderR.y, boneForeR.x, boneForeR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneForeL.x, boneForeL.y, boneHandL.x, boneHandL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneForeR.x, boneForeR.y, boneHandR.x, boneHandR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneNeck.x, boneNeck.y, boneWaist.x, boneWaist.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneWaist.x, boneWaist.y, boneKneeL.x, boneKneeL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneWaist.x, boneWaist.y, boneKneeR.x, boneKneeR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneKneeL.x, boneKneeL.y, boneFeetL.x, boneFeetL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneKneeR.x, boneKneeR.y, boneFeetR.x, boneFeetR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
            }
            else if (playerTeam != myTeam && espSkeltonE == true)
            {
                Render::DrawLine(boneNeck.x, boneNeck.y, boneHead.x, boneHead.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneNeck.x, boneNeck.y, boneShoulderR.x, boneShoulderR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneNeck.x, boneNeck.y, boneShoulderL.x, boneShoulderL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneShoulderL.x, boneShoulderL.y, boneForeL.x, boneForeL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneShoulderR.x, boneShoulderR.y, boneForeR.x, boneForeR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneForeL.x, boneForeL.y, boneHandL.x, boneHandL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneForeR.x, boneForeR.y, boneHandR.x, boneHandR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneNeck.x, boneNeck.y, boneWaist.x, boneWaist.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneWaist.x, boneWaist.y, boneKneeL.x, boneKneeL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneWaist.x, boneWaist.y, boneKneeR.x, boneKneeR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneKneeL.x, boneKneeL.y, boneFeetL.x, boneFeetL.y, ImColor(1.f, 1.f, 1.f), 1.5f);
                Render::DrawLine(boneKneeR.x, boneKneeR.y, boneFeetR.x, boneFeetR.y, ImColor(1.f, 1.f, 1.f), 1.5f);
            }

            //Health Bar If(s)
            if (playerTeam == myTeam && espHealthBarT == true)
            {
                if (playerHealth <= 100 && playerHealth >= 76)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, width, height / 60, ImColor(0.f, 1.f, 0.f));

                if (playerHealth <= 75 && playerHealth >= 51)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 1.5f), height / 60, ImColor(255.f, 255.f, 0.f));

                if (playerHealth <= 50 && playerHealth >= 26)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 2.5f), height / 60, ImColor(255, 127, 80));

                if (playerHealth <= 25 && playerHealth >= 1)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 4.5f), height / 60, ImColor(1.f, 0.f, 0.f));
            }
            else if (playerTeam != myTeam && espHealthBarE == true)
            {
                if (playerHealth <= 100 && playerHealth >= 76)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, width, height / 60, ImColor(0.f, 1.f, 0.f));

                if (playerHealth <= 75 && playerHealth >= 51)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 1.5f), height / 60, ImColor(255.f, 255.f, 0.f));

                if (playerHealth <= 50 && playerHealth >= 26)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 2.5f), height / 60, ImColor(255, 127, 80));

                if (playerHealth <= 25 && playerHealth >= 1)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 4.5f), height / 60, ImColor(1.f, 0.f, 0.f));
            }
        }
    }
    void fovJChanger(const int ProcessId, uintptr_t Client, uintptr_t Engine) {
        const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        const DWORD pid = ProcessId;
        const std::uintptr_t client = Client;
        const std::uintptr_t engine = Engine;
        const auto local_player_cont = driver::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerController);
        static int fovchik;
        static bool bToggled = false;
        if (GetAsyncKeyState('J') && !bToggled) {
            if (fovchik == 120) {
                fovchik = 20;
            }
            else if (fovchik != 120) {
                fovchik = 120;
            }
            bToggled = true;
        }
        else if (!GetAsyncKeyState('J')) {
            bToggled = false;
        }
        driver::write_memory(driver, local_player_cont + CBasePlayerController::m_iDesiredFOV, fovchik);
    }

    void Bhoppin(const int ProcessId, uintptr_t Client, uintptr_t Engine) {
        if (Bhop == true)
        { 
        
        const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        const DWORD pid = ProcessId;
        const std::uintptr_t client = Client;
        const std::uintptr_t engine = Engine;
        const auto local_player_pawn = driver::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
        const auto flags = driver::read_memory<std::uint32_t>(driver, local_player_pawn + C_BaseEntity::m_fFlags);
        const bool in_air = flags & (1 << 0);
        const bool space_pressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
        const auto force_jump = driver::read_memory<DWORD>(driver, client + client_dll::dwForceJump);
        if (space_pressed && in_air) {
            Sleep(16);
            driver::write_memory(driver, client + client_dll::dwForceJump, 65537);
        }
        else if (space_pressed && !in_air) {
            driver::write_memory(driver, client + client_dll::dwForceJump, 256);

        }
        else if (!space_pressed && force_jump == 65537) {
            driver::write_memory(driver, client + client_dll::dwForceJump, 256);
        }
    }
    }

    void AntiFlash(const int ProcessId, uintptr_t Client, uintptr_t Engine) {
        const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        const DWORD pid = ProcessId;
        const std::uintptr_t client = Client;
        const std::uintptr_t engine = Engine;
        const auto local_player_pawn = driver::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
        driver::write_memory(driver, local_player_pawn + C_CSPlayerPawnBase::m_flFlashDuration, 0.f);
    }

    
}


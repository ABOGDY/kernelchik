#pragma once
#include "Vector.hpp"
#include "Render.hpp"
#include "offsets/offsets.hpp"
#include "offsets/client.dll.hpp"
#include "../ImGui/imgui.h"
#include "Global.hpp"
#include <Windows.h>
#include <stdio.h>

struct BoneJointData
{
    Vector3 Pos;
    char pad[0x14];
};
struct BoneJointPos
{
    Vector3 Pos;
    Vector2 ScreenPos;
    bool IsVisible = false;
};

namespace Cheats
{
    void espLoop() //const int ProcessId, uintptr_t Client, uintptr_t Engine
    {
        
        //const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        //const DWORD pid = ProcessId;
        //const std::uintptr_t client = Client;
        // const std::uintptr_t engine = Engine;
        //auto cameraServices = drivermem::read_memory<std::uintptr_t>(driver, local_player_pawn + C_BasePlayerPawn::m_pCameraServices);
        //const auto currentFOV = drivermem::read_memory<uint32_t>(driver, cameraServices + CCSPlayerBase_CameraServices::m_iFOV);
        uintptr_t LocalPlayerPawn = drivermem::read_memory<uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
        view_matrix_t viewMatrix = drivermem::read_memory<view_matrix_t>(driver, client + client_dll::dwViewMatrix);
        
        int myHealth = drivermem::read_memory<int>(driver, LocalPlayerPawn + C_BaseEntity::m_iHealth);
        int myTeam = drivermem::read_memory<int>(driver, LocalPlayerPawn + C_BaseEntity::m_iTeamNum);

        uintptr_t Entity = drivermem::read_memory<uintptr_t>(driver, client + client_dll::dwEntityList);
        static Vector3 oldangl;
        Vector3 nangle = drivermem::read_memory<Vector3>(driver, client + client_dll::dwViewAngles);

        float multplr = 1.4;
        //static ImVec2 oldmpos;
        float diffanglradx = (oldangl.z - nangle.z) * multplr;
        float diffanglrady = (oldangl.y - nangle.y) * multplr;
        //float diffanglradx = (mousemov.x) * (180.0 / M_PI) * multplr;
        //float diffanglrady = (mousemov.y) * (180.0 / M_PI) * multplr;

        Vector2 diffangl = Vector2(0, 0);//-diffanglradx, -diffanglrady); //
<<<<<<< HEAD
        //char tstline1[40];
        //char tstline2[40];
        //char tstline3[40];
        //sprintf_s(tstline1, "%f", diffanglradx);
        //sprintf_s(tstline2, "%f", diffanglrady);
        //sprintf_s(tstline3, "%f", nangle.z);
        //Render::DrawTextz(200, 200, ImColor(1.f, 0.f, 0.f, 1.f), tstline1);
        //Render::DrawTextz(200, 190, ImColor(1.f, 0.f, 0.f, 1.f), tstline2);
=======
        char tstline1[40];
        char tstline2[40];
        //char tstline3[40];
        sprintf_s(tstline1, "%f", diffanglradx);
        sprintf_s(tstline2, "%f", diffanglrady);
        //sprintf_s(tstline3, "%f", nangle.z);
        Render::DrawTextz(200, 200, ImColor(1.f, 0.f, 0.f, 1.f), tstline1);
        Render::DrawTextz(200, 190, ImColor(1.f, 0.f, 0.f, 1.f), tstline2);
>>>>>>> 2895b3fe963cf7ffe88b60b3caf9070c168a5f08
        //Render::DrawTextz(200, 180, ImColor(1.f, 0.f, 0.f, 1.f), tstline3);
        for (int i = 1; i < 64; i++)
        {
            uintptr_t listEntity = drivermem::read_memory<uintptr_t>(driver, Entity + ((8 * (i & 0x7FFF) >> 9) + 16));
            if (listEntity == 0)
                continue;

            uintptr_t entityController = drivermem::read_memory<uintptr_t>(driver, listEntity + (120) * (i & 0x1FF));
            if (entityController == 0)
                continue;

            uintptr_t entityControllerPawn = drivermem::read_memory<uintptr_t>(driver, entityController + CCSPlayerController::m_hPlayerPawn);
            if (entityControllerPawn == 0)
                continue;

            listEntity = drivermem::read_memory<uintptr_t>(driver, Entity + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 16));
            if (listEntity == 0)
                continue;

            uintptr_t entityPawn = drivermem::read_memory<uintptr_t>(driver, listEntity + (120) * (entityControllerPawn & 0x1FF));
            if (entityPawn == 0)
                continue;

            //Entity Values
            int playerTeam = drivermem::read_memory<int>(driver, entityPawn + C_BaseEntity::m_iTeamNum);
            int playerHealth = drivermem::read_memory<int>(driver, entityPawn + C_BaseEntity::m_iHealth);
            int playerArmor = drivermem::read_memory<int>(driver, entityPawn + C_CSPlayerPawnBase::m_ArmorValue);
            
            
            if (playerHealth <= 1 || playerHealth > 100)
                continue;

            if (entityPawn == LocalPlayerPawn)
                continue;

            //Rendering

            //Bones Pointer
            uint64_t gameScene = drivermem::read_memory<uint64_t>(driver, entityPawn + C_BaseEntity::m_pGameSceneNode);

            //Entity Feet
            Vector3 origin = drivermem::read_memory<Vector3>(driver, gameScene + CGameSceneNode::m_vecOrigin); 

            //Bones
            uint64_t boneArray = drivermem::read_memory<uint64_t>(driver, gameScene + CSkeletonInstance::m_modelState + 0x80);
            Vector3 playerHead = drivermem::read_memory<Vector3>(driver, boneArray + 6 * 32);
            Vector3 playerNeck = drivermem::read_memory<Vector3>(driver, boneArray + 5 * 32);
            Vector3 playerShoulderL = drivermem::read_memory<Vector3>(driver, boneArray + 13 * 32);
            Vector3 playerShoulderR = drivermem::read_memory<Vector3>(driver, boneArray + 8 * 32);
            Vector3 playerForeL = drivermem::read_memory<Vector3>(driver, boneArray + 14 * 32);
            Vector3 playerForeR = drivermem::read_memory<Vector3>(driver, boneArray + 9 * 32);
            Vector3 playerHandL = drivermem::read_memory<Vector3>(driver, boneArray + 16 * 32);
            Vector3 playerHandR = drivermem::read_memory<Vector3>(driver, boneArray + 11 * 32);
            Vector3 playerWaist = drivermem::read_memory<Vector3>(driver, boneArray + 0 * 32);
            Vector3 playerKneeL = drivermem::read_memory<Vector3>(driver, boneArray + 26 * 32);
            Vector3 playerKneeR = drivermem::read_memory<Vector3>(driver, boneArray + 23 * 32);
            Vector3 playerFeetL = drivermem::read_memory<Vector3>(driver, boneArray + 27 * 32);
            Vector3 playerFeetR = drivermem::read_memory<Vector3>(driver, boneArray + 24 * 32);

            //sadasd
            //ScreenHead
            Vector3 head = { origin.x, origin.y, origin.z + 75.f };
            
            
            //WorldToScreen Calls
            Vector3 screenPos = origin.WorldtoScreen(viewMatrix) + Vector3(diffangl.y, diffangl.x , 0.f);
            Vector3 screenHead = head.WorldtoScreen(viewMatrix) + Vector3(diffangl.y, diffangl.x , 0.f);
            Vector3 boneHead = playerHead.WorldtoScreen(viewMatrix) + Vector3(diffangl.y, diffangl.x , 0.f);
            Vector3 boneNeck = playerNeck.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x ,0.f);
            Vector3 boneShoulderL = playerShoulderL.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneShoulderR = playerShoulderR.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneForeL = playerForeL.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneForeR = playerForeR.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneHandL = playerHandL.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneHandR = playerHandR.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneWaist = playerWaist.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneKneeL = playerKneeL.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneKneeR = playerKneeR.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneFeetL = playerFeetL.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);
            Vector3 boneFeetR = playerFeetR.WorldtoScreen(viewMatrix) + Vector3(diffangl.y , diffangl.x , 0.f);

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
            struct CSEntity {
                char name[128];
            };
            char tstline1[128];
            CSEntity enmname = drivermem::read_memory<CSEntity>(driver, entityController + CBasePlayerController::m_iszPlayerName);
            sprintf_s(tstline1, "%s", enmname);
            if (playerTeam == myTeam && espHeadT == true)
                Render::DrawTextz(screenHead.x, screenHead.y-10, ImColor(1.f, 1.f, 1.f, 1.f), tstline1);
            else if (playerTeam != myTeam && espHeadE == true)
                Render::DrawTextz(screenHead.x, screenHead.y-10, ImColor(1.f, 1.f, 1.f, 1.f), tstline1);

            //float Yaw = atan2f(OppPos.y, OppPos.x) * 57.295779513 - Local.Pawn.ViewAngle.y;
            //float Pitch = -atan(OppPos.z / Distance) * 57.295779513 - Local.Pawn.ViewAngle.x;
            //Esp Skelton
            //char tstline1[40];
            //char tstline2[40];
            //sprintf_s(tstline1, "%d", diffangl.y);
            //sprintf_s(tstline2, "%d", diffangl.x);
            //Render::DrawTextz(200, 200, ImColor(1.f, 0.f, 0.f, 1.f), tstline1);
            //Render::DrawTextz(200, 180, ImColor(1.f, 0.f, 0.f, 1.f), tstline2);
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
            int heightmult = height / 30;
            if (heightmult <= 0) {
                heightmult = 1;
            }
            else if (heightmult >= 40) {
                heightmult = 40;
            }
            if (playerTeam == myTeam && espHealthBarT == true)
            {
                if (playerHealth <= 100 && playerHealth >= 76)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, width, heightmult, ImColor(0.f, 1.f, 0.f));

                if (playerHealth <= 75 && playerHealth >= 51)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 1.5f), heightmult, ImColor(255.f, 255.f, 0.f));

                if (playerHealth <= 50 && playerHealth >= 26)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 2.5f), heightmult, ImColor(255, 127, 80));

                if (playerHealth <= 25 && playerHealth >= 1)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 4.5f), heightmult, ImColor(1.f, 0.f, 0.f));
            }
            else if (playerTeam != myTeam && espHealthBarE == true)
            {
                if (playerHealth <= 100 && playerHealth >= 76)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, width, heightmult, ImColor(0.f, 1.f, 0.f));

                if (playerHealth <= 75 && playerHealth >= 51)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 1.5f), heightmult, ImColor(255.f, 255.f, 0.f));

                if (playerHealth <= 50 && playerHealth >= 26)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 2.5f), heightmult, ImColor(255, 127, 80));

                if (playerHealth <= 25 && playerHealth >= 1)
                    Render::DrawHealhBar(screenPos.x - width / 2, screenPos.y, (width / 4.5f), heightmult, ImColor(1.f, 0.f, 0.f));
            }
            //if (tmr <= 0) {
            //    tmr = 10;
            //}
            //else if (tmr > 0) {
            //    tmr -= 1;
            //}
            //oldmpos.x = mousemov;
        }

        oldangl = nangle;
        //Sleep(60);
    }
    
    void fovJChanger() {
        //const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        //const DWORD pid = ProcessId;
        //const std::uintptr_t client = Client;
        //const std::uintptr_t engine = Engine;
        const auto local_player_cont = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerController);
        static bool bToggled = false;
<<<<<<< HEAD
        drivermem::write_memory(driver, local_player_cont + CBasePlayerController::m_iDesiredFOV, FOV);
        if (FovButton == false) {
            FOV = 90;
        }
        
        //const auto local_player_pawn = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
        //static bool bToggled = false;

        //auto cameraServices = drivermem::read_memory<std::uintptr_t>(driver, local_player_pawn + C_BasePlayerPawn::m_pCameraServices);
        //const auto currentFOV = drivermem::read_memory<uint32_t>(driver, cameraServices + CCSPlayerBase_CameraServices::m_iFOV);

        //bool isScoped = drivermem::read_memory<bool>(driver, local_player_pawn + C_CSPlayerPawnBase::m_bIsScoped);
            //if (!isScoped && currentFOV != FOV) {
            //    drivermem::write_memory(driver, cameraServices + CCSPlayerBase_CameraServices::m_iFOV, FOV);
            //}
=======
        //drivermem::write_memory(driver, local_player_cont + CBasePlayerController::m_iDesiredFOV, FOV);
        //if (FovButton == false) {
        //    FOV = 90;
        //}
        
        const auto local_player_pawn = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
        //static bool bToggled = false;

        auto cameraServices = drivermem::read_memory<std::uintptr_t>(driver, local_player_pawn + C_BasePlayerPawn::m_pCameraServices);
        const auto currentFOV = drivermem::read_memory<uint32_t>(driver, cameraServices + CCSPlayerBase_CameraServices::m_iFOV);

        bool isScoped = drivermem::read_memory<bool>(driver, local_player_pawn + C_CSPlayerPawnBase::m_bIsScoped);
            if (!isScoped && currentFOV != FOV) {
                drivermem::write_memory(driver, cameraServices + CCSPlayerBase_CameraServices::m_iFOV, FOV);
            }
>>>>>>> 2895b3fe963cf7ffe88b60b3caf9070c168a5f08
        
    }

    void Bhoppin() { //const int ProcessId, uintptr_t Client, uintptr_t Engine
        if (Bhopbl == true)
        {
            static bool lockon = false;
            static int tmr = -4;
            static int stmr = -4;
            static int oldstmr = -4;
            static int jmpstr = 256;
            static int oldjstr = 256;
            static int counter = 0;
            static int fjstr = 256;
            static int oldfjstr = 256;
            //const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            //const DWORD pid = ProcessId;
            //const std::uintptr_t client = Client;
            //const std::uintptr_t engine = Engine;
            const auto local_player_pawn = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
            const auto flags = drivermem::read_memory<std::uint32_t>(driver, local_player_pawn + C_BaseEntity::m_fFlags);
            const bool in_air = flags & (1 << 0);
            const bool space_pressed = GetAsyncKeyState(VK_SPACE) & 0x8000;
            const auto force_jump = drivermem::read_memory<DWORD>(driver, client + client_dll::dwForceJump);
<<<<<<< HEAD
            const bool legacy = true;
=======
            const bool legacy = false;
>>>>>>> 2895b3fe963cf7ffe88b60b3caf9070c168a5f08

            if (legacy == true) {
                if (space_pressed && in_air) {
                    Sleep(16);
                    drivermem::write_memory(driver, client + client_dll::dwForceJump, 65537);
                }
                else if (space_pressed && !in_air) {
                    drivermem::write_memory(driver, client + client_dll::dwForceJump, 256);
                    drivermem::write_memory(driver, client + client_dll::dwForceJump, 65537);
                    drivermem::write_memory(driver, client + client_dll::dwForceJump, 256);
                }
                else if (!space_pressed && force_jump == 65537) {
                    drivermem::write_memory(driver, client + client_dll::dwForceJump, 256);
                }
                else if (space_pressed && in_air && force_jump == 65537) {
                    drivermem::write_memory(driver, client + client_dll::dwForceJump, 256);
                }
                //C_BasePlayerPawn::m_pMovementServices CPlayer_MovementServices::m_flLeftMove CCSPlayer_MovementServices::m_vecLeft
            }   //CPlayer_MovementServices::m_vecLastMovementImpulses CPlayer_MovementServices::m_pButtonPressedCmdNumber
            else if (legacy == false) {
                const auto local_player_pawn = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
                const auto mservices = drivermem::read_memory<std::uintptr_t>(driver, local_player_pawn + C_BasePlayerPawn::m_pMovementServices);
                const auto lvec = drivermem::read_memory<Vector3>(driver, mservices + CPlayer_MovementServices::m_vecLastMovementImpulses);//CPlayer_MovementServices::m_flLeftMove);
                const auto buttoncmd = drivermem::read_memory<uint32_t>(driver, mservices + CPlayer_MovementServices::m_pButtonPressedCmdNumber);
                const auto mvecqang = drivermem::read_memory<Vector3>(driver, local_player_pawn + C_BasePlayerPawn::m_vOldOrigin);
                const auto plcontroller = drivermem::read_memory<std::uintptr_t>(driver, local_player_pawn + C_BasePlayerPawn::m_hController);
                const auto plname = drivermem::read_memory<char>(driver, plcontroller + CBasePlayerController::m_iszPlayerName);
                //C_BasePlayerPawn::m_hController CBasePlayerController::m_iszPlayerName
                if (space_pressed) {
                    if (!in_air) {
                        counter = 0;
                        jmpstr = 256;
                    }
                    if (in_air) {
                        if (oldjstr != jmpstr) {
                            if (jmpstr == 65537) {
                                if (counter > 3) {
                                    fjstr = jmpstr;
                                    oldjstr = jmpstr;
                                }
                                counter += 1;
                            }
                            else if (jmpstr == 256) {
                                if (counter > 3) {
                                    fjstr = jmpstr;
                                    oldjstr = jmpstr;
                                }
                                counter += 1;
                            }
                        }
                        else if (oldjstr == jmpstr) {
                            if (jmpstr == 65537) {
                                if (counter > 3) {
                                    jmpstr = 256;
                                    fjstr = jmpstr;
                                    oldjstr = jmpstr;
                                }
                                counter += 1;
                            }
                            else if (jmpstr == 256) {
                                if (counter > 3) {
                                    jmpstr = 65537;
                                    fjstr = jmpstr;
                                    oldjstr = jmpstr; 
                                }
                                counter += 1;
                            }
                        }

                    }
                
                if (oldfjstr != jmpstr) {
                    drivermem::write_memory(driver, client + client_dll::dwForceJump, fjstr);
                    oldfjstr = jmpstr;
                }
                }
                //drivermem::write_memory(driver, local_player_pawn + C_BasePlayerPawn::m_vOldOrigin, mvecqang + Vector3(0,1,0));
                
                
            }
        }
    }

    void AntiFlash() { 
        if (AntiFlasha == true) {
            const auto local_player_pawn = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
            drivermem::write_memory(driver, local_player_pawn + C_CSPlayerPawnBase::m_flFlashDuration, 0.f);
        }
    }

    void AimBot() { //const int ProcessId, uintptr_t Client, uintptr_t Engine
        enum BONEINDEX : DWORD
        {
            head = 6,
            neck_0 = 5,
            spine_1 = 4,
            spine_2 = 2,
            pelvis = 0,
            arm_upper_L = 8,
            arm_lower_L = 9,
            hand_L = 10,
            arm_upper_R = 13,
            arm_lower_R = 14,
            hand_R = 15,
            hand_R2 = 16,
            leg_upper_L = 22,
            leg_lower_L = 23,
            ankle_L = 24,
            leg_upper_R = 25,
            leg_lower_R = 26,
            ankle_R = 27,
        };
        enum cs_weapon_type : std::uint32_t
        {
            weapon_type_knife = 0,
            weapon_type_pistol = 1,
            weapon_type_submachinegun = 2,
            weapon_type_rifle = 3,
            weapon_type_shotgun = 4,
            weapon_type_sniper_rifle = 5,
            weapon_type_machinegun = 6,
            weapon_type_c4 = 7,
            weapon_type_taser = 8,
            weapon_type_grenade = 9,
            weapon_type_equipment = 10,
            weapon_type_stackable_item = 11,
            weapon_type_fists = 12,
            weapon_type_breach_charge = 13,
            weapon_type_bump_mine = 14,
            weapon_type_tablet = 15,
            weapon_type_melee = 16,
            weapon_type_shield = 17,
            weapon_type_zone_repulsor = 18,
            weapon_type_unknown = 19,
        };
        enum weaponslist {
            nothing = 0,
            deagle = 1,
            dualberretas = 2,
            fiveseven = 3,
            glock = 4,
            ak47 = 7,
            aug = 8,
            awp = 9,
            famas = 10,
            g3sg1 = 11,
            galil = 13,
            m248 = 14,
            m4a4 = 16,
            mac9 = 17,
            p90 = 19,
            mp5sd = 23,
            ump = 24,
            xm = 25,
            ppbizon = 26,
            mag7 = 27,
            negev = 28,
            savedoff = 29,
            tec = 30,
            zeus = 31,
            p2000 = 32,
            mp7 = 33,
            mp9 = 34,
            nova = 35,
            scar = 38,
            sg553 = 39,
            ssg09 = 40,
            ctknife = 42,
            flashbang = 43,
            he = 44,
            smoke = 45,
            moly = 46,
            decoy = 47,
            incgr = 48,
            bomba = 49,
            knife = 59,
            m4a1s = 60,
            cz75 = 63,
            revolver = 64,
        };
        //trigger
        closestvectrx = 0;
        //const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        //const DWORD pid = ProcessId;
        //const std::uintptr_t client = Client;
        //const std::uintptr_t engine = Engine;

        uintptr_t LocalPlayerPawn = drivermem::read_memory<uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
        uintptr_t LocalPlayerCont = drivermem::read_memory<uintptr_t>(driver, client + client_dll::dwLocalPlayerController);
        uintptr_t Entity = drivermem::read_memory<uintptr_t>(driver, client + client_dll::dwEntityList);
        view_matrix_t viewMatrix = drivermem::read_memory<view_matrix_t>(driver, client + client_dll::dwViewMatrix);
        uint64_t plscene = drivermem::read_memory<uint64_t>(driver, LocalPlayerPawn + C_BaseEntity::m_pGameSceneNode);
        Vector3 locang = drivermem::read_memory<Vector3>(driver, client + client_dll::dwViewAngles);

        int myHealth = drivermem::read_memory<int>(driver, LocalPlayerPawn + C_BaseEntity::m_iHealth);
        int myTeam = drivermem::read_memory<int>(driver, LocalPlayerPawn + C_BaseEntity::m_iTeamNum);
        auto myWeapon = drivermem::read_memory<uintptr_t>(driver, LocalPlayerPawn + C_CSPlayerPawnBase::m_pClippingWeapon);
        auto wepindx = drivermem::read_memory<int>(driver, myWeapon + C_EconEntity::m_AttributeManager + C_AttributeContainer::m_Item + C_EconItemView::m_iItemDefinitionIndex);//+ C_BasePlayerPawn::m_pWeaponServices + CPlayer_WeaponServices::m_hActiveWeapon + C_EconEntity::m_AttributeManager + C_AttributeContainer::m_Item + C_EconItemView::m_iItemDefinitionIndex);
        float outputx = 0;
        float outputy = 0;
        float outputz = 0;

        if ((wepindx > 59 || wepindx < 42) && wepindx != 0)
        {
            //drivermem::write_memory(driver, client + client_dll::dwViewAngles, Vector3(locang.x, locang.y + 1, locang.z));
            Vector3 closestang = Vector3(0, 0, 0);
            for (int i = 1; i < 64; i++)
            {
                uintptr_t listEntity = drivermem::read_memory<uintptr_t>(driver, Entity + (0x8 * (i & 0x7FFF) >> 9) + 0x10);
                if (listEntity == 0)
                    continue;

                uintptr_t entityController = drivermem::read_memory<uintptr_t>(driver, listEntity + (120) * (i & 0x1FF));
                if (entityController == 0)
                    continue;

                uintptr_t entityControllerPawn = drivermem::read_memory<uintptr_t>(driver, entityController + CCSPlayerController::m_hPlayerPawn);
                if (entityControllerPawn == 0)
                    continue;

                listEntity = drivermem::read_memory<uintptr_t>(driver, Entity + (0x8 * ((entityControllerPawn & 0x7FFF) >> 9) + 0x10));
                if (listEntity == 0)
                    continue;

                uintptr_t entityPawn = drivermem::read_memory<uintptr_t>(driver, listEntity + (120) * (entityControllerPawn & 0x1FF));
                if (entityPawn == 0)
                    continue;

                //Entity Values
                int playerTeam = drivermem::read_memory<int>(driver, entityPawn + C_BaseEntity::m_iTeamNum);
                int playerHealth = drivermem::read_memory<int>(driver, entityPawn + C_BaseEntity::m_iHealth);
                int playerArmor = drivermem::read_memory<int>(driver, entityPawn + C_CSPlayerPawnBase::m_ArmorValue);

                if (playerHealth <= 1 || playerHealth > 100)
                    continue;

                if (entityPawn == LocalPlayerPawn)
                    continue;

                //Rendering


                //Bones Pointer
                uint64_t gameScene = drivermem::read_memory<uint64_t>(driver, entityPawn + C_BaseEntity::m_pGameSceneNode);

                //Entity Feet
                Vector3 origin = drivermem::read_memory<Vector3>(driver, gameScene + CGameSceneNode::m_vecOrigin);
                Vector3 plorig = drivermem::read_memory<Vector3>(driver, plscene + CGameSceneNode::m_vecOrigin);
                //Bones
                uint64_t boneArray = drivermem::read_memory<uint64_t>(driver, gameScene + CSkeletonInstance::m_modelState + 0x80);
                Vector3 playerHead = drivermem::read_memory<Vector3>(driver, boneArray + 6 * 32);
                Vector3 playerNeck = drivermem::read_memory<Vector3>(driver, boneArray + 5 * 32);
                Vector3 playerShoulderL = drivermem::read_memory<Vector3>(driver, boneArray + 13 * 32);
                Vector3 playerShoulderR = drivermem::read_memory<Vector3>(driver, boneArray + 8 * 32);
                Vector3 playerForeL = drivermem::read_memory<Vector3>(driver, boneArray + 14 * 32);
                Vector3 playerForeR = drivermem::read_memory<Vector3>(driver, boneArray + 9 * 32);
                Vector3 playerHandL = drivermem::read_memory<Vector3>(driver, boneArray + 16 * 32);
                Vector3 playerHandR = drivermem::read_memory<Vector3>(driver, boneArray + 11 * 32);
                Vector3 playerWaist = drivermem::read_memory<Vector3>(driver, boneArray + 0 * 32);
                Vector3 playerKneeL = drivermem::read_memory<Vector3>(driver, boneArray + 26 * 32);
                Vector3 playerKneeR = drivermem::read_memory<Vector3>(driver, boneArray + 23 * 32);
                Vector3 playerFeetL = drivermem::read_memory<Vector3>(driver, boneArray + 27 * 32);
                Vector3 playerFeetR = drivermem::read_memory<Vector3>(driver, boneArray + 24 * 32);

                if (playerTeam != myTeam) {
                    Vector3 boneHead = playerHead.WorldtoScreen(viewMatrix);
                    Vector3 diffvec3 = playerHead - plorig;
                    Vector2 eyesangle = Vector2(locang.y, locang.x);
                    float diffx = eyesangle.x;

                }
                //float diffx = std::atan2(diffvec3.x, diffvec3.y)* (180 / std::numbers::pi_v<float>);
                //float diffy = std::atan2(diffvec3.x, diffvec3.z) * (180 / M_PI);
                //if (abs(closestang.x) < abs(diffx)) {
                //    closestang.x = diffx;
                //    closestvectrx = closestang.x;
                //   }

            }

            //auto plorig = drivermem::read_memory<Vector3>(driver, plscene + CGameSceneNode::m_vRenderOrigin);
            //double truncatedx = (double)((int)plorig.x * 100) / 100;

            //drivermem::write_memory(driver, client + client_dll::dwViewAngles, locang + Vector3(0, closestang.x / 70, 0));
        }
        outputx = locang.x;
        outputy = locang.y;
        char tstline1[40];
        char tstline2[40];
        char tstline3[40];
        sprintf_s(tstline1, "%d", outputx);
        sprintf_s(tstline2, "%d", outputy);
        sprintf_s(tstline3, "%d", outputz);
        Render::DrawTextz(200, 200, ImColor(1.f, 0.f, 0.f, 1.f), tstline1);
        Render::DrawTextz(200, 180, ImColor(1.f, 0.f, 0.f, 1.f), tstline2);
        Render::DrawTextz(200, 160, ImColor(1.f, 0.f, 0.f, 1.f), tstline3);



    }

    

    void TriggerBot() {
        
        enum weaponslist {
            nothing = 0,
            deagle = 1,
            dualberretas = 2,
            fiveseven = 3,
            glock = 4,
            ak47 = 7,
            aug = 8,
            awp = 9,
            famas = 10,
            g3sg1 = 11,
            galil = 13,
            m248 = 14,
            m4a4 = 16,
            mac9 = 17,
            p90 = 19,
            mp5sd = 23,
            ump = 24,
            xm = 25,
            ppbizon = 26,
            mag7 = 27,
            negev = 28,
            savedoff = 29,
            tec = 30,
            zeus = 31,
            p2000 = 32,
            mp7 = 33,
            mp9 = 34,
            nova = 35,
            scar = 38,
            sg553 = 39,
            ssg09 = 40,
            ctknife = 42,
            flashbang = 43,
            he = 44,
            smoke = 45,
            moly = 46,
            decoy = 47,
            incgr = 48,
            bomba = 49,
            knife = 59,
            m4a1s = 60,
            cz75 = 63,
            revolver = 64,
        };
        if (GetAsyncKeyState(VK_XBUTTON2) && TriggerBotbl == true) {
            uintptr_t LocalPlayerPawn = drivermem::read_memory<uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
            static int tmr = -4;
            static int mrls = -4;
            const auto local_player_pawn = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwLocalPlayerPawn);
            const auto crosshair_ent = drivermem::read_memory<std::int32_t>(driver, local_player_pawn + C_CSPlayerPawnBase::m_iIDEntIndex); //C_CSPlayerPawnBase::m_iIDEntIndex
            auto myWeapon = drivermem::read_memory<uintptr_t>(driver, LocalPlayerPawn + C_CSPlayerPawnBase::m_pClippingWeapon);
            auto wepindx = drivermem::read_memory<int>(driver, myWeapon + C_EconEntity::m_AttributeManager + C_AttributeContainer::m_Item + C_EconItemView::m_iItemDefinitionIndex);
            char tstline1[40];
            sprintf_s(tstline1, "%d", myWeapon);
            Render::DrawTextz(200, 200, ImColor(1.f, 0.f, 0.f, 1.f), tstline1);
            char tstline[40];
            sprintf_s(tstline, "%d", wepindx);
            Render::DrawTextz(200, 240, ImColor(1.f, 0.f, 0.f, 1.f), tstline);
            const bool legacy = true;
            if (crosshair_ent > 0) {
                const auto entlist = drivermem::read_memory<std::uintptr_t>(driver, client + client_dll::dwEntityList);
                const auto entEntry = drivermem::read_memory<std::uintptr_t>(driver, entlist + 0x8 * (crosshair_ent >> 9) + 0x10);
                const auto entity = drivermem::read_memory<std::uintptr_t>(driver, entEntry + (0x78 * (crosshair_ent & 0x1FF)));
                const auto Entityteam = drivermem::read_memory<std::int32_t>(driver, entity + C_BaseEntity::m_iTeamNum);
                const auto Playerteam = drivermem::read_memory<std::int32_t>(driver, local_player_pawn + C_BaseEntity::m_iTeamNum);
                const auto entityHp = drivermem::read_memory<std::int32_t>(driver, entity + C_BaseEntity::m_iHealth);
                if (legacy == true) {
                    if (entityHp > 0 && Entityteam != Playerteam && tmr <= 0)
                    {
                        Sleep(5);
                        POINT p;
                        GetCursorPos(&p);
                        mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                        mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                        tmr = 100;
<<<<<<< HEAD
=======
                        break;

                    case 9:
                        mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                        mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                        tmr = 8040;
                        break;
                    case 64:
                        mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                        //Sleep(80);
                        //mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                        tmr = 200;
                        break;
                    default:
                        mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                        mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                        tmr = 100;
                        break;
>>>>>>> 2895b3fe963cf7ffe88b60b3caf9070c168a5f08
                    }
                }
                if (legacy == false) {
                    if (entityHp > 0 && Entityteam != Playerteam && tmr <= 0)
                    {
                        POINT p;
                        GetCursorPos(&p);
                        switch (wepindx)
                        {
                        case 1:
                            mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                            mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                            tmr = 180;
                            break;

                        case 4:
                            mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                            mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                            tmr = 100;
                            break;

                        case 9:
                            mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                            mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                            tmr = 840;
                            break;
                        case 40:
                            mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                            mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                            tmr = 240;
                            break;
                        case 64:
                            mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                            //Sleep(80);
                             //mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                            tmr = 200;
                            break;
                        default:
                            mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
                            mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
                            tmr = 100;
                            break;
                        }

                    }
                    else if (Entityteam == Playerteam || Entityteam > 3) {
                        POINT p;
                        GetCursorPos(&p);
                        tmr = -4;
                        mrls = -4;
                    }
                }
            
            }
            if (tmr > 0) {
                tmr -= 1;
            }
            if (mrls > 0) {
                POINT p;
                GetCursorPos(&p);
                mrls -= 1;
            }
            if (mrls<=0) {
                POINT p;
                GetCursorPos(&p);
                mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
            }

            
        }

    }
        
    

}


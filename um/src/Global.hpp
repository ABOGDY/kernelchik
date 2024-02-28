#pragma once

#include <Windows.h>
#include "Vector.hpp"
//Screen Globals
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
//Menu Globals
bool espBoxT;
bool espBoxE;
bool espSkeltonT;
bool espSkeltonE;
bool espHeadT;
bool espHeadE;
bool espHealthBarT;
bool espHealthBarE;
bool menu;
float TeamFrameColor[3];
float EnemyFrameColor[3];
int TeamActiveCombo;
int EnemyActiveCombo;
const char* ComboSelections[3] = { "2D Box", "3D Box", "Corners" };
bool Bhop; 
//Debug globals
int closestvectrx;
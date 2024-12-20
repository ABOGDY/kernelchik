#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include "Vector.hpp"
#include "offsets/offsets.hpp"
#include "../ImGui/imgui.h"

static DWORD get_process_id(const wchar_t* process_name) {
	DWORD process_id = 0;

	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snap_shot == INVALID_HANDLE_VALUE)
		return process_id;
	PROCESSENTRY32W entry = {};
	entry.dwSize = sizeof(decltype(entry));

	if (Process32FirstW(snap_shot, &entry) == TRUE) {
		if (_wcsicmp(process_name, entry.szExeFile) == 0)
			process_id = entry.th32ProcessID;
		else {
			while (Process32NextW(snap_shot, &entry) == TRUE) {
				if (_wcsicmp(process_name, entry.szExeFile) == 0) {
					process_id = entry.th32ProcessID;
					break;
				}
			}
		}
	}

	CloseHandle(snap_shot);

	return process_id;
}

static std::uintptr_t get_module_base(const DWORD pid, const wchar_t* module_name) {
	std::uintptr_t module_base = 0;
	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
	if (snap_shot == INVALID_HANDLE_VALUE)
		return module_base;
	MODULEENTRY32W entry = {};
	entry.dwSize = sizeof(decltype(entry));
	if (Module32FirstW(snap_shot, &entry) == TRUE) {
		if (wcsstr(module_name, entry.szModule) != nullptr)
			module_base = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
		else {
			while (Module32NextW(snap_shot, &entry) == TRUE) {
				if (wcsstr(module_name, entry.szModule) != nullptr) {
					module_base = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
					break;
				}
			}
		}
	}
	CloseHandle(snap_shot);
	return module_base;
}

namespace drivermem {
    namespace codes {
        constexpr ULONG attach = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        constexpr ULONG read = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        constexpr ULONG write = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);//69
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


//Screen Globals
static ImVec2 mousemov;
const HANDLE driver = CreateFile(L"\\\\.\\Kernelchik", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
const DWORD pid = get_process_id(L"cs2.exe");
const std::uintptr_t client = get_module_base(pid, "client.dll");
const std::uintptr_t engine = get_module_base(pid, "engine2.dll");
int screenWidth = drivermem::read_memory<uintptr_t>(driver, engine + engine2_dll::dwWindowWidth);; //GetSystemMetrics(SM_CXSCREEN)
int screenHeight = drivermem::read_memory<uintptr_t>(driver,engine + engine2_dll::dwWindowHeight);  //GetSystemMetrics(SM_CYSCREEN)

//Timers
int trigtmr = -4;

//Menu Globals
bool espBoxT;
bool espBoxE;
bool espSkeltonT;
bool espSkeltonE = true;
bool espHeadT;
bool espHeadE = true;
bool espHealthBarT;
bool espHealthBarE = true;
bool menu;
float TeamFrameColor[3];
float EnemyFrameColor[3];
int TeamActiveCombo;
int EnemyActiveCombo;
const char* ComboSelections[3] = { "2D Box", "3D Box", "Corners" };
bool Bhopbl = true; 
bool FovButton = false;
bool AntiFlasha = true;
int FOV = 90;
bool AimBotbl;
bool TriggerBotbl = true;

//Debug globals
int closestvectrx;

//radar
bool show_radar = false;
float radar_range = 150;
float radar_point_size_proportion = 1;
bool show_radar_crossline = false;
ImColor radar_crossline_color = ImColor(255, 255, 255, 255);
int radar_type = 2;
float proportion = 3300;
bool custom_radar = false;
float radar_bg_alpha = 0.1;
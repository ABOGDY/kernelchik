#define _USE_MATH_DEFINES
#include <math.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <windowsx.h>
#include <sstream>
#include <cstdio>
#include <d3d11.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <mmsystem.h>

#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"

#include "offsets/client.dll.hpp"
#include "offsets/offsets.hpp"
#include "Vector.hpp"
#include "Cheats.hpp"
#include "Render.hpp"
#pragma comment(lib, "Winmm.lib")
struct Vectorr3
{
	float x, y, z;
};
struct Vectorr2
{
	float x, y;
};
template <typename T> int sign(T val) {
	return (T(0) < val) - (val < T(0));
}
void simulateKeyPress(WORD key) {
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	SendInput(1, &input, sizeof(INPUT));
}
namespace Math {
	constexpr double pi = 3.1415926535897932; // pi

	constexpr double toRad(const double x) {
		return x * (pi / 180.0);
	}

	constexpr double toDeg(const double x) {
		return x * (180.0 / pi);
	}
};
Vector3 CalculateAngle(const Vector3& localPosition,const Vector3& enemyPosition,const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param))
	{
		return 0L;
	}

	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0L;
	}
	return DefWindowProc(window, message, w_param, l_param);
}


INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
	
	if (drivermem::attach_to_process(driver, pid) == true)
	{
		std::cout << "Attachment successfull \n";
		const std::uintptr_t client = get_module_base(pid, L"client.dll");
		const std::uintptr_t engine = get_module_base(pid, L"engine2.dll");

		if (client != 0 && engine != 0) {
			WNDCLASSEXA wc{};
			wc.cbSize = sizeof(WNDCLASSEXW);
			wc.style = CS_CLASSDC;
			wc.hInstance = instance;
			wc.lpfnWndProc = window_procedure;
			wc.lpszClassName = " ";

			RegisterClassExA(&wc);
			//const HWND Overlay = FindWindowA(NULL, "Counter-Strike 2");
			HWND windhndl = FindWindowA(NULL, "Counter-Strike 2");
			RECT rect;
			GetWindowRect(windhndl, &rect);
			//int positionX = rect.left;
			//int positionY = rect.top;
			const HWND Overlay = CreateWindowExA(WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED, wc.lpszClassName, " ", WS_POPUP, rect.left+8, rect.bottom - screenHeight-8, screenWidth, screenHeight, nullptr, nullptr, wc.hInstance, nullptr);

			SetLayeredWindowAttributes(Overlay, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
			//SetLayeredWindowAttributes(windhndl, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
			{
				RECT client_area1{};
				GetClientRect(Overlay, &client_area1);
				//GetClientRect(windhndl, &client_area1);
				RECT window_area1{};
				GetWindowRect(Overlay, &window_area1);
				//GetWindowRect(windhndl, &window_area1);

				POINT diff1{};
				ClientToScreen(Overlay, &diff1);
				//ClientToScreen(windhndl, &diff1);

				const MARGINS margins1
				{
					window_area1.left + (diff1.x - window_area1.left),
					window_area1.top + (diff1.y - window_area1.top),
					client_area1.right,
					client_area1.bottom
				};
				
				DwmExtendFrameIntoClientArea(Overlay, &margins1);
				//DwmExtendFrameIntoClientArea(windhndl, &margins1);
			}

			DXGI_SWAP_CHAIN_DESC overlayDesc{};
			overlayDesc.BufferDesc.RefreshRate.Numerator = 140U; //fps
			overlayDesc.BufferDesc.RefreshRate.Denominator = 1U;
			overlayDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			overlayDesc.SampleDesc.Count = 1U;
			overlayDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			overlayDesc.BufferCount = 2U;
			overlayDesc.OutputWindow = Overlay;
			//overlayDesc.OutputWindow = windhndl;
			overlayDesc.Windowed = TRUE;
			overlayDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			overlayDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			constexpr D3D_FEATURE_LEVEL levels[2]
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_0
			};

			ID3D11Device* device{ nullptr };
			ID3D11DeviceContext* device_context{ nullptr };
			IDXGISwapChain* swap_chain{ nullptr };
			ID3D11RenderTargetView* render_target_view{ nullptr };
			D3D_FEATURE_LEVEL level{};

			D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0U, levels, 2U, D3D11_SDK_VERSION, &overlayDesc, &swap_chain, &device, &level, &device_context);

			ID3D11Texture2D* back_buffer{ nullptr };
			swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

			if (back_buffer)
			{
				device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
				back_buffer->Release();
			}
			
			ShowWindow(Overlay, cmd_show);
			UpdateWindow(Overlay);
			//ShowWindow(windhndl, cmd_show);
			//UpdateWindow(windhndl);

			ImGui::CreateContext();
			ImGui::StyleColorsClassic();

			ImGui_ImplWin32_Init(Overlay);;
			//ImGui_ImplWin32_Init(windhndl);;
			ImGui_ImplDX11_Init(device, device_context);

			

			while (true) {
				
				MSG msg;
				while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				if (menu == true)
					SetWindowLong(Overlay, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOPMOST);
					//SetWindowLong(windhndl, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOPMOST);
				else
					SetWindowLong(Overlay, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED);
					//SetWindowLong(windhndl, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_LAYERED);

				if (GetAsyncKeyState(VK_INSERT) < 0)
				{
					if (menu == false) {
						menu = true;
						PlaySoundA((LPCSTR)"C:\\hitsound.wav", NULL, SND_FILENAME | SND_ASYNC);
					}
					else {
						menu = false;
					}
					Sleep(150);
				}

				Menus::DrawInGameMenu();
				Cheats::espLoop();
				Cheats::Bhoppin();
				Cheats::AntiFlash();
				//Cheats::AimBot();
				Cheats::TriggerBot();
				Cheats::fovJChanger();
				Cheats::AntiFlash();
				ImGui::Render();

				constexpr float color[4](0.f, 0.f, 0.f, 0.f);
				device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
				device_context->ClearRenderTargetView(render_target_view, color);

				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				swap_chain->Present(0U, 0U);
				if (GetAsyncKeyState(VK_END)) {
					return 0;
				}
			}

			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();

			ImGui::DestroyContext();

			if (swap_chain)
			{
				swap_chain->Release();
			}
			if (device_context)
			{
				device_context->Release();
			}
			if (device)
			{
				device->Release();
			}
			if (render_target_view)
			{
				render_target_view->Release();
			}

			DestroyWindow(Overlay);
			UnregisterClassA(wc.lpszClassName, wc.hInstance);
		}
	}



	CloseHandle(driver);

	std::cin.get();

	return 0;
}


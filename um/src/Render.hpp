#pragma once
#include "../ImGui/imgui.h"
#include "Global.hpp"
#include <string>
#include <vector>
namespace Render
{
	void DrawRect(int x, int y, int w, int h, ImColor color, int thickness)
	{
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
	}

	void DrawHeadCircle(int x, int y, float radius, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x, y), radius, color, 0, thickness);
	}

	void DrawHealhBar(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
	}

	void DrawLine(int x1, int y1, int x2, int y2, ImColor color, float thickness)
	{
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
	}

	void DrawTextz(int x, int y, ImColor color, const char* text)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), color, text, NULL);
	}


	void AddPoint(const Vector3& LocalPos, const float& LocalYaw, const Vector3& Pos, ImColor Color, int Type, float Yaw)
	{
		Vector2 PointPos;
		float Distance;
		float Angle;

		Base_Radar::LocalYaw = LocalYaw;

		Distance = sqrt(pow(LocalPos.x - Pos.x, 2) + pow(LocalPos.y - Pos.y, 2));

		Angle = atan2(Pos.y - LocalPos.y, Pos.x - LocalPos.x) * 180 / M_PI;
		Angle = (Base_Radar::LocalYaw - Angle) * M_PI / 180;

		Distance = Distance / Base_Radar::Proportion * Base_Radar::RenderRange * 2;

		PointPos.x = Base_Radar::Pos.x + Distance * sin(Angle);
		PointPos.y = Base_Radar::Pos.y - Distance * cos(Angle);

		// Circle range
		//Distance = sqrt(pow(this->Pos.x - PointPos.x, 2) + pow(this->Pos.y - PointPos.y, 2));
		//if (Distance > this->RenderRange)
		//  return;

		if (PointPos.x < Base_Radar::Pos.x - Base_Radar::RenderRange || PointPos.x > Base_Radar::Pos.x + Base_Radar::RenderRange ||
			PointPos.y > Base_Radar::Pos.y + Base_Radar::RenderRange || PointPos.y < Base_Radar::Pos.y - Base_Radar::RenderRange)
			return;
		
		//std::vector<std::tuple< float, float, ImColor, int, float>> t1;PointPos, Color, Type, Yaw
		std::vector<Vector4, ImColor> t1(Vector4(PointPos.x, PointPos.y, Type, Yaw), Color);
		//float l = std::get<0>(t1);
		Base_Radar::Points.push_back(t1);
	}


	

	void DrawTriangle(Vector2 Center, ImColor Color, float Width, float Height, float Yaw)
	{
		Vector2 a, b, c;
		Vector2 Re_a, Re_b, Re_c;
		a = Vector2{ Center.x - Width / 2,Center.y };
		b = Vector2{ Center.x + Width / 2,Center.y };
		c = Vector2{ Center.x,Center.y - Height };
		a = VecMath::RevolveCoordinatesSystem(-Yaw, Center, a);
		b = VecMath::RevolveCoordinatesSystem(-Yaw, Center, b);
		c = VecMath::RevolveCoordinatesSystem(-Yaw, Center, c);
		ImGui::GetForegroundDrawList()->AddTriangleFilled(
			ImVec2(a.x, a.y),
			ImVec2(b.x, b.y),
			ImVec2(c.x, c.y),
			Color);
	}
	void RenderRadar(ImVec2 PointPos,ImColor PointColor,int PointType, float  PointYaw)
	{
		if (Base_Radar::Width <= 0)
			return;


		ImVec2 Line1first = ImVec2(Base_Radar::Pos.x - Base_Radar::Width / 2, Base_Radar::Pos.y);
		ImVec2 Line1second = ImVec2(Base_Radar::Pos.x + Base_Radar::Width / 2, Base_Radar::Pos.y);
		ImVec2 Line2first = ImVec2(Base_Radar::Pos.x, Base_Radar::Pos.y - Base_Radar::Width / 2);
		ImVec2 Line2second = ImVec2(Base_Radar::Pos.x, Base_Radar::Pos.y + Base_Radar::Width / 2);

		if (Base_Radar::Opened)
		{
			if (Base_Radar::ShowCrossLine)
			{
				ImGui::GetBackgroundDrawList()->AddLine(Line1first, Line1second, Base_Radar::CrossColor, 1);
				ImGui::GetBackgroundDrawList()->AddLine(Line2first, Line2second, Base_Radar::CrossColor, 1);
			}

			for (auto iter{ PointSingle.begin() }; iter != PointSingle.end(); iter++)
			{
				ImVec2  PointPos = std::get<0>(PointSingle);
				ImColor PointColor = std::get<1>(PointSingle);
				int    PointType = std::get<2>(PointSingle);
				float  PointYaw = std::get<3>(PointSingle);
				if (PointType == 0)
				{
					ImGui::GetBackgroundDrawList()->AddCircle(PointPos, Base_Radar::CircleSize, PointColor);
					ImGui::GetBackgroundDrawList()->AddCircleFilled(PointPos, Base_Radar::CircleSize, ImColor(0, 0, 0));
				}
				else if (PointType == 1)
				{
					Vector2 a, b, c;
					Vector2 Re_a, Re_b, Re_c;
					Vector2 Re_Point;
					float Angle = (Base_Radar::LocalYaw - PointYaw) + 180;

					Re_Point = VecMath::RevolveCoordinatesSystem(Angle, Base_Radar::Pos, Vector2(PointPos.x, PointPos.y));

					Re_a = Vector2(Re_Point.x, Re_Point.y + Base_Radar::ArrowSize);
					Re_b = Vector2(Re_Point.x - Base_Radar::ArrowSize / 1.5, Re_Point.y - Base_Radar::ArrowSize / 2);
					Re_c = Vector2(Re_Point.x + Base_Radar::ArrowSize / 1.5, Re_Point.y - Base_Radar::ArrowSize / 2);

					a = VecMath::RevolveCoordinatesSystem(-Angle, Base_Radar::Pos, Re_a);
					b = VecMath::RevolveCoordinatesSystem(-Angle, Base_Radar::Pos, Re_b);
					c = VecMath::RevolveCoordinatesSystem(-Angle, Base_Radar::Pos, Re_c);

					ImGui::GetBackgroundDrawList()->AddQuadFilled(
						ImVec2(a.x, a.y),
						ImVec2(b.x, b.y),
						ImVec2(PointPos.x, PointPos.y),
						ImVec2(c.x, c.y),
						PointColor
					);
					ImGui::GetBackgroundDrawList()->AddQuad(
						ImVec2(a.x, a.y),
						ImVec2(b.x, b.y),
						ImVec2(PointPos.x, PointPos.y),
						ImVec2(c.x, c.y),
						ImColor(0, 0, 0, 150),
						0.1
					);
				}
				else
				{
					ImVec2 TrianglePoint, TrianglePoint_1, TrianglePoint_2;
					float Angle = (Base_Radar::LocalYaw - PointYaw) - 90;

					ImGui::GetBackgroundDrawList()->AddCircleFilled(PointPos, 0.85 * Base_Radar::ArcArrowSize, PointColor, 30);
					ImGui::GetBackgroundDrawList()->AddCircle(PointPos, 0.95 * Base_Radar::ArcArrowSize, ImColor(0, 0, 0, 150), 0, 0.1);

					TrianglePoint.x = PointPos.x + (Base_Radar::ArcArrowSize + Base_Radar::ArcArrowSize / 3) * cos(-Angle * M_PI / 180);
					TrianglePoint.y = PointPos.y - (Base_Radar::ArcArrowSize + Base_Radar::ArcArrowSize / 3) * sin(-Angle * M_PI / 180);

					TrianglePoint_1.x = PointPos.x + Base_Radar::ArcArrowSize * cos(-(Angle - 30) * M_PI / 180);
					TrianglePoint_1.y = PointPos.y - Base_Radar::ArcArrowSize * sin(-(Angle - 30) * M_PI / 180);

					TrianglePoint_2.x = PointPos.x + Base_Radar::ArcArrowSize * cos(-(Angle + 30) * M_PI / 180);
					TrianglePoint_2.y = PointPos.y - Base_Radar::ArcArrowSize * sin(-(Angle + 30) * M_PI / 180);

					ImGui::GetBackgroundDrawList()->PathLineTo(TrianglePoint);
					ImGui::GetBackgroundDrawList()->PathLineTo(TrianglePoint_1);
					ImGui::GetBackgroundDrawList()->PathLineTo(TrianglePoint_2);
					ImGui::GetBackgroundDrawList()->PathFillConvex(ImColor(220, 220, 220, 240));
				}
			}
		}

		if (Base_Radar::Points.size() > 0)
			Base_Radar::Points.clear();
	}
}

namespace Menus
{
	void DrawInGameMenu()
	{
		if (menu == true)
		{
			ImGui::SetNextWindowPos({ 0,0 });
			ImGui::SetNextWindowSize({ 250,345 });

			ImGui::Begin("", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);

			ImGui::PushStyleColor(ImGuiCol_Tab, IM_COL32(220, 20, 60, 255));
			ImGui::PushStyleColor(ImGuiCol_TabHovered, IM_COL32(220, 20, 60, 255));
			ImGui::PushStyleColor(ImGuiCol_TabActive, IM_COL32(105, 105, 105, 255));
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
			ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(220, 20, 60, 255));
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(220, 20, 60, 255));

			if (ImGui::BeginTabBar("Tabs"))
			{
				if (ImGui::BeginTabItem("ESP"))
				{
					if (ImGui::BeginTabBar("Tabs"))
					{
						if (ImGui::BeginTabItem("Team Settings"))
						{
							if (ImGui::BeginChild(1, ImVec2({ 235,135 }), true, ImGuiWindowFlags_NoScrollbar))
							{
								ImGui::Checkbox("Frame", &espBoxT);

								ImGui::Combo("Styles", &TeamActiveCombo, ComboSelections, 3);

								ImGui::Checkbox("Skelton", &espSkeltonT);
								ImGui::Checkbox("Head Circle", &espHeadT);
								ImGui::Checkbox("Health Bar", &espHealthBarT);
							}
							ImGui::EndChild();

							if (ImGui::BeginChild(2, ImVec2({ 235,145, }), true, ImGuiWindowFlags_NoScrollbar))
								ImGui::ColorPicker3("", TeamFrameColor, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
							ImGui::EndChild();

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Enemy Settings"))
						{
							if (ImGui::BeginChild(1, ImVec2({ 235,135 }), true, ImGuiWindowFlags_NoScrollbar))
							{
								ImGui::Checkbox("Frame", &espBoxE);

								ImGui::Combo("Styles", &EnemyActiveCombo, ComboSelections, 3);

								ImGui::Checkbox("Skelton", &espSkeltonE);
								ImGui::Checkbox("Head Circle", &espHeadE);
								ImGui::Checkbox("Health Bar", &espHealthBarE);
							}
							ImGui::EndChild();

							if (ImGui::BeginChild(2, ImVec2({ 235,145, }), true, ImGuiWindowFlags_NoScrollbar))
								ImGui::ColorPicker3("", EnemyFrameColor, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
							ImGui::EndChild();

							ImGui::EndTabItem();
						}
					}
					ImGui::EndTabBar();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Aimbot"))
				{
					if (ImGui::BeginChild(1, ImVec2({ 235,135 }), true, ImGuiWindowFlags_NoScrollbar))
					{
						ImGui::Checkbox("Aim Bot", &AimBotbl);
						ImGui::Checkbox("Trigger Bot", &TriggerBotbl);

					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Triggerbot"))
				{
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Misc"))
				{
					if (ImGui::BeginChild(1, ImVec2({ 235,135 }), true, ImGuiWindowFlags_NoScrollbar))
					{

						ImGui::Checkbox("BH", &Bhopbl);
						
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
					
				}
			}

			ImGui::PopStyleColor();
			ImGui::EndTabBar();

			ImGui::End();
		}
	}

}
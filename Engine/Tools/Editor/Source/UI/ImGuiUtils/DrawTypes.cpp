#include "cspch.hpp"
#include "DrawTypes.hpp"
#include "Script/FieldMacros.hpp"

#include <imgui.h>
#include <IconsFontAwesome6.h>

namespace Cosmic::ImGuiUtils
{

	namespace Utils
	{

		static float32 AfterTextIndentValue()
		{
			static float32 value = 0.0f;

			// Calculate it once

			if (value == 0.0f)
			{
				constexpr size_t size = CS_FIELD_NAME_MAX_LENGTH + 2;
				char testStr[size];

				for (int32 i = 0; i < size; i++)
					testStr[i] = 'a';

				testStr[size - 1] = 0;

				value = ImGui::CalcTextSize(testStr).x + 4.0f;
			}

			return value;
		}

		static float32 GetLineHeight()
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiIO& io = ImGui::GetIO();
			float32 lineHeight = io.FontDefault->FontSize + style.FramePadding.y * 2;

			return lineHeight;
		}

		static void DrawMultiComponentFloat(ImGuiID& id, float32& value, float32 resetValue, float32 min, float32 max, float32 dragWidth, const char* buttonText, const ImVec2& buttonSize, const ImVec4& buttonColor, const ImVec4& buttonHoveredColor, const ImVec4& buttonActiveColor, bool last = false, float32 totalWidth = 0.0f)
		{
			ImGui::PushStyleColor(ImGuiCol_Button,        buttonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  buttonActiveColor);

			ImGui::PushID(id++);

			if (ImGui::Button(buttonText, buttonSize))
				value = resetValue;

			ImGui::PopID();
			ImGui::SameLine();

			if (last)
			{
				float32 intendedLastCursorPos   = Utils::AfterTextIndentValue() + totalWidth;
				float32 currentLastCursorPos    = ImGui::GetCursorPosX() + dragWidth;

				dragWidth += intendedLastCursorPos - currentLastCursorPos;
			}

			ImGui::PushItemWidth(dragWidth);
			ImGui::PushID(id++);
			ImGui::DragFloat("", &value, 0.1f, min, max, "%.2f");
			ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::PopStyleColor(3);

			if (!last)
				ImGui::SameLine();
		}

	}

	void DrawFloat(const String& name, float32& value, const float32& resetValue /*= 0.0f*/, const float32& min /*= 0.0f*/, const float32& max /*= 0.0f*/)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(4, 4));

		ImGui::AlignTextToFramePadding();

		ImGuiID baseID = ImGui::GetID(name.c_str());

		ImGui::PushID(baseID++);
		ImGui::Text(name.c_str());
		ImGui::PopID();

		float32 textWidth = ImGui::CalcItemWidth();

		ImGui::SameLine(Utils::AfterTextIndentValue());

		float32 lineHeight = Utils::GetLineHeight();

		const char* resetStr = ICON_FA_ARROW_ROTATE_RIGHT;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float32 dragFloatWidth = ImGui::GetContentRegionAvailWidth() - buttonSize.x;

		ImGui::PushItemWidth(dragFloatWidth);
		ImGui::PushID(baseID++);
		ImGui::DragFloat("##asdf", &value, 0.1f, min, max, "%.2f");
		ImGui::PopID();
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushID(baseID);

		if (ImGui::Button(resetStr, buttonSize))
			value = resetValue;

		ImGui::PopID();
		ImGui::PopStyleVar(2);
	}

	void DrawFloat2(const String& name, float2& value, const float2& resetValue /*= { 0.0f, 0.0f }*/, const float2& min /*= { 0.0f, 0.0f }*/, const float2& max /*= { 0.0f, 0.0f }*/)
	{

	}

	void DrawFloat3(const String& name, float3& value, const float3& resetValue /*= { 0.0f, 0.0f, 0.0f }*/, const float3& min /*= { 0.0f, 0.0f, 0.0f }*/, const float3& max /*= { 0.0f, 0.0f, 0.0f }*/)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(0, 4));

		ImGui::AlignTextToFramePadding();

		ImGuiID baseID = ImGui::GetID(name.c_str());

		ImGui::PushID(baseID++);
		ImGui::Text(name.c_str());

		float32 width = ImGui::CalcTextSize(name.c_str()).x;
		float32 actualWidth = Utils::AfterTextIndentValue();

		ImGui::PopID();

		ImGui::SameLine(Utils::AfterTextIndentValue());

		float32 lineHeight = Utils::GetLineHeight();
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float64 widthWithoutResetButton = (float64)ImGui::GetContentRegionAvailWidth() - (float64)buttonSize.x;
		float64 dragFloatWidth          = (widthWithoutResetButton - (float64)buttonSize.x * 3.0) / 3.0;

		ImVec4 buttonColorX        = { 0.8f, 0.1f,  0.15f, 1.0f };
		ImVec4 buttonHoveredColorX = { 0.9f, 0.2f,  0.2f,  1.0f };
		ImVec4 buttonActiveColorX  = { 0.8f, 0.1f,  0.15f, 1.0f };
		ImVec4 buttonColorY        = { 0.2f, 0.7f,  0.2f,  1.0f };
		ImVec4 buttonHoveredColorY = { 0.3f, 0.8f,  0.3f,  1.0f };
		ImVec4 buttonActiveColorY  = { 0.2f, 0.7f,  0.2f,  1.0f };
		ImVec4 buttonColorZ        = { 0.1f, 0.25f, 0.8f,  1.0f };
		ImVec4 buttonHoveredColorZ = { 0.2f, 0.35f, 0.9f,  1.0f };
		ImVec4 buttonActiveColorZ  = { 0.1f, 0.25f, 0.8f,  1.0f };

		Utils::DrawMultiComponentFloat(baseID, value.x, resetValue.x, min.x, min.y, dragFloatWidth, "X", buttonSize, buttonColorX, buttonHoveredColorX, buttonActiveColorX);
		Utils::DrawMultiComponentFloat(baseID, value.y, resetValue.y, min.y, min.y, dragFloatWidth, "Y", buttonSize, buttonColorY, buttonHoveredColorY, buttonActiveColorY);
		Utils::DrawMultiComponentFloat(baseID, value.z, resetValue.z, min.z, min.z, dragFloatWidth, "Z", buttonSize, buttonColorZ, buttonHoveredColorZ, buttonActiveColorZ, true, widthWithoutResetButton);

		ImGui::SameLine();

		ImGui::PopStyleVar(2);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(4, 4));

		ImGui::SameLine();

		const char* resetButtonStr = ICON_FA_ARROW_ROTATE_RIGHT;

		ImGui::PushID(baseID++);

		if (ImGui::Button(resetButtonStr, buttonSize))
		{
			value.x = resetValue.x;
			value.y = resetValue.y;
			value.z = resetValue.z;
		}

		ImGui::PopID();
		ImGui::PopStyleVar(2);
	}

	void DrawFloat4(const String& name, float4& value, const float4& resetValue /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/, const float4& min /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/, const float4& max /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/)
	{

	}

	void DrawInt(const String& name, int32& value, const int32& resetValue /*= 0*/, const int32& min /*= 0*/, const int32& max /*= 0*/)
	{

	}

	void DrawInt2(const String& name, int2& value, const int2& resetValue /*= { 0, 0 }*/, const int2& min /*= { 0, 0 }*/, const int2& max /*= { 0, 0 }*/)
	{

	}

	void DrawInt3(const String& name, int3& value, const int3& resetValue /*= { 0, 0, 0 }*/, const int3& min /*= { 0, 0, 0 }*/, const int3& max /*= { 0, 0, 0 }*/)
	{

	}

	void DrawInt4(const String& name, int4& value, const int4& resetValue /*= { 0, 0, 0, 0 }*/, const int4& min /*= { 0, 0, 0, 0 }*/, const int4& max /*= { 0, 0, 0, 0 }*/)
	{

	}

	void DrawUInt(const String& name, uint32& value, const uint32& resetValue /*= 0*/, const uint32& min /*= 0*/, const uint32& max /*= 0*/)
	{

	}

	void DrawUInt2(const String& name, uint2& value, const uint2& resetValue /*= { 0, 0 }*/, const uint2& min /*= { 0, 0 }*/, const uint2& max /*= { 0, 0 }*/)
	{

	}

	void DrawUInt3(const String& name, uint3& value, const uint3& resetValue /*= { 0, 0, 0 }*/, const uint3& min /*= { 0, 0, 0 }*/, const uint3& max /*= { 0, 0, 0 }*/)
	{

	}

	void DrawUInt4(const String& name, uint4& value, const uint4& resetValue /*= { 0, 0, 0, 0 }*/, const uint4& min /*= { 0, 0, 0, 0 }*/, const uint4& max /*= { 0, 0, 0, 0 }*/)
	{

	}

}
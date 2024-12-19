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

		static bool DrawMultiComponentFloat(ImGuiID& id, float32& value, float32 resetValue, float32 min, float32 max, float32 dragWidth, const char* buttonText, const ImVec2& buttonSize, const ImVec4& buttonColor, const ImVec4& buttonHoveredColor, const ImVec4& buttonActiveColor, bool last = false, float32 totalWidth = 0.0f)
		{
			bool changed = false;

			ImGui::PushStyleColor(ImGuiCol_Button,        buttonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  buttonActiveColor);

			ImGui::PushID(id++);

			if (ImGui::Button(buttonText, buttonSize))
			{
				value   = resetValue;
				changed = true;
			}

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
			changed = changed || ImGui::DragFloat("", &value, 0.1f, min, max, "%.2f");
			ImGui::PopID();
			ImGui::PopItemWidth();
			ImGui::PopStyleColor(3);

			if (!last)
				ImGui::SameLine();

			return changed;
		}

		static TooltipCallback sTooltipCallback = nullptr;

		static void CallTooltipCallback()
		{
			if (sTooltipCallback)
				sTooltipCallback();
		}

	}

	void SetTooltipCallback(TooltipCallback callback)
	{
		Utils::sTooltipCallback = callback;
	}

	bool DrawFloat(const String& name, float32& value, const float32& resetValue /*= 0.0f*/, const float32& min /*= 0.0f*/, const float32& max /*= 0.0f*/)
	{
		bool changed = false;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(4, 4));

		ImGui::AlignTextToFramePadding();

		ImGuiID baseID = ImGui::GetID(name.c_str());

		ImGui::PushID(baseID++);
		ImGui::Text(name.c_str());

		if (ImGui::IsItemHovered())
			Utils::CallTooltipCallback();

		ImGui::PopID();

		float32 textWidth = ImGui::CalcItemWidth();

		ImGui::SameLine(Utils::AfterTextIndentValue());

		float32 lineHeight = Utils::GetLineHeight();

		const char* resetStr = ICON_FA_ARROW_ROTATE_RIGHT;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float32 dragFloatWidth = ImGui::GetContentRegionAvailWidth() - buttonSize.x;

		ImGui::PushItemWidth(dragFloatWidth);
		ImGui::PushID(baseID++);
		changed = ImGui::DragFloat("##asdf", &value, 0.1f, min, max, "%.2f");
		ImGui::PopID();
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushID(baseID);

		if (ImGui::Button(resetStr, buttonSize))
		{
			value   = resetValue;
			changed = true;
		}

		ImGui::PopID();
		ImGui::PopStyleVar(2);

		return changed;
	}

	bool DrawFloat2(const String& name, float2& value, const float2& resetValue /*= { 0.0f, 0.0f }*/, const float2& min /*= { 0.0f, 0.0f }*/, const float2& max /*= { 0.0f, 0.0f }*/)
	{
		return false;
	}

	bool DrawFloat3(const String& name, float3& value, const float3& resetValue /*= { 0.0f, 0.0f, 0.0f }*/, const float3& min /*= { 0.0f, 0.0f, 0.0f }*/, const float3& max /*= { 0.0f, 0.0f, 0.0f }*/)
	{
		bool changed = false;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(0, 4));

		ImGui::AlignTextToFramePadding();

		ImGuiID baseID = ImGui::GetID(name.c_str());

		ImGui::PushID(baseID++);
		ImGui::Text(name.c_str());

		if (ImGui::IsItemHovered())
			Utils::CallTooltipCallback();

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

		changed = changed || Utils::DrawMultiComponentFloat(baseID, value.x, resetValue.x, min.x, min.y, dragFloatWidth, "X", buttonSize, buttonColorX, buttonHoveredColorX, buttonActiveColorX);
		changed = changed || Utils::DrawMultiComponentFloat(baseID, value.y, resetValue.y, min.y, min.y, dragFloatWidth, "Y", buttonSize, buttonColorY, buttonHoveredColorY, buttonActiveColorY);
		changed = changed || Utils::DrawMultiComponentFloat(baseID, value.z, resetValue.z, min.z, min.z, dragFloatWidth, "Z", buttonSize, buttonColorZ, buttonHoveredColorZ, buttonActiveColorZ, true, widthWithoutResetButton);

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
			changed = true;
		}

		ImGui::PopID();
		ImGui::PopStyleVar(2);

		return changed;
	}

	bool DrawFloat4(const String& name, float4& value, const float4& resetValue /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/, const float4& min /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/, const float4& max /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/)
	{
		return false;
	}

	bool DrawInt(const String& name, int32& value, const int32& resetValue /*= 0*/, const int32& min /*= 0*/, const int32& max /*= 0*/)
	{
		return false;
	}

	bool DrawInt2(const String& name, int2& value, const int2& resetValue /*= { 0, 0 }*/, const int2& min /*= { 0, 0 }*/, const int2& max /*= { 0, 0 }*/)
	{
		return false;
	}

	bool DrawInt3(const String& name, int3& value, const int3& resetValue /*= { 0, 0, 0 }*/, const int3& min /*= { 0, 0, 0 }*/, const int3& max /*= { 0, 0, 0 }*/)
	{
		return false;
	}

	bool DrawInt4(const String& name, int4& value, const int4& resetValue /*= { 0, 0, 0, 0 }*/, const int4& min /*= { 0, 0, 0, 0 }*/, const int4& max /*= { 0, 0, 0, 0 }*/)
	{
		return false;
	}

	bool DrawUInt(const String& name, uint32& value, const uint32& resetValue /*= 0*/, const uint32& min /*= 0*/, const uint32& max /*= 0*/)
	{
		return false;
	}

	bool DrawUInt2(const String& name, uint2& value, const uint2& resetValue /*= { 0, 0 }*/, const uint2& min /*= { 0, 0 }*/, const uint2& max /*= { 0, 0 }*/)
	{
		return false;
	}

	bool DrawUInt3(const String& name, uint3& value, const uint3& resetValue /*= { 0, 0, 0 }*/, const uint3& min /*= { 0, 0, 0 }*/, const uint3& max /*= { 0, 0, 0 }*/)
	{
		return false;
	}

	bool DrawUInt4(const String& name, uint4& value, const uint4& resetValue /*= { 0, 0, 0, 0 }*/, const uint4& min /*= { 0, 0, 0, 0 }*/, const uint4& max /*= { 0, 0, 0, 0 }*/)
	{
		return false;
	}

	bool DrawVec2(const String& name, glm::vec2& value, const glm::vec2& resetValue /*= { 0.0f, 0.0f }*/, const glm::vec2& min /*= { 0.0f, 0.0f }*/, const glm::vec2& max /*= { 0.0f, 0.0f }*/)
	{
		return DrawFloat2(name, *(float2*)&value, *(float2*)&resetValue, *(float2*)&min, *(float2*)&max);
	}

	bool DrawVec3(const String& name, glm::vec3& value, const glm::vec3& resetValue /*= { 0.0f, 0.0f, 0.0f }*/, const glm::vec3& min /*= { 0.0f, 0.0f, 0.0f }*/, const glm::vec3& max /*= { 0.0f, 0.0f, 0.0f }*/)
	{
		return DrawFloat3(name, *(float3*)&value, *(float3*)&resetValue, *(float3*)&min, *(float3*)&max);
	}

	bool DrawVec4(const String& name, glm::vec4& value, const glm::vec4& resetValue /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/, const glm::vec4& min /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/, const glm::vec4& max /*= { 0.0f, 0.0f, 0.0f, 0.0f }*/)
	{
		return DrawFloat4(name, *(float4*)&value, *(float4*)&resetValue, *(float4*)&min, *(float4*)&max);
	}

	bool DrawString(const String& name, String& value, const String& resetValue)
	{
		ImGuiID baseID = ImGui::GetID(name.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(4, 4));

		ImGui::AlignTextToFramePadding();

		ImGui::PushID(baseID++);
		ImGui::Text(name.c_str());

		if (ImGui::IsItemHovered())
			Utils::CallTooltipCallback();

		ImGui::PopID();

		ImGui::SameLine(Utils::AfterTextIndentValue());

		auto callback = [](ImGuiInputTextCallbackData* data) -> int32
		{
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
			{
				String* userData = (String*)data->UserData;
				userData->resize(data->BufSize);
				data->Buf = (char*)userData->c_str();
			}
			return 0;
		};

		float32 lineHeight = Utils::GetLineHeight();
		ImVec2  buttonSize = { lineHeight, lineHeight };
		float32 width      = ImGui::GetContentRegionAvailWidth() - buttonSize.x;

		ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_AutoSelectAll;

		ImGui::PushItemWidth(width);
		ImGui::PushID(baseID++);
		bool changed = ImGui::InputText("", (char*)value.c_str(), value.size(), flags, callback, (void*)&value);
		ImGui::PopID();
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushID(baseID++);

		if (ImGui::Button(ICON_FA_ARROW_ROTATE_RIGHT, buttonSize))
		{
			changed = true;
			value   = resetValue;
		}
		
		ImGui::PopID();
		ImGui::PopStyleVar(2);

		return changed;
	}

    bool DrawEnumStr(const String& name, String& currentValue, const Vector<String>& values, const String& resetValue)
	{
		ImGuiID baseID = ImGui::GetID(name.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,  ImVec2(4, 4));

		ImGui::AlignTextToFramePadding();

		ImGui::PushID(baseID++);
		ImGui::Text(name.c_str());

		if (ImGui::IsItemHovered())
			Utils::CallTooltipCallback();

		ImGui::PopID();

		ImGui::SameLine(Utils::AfterTextIndentValue());

		bool changed = false;

		float32 lineHeight = Utils::GetLineHeight();
		ImVec2  buttonSize = { lineHeight, lineHeight };
		float32 width      = ImGui::GetContentRegionAvailWidth() - buttonSize.x;

		ImGui::PushItemWidth(width);
		ImGui::PushID(baseID++);

		if (ImGui::BeginCombo("", currentValue.c_str()))
		{
			for (int16 i = 0; i < values.size(); i++)
			{
				bool isSelected = currentValue == values[i];

				ImGui::PushID(baseID++);

				if (ImGui::Selectable(values[i].c_str(), isSelected))
				{
					changed      = true;
					currentValue = values[i];
				}

				ImGui::PopID();

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::PopID();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushID(baseID++);

		if (ImGui::Button(ICON_FA_ARROW_ROTATE_RIGHT, buttonSize))
		{
			changed      = true;
			currentValue = resetValue;
		}

		ImGui::PopID();
		ImGui::PopStyleVar(2);

		return changed;
	}

	bool DrawEnumInt16(const String& name, int16& value, const int16 resetValue, const int16 lastValue, EnumToStringCallback toStr, EnumFromStringCallback fromStr)
	{
		static UnorderedMap<ImGuiID, EnumStringData> sStringDataMap;

		ImGuiID id = ImGui::GetID(name.c_str());

		if (sStringDataMap.find(id) == sStringDataMap.end())
		{
			EnumStringData strData;
			strData.CurrentValue = toStr(value);
			strData.ResetValue   = toStr(resetValue);
			strData.Values       = Vector<String>(lastValue);

			for (int16 i = 0; i < lastValue; i++)
				strData.Values[i] = toStr(i);

			sStringDataMap[id] = strData;
		}

		EnumStringData& strData = sStringDataMap[id];

		if (DrawEnumStr(name, strData.CurrentValue, strData.Values, strData.ResetValue))
		{
			value = fromStr(strData.CurrentValue.c_str());
			return true;
		}

		return false;
	}

}
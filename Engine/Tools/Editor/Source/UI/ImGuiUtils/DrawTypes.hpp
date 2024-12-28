#pragma once
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"

#include <glm/glm.hpp>

namespace Cosmic::ImGuiUtils
{

    using TooltipCallback = std::function<void()>;

    void SetTooltipCallback(TooltipCallback callback);
    void Tooltip(const char* text);

    float32 GetLineHeight();

    bool DrawFloat( const String& name, float32& value, const float32& resetValue = 0.0f,                       const float32& min = 0.0f,                       const float32& max = 0.0f);
    bool DrawFloat2(const String& name, float2&  value, const float2&  resetValue = { 0.0f, 0.0f },             const float2&  min = { 0.0f, 0.0f },             const float2&  max = { 0.0f, 0.0f });
    bool DrawFloat3(const String& name, float3&  value, const float3&  resetValue = { 0.0f, 0.0f, 0.0f },       const float3&  min = { 0.0f, 0.0f, 0.0f },       const float3&  max = { 0.0f, 0.0f, 0.0f });
    bool DrawFloat4(const String& name, float4&  value, const float4&  resetValue = { 0.0f, 0.0f, 0.0f, 0.0f }, const float4&  min = { 0.0f, 0.0f, 0.0f, 0.0f }, const float4&  max = { 0.0f, 0.0f, 0.0f, 0.0f });

    bool DrawInt( const String& name, int32& value, const int32& resetValue = 0,              const int32& min = 0,              const int32& max = 0);
    bool DrawInt2(const String& name, int2&  value, const int2&  resetValue = { 0, 0 },       const int2&  min = { 0, 0 },       const int2&  max = { 0, 0 });
    bool DrawInt3(const String& name, int3&  value, const int3&  resetValue = { 0, 0, 0 },    const int3&  min = { 0, 0, 0 },    const int3&  max = { 0, 0, 0 });
    bool DrawInt4(const String& name, int4&  value, const int4&  resetValue = { 0, 0, 0, 0 }, const int4&  min = { 0, 0, 0, 0 }, const int4&  max = { 0, 0, 0, 0 });

    bool DrawUInt( const String& name, uint32& value, const uint32& resetValue = 0,              const uint32& min = 0,              const uint32& max = 0);
    bool DrawUInt2(const String& name, uint2&  value, const uint2&  resetValue = { 0, 0 },       const uint2&  min = { 0, 0 },       const uint2&  max = { 0, 0 });
    bool DrawUInt3(const String& name, uint3&  value, const uint3&  resetValue = { 0, 0, 0 },    const uint3&  min = { 0, 0, 0 },    const uint3&  max = { 0, 0, 0 });
    bool DrawUInt4(const String& name, uint4&  value, const uint4&  resetValue = { 0, 0, 0, 0 }, const uint4&  min = { 0, 0, 0, 0 }, const uint4&  max = { 0, 0, 0, 0 });

    bool DrawVec2(const String& name, glm::vec2&  value, const glm::vec2&  resetValue = { 0.0f, 0.0f },             const glm::vec2&  min = { 0.0f, 0.0f },             const glm::vec2&  max = { 0.0f, 0.0f });
    bool DrawVec3(const String& name, glm::vec3&  value, const glm::vec3&  resetValue = { 0.0f, 0.0f, 0.0f },       const glm::vec3&  min = { 0.0f, 0.0f, 0.0f },       const glm::vec3&  max = { 0.0f, 0.0f, 0.0f });
    bool DrawVec4(const String& name, glm::vec4&  value, const glm::vec4&  resetValue = { 0.0f, 0.0f, 0.0f, 0.0f }, const glm::vec4&  min = { 0.0f, 0.0f, 0.0f, 0.0f }, const glm::vec4&  max = { 0.0f, 0.0f, 0.0f, 0.0f });

    bool DrawString(const String& name, String& value, const String& resetValue);

	struct EnumStringData
	{
		String         CurrentValue;
		String         ResetValue;
		Vector<String> Values;
	};

    bool DrawEnumStr(const String& name, String& currentValue, const Vector<String>& values, const String& resetValue);
    bool DrawEnumInt16(const String& name, int16& value, const int16 resetValue, const int16 lastValue, EnumToStringCallback toStr, EnumFromStringCallback fromStr);

    template<typename T, typename ToStr, typename FromStr>
    bool DrawEnum(const String& name, T& value, const T resetValue, ToStr toStr, FromStr fromStr)
    {
        return DrawEnumInt16(
            name, (int16&)value, (int16)resetValue, (int16)T::Last,
            [&](int16 value) -> const char*
            {
                return toStr((T)value);
            },
            [&](const char* str) -> int16
            {
                return (int16)fromStr(str);
            }
        );
    }

}

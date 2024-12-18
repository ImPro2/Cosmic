#pragma once
#include "Base/Types.hpp"
#include "Base/Tuples.hpp"

namespace Cosmic::ImGuiUtils
{

    void DrawFloat( const String& name, float32& value, const float32& resetValue = 0.0f,                       const float32& min = 0.0f,                       const float32& max = 0.0f);
    void DrawFloat2(const String& name, float2&  value, const float2&  resetValue = { 0.0f, 0.0f },             const float2&  min = { 0.0f, 0.0f },             const float2&  max = { 0.0f, 0.0f });
    void DrawFloat3(const String& name, float3&  value, const float3&  resetValue = { 0.0f, 0.0f, 0.0f },       const float3&  min = { 0.0f, 0.0f, 0.0f },       const float3&  max = { 0.0f, 0.0f, 0.0f });
    void DrawFloat4(const String& name, float4&  value, const float4&  resetValue = { 0.0f, 0.0f, 0.0f, 0.0f }, const float4&  min = { 0.0f, 0.0f, 0.0f, 0.0f }, const float4&  max = { 0.0f, 0.0f, 0.0f, 0.0f });

    void DrawInt( const String& name, int32& value, const int32& resetValue = 0,              const int32& min = 0,              const int32& max = 0);
    void DrawInt2(const String& name, int2&  value, const int2&  resetValue = { 0, 0 },       const int2&  min = { 0, 0 },       const int2&  max = { 0, 0 });
    void DrawInt3(const String& name, int3&  value, const int3&  resetValue = { 0, 0, 0 },    const int3&  min = { 0, 0, 0 },    const int3&  max = { 0, 0, 0 });
    void DrawInt4(const String& name, int4&  value, const int4&  resetValue = { 0, 0, 0, 0 }, const int4&  min = { 0, 0, 0, 0 }, const int4&  max = { 0, 0, 0, 0 });

    void DrawUInt( const String& name, uint32& value, const uint32& resetValue = 0,              const uint32& min = 0,              const uint32& max = 0);
    void DrawUInt2(const String& name, uint2&  value, const uint2&  resetValue = { 0, 0 },       const uint2&  min = { 0, 0 },       const uint2&  max = { 0, 0 });
    void DrawUInt3(const String& name, uint3&  value, const uint3&  resetValue = { 0, 0, 0 },    const uint3&  min = { 0, 0, 0 },    const uint3&  max = { 0, 0, 0 });
    void DrawUInt4(const String& name, uint4&  value, const uint4&  resetValue = { 0, 0, 0, 0 }, const uint4&  min = { 0, 0, 0, 0 }, const uint4&  max = { 0, 0, 0, 0 });

}

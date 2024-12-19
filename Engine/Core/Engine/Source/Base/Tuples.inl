#pragma once

namespace Cosmic
{

    inline float2 operator+(const float2& left, const float2& right)
    {
        return { left.x + right.x, left.y + right.y };
    }

    inline float2 operator-(const float2& left, const float2& right)
    {
        return { left.x - right.x, left.y - right.y };
    }

    inline float2 operator*(const float32 left, const float2& right)
    {
        return { left * right.x, left * right.y };
    }

    inline float2 operator*(const float2& left, const float32 right)
    {
        return { left.x * right, left.y * right };
    }

    inline float2 operator/(const float2& left, const float32 right)
    {
        return { left.x / right, left.y / right };
    }

    inline float3 operator+(const float3& left, const float3& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z };
    }

    inline float3 operator-(const float3& left, const float3& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z };
    }

    inline float3 operator*(const float32 left, const float3& right)
    {
        return { left * right.x, left * right.y, left * right.z };
    }

    inline float3 operator*(const float3& left, const float32 right)
    {
        return { left.x * right, left.y * right, left.z * right };
    }

    inline float3 operator/(const float3& left, const float32 right)
    {
        return { left.x / right, left.y / right, left.z / right };
    }

    inline float4 operator+(const float4& left, const float4& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w };
    }

    inline float4 operator-(const float4& left, const float4& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w };
    }

    inline float4 operator*(const float32 left, const float4& right)
    {
        return { left * right.x, left * right.y, left * right.z, left * right.w };
    }

    inline float4 operator*(const float4& left, const float32 right)
    {
        return { left.x * right, left.y * right, left.z * right, left.w * right };
    }

    inline float4 operator/(const float4& left, const float32 right)
    {
        return { left.x / right, left.y / right, left.z / right, left.w / right };
    }

    inline int2 operator+(const int2& left, const int2& right)
    {
        return { left.x + right.x, left.y + right.y };
    }

    inline int2 operator-(const int2& left, const int2& right)
    {
        return { left.x - right.x, left.y - right.y };
    }

    inline int2 operator*(const int32 left, const int2& right)
    {
        return { left * right.x, left * right.y };
    }

    inline int2 operator*(const int2& left, const int32 right)
    {
        return { left.x * right, left.y * right };
    }

    inline int2 operator/(const int2& left, const int32 right)
    {
        return { left.x / right, left.y / right };
    }

    inline int3 operator+(const int3& left, const int3& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z };
    }

    inline int3 operator-(const int3& left, const int3& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z };
    }

    inline int3 operator*(const int32 left, const int3& right)
    {
        return { left * right.x, left * right.y, left * right.z };
    }

    inline int3 operator*(const int3& left, const int32 right)
    {
        return { left.x * right, left.y * right, left.z * right };
    }

    inline int3 operator/(const int3& left, const int32 right)
    {
        return { left.x / right, left.y / right, left.z / right };
    }

    inline int4 operator+(const int4& left, const int4& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w };
    }

    inline int4 operator-(const int4& left, const int4& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w };
    }

    inline int4 operator*(const int32 left, const int4& right)
    {
        return { left * right.x, left * right.y, left * right.z, left * right.w };
    }

    inline int4 operator*(const int4& left, const int32 right)
    {
        return { left.x * right, left.y * right, left.z * right, left.w * right };
    }

    inline int4 operator/(const int4& left, const int32 right)
    {
        return { left.x / right, left.y / right, left.z / right, left.w / right };
    }

    inline uint2 operator+(const uint2& left, const uint2& right)
    {
        return { left.x + right.x, left.y + right.y };
    }

    inline uint2 operator-(const uint2& left, const uint2& right)
    {
        return { left.x - right.x, left.y - right.y };
    }

    inline uint2 operator*(const uint32 left, const uint2& right)
    {
        return { left * right.x, left * right.y };
    }

    inline uint2 operator*(const uint2& left, const uint32 right)
    {
        return { left.x * right, left.y * right };
    }

    inline uint2 operator/(const uint2& left, const uint32 right)
    {
        return { left.x / right, left.y / right };
    }

    inline uint3 operator+(const uint3& left, const uint3& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z };
    }

    inline uint3 operator-(const uint3& left, const uint3& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z };
    }

    inline uint3 operator*(const uint32 left, const uint3& right)
    {
        return { left * right.x, left * right.y, left * right.z };
    }

    inline uint3 operator*(const uint3& left, const uint32 right)
    {
        return { left.x * right, left.y * right, left.z * right };
    }

    inline uint3 operator/(const uint3& left, const uint32 right)
    {
        return { left.x / right, left.y / right, left.z / right };
    }

    inline uint4 operator+(const uint4& left, const uint4& right)
    {
        return { left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w };
    }

    inline uint4 operator-(const uint4& left, const uint4& right)
    {
        return { left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w };
    }

    inline uint4 operator*(const uint32 left, const uint4& right)
    {
        return { left * right.x, left * right.y, left * right.z, left * right.w };
    }

    inline uint4 operator*(const uint4& left, const uint32 right)
    {
        return { left.x * right, left.y * right, left.z * right, left.w * right };
    }

    inline uint4 operator/(const uint4& left, const uint32 right)
    {
        return { left.x / right, left.y / right, left.z / right, left.w / right };
    }

}

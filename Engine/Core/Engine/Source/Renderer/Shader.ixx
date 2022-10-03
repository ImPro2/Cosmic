module;
#include "cspch.hpp"
#include <glm/glm.hpp>
export module Cosmic.Renderer.Shader;

import Cosmic.Base;

namespace Cosmic
{

    export enum class EShaderDataType
    {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Mat2,
        Mat3,
        Mat4,
        Bool
    };

    export class Shader
    {
    public:
        Shader(const String& filePath);
        Shader(const String& name, const String& vertexSrc, const String& fragmentSrc);
        ~Shader() = default;

    public:
        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;

    public:
        virtual void SetFloat (const String& name,       float32    value) = 0;
        virtual void SetFloat2(const String& name, const float2&    value) = 0;
        virtual void SetFloat3(const String& name, const float3&    value) = 0;
        virtual void SetFloat4(const String& name, const float4&    value) = 0;
        
        virtual void SetVec2  (const String& name, const glm::vec2& value) = 0;
        virtual void SetVec3  (const String& name, const glm::vec3& value) = 0;
        virtual void SetVec4  (const String& name, const glm::vec4& value) = 0;
        
        virtual void SetInt   (const String& name,       int32      value) = 0;
        virtual void SetInt2  (const String& name, const int2&      value) = 0;
        virtual void SetInt3  (const String& name, const int3&      value) = 0;
        virtual void SetInt4  (const String& name, const int4&      value) = 0;
        
        virtual void SetMat2  (const String& name, const glm::mat2& value) = 0;
        virtual void SetMat3  (const String& name, const glm::mat3& value) = 0;
        virtual void SetMat4  (const String& name, const glm::mat4& value) = 0;
        
        virtual void SetBool  (const String& name, bool             value) = 0;

    public:
        const String& GetName()     const { return mName;     }
        const String& GetFilePath() const { return mFilePath; }

    private:
        String mName;
        String mFilePath;
    };

    export Ref<Shader> CreateShader(const String& filePath);
    export Ref<Shader> CreateShader(const String& name, const String& vertexSrc, const String& fragmentSrc);


}

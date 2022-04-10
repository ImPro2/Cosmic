module;
#include "cspch.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
export module Cosmic.Impl.RendererAPI.OpenGL.OpenGLShader;

import Cosmic.Base;
import Cosmic.Renderer.Shader;

namespace Cosmic
{

    export class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const String& filePath);
        OpenGLShader(const String& name, const String& vertexSrc, const String& fragmentSrc);
        virtual ~OpenGLShader();

    public:
        void Bind()   const override;
        void Unbind() const override;

    public:
        void SetFloat(const String& name, float32    value)      override;
        void SetFloat2(const String& name, const float2& value)  override;
        void SetFloat3(const String& name, const float3& value)  override;
        void SetFloat4(const String& name, const float4& value)  override;

        void SetVec2(const String& name, const glm::vec2& value) override;
        void SetVec3(const String& name, const glm::vec3& value) override;
        void SetVec4(const String& name, const glm::vec4& value) override;

        void SetInt(const String& name, int32      value)        override;
        void SetInt2(const String& name, const int2& value)      override;
        void SetInt3(const String& name, const int3& value)      override;
        void SetInt4(const String& name, const int4& value)      override;

        void SetMat2(const String& name, const glm::mat2& value) override;
        void SetMat3(const String& name, const glm::mat3& value) override;
        void SetMat4(const String& name, const glm::mat4& value) override;

        void SetBool(const String& name, bool             value) override;

    public:
        uint32 GetRendererID() const { return mRendererID; }

    private:
        String                       ReadFile(const String& path);
        UnorderedMap<GLenum, String> PreProcess(const String& source);
        GLenum                       CreateProgram();
        GLenum                       CreateAndCompileShader(GLenum type, const String& source);

    private:
        uint32                       mRendererID;
        UnorderedMap<GLenum, String> mOpenGLSourceCode; // for debugging purposes, I think
    };

}
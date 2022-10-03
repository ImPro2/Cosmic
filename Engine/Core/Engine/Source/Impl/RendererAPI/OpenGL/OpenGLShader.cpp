module;
#include "cspch.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
module Cosmic.Impl.RendererAPI.OpenGL.OpenGLShader;

CS_MODULE_LOG_INFO(Cosmic, Cosmic.Impl.RendererAPI.OpenGL.OpenGLShader);

import Cosmic.App.Log;

namespace Cosmic
{

    static GLenum StringToOpenGLShaderType(const String& type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        CS_BREAK();
        return 0;
    }

    static const char* OpenGLShaderTypeToStr(GLenum type)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER:   return "vertex";
            case GL_FRAGMENT_SHADER: return "fragment";
        }

        CS_BREAK();
        return 0;
    }

    OpenGLShader::OpenGLShader(const String& filePath)
        : Shader(filePath)
    {
        CS_PROFILE_FN();

        String source     = ReadFile(filePath);
        mOpenGLSourceCode = PreProcess(source);
        mRendererID       = CreateProgram();

        Bind();
    }

    OpenGLShader::OpenGLShader(const String& name, const String& vertexSrc, const String& fragmentSrc)
        : Shader(name, vertexSrc, fragmentSrc)
    {
        CS_PROFILE_FN();

        mOpenGLSourceCode[GL_VERTEX_SHADER]   = vertexSrc;
        mOpenGLSourceCode[GL_FRAGMENT_SHADER] = fragmentSrc;

        mRendererID = CreateProgram();

        Bind();
    }

    OpenGLShader::~OpenGLShader()
    {
        CS_PROFILE_FN();
    }

    void OpenGLShader::Bind() const
    {
        CS_PROFILE_FN();

        glUseProgram(mRendererID);
    }

    void OpenGLShader::Unbind() const
    {
        CS_PROFILE_FN();

        glUseProgram(0);
    }

    String OpenGLShader::ReadFile(const String& path)
    {
        CS_PROFILE_FN();

        String result;
        std::ifstream in(path, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                CS_LOG_ERROR("Could not read from file {}", GetFilePath());
            }
        }
        else
        {
            CS_LOG_ERROR("Could not open file {}", GetFilePath());
        }

        return result;
    }

    UnorderedMap<GLenum, String> OpenGLShader::PreProcess(const String& source)
    {
        CS_PROFILE_FN();

        UnorderedMap<GLenum, String> sources;

        const char* typeToken = "#type";
        size_t      pos       = source.find(typeToken, 0);

        while (pos != String::npos)
        {
            size_t eol   = source.find_first_of("\r\n", pos);  // End of shader type declaration line
            size_t begin = pos + strlen(typeToken) + 1;        // Start of shader type name
            String type  = source.substr(begin, eol - begin);

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); // start of shader code after shader type declaration
            pos                = source.find(typeToken, nextLinePos);   // start of next shader type declaration line

            sources[StringToOpenGLShaderType(type)] = (pos == String::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return sources;
    }

    GLenum OpenGLShader::CreateProgram()
    {
        CS_PROFILE_FN();

        uint32 programID = glCreateProgram();

        UnorderedMap<GLenum, String> sources = mOpenGLSourceCode;
        Vector<GLenum> shaderIDs = {};

        for (auto& kv : sources)
        {
            GLenum shaderID = CreateAndCompileShader(kv.first, kv.second);
            glAttachShader(programID, shaderID);

            shaderIDs.push_back(shaderID);
        }

        glLinkProgram(programID);
        glValidateProgram(programID);

        int isLinked;
        glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            int length;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);

            char* message = (char*)alloca(length * sizeof(char*));
            glGetProgramInfoLog(programID, length, &length, message);

            CS_LOG_ERROR("Failed to link shader {}:\n{}", GetFilePath(), message);

            glDeleteProgram(programID);
        }

        for (GLenum shaderID : shaderIDs)
        {
            glDetachShader(programID, shaderID);
            glDeleteShader(shaderID);
        }

        return programID;
    }

    GLenum OpenGLShader::CreateAndCompileShader(GLenum type, const String& source)
    {
        CS_PROFILE_FN();

        uint32 shaderID = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shaderID, 1, &src, nullptr);
        glCompileShader(shaderID);

        int compileResult;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
        if (compileResult == GL_FALSE)
        {
            int length;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char*));
            glGetShaderInfoLog(shaderID, length, &length, message);

            CS_LOG_ERROR("Failed to compile {} shader. Reason:\n{}", OpenGLShaderTypeToStr(type), message);

            glDeleteShader(shaderID);
            CS_BREAK();
            return -1;
        }

        return shaderID;
    }

    int32 OpenGLShader::GetUniformLocation(const String& name)
    {
        CS_PROFILE_FN();

        if (mUniformLocationCache.contains(name))
            return mUniformLocationCache.at(name);

        int32 loc = glGetUniformLocation(mRendererID, name.c_str());
        if (loc == -1) CS_LOG_ERROR("Unidentified uniform '{}'", name);

        mUniformLocationCache[name] = loc;
        return loc;
    }

    void OpenGLShader::SetFloat(const String& name, float32 value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform1f(loc, value);
    }

    void OpenGLShader::SetFloat2(const String& name, const float2& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform2f(loc, value.x, value.y);
    }

    void OpenGLShader::SetFloat3(const String& name, const float3& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform3f(loc, value.x, value.y, value.z);
    }

    void OpenGLShader::SetFloat4(const String& name, const float4& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform4f(loc, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetVec2(const String& name, const glm::vec2& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform2f(loc, value.x, value.y);
    }

    void OpenGLShader::SetVec3(const String& name, const glm::vec3& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform3f(loc, value.x, value.y, value.z);
    }

    void OpenGLShader::SetVec4(const String& name, const glm::vec4& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform4f(loc, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetInt(const String& name, int32 value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform1i(loc, value);
    }

    void OpenGLShader::SetInt2(const String& name, const int2& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform2i(loc, value.x, value.y);
    }

    void OpenGLShader::SetInt3(const String& name, const int3& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform3i(loc, value.x, value.y, value.z);
    }

    void OpenGLShader::SetInt4(const String& name, const int4& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform4i(loc, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat2(const String& name, const glm::mat2& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetMat3(const String& name, const glm::mat3& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetMat4(const String& name, const glm::mat4& value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetBool(const String& name, bool value)
    {
        CS_PROFILE_FN();

        int32 loc = GetUniformLocation(name);
        glUniform1i(loc, value ? 1 : 0);
    }

}

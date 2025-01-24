#pragma once
#include "Base/Base.hpp"
#include "ShaderReflectionData.hpp"

namespace Cosmic
{

	struct SpirvBinary
	{
		Buffer Binary;
	};

	class Shader;

	using ShaderSourceMap = UnorderedMap<EShaderStage, String>;

	class ShaderCompiler
	{
	public:
		static ShaderSourceMap PreProcess(const String& src);

		// Links spirv binaries
		static SpirvBinary CompileToSpirv(const String& shaderName, EShaderSourceLanguage language, const ShaderSourceMap& srcMap);

		static ShaderReflectionData Reflect(const SpirvBinary& spirv);

	private:
		static Vector<uint32> LinkSpirv(const Vector<Vector<uint32>>& spirvs);
		static Vector<uint32> CompileToSpirv(const String& shaderName, EShaderSourceLanguage language, EShaderStage stage, const String& src);
	};

}

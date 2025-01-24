#pragma once
#include "Memory/Memory.hpp"

namespace Cosmic
{

	class Application;
	class Shader;

	using ShaderNameMap = UnorderedMap<String, Ref<Shader>>;

	class ShaderLibrary
	{
	private:
		const PersistentRef<ShaderLibrary>& Init();
		void Shutdown();

	private:
		void RegisterShader(const Ref<Shader>& shader);

	public:

	private:
		inline static PersistentRef<ShaderLibrary> sInstance;

		ShaderNameMap mShaderNameMap;


		friend class Application;
		friend class Shader;
	};

}

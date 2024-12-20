#pragma once
#include "Base/Base.hpp"
#include "App/FileSystem.hpp"
#include "App/File.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components.hpp"
#include "Memory/Memory.hpp"

namespace Cosmic
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

	public:
		void Serialize(const File& path);
		void Deserialize(const File& path);

		void SerializeRuntime(const String& path);
		void DeserializeRuntime(const String& path);

	private:
		Ref<Scene> mScene;
	};

}

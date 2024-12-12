#pragma once
#include "Base/Base.hpp"
#include "App/FileSystem.hpp"
#include "App/File.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components.hpp"
#include "Memory/StrongRef.hpp"

namespace Cosmic
{

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

	public:
		void Serialize(const String& path);
		void Deserialize(const String& path);

		void SerializeRuntime(const String& path);
		void DeserializeRuntime(const String& path);

	private:
		Ref<Scene> mScene;
	};

}

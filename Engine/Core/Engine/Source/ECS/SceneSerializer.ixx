module;
#include "cspch.hpp"
export module Cosmic.ECS.SceneSerializer;

import Cosmic.Base;
import Cosmic.App.FileSystem;
import Cosmic.App.File;
import Cosmic.ECS.Scene;
import Cosmic.ECS.Entity;
import Cosmic.ECS.Components;

namespace Cosmic
{

	export class SceneSerializer
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

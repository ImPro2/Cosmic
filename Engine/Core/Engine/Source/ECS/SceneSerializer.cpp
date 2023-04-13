module;
#include "cspch.hpp"
#include <yaml-cpp/yaml.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <fstream>
module Cosmic.ECS.SceneSerializer;

namespace YAML
{

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Cosmic::float4>
	{
		static Node encode(const Cosmic::float4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, Cosmic::float4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Cosmic
{

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "123456789";

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "Tag Component" << YAML::Value << YAML::BeginMap;

			const char* tag = entity.GetComponent<TagComponent>().Tag.c_str();
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "Transform Component" << YAML::Value << YAML::BeginMap;

			auto& component = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "Translation" << YAML::Value << YAML::Flow << YAML::BeginSeq << component.Translation.x << component.Translation.y << component.Translation.z << YAML::EndSeq;
			out << YAML::Key << "Rotation"    << YAML::Value << YAML::Flow << YAML::BeginSeq << component.Rotation.x    << component.Rotation.y    << component.Rotation.z    << YAML::EndSeq;
			out << YAML::Key << "Scale"       << YAML::Value << YAML::Flow << YAML::BeginSeq << component.Scale.x       << component.Scale.y       << component.Scale.z       << YAML::EndSeq;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "Sprite Renderer Component" << YAML::Value << YAML::BeginMap;
		
			float4 color = entity.GetComponent<SpriteRendererComponent>().Color;
			out << YAML::Key << "Color" << YAML::Value << YAML::Flow << YAML::BeginSeq << color.r << color.g << color.b << color.a << YAML::EndSeq;
		
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "Camera Component" << YAML::Value << YAML::BeginMap;

			auto& component = entity.GetComponent<CameraComponent>();
			const char* projectionType = (component.Camera.GetProjectionType() == EProjectionType::Perspective) ? "Perspective" : "Orthographic";

			out << YAML::Key << "Scene Camera";
			out << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Projection Type" << YAML::Value << projectionType;
			out << YAML::Key << "Orthographic Size" << YAML::Value << component.Camera.GetOrthographicSize();
			out << YAML::Key << "Orthographic Near" << YAML::Value << component.Camera.GetOrthographicNearClip();
			out << YAML::Key << "Orthographic Far" << YAML::Value << component.Camera.GetOrthographicFarClip();
			out << YAML::Key << "Perspective FOV" << YAML::Value << component.Camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "Perspective Near" << YAML::Value << component.Camera.GetPerspectiveNearClip();
			out << YAML::Key << "Perspective Far" << YAML::Value << component.Camera.GetPerspectiveFarClip();
			out << YAML::Key << "Aspect Ratio" << YAML::Value << component.Camera.GetAspectRatio();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << component.Primary;
			out << YAML::Key << "Fixed Aspect Ratio" << YAML::Value << component.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "Native Script Component" << YAML::Value << YAML::BeginMap;

			auto& component = entity.GetComponent<NativeScriptComponent>();

			out << YAML::Key << "Class Name" << YAML::Value << component.ClassName;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: mScene(scene)
	{
	}

	void SceneSerializer::Serialize(const String& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		mScene->ForEachEntity([&](Entity entity)
		{
			SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}

	void SceneSerializer::Deserialize(const String& path)
	{
		YAML::Node file = YAML::LoadFile(path);

		auto sceneName = file["Scene"].as<String>();
		YAML::Node entities = file["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				uint64 uuid = entity["Entity"].as<uint64>();

				String name;
				auto tagComponent = entity["Tag Component"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deserializedEntity = mScene->CreateEntity(name);

				auto transformComponent = entity["Transform Component"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto spriteRendererComponent = entity["Sprite Renderer Component"];
				if (spriteRendererComponent)
				{
					auto& component = deserializedEntity.AddComponent<SpriteRendererComponent>();
					component.Color = spriteRendererComponent["Color"].as<float4>();
				}

				auto cameraComponent = entity["Camera Component"];
				if (cameraComponent)
				{
					auto& component = deserializedEntity.AddComponent<CameraComponent>();
					auto sceneCamera = cameraComponent["Scene Camera"];
					
					EProjectionType projType = (sceneCamera["Projection Type"].as<std::string>() == "Perspective") ? EProjectionType::Perspective : EProjectionType::Orthographic;
					float32 orthoSize = sceneCamera["Orthographic Size"].as<float>();
					float32 orthoNear = sceneCamera["Orthographic Near"].as<float>();
					float32 orthoFar  = sceneCamera["Orthographic Far"].as<float>();

					float32 persFOV  = sceneCamera["Perspective FOV"].as<float>();
					float32 persNear = sceneCamera["Perspective Near"].as<float>();
					float32 persFar  = sceneCamera["Perspective Far"].as<float>();

					component.Camera.SetOrthographicAndPerspective(projType, orthoSize, orthoNear, orthoFar, persFOV, persNear, persFar);

					component.Primary = cameraComponent["Primary"].as<bool>();
					component.FixedAspectRatio = cameraComponent["Fixed Aspect Ratio"].as<bool>();
				}

				auto nativeScriptComponent = entity["Native Script Component"];
				if (nativeScriptComponent)
				{
					auto& component = deserializedEntity.AddComponent<NativeScriptComponent>();
					component.ClassName = nativeScriptComponent["Class Name"].as<std::string>();
				}
			}
		}
	}

}

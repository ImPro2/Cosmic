#include "cspch.hpp"
#include "ECS/SceneSerializer.hpp"
#include <yaml-cpp/yaml.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <fstream>

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
		out << YAML::Key << "Entity" << YAML::Value << entity.GetComponent<EntityMetadataComponent>().ID;

		{
			out << YAML::Key << "EntityMetadataComponent" << YAML::Value << YAML::BeginMap;

			auto& metadata = entity.GetComponent<EntityMetadataComponent>();

			out << YAML::Key << "Tag"       << YAML::Value << metadata.Tag.c_str();
			out << YAML::Key << "IsVisible" << YAML::Value << metadata.IsVisible;

			out << YAML::Key << "ChildrenCount" << YAML::Value << metadata.ChildrenCount;
			out << YAML::Key << "FirstChildID"  << YAML::Value << (metadata.FirstChild ? metadata.FirstChild.GetComponent<EntityMetadataComponent>().ID : -1);
			out << YAML::Key << "NextID"        << YAML::Value << (metadata.Next       ? metadata.Next.GetComponent<EntityMetadataComponent>().ID       : -1);
			out << YAML::Key << "PrevID"        << YAML::Value << (metadata.Prev       ? metadata.Prev.GetComponent<EntityMetadataComponent>().ID       : -1);
			out << YAML::Key << "ParentID"      << YAML::Value << (metadata.Parent     ? metadata.Parent.GetComponent<EntityMetadataComponent>().ID     : -1);

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;

			auto& component = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "IsRelative" << YAML::Value << component.IsRelative;

			out << YAML::Key << "Translation" << YAML::Value << YAML::Flow << YAML::BeginSeq << component.Translation.x << component.Translation.y << component.Translation.z << YAML::EndSeq;
			out << YAML::Key << "Rotation"    << YAML::Value << YAML::Flow << YAML::BeginSeq << component.Rotation.x    << component.Rotation.y    << component.Rotation.z    << YAML::EndSeq;
			out << YAML::Key << "Scale"       << YAML::Value << YAML::Flow << YAML::BeginSeq << component.Scale.x       << component.Scale.y       << component.Scale.z       << YAML::EndSeq;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent" << YAML::Value << YAML::BeginMap;
		
			float4 color = entity.GetComponent<SpriteRendererComponent>().Color;
			out << YAML::Key << "Color" << YAML::Value << YAML::Flow << YAML::BeginSeq << color.r << color.g << color.b << color.a << YAML::EndSeq;
		
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;

			auto& component = entity.GetComponent<CameraComponent>();
			const char* projectionType = (component.Camera.GetProjectionType() == EProjectionType::Perspective) ? "Perspective" : "Orthographic";

			out << YAML::Key << "SceneCamera";

			out << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "ProjectionType"   << YAML::Value << projectionType;
			out << YAML::Key << "OrthographicSize" << YAML::Value << component.Camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << component.Camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar"  << YAML::Value << component.Camera.GetOrthographicFarClip();
			out << YAML::Key << "PerspectiveFOV"   << YAML::Value << component.Camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear"  << YAML::Value << component.Camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar"   << YAML::Value << component.Camera.GetPerspectiveFarClip();
			out << YAML::Key << "AspectRatio"      << YAML::Value << component.Camera.GetAspectRatio();
			out << YAML::EndMap;

			out << YAML::Key << "Primary"          << YAML::Value << component.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << component.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "NativeScriptComponent" << YAML::Value << YAML::BeginMap;

			auto& component = entity.GetComponent<NativeScriptComponent>();

			out << YAML::Key << "ClassName" << YAML::Value << component.ClassName;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: mScene(scene)
	{
	}

	void SceneSerializer::Serialize(const File& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Scene"    << YAML::Value << path.GetName().c_str();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		mScene->ForEachRootEntity([&](Entity entity)
		{
			SerializeEntity(out, entity);

			mScene->ForEachChildRecurseBottomUp(entity, [&](Entity entity)
			{
				SerializeEntity(out, entity);
			});
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path.GetAbsolutePath());
		fout << out.c_str();
		fout.close();
	}

	void SceneSerializer::Deserialize(const File& path)
	{
		YAML::Node file = YAML::LoadFile(path.GetAbsolutePath().GetString());

		auto sceneName = file["Scene"].as<String>();
		YAML::Node entities = file["Entities"];

		if (entities)
		{
			EntityIDMetadataMap idMetadataMap;

			for (auto entity : entities)
			{
				auto entityMetadataComponent = entity["EntityMetadataComponent"];

				EntityMetadataComponent metadata;
				metadata.ID            = entity["Entity"].as<int32>();
				metadata.Tag           = entityMetadataComponent["Tag"].as<String>();
				metadata.IsVisible     = entityMetadataComponent["IsVisible"].as<bool>();
				metadata.ChildrenCount = entityMetadataComponent["ChildrenCount"].as<size_t>();

				EntityIDMetadata idMetadata;
				idMetadata.FirstChildID = entityMetadataComponent["FirstChildID"].as<int32>();
				idMetadata.NextID       = entityMetadataComponent["NextID"].as<int32>();
				idMetadata.PrevID       = entityMetadataComponent["PrevID"].as<int32>();
				idMetadata.ParentID     = entityMetadataComponent["ParentID"].as<int32>();

				Entity deserializedEntity = mScene->CreateSerializedEntity(metadata, idMetadata, idMetadataMap);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();

					tc.IsRelative  = transformComponent["IsRelative"].as<bool>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation    = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale       = transformComponent["Scale"].as<glm::vec3>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& component = deserializedEntity.AddComponent<SpriteRendererComponent>();

					component.Color = spriteRendererComponent["Color"].as<float4>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& component = deserializedEntity.AddComponent<CameraComponent>();
					auto sceneCamera = cameraComponent["SceneCamera"];
					
					EProjectionType projType = (sceneCamera["ProjectionType"].as<std::string>() == "Perspective") ? EProjectionType::Perspective : EProjectionType::Orthographic;
					float32 orthoSize = sceneCamera["OrthographicSize"].as<float32>();
					float32 orthoNear = sceneCamera["OrthographicNear"].as<float32>();
					float32 orthoFar  = sceneCamera["OrthographicFar"].as<float32>();

					float32 persFOV  = sceneCamera["PerspectiveFOV"].as<float32>();
					float32 persNear = sceneCamera["PerspectiveNear"].as<float32>();
					float32 persFar  = sceneCamera["PerspectiveFar"].as<float32>();

					component.Camera.SetOrthographicAndPerspective(projType, orthoSize, orthoNear, orthoFar, persFOV, persNear, persFar);

					component.Primary          = cameraComponent["Primary"].as<bool>();
					component.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto nativeScriptComponent = entity["NativeScriptComponent"];
				if (nativeScriptComponent)
				{
					auto& component = deserializedEntity.AddComponent<NativeScriptComponent>();

					component.ClassName  = nativeScriptComponent["ClassName"].as<String>();
				}
			}

			mScene->RegisterSerializedEntities(idMetadataMap);

#if 0
			mScene->ForEachEntity([this, idMetadataMap](Entity entity)
			{
				auto& metadata = entity.GetComponent<EntityMetadataComponent>();
				const EntityIDMetadata& idMetadata = idMetadataMap.at(metadata.ID);

				metadata.FirstChild = mScene->FindEntityByID(idMetadata.FirstChildID);
				metadata.Next       = mScene->FindEntityByID(idMetadata.NextID);
				metadata.Prev       = mScene->FindEntityByID(idMetadata.PrevID);
				metadata.Parent     = mScene->FindEntityByID(idMetadata.ParentID);

				mScene->RegisterSerializedEntity(entity);
			});
#endif
		}
	}

}

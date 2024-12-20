#include "cspch.hpp"
#include "Entity.hpp"
#include "Components.hpp"

namespace Cosmic
{

	void Entity::AddComponent(EComponentType type, IComponent* component)
	{
		switch (type)
		{
			case EComponentType::EntityMetadata: AddComponent<EntityMetadataComponent>(*static_cast<EntityMetadataComponent*>(component)); break;
			case EComponentType::Transform:      AddComponent<TransformComponent>     (*static_cast<TransformComponent*>     (component)); break;
			case EComponentType::SpriteRenderer: AddComponent<SpriteRendererComponent>(*static_cast<SpriteRendererComponent*>(component)); break;
			case EComponentType::Camera:         AddComponent<CameraComponent>        (*static_cast<CameraComponent*>        (component)); break;
			case EComponentType::NativeScript:   AddComponent<NativeScriptComponent>  (*static_cast<NativeScriptComponent*>  (component)); break;
		}
	}

	void Entity::RemoveComponent(EComponentType type)
	{
		switch (type)
		{
			case EComponentType::EntityMetadata: RemoveComponent<EntityMetadataComponent>(); break;
			case EComponentType::Transform:      RemoveComponent<TransformComponent>();      break;
			case EComponentType::SpriteRenderer: RemoveComponent<SpriteRendererComponent>(); break;
			case EComponentType::Camera:         RemoveComponent<CameraComponent>();         break;
			case EComponentType::NativeScript:   RemoveComponent<NativeScriptComponent>();   break;
		}
	}


	IComponent* Entity::GetComponent(EComponentType type)
	{
		switch (type)
		{
			case EComponentType::EntityMetadata: return static_cast<IComponent*>(&GetComponent<EntityMetadataComponent>());
			case EComponentType::Transform:      return static_cast<IComponent*>(&GetComponent<TransformComponent>());
			case EComponentType::SpriteRenderer: return static_cast<IComponent*>(&GetComponent<SpriteRendererComponent>());
			case EComponentType::Camera:         return static_cast<IComponent*>(&GetComponent<CameraComponent>());
			case EComponentType::NativeScript:   return static_cast<IComponent*>(&GetComponent<NativeScriptComponent>());
		}
	}

	bool Entity::HasComponent(EComponentType type)
	{
		switch (type)
		{
			case EComponentType::EntityMetadata: return HasComponent<EntityMetadataComponent>();
			case EComponentType::Transform:      return HasComponent<TransformComponent>();
			case EComponentType::SpriteRenderer: return HasComponent<SpriteRendererComponent>();
			case EComponentType::Camera:         return HasComponent<CameraComponent>();
			case EComponentType::NativeScript:   return HasComponent<NativeScriptComponent>();
		}
	}

}

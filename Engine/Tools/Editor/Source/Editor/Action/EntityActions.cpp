#include "cspch.hpp"
#include "EntityActions.hpp"
#include "App/Module.hpp"
#include "Editor/Panels/SceneHierarchyPanel.hpp"

namespace Cosmic
{

	IEntityAction::IEntityAction(const Vector<Entity>& entities, const Ref<Scene>& scene)
		: mScene(scene)
	{
		mSceneCopy = CreateRef<Scene>();

		// Copy the entities' data if it was removed from the original scene and needs to be added again e.g.
		for (Entity entity : entities)
			mSceneCopy->AddEntity(entity);
	}

	void EntityAddedAction::Undo()
	{
		Ref<SceneHierarchyPanel> sceneHierarchyPanel = ModuleSystem::Get<SceneHierarchyPanel>();
		sceneHierarchyPanel->ClearSelectedEntities();

		mSceneCopy->ForEachEntity([this](Entity entity)
		{
			mScene->RemoveEntity(entity);
		});
	}

	void EntityAddedAction::Redo()
	{
		Ref<SceneHierarchyPanel> sceneHierarchyPanel = ModuleSystem::Get<SceneHierarchyPanel>();
		Vector<Entity> entities;

		mSceneCopy->ForEachEntity([&](Entity entity)
		{
			entities.push_back(mScene->AddEntity(entity));
		});

		sceneHierarchyPanel->SetSelectedEntities(entities);
	}

	void EntityRemovedAction::Undo()
	{
		Ref<SceneHierarchyPanel> sceneHierarchyPanel = ModuleSystem::Get<SceneHierarchyPanel>();
		Vector<Entity> entities;

		mSceneCopy->ForEachEntity([&](Entity entity)
		{
			entities.push_back(mScene->AddEntity(entity));
		});

		sceneHierarchyPanel->SetSelectedEntities(entities);
	}

	void EntityRemovedAction::Redo()
	{
		Ref<SceneHierarchyPanel> sceneHierarchyPanel = ModuleSystem::Get<SceneHierarchyPanel>();
		sceneHierarchyPanel->ClearSelectedEntities();

		mSceneCopy->ForEachEntity([this](Entity entity)
		{
			mScene->RemoveEntity(entity);
		});
	}

}

#pragma once
#include "Base/Base.hpp"
#include "ECS/Scene.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Components.hpp"
#include "IEditorAction.hpp"

namespace Cosmic
{

	class IEntityAction : public IEditorAction
	{
	public:
		IEntityAction(const Vector<Entity>& entities, const Ref<Scene>& scene);

	protected:
		Ref<Scene> mScene;
		Ref<Scene> mSceneCopy;
	};

	class EntityAddedAction : public IEntityAction
	{
	public:
		EntityAddedAction(const Vector<Entity>& entities, const Ref<Scene>& scene)
			: IEntityAction(entities, scene)
		{
		}

	public:
		virtual void Undo() override;
		virtual void Redo() override;
	};

	class EntityRemovedAction : public IEntityAction
	{
	public:
		EntityRemovedAction(const Vector<Entity>& entities, const Ref<Scene>& scene)
			: IEntityAction(entities, scene)
		{
		}

	public:
		virtual void Undo() override;
		virtual void Redo() override;
	};

}

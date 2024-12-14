#pragma once
#include "Editor/Event/EditorEvents.hpp"
#include "Project/Project.hpp"

namespace Cosmic
{

	struct IProjectEvent : public IEvent
	{
		IProjectEvent(const WeakRef<Project>& project)
			: mProject(project)
		{
		}

		const WeakRef<Project>& GetProject() const { return mProject; }

	private:
		WeakRef<Project> mProject;
	};

	struct ProjectSavedEvent : public IProjectEvent
	{
		ProjectSavedEvent(const WeakRef<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectSaved);
	};

	struct ProjectSavedAsEvent : public IProjectEvent
	{
		ProjectSavedAsEvent(const WeakRef<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectSavedAs);
	};

	struct ProjectOpenedEvent : public IProjectEvent
	{
		ProjectOpenedEvent(const WeakRef<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectOpened);
	};

	struct ProjectNewEvent : public IProjectEvent
	{
		ProjectNewEvent(const WeakRef<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectNew);
	};

}

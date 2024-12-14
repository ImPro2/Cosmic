#pragma once
#include "Editor/Event/EditorEvents.hpp"
#include "Project/Project.hpp"

namespace Cosmic
{

	struct IProjectEvent : public IEvent
	{
		IProjectEvent(const Ref<Project>& project)
			: mProject(project)
		{
		}

		const Ref<Project>& GetProject() const { return mProject; }

	private:
		Ref<Project> mProject;
	};

	struct ProjectSavedEvent : public IProjectEvent
	{
		ProjectSavedEvent(const Ref<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectSaved);
	};

	struct ProjectSavedAsEvent : public IProjectEvent
	{
		ProjectSavedAsEvent(const Ref<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectSavedAs);
	};

	struct ProjectOpenedEvent : public IProjectEvent
	{
		ProjectOpenedEvent(const Ref<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectOpened);
	};

	struct ProjectNewEvent : public IProjectEvent
	{
		ProjectNewEvent(const Ref<Project>& project)
			: IProjectEvent(project)
		{
		}

		CS_EVENT_TYPE(EEditorEvent::ProjectNew);
	};

}

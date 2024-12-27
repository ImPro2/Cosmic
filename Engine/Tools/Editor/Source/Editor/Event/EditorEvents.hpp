#pragma once
#include "Event/IEvent.hpp"
#include "Base/Types.hpp"

namespace Cosmic
{

    enum class EEditorEvent : int16
    {
        SceneSaved = (int16)EEventType::Last, SceneSavedAs, SceneOpened, SceneNew, ScenePlay, SceneStop,
        ProjectSaved, ProjectSavedAs, ProjectOpened, ProjectNew,
        EntityAdded, EntityRemoved,
        ComponentAdded, ComponentRemoved, ComponentModified
    };

}

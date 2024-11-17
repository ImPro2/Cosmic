#pragma once
#include "App/Event/Events.hpp"
#include "App/Path.hpp"
#include "App/File.hpp"

namespace Cosmic
{

    struct DirectoryEvent : public Event
    {
        DirectoryEvent(const Path& dir)
            : mDirectory(dir)
        {
        }

        Path GetDirectory() const { return mDirectory; }

    private:
        Path mDirectory;
    };

	struct DirectoryAddedEvent : public DirectoryEvent
	{
		DirectoryAddedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryAdded);
	};

	struct DirectoryRemovedEvent : public DirectoryEvent
	{
		DirectoryRemovedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryRemoved);
	};

	struct DirectoryRenamedEvent : public DirectoryEvent
	{
		DirectoryRenamedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryRenamed);
	};

	struct DirectoryModifiedEvent : public DirectoryEvent
	{
		DirectoryModifiedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryModified);
	};

	struct FileEvent : public Event
	{
		FileEvent(const File& file)
			: mFile(file)
		{
		}

		File GetFile() const { return mFile; }

	private:
		File mFile;
	};

	struct FileAddedEvent : public FileEvent
	{
		FileAddedEvent(const File& file)
			: FileEvent(file)
		{
		}

        CS_EVENT_TYPE(EEventType::FileAdded);
	};

	struct FileRemovedEvent : public FileEvent
	{
		FileRemovedEvent(const File& file)
			: FileEvent(file)
		{
		}

        CS_EVENT_TYPE(EEventType::FileRemoved);
	};

	struct FileRenamedEvent : public FileEvent
	{
		FileRenamedEvent(const File& file)
			: FileEvent(file)
		{
		}
        
        CS_EVENT_TYPE(EEventType::FileRenamed);
	};

	struct FileModifiedEvent : public FileEvent
	{
	public:
		FileModifiedEvent(const File& file)
			: FileEvent(file)
		{
		}

        CS_EVENT_TYPE(EEventType::FileModified);
	};

}

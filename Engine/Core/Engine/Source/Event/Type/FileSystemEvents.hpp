#pragma once
#include "Event/IEvent.hpp"
#include "App/Path.hpp"
#include "App/File.hpp"

namespace Cosmic
{

    struct IDirectoryEvent : public IEvent
    {
        IDirectoryEvent(const Path& dir)
            : mDirectory(dir)
        {
        }

        Path GetDirectory() const { return mDirectory; }

    private:
        Path mDirectory;
    };

	struct DirectoryAddedEvent : public IDirectoryEvent
	{
		DirectoryAddedEvent(const Path& dir)
			: IDirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryAdded);
	};

	struct DirectoryRemovedEvent : public IDirectoryEvent
	{
		DirectoryRemovedEvent(const Path& dir)
			: IDirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryRemoved);
	};

	struct DirectoryRenamedEvent : public IDirectoryEvent
	{
		DirectoryRenamedEvent(const Path& dir)
			: IDirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryRenamed);
	};

	struct DirectoryModifiedEvent : public IDirectoryEvent
	{
		DirectoryModifiedEvent(const Path& dir)
			: IDirectoryEvent(dir)
		{
		}

        CS_EVENT_TYPE(EEventType::DirectoryModified);
	};

	struct IFileEvent : public IEvent
	{
		IFileEvent(const File& file)
			: mFile(file)
		{
		}

		File GetFile() const { return mFile; }

	private:
		File mFile;
	};

	struct FileAddedEvent : public IFileEvent
	{
		FileAddedEvent(const File& file)
			: IFileEvent(file)
		{
		}

        CS_EVENT_TYPE(EEventType::FileAdded);
	};

	struct FileRemovedEvent : public IFileEvent
	{
		FileRemovedEvent(const File& file)
			: IFileEvent(file)
		{
		}

        CS_EVENT_TYPE(EEventType::FileRemoved);
	};

	struct FileRenamedEvent : public IFileEvent
	{
		FileRenamedEvent(const File& file)
			: IFileEvent(file)
		{
		}
        
        CS_EVENT_TYPE(EEventType::FileRenamed);
	};

	struct FileModifiedEvent : public IFileEvent
	{
	public:
		FileModifiedEvent(const File& file)
			: IFileEvent(file)
		{
		}

        CS_EVENT_TYPE(EEventType::FileModified);
	};

}

#pragma once
#include "App/Event/Events.hpp"
#include "App/Path.hpp"
#include "App/File.hpp"

namespace Cosmic
{

    struct DirectoryEvent : public Event
    {
    public:
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
	public:
		DirectoryAddedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

		       EEventType GetType() const { return EEventType::DirectoryAdded; }
		static EEventType GetStaticType() { return EEventType::DirectoryAdded; }
	};

	struct DirectoryRemovedEvent : public DirectoryEvent
	{
	public:
		DirectoryRemovedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

		       EEventType GetType() const { return EEventType::DirectoryRemoved; }
		static EEventType GetStaticType() { return EEventType::DirectoryRemoved; }
	};

	struct DirectoryRenamedEvent : public DirectoryEvent
	{
	public:
		DirectoryRenamedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

		       EEventType GetType() const { return EEventType::DirectoryRenamed; }
		static EEventType GetStaticType() { return EEventType::DirectoryRenamed; }
	};

	struct DirectoryModifiedEvent : public DirectoryEvent
	{
	public:
		DirectoryModifiedEvent(const Path& dir)
			: DirectoryEvent(dir)
		{
		}

		       EEventType GetType() const { return EEventType::DirectoryModified; }
		static EEventType GetStaticType() { return EEventType::DirectoryModified; }
	};

	struct FileEvent : public Event
	{
	public:
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
	public:
		FileAddedEvent(const File& file)
			: FileEvent(file)
		{
		}

		       EEventType GetType() const { return EEventType::FileAdded; }
		static EEventType GetStaticType() { return EEventType::FileAdded; }
	};

	struct FileRemovedEvent : public FileEvent
	{
	public:
		FileRemovedEvent(const File& file)
			: FileEvent(file)
		{
		}

		       EEventType GetType() const { return EEventType::FileRemoved; }
		static EEventType GetStaticType() { return EEventType::FileRemoved; }
	};

	struct FileRenamedEvent : public FileEvent
	{
	public:
		FileRenamedEvent(const File& file)
			: FileEvent(file)
		{
		}

		       EEventType GetType() const { return EEventType::FileRenamed; }
		static EEventType GetStaticType() { return EEventType::FileRenamed; }
	};

	struct FileModifiedEvent : public FileEvent
	{
	public:
		FileModifiedEvent(const File& file)
			: FileEvent(file)
		{
		}

		       EEventType GetType() const { return EEventType::FileModified; }
		static EEventType GetStaticType() { return EEventType::FileModified; }
	};

}

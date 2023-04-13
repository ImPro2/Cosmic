module;
#include "cspch.hpp"
export module Cosmic.App.FileSystemEvents;

import Cosmic.App.Events;
import Cosmic.App.File;

namespace Cosmic
{

	export struct FileEvent : public Event
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

	export struct FileAddedEvent : public FileEvent
	{
	public:
		FileAddedEvent(const File& file)
			: FileEvent(file)
		{
		}

		       EEventType GetType() const { return EEventType::FileAdded; }
		static EEventType GetStaticType() { return EEventType::FileAdded; }
	};

	export struct FileRemovedEvent : public FileEvent
	{
	public:
		FileRemovedEvent(const File& file)
			: FileEvent(file)
		{
		}

		       EEventType GetType() const { return EEventType::FileRemoved; }
		static EEventType GetStaticType() { return EEventType::FileRemoved; }
	};

	export struct FileRenamedEvent : public FileEvent
	{
	public:
		FileRenamedEvent(const File& file)
			: FileEvent(file)
		{
		}

		       EEventType GetType() const { return EEventType::FileRenamed; }
		static EEventType GetStaticType() { return EEventType::FileRenamed; }
	};

	export struct FileModifiedEvent : public FileEvent
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

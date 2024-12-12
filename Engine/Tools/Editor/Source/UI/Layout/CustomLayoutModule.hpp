#pragma once
#include "App/Module.hpp"

#include <functional>

namespace Cosmic
{

	using CustomLayoutNameEnteredCallback = std::function<void(const String&)>;

	class CustomLayoutModule : public IModule
	{
	public:
		void OnImGuiRender() override;

	public:
		void OnNameEntered(CustomLayoutNameEnteredCallback callback);

	private:
		void Close(bool cancelled);

	private:
		CustomLayoutNameEnteredCallback mCallback;

		char mInputBuffer[256];

		inline static const String sWindowName = "Save Custom Layout...";
	};

}

#pragma once
#include "UI/Layout/Layout.hpp"
#include "App/Path.hpp"

#include <yaml-cpp/yaml.h>

namespace Cosmic
{

	class LayoutSerializer
	{
	public:
		LayoutSerializer(Vector<Layout>& layouts);

	public:
		bool Serialize(const Path& path);
		bool Deserialize(const Path& path);

	private:
		bool SerializeLayout(YAML::Emitter& out, Layout& layout);
		bool DeserializeLayout(YAML::Node& layoutMap, Layout& layout);

	private:
		Vector<Layout>& mLayouts;
		Path            mPath;
	};

}

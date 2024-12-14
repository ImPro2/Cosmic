#include "cspch.hpp"
#include "LayoutSerializer.hpp"

#include "App/File.hpp"
#include "Memory/SmartPtrs.hpp"
#include "Editor/Panels/Panels.hpp"

#include "Editor/EditorModule.hpp"
#include "Editor/Panels/Panels.hpp"

#include <fstream>

namespace Cosmic
{

	namespace Utils
	{

		template<typename F>
		static void IterDockNodeSerialize(YAML::Emitter& out, DockNode* node, F callback)
		{
			callback(node);

			if (node->Child1)
			{
				out << YAML::Key << "Child 1" << YAML::Value << YAML::BeginMap;
				IterDockNodeSerialize(out, node->Child1, callback);
				out << YAML::EndMap;
			}

			if (node->Child2)
			{
				out << YAML::Key << "Child 2" << YAML::Value << YAML::BeginMap;
				IterDockNodeSerialize(out, node->Child2, callback);
				out << YAML::EndMap;
			}
		}

		template<typename F>
		static void IterDockNodeDeserialize(YAML::Node& yamlNode, DockNode* dockNode, F callback)
		{
			callback(yamlNode, dockNode);

			if (YAML::Node child1 = yamlNode["Child 1"]; child1.IsDefined())
			{
				dockNode->Child1 = DockNode::Allocate();
				dockNode->Child1->Parent = dockNode;

				IterDockNodeDeserialize(child1, dockNode->Child1, callback);
			}

			if (YAML::Node child2 = yamlNode["Child 2"]; child2.IsDefined())
			{
				dockNode->Child2 = DockNode::Allocate();
				dockNode->Child2->Parent = dockNode;

				IterDockNodeDeserialize(child2, dockNode->Child2, callback);
			}
		}

	}

	LayoutSerializer::LayoutSerializer(Vector<Layout>& layouts)
		: mLayouts(layouts)
	{
	}

	bool LayoutSerializer::Serialize(const Path& path)
	{
		mPath = path;

		YAML::Emitter out;
		out << YAML::BeginSeq;

		for (Layout& layout : mLayouts)
			SerializeLayout(out, layout);

		out << YAML::EndSeq;

		std::ofstream of(path.GetString());
		of << out.c_str();
		of.close();

		return true;
	}

	bool LayoutSerializer::SerializeLayout(YAML::Emitter& out, Layout& layout)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Layout" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "Name" << YAML::Value << layout.GetName();
		out << YAML::Key << "Root" << YAML::Value << YAML::BeginMap;

		Utils::IterDockNodeSerialize(out, layout.GetRootNode(), [&](DockNode* node)
		{
			out << YAML::Key << "ID" << YAML::Value << node->ID;

			switch (node->SplitDir)
			{
				case EDockSplitDir::None:
				{
					if (Ref<IPanel> panel = node->Panel.Own())
						out << YAML::Key << "PanelName" << YAML::Value << panel->GetPanelName();
					break;
				}
				case EDockSplitDir::Stack:
				{
					out << YAML::Key << "SplitMode" << YAML::Value << EDockSplitDirToStr(node->SplitDir);
					break;
				}
				default:
				{
					out << YAML::Key << "SplitMode" << YAML::Value << EDockSplitDirToStr(node->SplitDir);
					out << YAML::Key << "SplitPercent" << YAML::Value << node->SplitPercent;
					break;
				}
			}
		});

		out << YAML::EndMap;
		out << YAML::EndMap;
		out << YAML::EndMap;

		return true;
	}

	bool LayoutSerializer::Deserialize(const Path& path)
	{
		mLayouts.clear();

		if (!FileSystem::Exists(path))
			FileSystem::CreateFile(path);

		YAML::Node file;

		try
		{
			file = YAML::LoadFile(path);
		}
		catch (YAML::Exception& e)
		{
			return false;
		}

		for (YAML::Node layoutMap : file)
		{
			mLayouts.emplace_back();

			if (!DeserializeLayout(layoutMap, mLayouts[mLayouts.size() - 1]))
				return false;
		}

		return true;
	}

	bool LayoutSerializer::DeserializeLayout(YAML::Node& layoutMap, Layout& layout)
	{
		layoutMap = layoutMap["Layout"];

		const Panels& panels = ModuleSystem::Get<EditorModule>()->GetPanels();

		YAML::Node yamlRoot = layoutMap["Root"];
		DockNode* dockNodeRoot = DockNode::Allocate();

		Utils::IterDockNodeDeserialize(yamlRoot, dockNodeRoot, [&](YAML::Node& yamlNode, DockNode* dockNode)
		{
			dockNode->ID = yamlNode["ID"].as<uint32>();

			if (YAML::Node panelName = yamlNode["PanelName"]; panelName.IsDefined())
			{
				dockNode->Panel = panels.FindPanelByName(panelName.as<std::string>());
			}
			else if (YAML::Node splitMode = yamlNode["SplitMode"]; splitMode.IsDefined())
			{
				dockNode->SplitDir = EDockSplitDirFromStr(splitMode.as<std::string>().c_str());

				if (dockNode->SplitDir != EDockSplitDir::Stack)
				{
					dockNode->SplitPercent = yamlNode["SplitPercent"].as<float32>();
				}
			}
		});

		layout.SetName(layoutMap["Name"].as<std::string>());
		layout.SetRootNode(dockNodeRoot);

		return true;
	}

}

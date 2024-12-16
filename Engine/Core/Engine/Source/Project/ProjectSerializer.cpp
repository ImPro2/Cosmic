#include "App/Log/LogMacros.hpp"
#include "Base/Macros.hpp"
#include "cspch.hpp"
#include "ProjectSerializer.hpp"

#include <mutex>
#include <fstream>
#include <yaml-cpp/yaml.h>

CS_MODULE_LOG_INFO(Cosmic, Project.ProjectSerializer);

namespace Cosmic
{

    ProjectSerializer::ProjectSerializer(Ref<Project> project)
        : mProject(project)
    {
    }

    void ProjectSerializer::Serialize(const Path& path)
    {
        const ProjectInfo& info = mProject->GetInfo();

        YAML::Emitter out;
        out << YAML::BeginMap;

        out << YAML::Key << "StartScenePath"     << YAML::Value << info.StartScenePath.GetAbsolutePath().GetString();
        out << YAML::Key << "ScriptAssemblyPath" << YAML::Value << info.ScriptAssemblyPath.GetAbsolutePath().GetString();
        out << YAML::Key << "AssetsDirectory"    << YAML::Value << info.AssetsDirectory.GetString();

        out << YAML::EndMap;

        std::ofstream fout(path);
        fout << out.c_str();
        fout.close();
    }

    bool ProjectSerializer::Deserialize(const Path& path)
    {
        ProjectInfo& info = mProject->GetInfo();

        YAML::Node file;

        try
        {
            file = YAML::LoadFile(path);
        }
        catch (YAML::ParserException e)
        {
            CS_LOG_ERROR("Failed to deserialize project file {}. {}", path.GetString().c_str(), e.what());
            return false;
        }

        info.ProjectFilePath    = path;
        info.StartScenePath     = Path(file["StartScenePath"].as<String>());
        info.ScriptAssemblyPath = Path(file["ScriptAssemblyPath"].as<String>());
        info.AssetsDirectory    = file["AssetsDirectory"].as<String>();

        return true;
    }

}

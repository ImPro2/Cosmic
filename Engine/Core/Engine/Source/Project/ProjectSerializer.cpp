#include "App/Log/LogMacros.hpp"
#include "Base/Macros.hpp"
#include "cspch.hpp"
#include "ProjectSerializer.hpp"

#include <mutex>
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

        out << YAML::Key << "Project Name" << YAML::Value << info.ProjectName;
        out << YAML::Key << "Start Scene Path" << YAML::Value << info.StartScenePath.GetAbsolutePath().GetString();
        out << YAML::Key << "Assets Directory" << YAML::Value << info.AssetsDirectory.GetString();

        out << YAML::EndMap;

        File file = File(path);
        file.Write(out.c_str());
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

        info.ProjectFilePath = path;
        info.ProjectName     = file["Project Name"].as<String>();
        info.StartScenePath  = Path(file["Start Scene Path"].as<String>());
        info.AssetsDirectory = file["Assets Directory"].as<String>();

        return true;
    }

}

/*************************************************************************************************************************************
* Title: serialize.cpp
* Author: Jennifer Assid
* Date of Creation: 19 Month 20XX
*
**************************************************************************************************************************************/
#include "serialize.h"

#include <fstream>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include "error_logging.h"

Serialize::Serialize(std::string filepath) : filepath_(filepath), parsed_(false)
{
    std::ifstream ifs(filepath.c_str());

    if (ifs.is_open() == false)
    {
        std::ofstream ofs(filepath.c_str());

        if (ofs.is_open() == false)
        {
            Error_Logging::Get_Instance()->Record_Message(
                Error_Logging::Get_Instance()->Format_Output("Failed to create file \"%s\"",
                    filepath.c_str()),
                Error_Logging::Message_Level::ot_Error,
                "Serialize",
                "Serialize");
            return;
        }

        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Created file \"%s\"",
                filepath.c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Serialize",
            "Serialize");

        ofs << "{\n}";

        ofs.close();
        ifs.open(filepath.c_str());

        if (ifs.is_open() == false)
        {
            Error_Logging::Get_Instance()->Record_Message(
                Error_Logging::Get_Instance()->Format_Output("Failed to open file \"%s\"",
                    filepath.c_str()),
                Error_Logging::Message_Level::ot_Error,
                "Serialize",
                "Serialize");
            return;
        }

        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Openned file \"%s\"",
                filepath.c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Serialize",
            "Serialize");
    }

    rapidjson::IStreamWrapper isw(ifs);

    if (document_.ParseStream(isw).HasParseError())
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Failed to parse file (empty) \"%s\"",
                filepath.c_str()),
            Error_Logging::Message_Level::ot_Error,
            "Serialize",
            "Serialize");
    }
    else
    {
        Error_Logging::Get_Instance()->Record_Message(
            Error_Logging::Get_Instance()->Format_Output("Successfully parsed file \"%s\"",
                filepath.c_str()),
            Error_Logging::Message_Level::ot_Information,
            "Serialize",
            "Serialize");
        parsed_ = true;
    }

    ifs.close();
}

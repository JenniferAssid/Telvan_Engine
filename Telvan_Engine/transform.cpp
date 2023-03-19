#include "transform.h"

void Transform::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
    writer.Key("transform");
    writer.StartObject();

    writer.Key("translation");
    writer.StartArray();
    writer.Double(translation_.x);
    writer.Double(translation_.y);
    writer.Double(translation_.z);
    writer.EndArray();

    writer.Key("scale");
    writer.StartArray();
    writer.Double(scale_.x);
    writer.Double(scale_.y);
    writer.EndArray();

    writer.Key("rotation");
    writer.Double(rotation_);

    writer.EndObject();
}

void Transform::Read_From(rapidjson::Document& document)
{
    if (document.HasMember("transform") == false) return;

    if (document["transform"].GetObject().HasMember("translation") &&
        document["transform"]["translation"].IsArray())
    {
        const rapidjson::Value& translation = document["transform"]["translation"];

        translation_.x = (float)translation[0].GetDouble();
        translation_.y = (float)translation[1].GetDouble();
        translation_.z = (float)translation[2].GetDouble();
    }

    if (document["transform"].GetObject().HasMember("scale") &&
        document["transform"]["scale"].IsArray())
    {
        const rapidjson::Value& scale = document["transform"]["scale"];

        scale_.x = (float)scale[0].GetDouble();
        scale_.y = (float)scale[1].GetDouble();
    }

    if (document["transform"].GetObject().HasMember("rotation") &&
        document["transform"]["rotation"].IsDouble())
    {
        rotation_ = (float)document["transform"]["rotation"].GetDouble();
    }
}

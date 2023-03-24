#include "transform.h"

void Transform::write_to(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
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

Component* Transform::Clone()
{
    Transform* transform = new Transform();
    transform->translation_ = translation_;
    transform->scale_ = scale_;
    transform->rotation_ = rotation_;

    return (Component*)transform;
}

void Transform::Write_To(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, bool preserve_values)
{
    if (preserve_values == true)
    {
        write_to(writer);
        return;
    }

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
    writer.Double(-1.0);
    writer.Double(-1.0);
    writer.EndArray();

    writer.Key("rotation");
    writer.Double(0.0);

    writer.EndObject();
}

// TODO: Switch the document intake into an object intake?
void Transform::Read_From(rapidjson::GenericObject<false, rapidjson::Value>& writer)
{
    if (writer.HasMember("transform") == false) return;

    if (writer["transform"].GetObject().HasMember("translation") &&
        writer["transform"]["translation"].IsArray())
    {
        const rapidjson::Value& translation = writer["transform"]["translation"];

        translation_.x = (float)translation[0].GetDouble();
        translation_.y = (float)translation[1].GetDouble();
        translation_.z = (float)translation[2].GetDouble();
    }

    if (writer["transform"].GetObject().HasMember("scale") &&
        writer["transform"]["scale"].IsArray())
    {
        const rapidjson::Value& scale = writer["transform"]["scale"];

        scale_.x = (float)scale[0].GetDouble();
        scale_.y = (float)scale[1].GetDouble();
    }

    if (writer["transform"].GetObject().HasMember("rotation") &&
        writer["transform"]["rotation"].IsDouble())
    {
        rotation_ = (float)writer["transform"]["rotation"].GetDouble();
    }
}

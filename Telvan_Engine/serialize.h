#pragma once

#ifndef SERIALIZE_CLASS_H
#define SERIALIZE_CLASS_H

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <string>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>

class Serialize
{
private:
    std::string filepath_;
    bool parsed_;

public:
    Serialize(std::string filepath);

    rapidjson::Document document_;

    /*template <typename Writer>
    void Write_To(Writer& writer, std::string key, std::string value);
    template <typename Writer>
    void Write_To(Writer& writer, std::string key, bool value);
    template <typename Writer>
    void Write_To(Writer& writer, std::string key, int value);
    template <typename Writer>
    void Write_To(Writer& writer, std::string key, float value);
    template <typename Writer>
    void Write_To(Writer& writer, std::string key, double value);
    template <typename Writer>
    void Write_To(Writer& writer, std::string key, glm::vec2 value);
    template <typename Writer>
    void Write_To(Writer& writer, std::string key, glm::vec3 value);*/
};

#endif // !SERIALIZE_CLASS_H

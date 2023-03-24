#pragma once

#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <string>
#include <filesystem>

class Texture
{
public:
    unsigned int ID;
    std::string Name;

    unsigned int Width, Height;

    unsigned int Internal_Format;
    unsigned int Image_Format;

    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;

public:
    Texture();
    Texture(std::filesystem::path path);

    void Generate(unsigned int width,
        unsigned int height,
        unsigned char* data);

    void Bind() const;
};

#endif
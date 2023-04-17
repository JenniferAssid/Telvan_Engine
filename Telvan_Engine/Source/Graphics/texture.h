/*************************************************************************************************************************************
* Title: texture.h
* Author: Jennifer Assid
* Date of Creation: 13 March 2023
*
**************************************************************************************************************************************/
#pragma once

#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <string>
#include <filesystem>

class Texture
{
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
protected:
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

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
public:
    Texture();
    Texture(std::filesystem::path path);

    void Generate(unsigned int width,
        unsigned int height,
        unsigned char* data);

    void Bind() const;
};

#endif
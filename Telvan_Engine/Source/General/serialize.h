/*************************************************************************************************************************************
* Title: serialize.h
* Author: Jennifer Assid
* Date of Creation: 19 Month 20XX
*
**************************************************************************************************************************************/
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
/**************************************************** ENUMS & VARIABLE CREATION ******************************************************/
public:

/************************************************************ VARIABLES **************************************************************/
private:
    std::string filepath_;
    bool parsed_;

protected:
public:
    rapidjson::Document document_;

/************************************************************ FUNCTIONS **************************************************************/
private:
protected:
public:
    Serialize(std::string filepath);
};

#endif // !SERIALIZE_CLASS_H

/**
 * Project   AlgoVi
 *
 * @file     TProgLanguages.hpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#pragma once

#include <string>

namespace NCommand
{

struct ProgLanguage
{
    enum EType
    {
        CPP,
        JAVA,
        PYTHON,
        BINARY,
    };
};

inline ProgLanguage::EType getProgLanguageType(const std::string& type)
{
    if(type == "cpp")
    {
        return ProgLanguage::CPP;
    }
    else if(type == "java")
    {
        return ProgLanguage::JAVA;
    }
    else if(type == "py")
    {
        return ProgLanguage::PYTHON;
    }
    else
    {
        return ProgLanguage::BINARY;
    }
}

} // namespace NCommand

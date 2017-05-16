/**
 * Project   AlgoVi
 *
 * @file     TProgLanguages.hpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#pragma once

#include <QStringList>
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
        HASKELL,
        BINARY,
        TOTAL,
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
    else if(type == "hs")
    {
        return ProgLanguage::HASKELL;
    }
    else
    {
        return ProgLanguage::BINARY;
    }
}

inline std::string toString(ProgLanguage::EType lang)
{
    if(lang == ProgLanguage::CPP)
    {
        return "c++";
    }
    else if(lang == ProgLanguage::JAVA)
    {
        return "java";
    }
    else if(lang == ProgLanguage::PYTHON)
    {
        return "python";
    }
    else if(lang == ProgLanguage::HASKELL)
    {
        return "haskell";
    }
    else
    {
        return "binary-file";
    }
}

inline QStringList getProgLanguageList()
{
    QStringList ret;
    for(std::size_t i = 0; i < static_cast<std::size_t>(ProgLanguage::TOTAL); ++i)
    {
        ret.append(QString::fromStdString(toString(static_cast<ProgLanguage::EType>(i))));
    }
    return ret;
}

} // namespace NCommand

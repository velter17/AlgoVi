/**
 * Project   AlgoVi
 *
 * @file     CTerminalBase.cpp
 * @author   Dmytro Sadovyi
 * @date     29.03.2017
 */

#include <QDebug>

#include "controller/terminal/CTerminalBase.hpp"

namespace NController
{

CTerminalBase::CTerminalBase()
{

}

void CTerminalBase::lock()
{
    qDebug () << "CTerminalBase::lock()";
}

void CTerminalBase::unlock()
{
    qDebug () << "CTerminalBase::unlock()";
}


} // namespace NController

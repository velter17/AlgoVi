/**
 * Project   AlgoVi
 *
 * @file     CTerminalBase.hpp
 * @author   Dmytro Sadovyi
 * @date     29.03.2017
 */

#pragma once

#include "controller/terminal/ITerminal.hpp"

namespace NController
{

class CTerminalBase : public QObject, public ITerminal
{
    Q_OBJECT
public: // methods
    CTerminalBase();

    void lock() override;
    void unlock() override;

signals:
    void command(const QString& cmd);
};

} // namespace NController


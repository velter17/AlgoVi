/**
 * Project   AlgoVi
 *
 * @file     CEmptyJob.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"

namespace NAlgoVi
{

class CEmptyJob : public IJob
{
public: // methods
    CEmptyJob();

    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
};

} // namespace NAlgoVi


/**
 * Project   AlgoVi
 *
 * @file     CEmptyJob.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include "../CEmptyJob.hpp"

namespace NAlgoVi
{

CEmptyJob::CEmptyJob()
{

}

void CEmptyJob::run(const QStringList& args)
{
   emit started();
   emit finished();
}

void CEmptyJob::appendData(const QString& data)
{

}

void CEmptyJob::terminate()
{

}


} // namespace NAlgoVi

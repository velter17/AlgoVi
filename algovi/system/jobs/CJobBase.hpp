/**
 * Project   AlgoVi
 *
 * @file     CJobBase.hpp
 * @author   Dmytro Sadovyi
 * @date     13.05.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"
#include "controller/CController.hpp"

namespace NAlgoVi
{

template <typename T>
class CJobBase : public IJob
{
public: // methods
    CJobBase(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;
    QStringList getArguments() override;
    QStringList getArgumentValues(const QString& arg) override;
protected: // fields
    T* mProc;
    NController::CController* mControllerPtr;
};

template <typename T>
inline CJobBase<T>::CJobBase(NController::CController* controller)
   : mControllerPtr(controller)
{
   mProc = new T();
   if(mProc->getSettings().getRevCtrlLogic())
   {
      mControllerPtr->setTerminalRevCtrlLogic();
   }
}

template <typename T>
inline void CJobBase<T>::run(const QStringList& args)
{
   emit started();
   connect(mProc, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
   connect(mProc, SIGNAL(logHtml(QString)), mControllerPtr, SLOT(handleLogHtml(QString)));
   connect(mProc, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
   connect(mProc, &T::finished, [this](int code)
   {
       mProc->deleteLater();
       emit finished();
   });
   mProc->setArgs(args);
   mProc->run();
}

template <typename T>
inline void CJobBase<T>::appendData(const QString &data)
{
   mProc->appendData(data);
}

template <typename T>
inline void CJobBase<T>::terminate()
{
   mProc->terminate();
}

template <typename T>
inline QStringList CJobBase<T>::getArguments()
{
   return mProc->getOptionsList();
}

template <typename T>
inline QStringList CJobBase<T>::getArgumentValues(const QString& arg)
{
   return mProc->getOptionValues(arg);
}

} // namespace NAlgoVi


/**
 * Project   AlgoVi
 *
 * @file     CCompilerExecutor.hpp
 * @author   Dmytro Sadovyi
 * @date     02.05.2017
 */

#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QTemporaryDir>
#include <QMap>

#include "framework/commands/private/CCompilerExecutor.hpp"
#include "framework/commands/CSystemCommand.hpp"
#include "framework/settings/CRunnerSettings.hpp"

namespace NCommand
{

class CCompilerStorage
{
public: // types
    struct SData
    {
        QString mCodePath;
        QString mAppPath;
        QStringList mFlags;
        QFileInfo mFileInfo;
        QDateTime mLastModified;
        SData(){}
        SData(const QString& code, const QStringList& flags, const QString& appPath)
        {
            mCodePath = code;
            mFlags = flags;
            mAppPath = appPath;
            mLastModified = QDateTime();
            mFileInfo.setFile(mCodePath);
            qDebug () << "new SData " << mCodePath << " -> " << mAppPath;
        }
        bool isModified()
        {
            mFileInfo.refresh();
            return mLastModified.toTime_t() != mFileInfo.lastModified().toTime_t();
        }
        void setUpToDate()
        {
            mFileInfo.refresh();
            mLastModified = mFileInfo.lastModified();
        }
    };
public: // methods
    CCompilerStorage(const CCompilerStorage&) = delete;
    static CCompilerStorage& getInstance();
    SData& getData(const QString& codePath);

private: //methods
    CCompilerStorage();
    QString getNewAppPath();
private: // fields
    QTemporaryDir mTempDir;
    QMap <QString, SData> mData;
    int mAppNum;
};

CCompilerStorage::CCompilerStorage()
{
    mTempDir.setAutoRemove(true);
    mAppNum = 0;
}

CCompilerStorage& CCompilerStorage::getInstance()
{
    static CCompilerStorage instance;
    return instance;
}

CCompilerStorage::SData& CCompilerStorage::getData(const QString& codePath)
{
    if(mData.find(codePath) == mData.end())
    {
        // TODO : flags!!!
        mData.insert(codePath, SData(codePath, QStringList(), getNewAppPath()));
    }
    return mData[codePath];
}

QString CCompilerStorage::getNewAppPath()
{
    QString ret = mTempDir.path() + "/" + QString("app%1").arg(mAppNum++);
    return ret;
}


CCompilerExecutor::CCompilerExecutor()
{

}

void CCompilerExecutor::compile(const QString& codePath,
                                const QStringList& flags,
                                const QString &lang,
                                bool forced)
{
    qDebug () << "CCompilerExecutor::compile " << codePath << " " << flags;
    emit started();

    const NSettings::tContainer& compilers = NSettings::CRunnerSettings::getInstance().getCompilers();

    if(lang == "binary")
    {
        emit finished(codePath);
        return;
    }
    else
    {
        CCompilerStorage::SData& data = CCompilerStorage::getInstance().getData(codePath);
        if(!forced && !data.isModified() && flags == data.mFlags)
        {
            emit finished(data.mAppPath);
            return;
        }

        QString compilerStr = NSettings::CRunnerSettings::getInstance().getCompilers()[lang];
        emit log(" [ Info ] compilation of " + codePath + " [" + lang + "] ...\n");

        compilerStr.replace("$SRC_PATH$", data.mCodePath);
        compilerStr.replace("$BIN_PATH$", data.mAppPath);
        QStringList args;
        args << compilerStr.split(" ");
        args << flags;
        CSystemCommand* compiler = new CSystemCommand;
        compiler->setArgs(args);
        connect(compiler, &CSystemCommand::log, [this](const QString& msg){
            emit log(msg);
        });
        connect(compiler, &CSystemCommand::error, [this](const QString& msg){
            emit error(msg);
        });
        connect(compiler, &CSystemCommand::finished, [this, &data, compiler, flags](int code){
            if(code == 0)
            {
                data.setUpToDate();
                data.mFlags = flags;
                emit log(" [ Info ] Ready\n");
                emit finished(data.mAppPath);
            }
            else
            {
                emit log(" [ Info ] Compilation was finished with error\n");
                emit finished("");
            }
            compiler->deleteLater();
        });
        compiler->run();
    }
}

} // namespace NCommand

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
                                ProgLanguage::EType lang)
{
    qDebug () << "CCompilerExecutor::compile " << codePath << " " << flags;
    emit started();
    if(lang == ProgLanguage::BINARY)
    {
        emit finished(codePath);
        return;
    }
    else
    {
        const static QString cppCompilerStr = "g++ -std=c++11 -w -O2 $SRC_PATH$ -o $BIN_PATH$";
        const static QString javaCompilerStr = "javac -d $BIN_PATH$ $SRC_PATH$";

        CCompilerStorage::SData& data = CCompilerStorage::getInstance().getData(codePath);
        if(!data.isModified())
        {
            //TODO check flags
            emit finished(data.mAppPath);
            return;
        }

        // TODO fix lang identification
        // TODO config file instead of "const static..."
        QString compilerStr = lang == ProgLanguage::CPP ? cppCompilerStr : javaCompilerStr;
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
        connect(compiler, &CSystemCommand::finished, [this, &data, compiler](int code){
            if(code == 0)
            {
                data.setUpToDate();
                emit finished(data.mAppPath);
            }
            else
            {
                emit finished("");
            }
            compiler->deleteLater();
        });
        compiler->run();
    }
}

} // namespace NCommand

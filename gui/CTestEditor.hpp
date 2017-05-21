/**
 * Project   AlgoVi
 *
 * @file     CTestEditor.hpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#pragma once

#include <QMainWindow>
#include <QCloseEvent>

#include "framework/common/ClassGenerators.hpp"

#ifdef ALGOVI_HAS_QTWEBKIT
namespace Novile
{
class Editor;
}
#else
#include <QTextEdit>
#endif


namespace NAlgoViGUI
{

typedef QPair<QString, QString> tData;

class CTestEditorSettings
{
ADD_ATTRIBUTE(CTestEditorSettings, ReadOnly, bool)
ADD_ATTRIBUTE(CTestEditorSettings, Test, tData)
};

class CTestEditor : public QMainWindow
{
   Q_OBJECT
public: // methods
    CTestEditor(const CTestEditorSettings& settings);
    ~CTestEditor();

    void closeEvent(QCloseEvent *e) override;
    void terminate();

private slots:
    void abort();
    void finish();

signals:
    void canceled();
    void ok(const tData& test);

private: // fields
    QLayout* mMainLayout;
#ifdef ALGOVI_HAS_QTWEBKIT
    Novile::Editor* mInputEditor;
    Novile::Editor* mOutputEditor;
#else
    QTextEdit* mInputEditor;
    QTextEdit* mOutputEditor;
#endif
    bool mFinished;
};

} // namespace NAlgoViGUI


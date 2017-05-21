/**
 * Project   AlgoVi
 *
 * @file     CTestEditor.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QDebug>
#include <QLayout>
#include <QPushButton>
#include <QLabel>

#include "../CTestEditor.hpp"
#include "externalLibs/novileEditor/src/editor.h"

namespace
{

#ifdef ALGOVI_HAS_QTWEBKIT
Novile::Editor* makeEditor(QWidget* parent,
                           const NAlgoViGUI::CTestEditorSettings& settings)
{
   Novile::Editor* ret = new Novile::Editor(parent);
   ret->setReadOnly(settings.getReadOnly());
   ret->setHighlightMode(Novile::Editor::HighlightMode::ModeMarkdown);
   ret->setActiveLineHighlighted(true);
   ret->setGutterShown(true);
   ret->setIndentationShown(true);
   ret->setInvisiblesShown(true);
   ret->setTheme(2);
   ret->setCursorPosition(0, 0);
   return ret;
}
#endif

}

namespace NAlgoViGUI
{

const int sDefaultWindowHeight = 400;
const int sDefaultWindowWidth = 600;

CTestEditor::CTestEditor(const CTestEditorSettings& settings)
   : mFinished(false)
{
   mMainLayout = new QVBoxLayout;
   QLayout* editorsLayout = new QVBoxLayout;
#ifdef ALGOVI_HAS_QTWEBKIT
   mInputEditor = makeEditor(this, settings);
   mOutputEditor = makeEditor(this, settings);
#else
   mInputEditor = new QTextEdit();
   mOutputEditor = new QTextEdit();
#endif
   mInputEditor->setText(settings.getTest().first);
   mOutputEditor->setText(settings.getTest().second);

   editorsLayout->addWidget(new QLabel("Input"));
   editorsLayout->addWidget(mInputEditor);
   editorsLayout->addWidget(new QLabel("Output"));
   editorsLayout->addWidget(mOutputEditor);

   mMainLayout->addItem(editorsLayout);

   QLayout* buttonLayout = new QVBoxLayout;
   QPushButton* okButton = new QPushButton("Ok");
   QPushButton* cancelButton = new QPushButton("Cancel");
   connect(cancelButton, &QPushButton::clicked, [this](){
      abort();
   });
   connect(okButton, &QPushButton::clicked, [this](){
      finish();
   });
   buttonLayout->addWidget(okButton);
   buttonLayout->addWidget(cancelButton);

   mMainLayout->addItem(buttonLayout);

   setCentralWidget(new QWidget);
   centralWidget()->setLayout(mMainLayout);
   mMainLayout->setMargin(0);

   setGeometry(geometry().x(),
               geometry().y(),
               sDefaultWindowWidth,
               sDefaultWindowHeight);

   setWindowTitle("AlgoVi | TestEditor");
}

CTestEditor::~CTestEditor()
{
   qDebug () << "~CTestEditor()";
   mInputEditor->deleteLater();
   mOutputEditor->deleteLater();
}

void CTestEditor::closeEvent(QCloseEvent *e)
{
   terminate();
}

void CTestEditor::terminate()
{
   if(!mFinished)
   {
      mFinished = true;
      emit canceled();
   }
}

void CTestEditor::abort()
{
   terminate();
}

void CTestEditor::finish()
{
   mFinished = true;
#if ALGOVI_HAS_QTWEBKIT
   emit ok(tData(mInputEditor->text(), mOutputEditor->text()));
#else
   emit ok(tData(mInputEditor->toPlainText(), mOutputEditor->toPlainText()));
#endif
}


} // namespace NAlgoViGUI

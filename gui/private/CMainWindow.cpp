/**
 * Project   AlgoVi
 *
 * @file     CMainWindow.cpp
 * @author   Dmytro Sadovyi
 * @date     28.03.2017
 */

#include <QLayout>
#include <QPlainTextEdit>

#include "gui/CMainWindow.hpp"

namespace NMainWindow
{

const int sDefaultWindowHeight = 400;
const int sDefaultWindowWidth = 600;

CMainWindow::CMainWindow()
{
    mMainLayout = new QVBoxLayout;
    QPlainTextEdit* editor = new QPlainTextEdit(this);
    mMainLayout->addWidget(editor);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mMainLayout);
    mMainLayout->setMargin(0);

    setGeometry(geometry().x(),
                geometry().y(),
                sDefaultWindowWidth,
                sDefaultWindowHeight);

    setWindowTitle("AlgoVi | beta 1.0");
}

CMainWindow::~CMainWindow()
{
    delete mMainLayout;
}


} // namespace NMainWindow

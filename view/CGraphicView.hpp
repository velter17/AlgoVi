/**
 * Project   AlgoVi
 *
 * @file     CGraphicView.hpp
 * @author   Dmytro Sadovyi
 * @date     22.05.2017
 */

#pragma once

#include <QMainWindow>
#include <QLayout>

#include "view/CGraphicsSceneBase.hpp"

namespace NView
{

class CGraphicView : public QMainWindow
{
public: // methods
    CGraphicView(CGraphicsSceneBase* scene);
    ~CGraphicView();

private: // fields
    CGraphicsSceneBase* mScenePtr;
    QLayout* mLayout;

};

} // namespace NView


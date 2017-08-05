/**
 * Project   AlgoVi
 *
 * @file     CGraphicView.cpp
 * @author   Dmytro Sadovyi
 * @date     22.05.2017
 */

#include <QGraphicsView>
#include <QFrame>
#include <QLabel>
#include <QSpacerItem>

#include "view/CGraphicView.hpp"

namespace NView
{

const int sDefaultWindowHeight = 600;
const int sDefaultWindowWidth = 600;

CGraphicView::CGraphicView(CGraphicsSceneBase* scene)
   : mScenePtr(scene)
{
   mLayout = new QVBoxLayout;

   QFrame* frame = new QFrame();
   QLabel* label = new QLabel("Dima Sadovyi");
   QLayout* someLayout = new QHBoxLayout;
   someLayout->addWidget(label);
   frame->setLayout(someLayout);
   mLayout->addWidget(frame);

   QGraphicsView* view = new QGraphicsView(this);
   mLayout->addWidget(view);
   view->setScene(scene);

   QLayout* bottomLayout = new QHBoxLayout;
   QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
   bottomLayout->addItem(spacer);
   QLabel* sceneRectLabel = new QLabel("[]");
   bottomLayout->addWidget(sceneRectLabel);
   mLayout->addItem(bottomLayout);
   connect(scene, &QGraphicsScene::sceneRectChanged, [this, sceneRectLabel](const QRectF& rect){
      sceneRectLabel->setText("[" + QString::number(rect.x()) + ", " + QString::number(rect.y()));
   });

   setCentralWidget(new QWidget);
   centralWidget()->setLayout(mLayout);
   mLayout->setMargin(0);

   setGeometry(geometry().x(),
               geometry().y(),
               sDefaultWindowWidth,
               sDefaultWindowHeight);

   setWindowTitle("AlgoVi | Visualization");
}

CGraphicView::~CGraphicView()
{
   mLayout->deleteLater();
}


} // namespace NGraphics

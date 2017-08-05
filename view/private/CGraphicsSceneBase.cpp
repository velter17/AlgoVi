/**
 * Project   AlgoVi
 *
 * @file     CGraphicsSceneBase.cpp
 * @author   Dmytro Sadovyi
 * @date     22.05.2017
 */

#include <QDebug>
#include <QPainter>

#include "view/CGraphicsSceneBase.hpp"

namespace NView
{

const int sDefaultSceneHeight = 10000;
const int sDefaultSceneWidth = 10000;

CGraphicsSceneBase::CGraphicsSceneBase()
{
   setSceneRect(0, 0, sDefaultSceneWidth, sDefaultSceneHeight);
}

CGraphicsSceneBase::~CGraphicsSceneBase()
{

}

void CGraphicsSceneBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug () << "Pressed!!!!";
}

void CGraphicsSceneBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void CGraphicsSceneBase::drawForeground(QPainter* painter, const QRectF& rect)
{
   const int gridSize = 10;
   painter->setOpacity(0.15);
   qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
   qreal top = int(rect.top()) - (int(rect.top()) % gridSize);
   QVarLengthArray<QLineF, 100> lines;
   for (qreal x = left; x < rect.right(); x += gridSize)
     lines.append(QLineF(x, rect.top(), x, rect.bottom()));
   for (qreal y = top; y < rect.bottom(); y += gridSize)
     lines.append(QLineF(rect.left(), y, rect.right(), y));
   painter->drawLines(lines.data(), lines.size());
   painter->setOpacity(1.0);
   QGraphicsScene::drawForeground(painter, rect);
}


} // namespace NView

/**
 * Project   AlgoVi
 *
 * @file     CGraphicsSceneBase.hpp
 * @author   Dmytro Sadovyi
 * @date     22.05.2017
 */

#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace NView
{

class CGraphicsSceneBase : public QGraphicsScene
{
   Q_OBJECT
public: // methods
    CGraphicsSceneBase();
    ~CGraphicsSceneBase();

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void drawForeground(QPainter* painter, const QRectF& rect);
};

} // namespace NView


#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsObject>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QApplication>
#include <QMenu>

#include "Connection.hpp"

#include "FlowScene.hpp"
#include "NodeGeometry.hpp"
#include "NodeState.hpp"

namespace QtNodes
{

class FlowScene;
class FlowItemEntry;
class NodeGroup;

class NodeGroupGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    NodeGroupGraphicsObject(QRectF rubberBandRect, FlowScene &scene, NodeGroup& nodeGroup);

    virtual ~NodeGroupGraphicsObject();

    NodeGroup&
    nodeGroup();

    NodeGroup const&
    nodeGroup() const;

    QRectF
    boundingRect() const override;

    enum { Type = UserType + 2 };

    int type() const override { return Type; }

    QRectF resizeRect() const;
    QPolygonF resizePoly() const;

protected:
  void
  paint(QPainter*                       painter,
        QStyleOptionGraphicsItem const* option,
        QWidget*                        widget = nullptr) override;

  QVariant
  itemChange(GraphicsItemChange change, const QVariant &value) override;

  void
  mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  void
  mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

  void
  mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  void
  hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

  void
  hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

  void
  hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

  void
  mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

  void
  contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:

  FlowScene & _scene;

  NodeGroup& _nodeGroup;

  double _width;
  double _height;
  QPointF _topLeft;

  bool _hovered = false;
  bool _resizing = false;

};
}

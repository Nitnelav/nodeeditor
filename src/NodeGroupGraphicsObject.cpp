#include "NodeGroupGraphicsObject.hpp"

#include "NodeGroup.hpp"
#include "NodeGraphicsObject.hpp"

#include <QDebug>
#include <QStyleOptionGraphicsItem>

using QtNodes::NodeGraphicsObject;
using QtNodes::NodeGroupGraphicsObject;
using QtNodes::FlowScene;
using QtNodes::NodeGroup;


NodeGroupGraphicsObject::NodeGroupGraphicsObject(QRectF rubberBandRect, FlowScene &scene, NodeGroup& nodeGroup):
    _scene(scene),
    _nodeGroup(nodeGroup),
    _width(rubberBandRect.width()),
    _height(rubberBandRect.height()),
    _topLeft(rubberBandRect.topLeft())
{
    _scene.addItem(this);

    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setCacheMode( QGraphicsItem::DeviceCoordinateCache );

    setAcceptHoverEvents(true);

    setZValue(-1);
}

NodeGroupGraphicsObject::~NodeGroupGraphicsObject()
{
    _scene.removeItem(this);
}

NodeGroup &NodeGroupGraphicsObject::nodeGroup()
{
    return _nodeGroup;
}

const NodeGroup &NodeGroupGraphicsObject::nodeGroup() const
{
    return _nodeGroup;
}

QRectF NodeGroupGraphicsObject::boundingRect() const
{
    return QRectF(_topLeft, QSizeF(_width, _height));
}

QRectF NodeGroupGraphicsObject::resizeRect() const
{
    unsigned int rectSize = 20;
    QRectF rect(_width - rectSize, _height - rectSize, rectSize, rectSize);
    return rect.translated(_topLeft);
}

QPolygonF NodeGroupGraphicsObject::resizePoly() const
{
    unsigned int rectSize = 20;
    QPolygonF poly;
    poly << QPointF(_width - rectSize, _height);
    poly << QPointF(_width, _height - rectSize);
    poly << QPointF(_width, _height);
    return poly.translated(_topLeft);
}

void NodeGroupGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::RenderHint::Antialiasing);
    QBrush brush(_nodeGroup.color(128));
    painter->setBrush(brush);
    QPen pen(_nodeGroup.color());
    pen.setWidth(2);
    if (isSelected()) {
        pen.setColor(_nodeGroup.color().lighter(150));
    }
    if (_hovered) {
        pen.setColor(_nodeGroup.color().lighter(180));
    }
    painter->setPen(pen);
    painter->drawRoundedRect(boundingRect(), 5.0, 5.0);

    painter->setPen(Qt::white);
    QPointF textPoint = boundingRect().topLeft();
    painter->drawText(textPoint + QPointF(10, 15), _nodeGroup.title());

    painter->setPen(Qt::NoPen);
    QBrush resizeBrush(Qt::gray, Qt::BDiagPattern);
    painter->setBrush(resizeBrush);
    painter->drawPolygon(resizePoly());
}

QVariant NodeGroupGraphicsObject::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        for (QGraphicsItem* item: childItems()) {
            NodeGraphicsObject* ngo = static_cast<NodeGraphicsObject*>(item);
            ngo->moveConnections();
        }
    }

    return QGraphicsObject::itemChange(change, value);
}

void NodeGroupGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (resizeRect().contains(event->pos())) {
        _resizing = true;
    }
    return QGraphicsObject::mousePressEvent(event);
}

void NodeGroupGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_resizing) {

        auto diff = event->pos() - event->lastPos();

        prepareGeometryChange();

        _width += diff.rx();
        if (_width < 50) {
            _width = 50;
        }
        _height += diff.ry();
        if (_height < 50) {
            _height = 50;
        }

        update();

        event->accept();
        return;
    }
    return QGraphicsObject::mouseMoveEvent(event);
}

void NodeGroupGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (_resizing) {
        QList<QGraphicsItem *> overlapItems = collidingItems();
        for (QGraphicsItem* item: overlapItems) {
            QPointF position = mapFromScene(item->scenePos());
            item->setParentItem(this);
            item->setPos(position);
        }
        for (QGraphicsItem* item: childItems()) {
            if (!overlapItems.contains(item)) {
                QPointF position = item->scenePos();
                item->setParentItem(nullptr);
                item->setPos(position);
            }
        }
    }
    _resizing = false;
    return QGraphicsObject::mouseReleaseEvent(event);
}

void NodeGroupGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    _hovered = true;
    return QGraphicsObject::hoverEnterEvent(event);
}

void NodeGroupGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    _hovered = false;
    setCursor(QCursor());
    return QGraphicsObject::hoverLeaveEvent(event);
}

void NodeGroupGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (resizeRect().contains(event->pos())) {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    } else {
        setCursor(QCursor());
    }
    return QGraphicsObject::hoverMoveEvent(event);
}

void NodeGroupGraphicsObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsObject::mouseDoubleClickEvent(event);
}

void NodeGroupGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    _scene.groupContextMenu(nodeGroup(), mapToScene(event->pos()));
}

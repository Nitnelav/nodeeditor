#include "NodeGroup.hpp"

using QtNodes::NodeGroup;
using QtNodes::NodeGroupGraphicsObject;

NodeGroup::NodeGroup():
    _uid(QUuid::createUuid()),
    _graphicsObject(nullptr),
    _title("New Group"),
    _color(21, 43, 77)
{

}

const NodeGroupGraphicsObject &NodeGroup::graphicsObject() const
{
    return *_graphicsObject.get();
}

NodeGroupGraphicsObject &NodeGroup::graphicsObject()
{
    return *_graphicsObject.get();
}

void NodeGroup::setGraphicsObject(std::unique_ptr<QtNodes::NodeGroupGraphicsObject> &&graphics)
{
    _graphicsObject = std::move(graphics);
}

QUuid NodeGroup::id() const
{
    return _uid;
}

QString NodeGroup::title() const
{
    return _title;
}

void NodeGroup::setTitle(const QString &title)
{
    _title = title;
}

QColor NodeGroup::color(int alpha) const
{
    QColor color(_color);
    color.setAlpha(alpha);
    return color;
}

void NodeGroup::setColor(const QColor &color)
{
    _color = color;
}

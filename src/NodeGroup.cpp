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


QJsonObject NodeGroup::save() const
{
    QJsonObject groupJson;

    groupJson["id"] = _uid.toString();

    groupJson["color"] = _color.name(QColor::HexArgb);
    groupJson["title"] = _title;

    QJsonObject obj;
    obj["x"] = _graphicsObject->pos().x();
    obj["y"] = _graphicsObject->pos().y();
    obj["w"] = _graphicsObject->boundingRect().width();
    obj["h"] = _graphicsObject->boundingRect().height();
    groupJson["position"] = obj;

    return groupJson;
}

void NodeGroup::restore(const QJsonObject &json)
{
    _uid = QUuid(json["id"].toString());

    _color = QColor(json["color"].toString());
    _title = json["title"].toString();

    QJsonObject positionJson = json["position"].toObject();
    QPointF     point(positionJson["x"].toDouble(),
                      positionJson["y"].toDouble());
    _graphicsObject->setPos(point);
    _graphicsObject->setWidth(positionJson["w"].toDouble());
    _graphicsObject->setHeight(positionJson["h"].toDouble());

    _graphicsObject->updateChilds();
}

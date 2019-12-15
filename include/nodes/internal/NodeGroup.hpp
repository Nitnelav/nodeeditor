#pragma once

#include <QObject>
#include <QRectF>
#include <QUuid>

#include "NodeGroupGraphicsObject.hpp"
#include "memory.hpp"

namespace QtNodes {

class NodeGroupGraphicsObject;

class NodeGroup: public QObject
{
    Q_OBJECT

public:
    NodeGroup();

    NodeGroupGraphicsObject const & graphicsObject() const;
    NodeGroupGraphicsObject & graphicsObject();

    void setGraphicsObject(std::unique_ptr<NodeGroupGraphicsObject>&& graphics);

    QUuid id() const;

    QString title() const;
    void setTitle(const QString &title);

    QColor color(int alpha = 255) const;
    void setColor(const QColor &color);

private:
    QUuid _uid;
    std::unique_ptr<NodeGroupGraphicsObject> _graphicsObject;

    QString _title;
    QColor _color;
};
}


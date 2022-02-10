#pragma once

#include <QFont>

#include "item.h"
#include "qschematic_export.h"

namespace QSchematic
{

  class QSCHEMATIC_EXPORT Label : public Item
  {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Label)

    friend class CommandLabelRename;

  signals:
    void textChanged(const QString &newText);
    void doubleClicked();

  public:
    Label(int type = Item::LabelType, QGraphicsItem *parent = nullptr);
    ~Label() override = default;

    gpds::container to_container() const override;
    void from_container(const gpds::container &container) override;
    std::shared_ptr<Item> deepCopy() const override;

    QRectF boundingRect() const final;
    QPainterPath shape() const final;

    void setText(const QString &text);
    QString text() const;
    void setFont(const QFont &font);
    QFont font() const;
    void setHasConnectionPoint(bool enabled);
    bool hasConnectionPoint() const;
    void setConnectionPoint(const QPointF &connectionPoint); // Parent coordinates
    QRectF textRect() const;

    // Added by PT to change the text color
    const QColor &textColor() const;
    void setTextColor(const QColor &newText_color);

  protected:
    void copyAttributes(Label &dest) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

  private:
    void calculateTextRect();

    QString _text;
    QFont _font;
    QRectF _textRect;
    bool _hasConnectionPoint;
    QPointF _connectionPoint; // Parent coordinates

    // Added by PT to change the text color
    QColor _text_color = Qt::black;
  };

}

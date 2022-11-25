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
    enum Mode {
        None,
//        Resize,
        Rotate,
    };
    Q_ENUM(Mode)

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

    // Added by PT to rotate the label
    bool allowMouseRotate() const;
    void setAllowMouseRotate(bool newAllowMouseRotate);
    bool canSnapToGrid() const;
    Mode mode() const;
    void setMode(Mode newMode);
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void update() override;
    static bool globalAllowMouseRotate();
    static void setGlobalAllowMouseRotate(bool newGlobalAllowMouseRotate);

    protected:
    void copyAttributes(Label &dest) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    // Added by PT to rotate the label
    QRectF rotationHandle() const;
    virtual void paintRotateHandle(QPainter& painter);

  private:
    void calculateTextRect();

    QString _text;
    QFont _font;
    QRectF _textRect;
    bool _hasConnectionPoint;
    QPointF _connectionPoint; // Parent coordinates

    // Added by PT to change the text color
    QColor _text_color = Qt::black;
    // Added by PT to rotate the label
    Mode _mode;
    bool _allowMouseRotate;

    static bool _globalAllowMouseRotate;
  };

}

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetricsF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include "label.h"
#include "../commands/commandlabelrotate.h"
#include "../scene.h"
#include "../utils.h"
#include "qgraphicssceneevent.h"


const QColor COLOR_LABEL = QColor("#000000");
const QColor COLOR_LABEL_HIGHLIGHTED = Qt::yellow;//QColor("#dc2479");
const qreal LABEL_TEXT_PADDING = 2;

using namespace QSchematic;

bool Label::_globalAllowMouseRotate = true;

Label::Label(int type, QGraphicsItem* parent) : Item(type, parent),
  _hasConnectionPoint(true),
  _mode(None),
  _allowMouseRotate(true)
{
  setSnapToGrid(false);
}

gpds::container Label::to_container() const
{
  // Connection point
  gpds::container connectionPoint;
  connectionPoint.add_attribute("enabled", (_hasConnectionPoint ? "true" : "false"));
  connectionPoint.add_value("x", _connectionPoint.x());
  connectionPoint.add_value("y", _connectionPoint.y());

  // Root
  gpds::container root;
  addItemTypeIdToContainer(root);
  root.add_value("item", Item::to_container());
  root.add_value("text", text().toStdString());
  root.add_value("connection_point", connectionPoint);
  root.add_value("allow_mouse_rotate", allowMouseRotate());
  return root;
}

void Label::from_container(const gpds::container& container)
{
  Item::from_container(*container.get_value<gpds::container*>("item").value());
  setText(QString::fromStdString(container.get_value<std::string>("text").value_or("")));

  // Connection point
  const gpds::container* connectionPointContainer = container.get_value<gpds::container*>("connection_point").value_or(nullptr);

  if (connectionPointContainer)
  {
    auto attributeString = connectionPointContainer->get_attribute<std::string>("enabled");

    if (attributeString.has_value())
    {
      _hasConnectionPoint = (attributeString.value() == "true");
    }

    _connectionPoint.setX(connectionPointContainer->get_value<double>("x").value_or(0));
    _connectionPoint.setY(connectionPointContainer->get_value<double>("y").value_or(0));
  }

  setAllowMouseRotate(container.get_value<bool>("allow_mouse_rotate").value_or(true));

}

std::shared_ptr<Item> Label::deepCopy() const
{
  auto clone = std::make_shared<Label>(type(), parentItem());
  copyAttributes(*clone);

  return clone;
}

void Label::copyAttributes(Label& dest) const
{
  // Base class
  Item::copyAttributes(dest);

  // Attributes
  dest._text = _text;
  dest._font = _font;
  dest._textRect = _textRect;
  dest._hasConnectionPoint = _hasConnectionPoint;
  dest._connectionPoint = _connectionPoint;
  dest._allowMouseRotate = _allowMouseRotate;
  dest._mode = _mode;
}

QRectF Label::boundingRect() const
{
  QRectF rect = _textRect;

  //  if (isHighlighted())
  //  {
  //    rect = rect.united(QRectF(_textRect.center(), mapFromParent(_connectionPoint)));
  //  }

  // Rotate handle
  if (isSelected() && _allowMouseRotate)
  {
    rect = rect.united(rotationHandle());
  }

  return rect;
}

QPainterPath Label::shape() const
{
  QPainterPath path;
  path.addRect(boundingRect());
  return path;
}

void Label::setText(const QString& text)
{
  _text = text;
  calculateTextRect();
  emit textChanged(_text);
}

void Label::setFont(const QFont& font)
{
  _font = font;

  calculateTextRect();
}

void Label::setHasConnectionPoint(bool enabled)
{
  _hasConnectionPoint = enabled;
}

bool Label::hasConnectionPoint() const
{
  return _hasConnectionPoint;
}

void Label::setConnectionPoint(const QPointF& connectionPoint)
{
  _connectionPoint = connectionPoint;

  update();
}

void Label::calculateTextRect()
{
  QFontMetricsF fontMetrics(_font);
  _textRect = fontMetrics.boundingRect(QRect(0, 0, 500, 100), NULL, _text);
  _textRect.adjust(-LABEL_TEXT_PADDING, -LABEL_TEXT_PADDING, LABEL_TEXT_PADDING, LABEL_TEXT_PADDING);
}

bool Label::globalAllowMouseRotate()
{
  return _globalAllowMouseRotate;
}

void Label::setGlobalAllowMouseRotate(bool newGlobalAllowMouseRotate)
{
  _globalAllowMouseRotate = newGlobalAllowMouseRotate;
}

Label::Mode Label::mode() const
{
  return _mode;
}

void Label::setMode(Mode newMode)
{
  _mode = newMode;
}

bool Label::allowMouseRotate() const
{
  if (!_globalAllowMouseRotate)
  {
    return false;
  }

  return _allowMouseRotate;
}

void Label::setAllowMouseRotate(bool newAllowMouseRotate)
{
  _allowMouseRotate = newAllowMouseRotate;
}

bool Label::canSnapToGrid() const
{
  // Only snap when the rotation is a multiple of 90
  return Item::snapToGrid() && fmod(rotation(), 90) == 0;
}

const QColor& Label::textColor() const
{
  return _text_color;
}

void Label::setTextColor(const QColor& newText_color)
{
  _text_color = newText_color;
  update();
}

QString Label::text() const
{
  return _text;
}

QFont Label::font() const
{
  return _font;
}

QRectF Label::textRect() const
{
  return _textRect;
}

void Label::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(option)
  Q_UNUSED(widget)

  // Draw a dashed line to the wire if selected
  if (isHighlighted())
  {
    // Line pen
    QPen penLine;
    penLine.setColor(COLOR_LABEL_HIGHLIGHTED);
    penLine.setStyle(Qt::DashLine);

    // Line brush
    QBrush brushLine;
    brushLine.setStyle(Qt::NoBrush);

    // Draw the connection line
    if (_hasConnectionPoint)
    {
      painter->setPen(penLine);
      painter->setBrush(brushLine);
      painter->drawLine(_textRect.center(), mapFromParent(_connectionPoint));
    }

    // Clear the text rectangle
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::transparent);
    painter->drawRect(_textRect.adjusted(penLine.width() / 2, penLine.width() / 2, -penLine.width() / 2, -penLine.width() / 2));

    // Draw the border around the label text
    painter->setPen(penLine);
    painter->setBrush(brushLine);
    painter->drawRect(_textRect);
  }

  // Text pen
  QPen textPen;
  textPen.setStyle(Qt::SolidLine);
  textPen.setColor(_text_color);

  // Text option
  QTextOption textOption;
  textOption.setWrapMode(QTextOption::NoWrap);
  textOption.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  // Draw the text
  painter->setPen(_text_color);
  painter->setBrush(Qt::NoBrush);
  painter->setFont(_font);
  painter->drawText(_textRect, _text, textOption);

  // Rotate handle
  if (isSelected() && allowMouseRotate())
  {
    paintRotateHandle(*painter);
  }
}

void Label::mouseDoubleClickEvent([[maybe_unused]] QGraphicsSceneMouseEvent* event)
{
  emit doubleClicked();
}

void Label::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  Q_ASSERT(scene());

  event->accept();

  QPointF newMousePos(event->scenePos());

  QRectF rect = scene()->sceneRect();

  // Don't accept any mouse move event outside the sheet
  if (!rect.contains(newMousePos))
  {
    // Hand over to base class
    Item::mouseMoveEvent(event);
    return;
  }

  switch (_mode)
  {
    case None:
    {
      Item::mouseMoveEvent(event);

      break;
    }

    case Rotate:
    {
      // Sanity check
      if (!_allowMouseRotate)
      {
        qFatal("Node::mouseMoveEvent(): _mode is 'Rotate' although _allowMouseRotate is false");
        break;
      }

      auto center = mapRectToScene(textRect()).center();

      auto delta = center - newMousePos;
      auto angle = fmod(qAtan2(delta.ry(), delta.rx()) * 180 / M_PI + 270, 360);

      //      if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
      //      {
      angle = qRound(angle / 90) * 90;
      //      }

      scene()->undoStack()->push(new CommandLabelRotate(this, angle));
    }
  }

}

void Label::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  event->accept();

  // Let the base class handle selection and so on
  Item::mousePressEvent(event);

  // Presume no mode
  _mode = None;

  // Rotation
  if (isSelected() && _allowMouseRotate)
  {
    if (rotationHandle().contains(event->pos().toPoint()))
    {
      _mode = Rotate;
    }
  }
}

void Label::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  event->accept();
  Item::mouseReleaseEvent(event);
  _mode = None;
}

QVariant Label::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  switch (change)
  {
    case QGraphicsItem::ItemPositionChange:
    {
      QPointF newPos = value.toPointF();

      Scene* currentScene = scene();

      if (currentScene != nullptr)
      {
        // the new position value is relative to parent item not the scene
        // the rect must be relative to the parent coordinate
        QRectF rect = mapRectToParent(mapRectFromScene(currentScene->sceneRect()));

        QPointF bottomLeftPos = newPos + textRect().bottomLeft();
        QPointF topRightPos = newPos + textRect().topRight();

        // Keep the label rect inside of the sheet
        if (!rect.contains(bottomLeftPos) || !rect.contains(topRightPos))
        {
          newPos.setX(qMin(rect.right() - textRect().right(), qMax(newPos.x(), rect.left() - textRect().left())));
          newPos.setY(qMin(rect.bottom() - textRect().bottom(), qMax(newPos.y(), rect.top() - textRect().top())));

          return newPos;
        }
      }

      return newPos;
    }

    default:
      return Item::itemChange(change, value);
  }
}

void Label::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
  Item::hoverMoveEvent(event);

  // Set the cursor
  {
    setCursor(Qt::ArrowCursor);

    if (isSelected() && _allowMouseRotate)
    {
      if (rotationHandle().contains(event->pos().toPoint()))
      {
        setCursor(Qt::SizeAllCursor);
      }
    }
  }
}

void Label::update()
{
  // The item class sets the origin to the center of the bounding box
  // but in this case we want to rotate around the center of the body
  setTransformOriginPoint(_textRect.center());
  QGraphicsObject::update();
}

QRectF Label::rotationHandle() const
{
  const QRectF& r = textRect();
  const int& resizeHandleSize = _settings.resizeHandleSize;
  return QRectF(Utils::centerPoint(r.topRight(), r.topLeft()) + QPointF(1, -resizeHandleSize * 3) - QPointF(resizeHandleSize, resizeHandleSize), QSizeF(2 * resizeHandleSize, 2 * resizeHandleSize));
}

void Label::paintRotateHandle(QPainter& painter)
{
  auto rect = rotationHandle();

  // Handle pen
  QPen handlePen;
  handlePen.setStyle(Qt::NoPen);
  painter.setPen(handlePen);

  // Handle Brush
  QBrush handleBrush;
  handleBrush.setStyle(Qt::SolidPattern);
  painter.setBrush(handleBrush);

  // Draw the outer handle
  handleBrush.setColor("#3fa9f5");
  painter.setBrush(handleBrush);
  painter.drawEllipse(rect.adjusted(-handlePen.width(), -handlePen.width(), handlePen.width() / 2, handlePen.width() / 2));

  // Draw the inner handle
  int adj = _settings.resizeHandleSize / 2;
  handleBrush.setColor(Qt::white);
  painter.setBrush(handleBrush);
  painter.drawEllipse(rect.adjusted(-handlePen.width() + adj, -handlePen.width() + adj, (handlePen.width() / 2) - adj, (handlePen.width() / 2) - adj));
}

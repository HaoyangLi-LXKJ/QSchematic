#include <memory>
#include "../items/label.h"
#include "commands.h"
#include "commandlabelrotate.h"

using namespace QSchematic;

CommandLabelRotate::CommandLabelRotate(QPointer<Label> label, qreal newAngle, QUndoCommand* parent) :
  UndoCommand(parent),
  _label(label),
  _newAngle(newAngle)
{
  _oldAngle = label->rotation();
  connectDependencyDestroySignal(_label.data());
  setText(QStringLiteral("Label rotate"));
}

int CommandLabelRotate::id() const
{
  return LabelRotateCommandType;
}

bool CommandLabelRotate::mergeWith(const QUndoCommand* command)
{
  // Check id
  if (id() != command->id())
  {
    return false;
  }

  // Check item
  const CommandLabelRotate* myCommand = static_cast<const CommandLabelRotate*>(command);

  if (_label != myCommand->_label)
  {
    return false;
  }

  // Merge
  _newAngle = myCommand->_newAngle;

  return true;
}

void CommandLabelRotate::undo()
{
  if (!_label)
  {
    return;
  }

  _label->setRotation(_oldAngle);

  // Recalculate position
  //  if (_label->canSnapToGrid())
  //  {
  //    _label->setPos(_label->itemChange(QGraphicsItem::ItemPositionChange, _label->pos()).toPointF());
  //  }
}

void CommandLabelRotate::redo()
{
  if (!_label)
  {
    return;
  }

  _label->setRotation(_newAngle);

  // Recalculate position
  //  if (_label->canSnapToGrid())
  //  {
  //    _label->setPos(_label->itemChange(QGraphicsItem::ItemPositionChange, _label->pos()).toPointF());
  //  }
}

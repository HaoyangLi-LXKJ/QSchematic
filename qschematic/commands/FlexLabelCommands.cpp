#include "commands.h"
#include "FlexLabelCommands.h"

#include "items/FlexLabel.h"
#include <qschematic/items/node.h>
#include <qschematic/items/connector.h>

using namespace Graphics;
using namespace QSchematic;

CommandChangeFlexLabelFontSize::CommandChangeFlexLabelFontSize(const QPointer<FlexLabel>& label, int font_size, QUndoCommand* parent) :
  UndoCommand(parent),
  _label(label),
  _new_size(font_size)
{
  _old_size = _label->font().pixelSize();
  connect(_label.data(), &QObject::destroyed, this, &UndoCommand::handleDependencyDestruction);
  setText(QStringLiteral("Change label size"));
}

int CommandChangeFlexLabelFontSize::id() const
{
  return LabelChangeFontSizeCommandType;
}

bool CommandChangeFlexLabelFontSize::mergeWith(const QUndoCommand* command)
{
  Q_UNUSED(command)

  return false;
}

void CommandChangeFlexLabelFontSize::undo()
{
  if (!_label || _old_size < 0 || _old_size > FlexLabel::MAX_FONT_SIZE)
  {
    return;
  }

  QFont current_font = _label->font();
  current_font.setPixelSize(_old_size);
  _label->setFont(current_font);
}

void CommandChangeFlexLabelFontSize::redo()
{
  if (!_label || _new_size < 0 || _new_size > FlexLabel::MAX_FONT_SIZE)
  {
    return;
  }

  QFont current_font = _label->font();
  current_font.setPixelSize(_new_size);
  _label->setFont(current_font);
}

CommandChangeFlexLabelFontColor::CommandChangeFlexLabelFontColor(const QPointer<FlexLabel>& label, QColor font_color, QUndoCommand* parent) :
  UndoCommand(parent),
  _label(label),
  _new_color(font_color)
{
  _old_color = _label->textColor();
  connect(_label.data(), &QObject::destroyed, this, &UndoCommand::handleDependencyDestruction);
  setText(QStringLiteral("Change label color"));
}

int CommandChangeFlexLabelFontColor::id() const
{
  return LabelChangeFontSizeCommandType;
}

bool CommandChangeFlexLabelFontColor::mergeWith(const QUndoCommand* command)
{
  Q_UNUSED(command)

  return false;
}

void CommandChangeFlexLabelFontColor::undo()
{
  if (!_label)
  {
    return;
  }

  _label->setTextColor(_old_color);
}

void CommandChangeFlexLabelFontColor::redo()
{
  if (!_label)
  {
    return;
  }

  _label->setTextColor(_new_color);
}


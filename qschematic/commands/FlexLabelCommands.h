#pragma once

#include "qschematic/commands/commandbase.h"
#include <QPointer>
#include <QColor>
#include <memory>

namespace Graphics
{
class FlexLabel;

class CommandChangeFlexLabelFontSize :
  public QSchematic::UndoCommand
{
public:
  CommandChangeFlexLabelFontSize(const QPointer<FlexLabel>& label, int font_size, QUndoCommand* parent = nullptr);

  int id() const override;
  bool mergeWith(const QUndoCommand* command) override;
  void undo() override;
  void redo() override;

private:
  QPointer<FlexLabel> _label;
  int _new_size;
  int _old_size;
};

class CommandChangeFlexLabelFontColor :
  public QSchematic::UndoCommand
{
public:
  CommandChangeFlexLabelFontColor(const QPointer<FlexLabel>& label, QColor font_color, QUndoCommand* parent = nullptr);

  int id() const override;
  bool mergeWith(const QUndoCommand* command) override;
  void undo() override;
  void redo() override;

private:
  QPointer<FlexLabel> _label;
  QColor _new_color;
  QColor _old_color;
};
}

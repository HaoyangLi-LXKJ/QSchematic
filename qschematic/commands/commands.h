#pragma once

namespace QSchematic
{
    enum CommandType {
  ItemMoveCommandType      = 0,
  ItemAddCommandType,
  ItemRemoveCommandType,
  ItemVisibilityCommandType,
  NodeResizeCommandType,
  LabelRenameCommandType,
  NodeRotateCommandType,
  RectItemResizeCommandType,
  RectItemRotateCommandType,
  WireNetRenameCommandType,
  WirePointMoveCommandType,
  LabelChangeFontSizeCommandType,

  LabelRotateCommandType,
  QSchematicCommandUserType = 1000


    };
}

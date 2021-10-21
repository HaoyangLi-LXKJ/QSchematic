#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>


#include <qschematic/scene.h>
#include "qschematic/commands/commandlabelrename.h"
#include "qschematic/commands/FlexLabelCommands.h"

#include "FlexLabel.h"

using namespace Graphics;

FlexLabel::FlexLabel(int type, QGraphicsItem* parent) :
  QSchematic::Label(type, parent)
{
}

FlexLabel::~FlexLabel()
{
}

gpds::container FlexLabel::to_container() const
{
  // Root
  gpds::container root;
  addItemTypeIdToContainer(root);
  root.add_value("label", QSchematic::Label::to_container());
  return root;
}

void FlexLabel::from_container(const gpds::container& container)
{
  // Root
  QSchematic::Label::from_container(*container.get_value<gpds::container*>("label").value());
}

std::shared_ptr<QSchematic::Item> FlexLabel::deepCopy() const
{
  auto clone = std::make_shared<FlexLabel>(ItemType::FlexLabelType, parentItem());
  copyAttributes(*(clone.get()));

  return clone;
}

void FlexLabel::copyAttributes(FlexLabel& dest) const
{
  QSchematic::Label::copyAttributes(dest);
}

void FlexLabel::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  // Create the menu
  QMenu menu;
  {
    // Text
    QAction* text = new QAction;
    text->setText("Change Label Text");
    connect(text, &QAction::triggered, [this]
    {
      if (!scene())
        return;

      const QString& newText = QInputDialog::getText(nullptr, "Change Label Text", "New label text", QLineEdit::Normal, this->text());

      scene()->undoStack()->push(new QSchematic::CommandLabelRename(this, newText));
    });

    QAction* text_size = new QAction;
    text_size->setText("Change Label Size");
    connect(text_size, &QAction::triggered, [this]
    {
      if (!scene())
        return;
      const int& newFontSize = QInputDialog::getInt(nullptr, "Change Label Size", "New label size", font().pixelSize(), 1, MAX_FONT_SIZE);

      changeFontSize(newFontSize);
    });

    // Assemble
    menu.addAction(text);
    menu.addAction(text_size);
    menu.addSeparator();
  }
  // Sow the menu
  menu.exec(event->screenPos());
}

void FlexLabel::changeFontSize(int size)
{
  if (!scene())
  {
    return;
  }

  scene()->undoStack()->push(new CommandChangeFlexLabelFontSize(this, size));
}

void FlexLabel::changeFontColor(QColor color)
{
  scene()->undoStack()->push(new CommandChangeFlexLabelFontColor(this, color));
}

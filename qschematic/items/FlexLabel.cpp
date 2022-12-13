#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>


#include <qschematic/scene.h>
#include "qschematic/commands/commandlabelrename.h"
#include "qschematic/commands/FlexLabelCommands.h"

#include "FlexLabel.h"

using namespace Graphics;

bool FlexLabel::_globalMenuEnable = true;
QString FlexLabel::_changeLabelTextText = "Change Label Text";
QString FlexLabel::_changeLabelSizeText = "Change Label Size";

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

  // Flexlabel properties
  gpds::container flexLabelProperties;
  flexLabelProperties.add_value("color", Label::textColor().name().toStdString());
  flexLabelProperties.add_value("size", Label::font().pointSize());

  root.add_value("flex_label_properties", flexLabelProperties);
  return root;
}

void FlexLabel::from_container(const gpds::container& container)
{
  // Root
  QSchematic::Label::from_container(*container.get_value<gpds::container*>("label").value());

  // Flexlabel properties
  const gpds::container* flexLabelProperties = container.get_value<gpds::container*>("flex_label_properties").value_or(nullptr);

  if (flexLabelProperties)
  {
    setTextColor(QColor(QString::fromStdString(flexLabelProperties->get_value<std::string>("color").value_or(""))));
    QFont current_font = font();
    int fontSize = flexLabelProperties->get_value<int>("size").value_or(0);

    if (fontSize > 0)
    {
      current_font.setPointSize(flexLabelProperties->get_value<int>("size").value_or(0));
    }

    setFont(current_font);
  }
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
  dest.setTextColor(textColor());
  dest.setFont(font());
}

const QString& FlexLabel::changeLabelTextText()
{
  return _changeLabelTextText;
}

void FlexLabel::setChangeLabelTextText(const QString& newChangeLabelTextText)
{
  _changeLabelTextText = newChangeLabelTextText;
}

const QString& FlexLabel::changeLabelSizeText()
{
  return _changeLabelSizeText;
}

void FlexLabel::setChangeLabelSizeText(const QString& newChangeLabelSizeText)
{
  _changeLabelSizeText = newChangeLabelSizeText;
}

bool FlexLabel::globalMenuEnable()
{
  return _globalMenuEnable;
}

void FlexLabel::setGlobalMenuEnable(bool newGlobalMenuEnable)
{
  _globalMenuEnable = newGlobalMenuEnable;
}

void FlexLabel::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  if (!_globalMenuEnable)
  {
    return;
  }

  // Create the menu
  QMenu menu;
  {
    // Text
    QAction* text = new QAction;
    text->setText(_changeLabelTextText);
    connect(text, &QAction::triggered, [this]
    {
      if (!scene())
        return;

      bool dialogOKPressed = true;
      const QString& newText = QInputDialog::getMultiLineText(nullptr, "Change Label Text", "New label text", this->text(), &dialogOKPressed);

      if (!dialogOKPressed)
        return;

      scene()->undoStack()->push(new QSchematic::CommandLabelRename(this, newText));
    });

    QAction* text_size = new QAction;
    text_size->setText(_changeLabelSizeText);
    connect(text_size, &QAction::triggered, [this]
    {
      if (!scene())
        return;

      bool dialogOKPressed = true;
      const int& newFontSize = QInputDialog::getInt(nullptr, "Change Label Size", "New label size", font().pointSize(), 1, MAX_FONT_SIZE, 1, &dialogOKPressed);

      if (!dialogOKPressed)
        return;

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

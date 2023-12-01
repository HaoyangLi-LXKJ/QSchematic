#pragma once

#include <qschematic/items/label.h>

#include "item.h"

namespace Graphics
{
class FlexLabel : public QSchematic::Label
{
  Q_OBJECT
  Q_DISABLE_COPY(FlexLabel)
public:

  explicit FlexLabel(int type = ItemType::FlexLabelType, QGraphicsItem* parent = nullptr);
  virtual ~FlexLabel() override;

  virtual gpds::container to_container() const override;
  virtual void from_container(const gpds::container& container) override;
  virtual std::shared_ptr<QSchematic::Item> deepCopy() const override;
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

  void changeFontSize(int size);

  void changeFontColor(QColor color);
  static const int MAX_FONT_SIZE = 400;

  static bool globalMenuEnable();
  static void setGlobalMenuEnable(bool newGlobalMenuEnable);

  static const QString &changeLabelTextText();
  static void setChangeLabelTextText(const QString &newChangeLabelTextText);
  static const QString &changeLabelSizeText();
  static void setChangeLabelSizeText(const QString &newChangeLabelSizeText);

  protected:
  void copyAttributes(FlexLabel& dest) const;

private:
  static bool _globalMenuEnable;
  static QString _changeLabelTextText;
  static QString _changeLabelSizeText;
};
}

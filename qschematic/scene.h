#pragma once

#include <memory>
#include <functional>
#include <QGraphicsScene>
#include <QScopedPointer>
#include <QGraphicsProxyWidget>
#include <QUndoStack>
#include <gpds/serialize.h>
#include "settings.h"
#include "items/item.h"
#include "items/wire.h"

namespace QSchematic {

    class Node;
    class Connector;
    class WireNet;

    class Scene : public QGraphicsScene, public Gpds::Serialize
    {
        Q_OBJECT
        Q_DISABLE_COPY(Scene)

    public:
        enum Mode {
            NormalMode,
            WireMode
        };
        Q_ENUM(Mode)

        explicit Scene(QObject* parent = nullptr);
        virtual ~Scene() override = default;

        virtual Gpds::Container toContainer() const override;
        virtual void fromContainer(const Gpds::Container& container) override;

        void setSettings(const Settings& settings);
        void setWireFactory(const std::function<std::unique_ptr<Wire>()>& factory);
        void setMode(Mode mode);
        void toggleWirePosture();

        bool isDirty() const;
        void clearIsDirty();

        void clear();
        bool addItem(const std::shared_ptr<Item>& item);
        bool removeItem(const std::shared_ptr<Item>& item);
        QList<std::shared_ptr<Item>> items() const;
        QList<std::shared_ptr<Item>> items(int itemType) const;
        QVector<std::shared_ptr<Item>> selectedItems() const;
        QList<std::shared_ptr<Node>> nodes() const;
        bool addWire(const std::shared_ptr<Wire>& wire);
        bool removeWire(const std::shared_ptr<Wire>& wire);
        QList<std::shared_ptr<Wire>> wires() const;
        QList<std::shared_ptr<WireNet>> nets() const;
        QList<std::shared_ptr<WireNet>> nets(const std::shared_ptr<WireNet>& wireNet) const;
        std::shared_ptr<WireNet> net(const std::shared_ptr<Wire>& wire) const;
        QList<std::shared_ptr<WireNet>> netsAt(const QPoint& point);
        QList<QPointF> connectionPoints() const;

        void undo();
        void redo();
        QUndoStack* undoStack() const;

    signals:
        void modeChanged(Mode newMode);
        void isDirtyChanged(bool isDirty);
        void itemAdded(const std::shared_ptr<Item>& item);
        void itemRemoved(const std::shared_ptr<Item>& item);
        void itemHighlightChanged(const std::shared_ptr<Item>& item, bool isHighlighted);

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
        virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
        virtual void dropEvent(QGraphicsSceneDragDropEvent* event) override;
        virtual void drawBackground(QPainter* painter, const QRectF& rect) override;

        /* This gets called just before the item is actually being moved by moveBy. Subclasses may
         * implement this to implement snapping to elements other than the grid
         */
        virtual QVector2D itemsMoveSnap(const std::shared_ptr<Item>& item, const QVector2D& moveBy) const;

    private:
        void renderCachedBackground();
        void setupNewItem(Item& item);
        void addWireNet(const std::shared_ptr<WireNet>& wireNet);
        QList<Item*> itemsAt(const QPointF& scenePos, Qt::SortOrder order = Qt::DescendingOrder) const;
        std::shared_ptr<Item> sharedItemPointer(const Item& item) const;

        QList<std::shared_ptr<Item>> _items;
        QList<std::shared_ptr<WireNet>> _nets;
        Settings _settings;
        QPixmap _backgroundPixmap;
        std::function<std::unique_ptr<Wire>()> _wireFactory;
        Mode _mode;
        std::shared_ptr<Wire> _newWire;
        bool _newWireSegment;
        bool _invertWirePosture;
        QPointF _lastMousePos;
        QList<std::shared_ptr<Item>> _selectedItems;
        QMap<std::shared_ptr<Item>, QPointF> _initialItemPositions;
        QPointF _initialCursorPosition;
        std::unique_ptr<QGraphicsProxyWidget> _popupInfobox;
        QUndoStack* _undoStack;

    private slots:
        void itemMoved(const Item& item, const QVector2D& movedBy);
        void itemRotated(const Item& item, const qreal rotation);
        void itemHighlightChanged(const Item& item, bool isHighlighted);
        void wireNetHighlightChanged(bool highlighted);
        void wirePointMoved(Wire& wire, WirePoint& point);
        void wireMovePoint(const QPointF& point, Wire& wire, const QVector2D& movedBy) const;
        QList<std::shared_ptr<Wire>> wiresConnectedTo(const Node& node, const QVector2D& offset) const;
        void showPopup(const Item& item);
    };

}
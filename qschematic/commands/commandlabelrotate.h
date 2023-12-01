#pragma once

#include "commandbase.h"
#include <QPointer>
#include <QPoint>
#include <QSize>

namespace QSchematic
{
    class Label;

    class QSCHEMATIC_EXPORT CommandLabelRotate :
        public UndoCommand
    {
    public:
        CommandLabelRotate(QPointer<Label> label, qreal rotation, QUndoCommand* parent = nullptr);

        int id() const override;
        bool mergeWith(const QUndoCommand* command) override;
        void undo() override;
        void redo() override;

    private:
        void updateText();

        QPointer<Label> _label;
        qreal _oldAngle;
        qreal _newAngle;
    };

}

#ifndef CONFMATWIDGET_H
#define CONFMATWIDGET_H

#include <QTableWidget>

class QUndoStack;

namespace Ui
{
    class ConfMatWidget;
}

class ConfMatWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit ConfMatWidget(QWidget* parent = 0);
    ~ConfMatWidget();

    QUndoStack*  m_undoStack;
};

#endif // CONFMATWIDGET_H

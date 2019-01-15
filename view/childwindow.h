#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include <QtGui>
#include <QWidget>
#include <QMdiSubWindow>

namespace Ui {
    class ChildWindow;
}

class ChildWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit ChildWindow(QWidget *parent = 0, QWidget *child = 0);
    ~ChildWindow();

private:
    QWidget *child_m;
};

#endif // CHILDWINDOW_H

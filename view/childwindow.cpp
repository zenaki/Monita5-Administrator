#include "childwindow.h"

#include <QMdiSubWindow>

ChildWindow::ChildWindow(QWidget *parent, QWidget *child) :
    QMdiSubWindow(parent)
{
    child_m = child;
    this->setWidget(child_m);
}

ChildWindow::~ChildWindow()
{
    delete child_m;
}

#ifndef INTERACTIVEWIDGET_H
#define INTERACTIVEWIDGET_H

#include <QtWidgets/QWidget>

namespace Ui {
class InteractiveWidget;
}

class InteractiveWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit InteractiveWidget(QWidget *parent = 0);
    ~InteractiveWidget();
    
private:
    Ui::InteractiveWidget *ui;
};

#endif // INTERACTIVEWIDGET_H

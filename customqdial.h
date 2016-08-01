#ifndef CUSTOMQDIAL_H
#define CUSTOMQDIAL_H

#include <QObject>
#include <QWidget>
#include <QDial>

class CustomQDial : public QDial
{
public:
    CustomQDial(QWidget *parent = 0);
};

#endif // CUSTOMQDIAL_H

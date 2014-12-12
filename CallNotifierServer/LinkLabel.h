#ifndef LINKLABEL_H
#define LINKLABEL_H

#include <QLabel>
#include "qevent.h"
#include "qtimer.h"

class QLinkLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QLinkLabel(QWidget *parent = 0);
    QString URL;

protected:
    virtual void mousePressEvent(QMouseEvent *ev);

    QTimer timer;
    int blinkingCounter;

signals:

public slots:

protected slots:
    void OnTimer();
};

#endif // LINKLABEL_H

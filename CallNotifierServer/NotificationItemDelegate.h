#ifndef CNOTIFICATIONITEMDELEGATE_H
#define CNOTIFICATIONITEMDELEGATE_H

#include <QStyledItemDelegate>

class CNotificationItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CNotificationItemDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:


protected:
};


#endif // CNOTIFICATIONITEMDELEGATE_H

#ifndef HISTORYITEMDELEGATE_H
#define HISTORYITEMDELEGATE_H

#include <QStyledItemDelegate>

class CHistoryItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CHistoryItemDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:


protected:
    static QImage statusImages[2];
};

#endif // HISTORYITEMDELEGATE_H

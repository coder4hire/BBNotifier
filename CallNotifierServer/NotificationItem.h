#ifndef NOTIFICATIONITEM_H
#define NOTIFICATIONITEM_H

#include "QDateTime"

class CNotificationItem
{
public:
    enum EType
    {
        EMPTY,
        RINGING_CALL,
        OFFHOOK_CALL,
        NOTIFICATION
    };

public:
    CNotificationItem();
    CNotificationItem(const QString& typeStr,const QString& phone,const QString& name,const QDateTime& dateTime,const QString& data);
    CNotificationItem(const QString& serializedString);

    QString Phone;
    QString Name;
    QString Data;
    QDateTime DateTime;
    EType Type;

    void SetType(const QString& strType);

    QString Serialize() const;

protected:
    static const QString typeNames[];

};

#endif // NOTIFICATIONITEM_H

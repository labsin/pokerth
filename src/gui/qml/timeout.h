#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <QObject>
#include "QTime"

class QTimer;

class Timeout : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int seconds READ seconds NOTIFY secondsChanged)
    Q_PROPERTY(int msgID READ msgID NOTIFY msgIDChanged)
    Q_PROPERTY(QString reason READ reason NOTIFY reasonChanged)

public:
    explicit Timeout(QObject *parent = 0);

    int seconds() const;

    int msgID() const
    {
        return m_msgID;
    }

    QString reason() const;


signals:

    void secondsChanged(int arg);

    void msgIDChanged(int arg);

    void reasonChanged(QString arg);

    void timeoutStarted(int seconds);

public slots:

    void setMsgID(int arg);
    void startTimeout(int seconds);
    void stopTimeout();

    void refresh();

private:
    int m_msgID;
    int m_timeout;
    QTime m_time;
    QTimer * m_timer;
};

#endif // TIMEOUT_H

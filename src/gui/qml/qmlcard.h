#ifndef QMLCARD_H
#define QMLCARD_H

#include <QObject>

class QmlCard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int card READ card WRITE setcard NOTIFY cardChanged)
    Q_PROPERTY(bool flipped READ flipped WRITE setflipped NOTIFY flippedChanged)
    Q_PROPERTY(bool dealt READ dealt WRITE setdealt NOTIFY dealtChanged)

public:
    explicit QmlCard(QObject *parent = 0);

    int card() const
    {
        return m_card;
    }

    bool flipped() const
    {
        return m_flipped;
    }

    bool dealt() const
    {
        return m_dealt;
    }

    static void registerType();

signals:

    void cardChanged(int arg);

    void flippedChanged(bool arg);

    void dealtChanged(bool arg);

public slots:

    void setcard(int arg)
    {
        if (m_card != arg) {
            m_card = arg;
            emit cardChanged(arg);
        }
    }
    void setflipped(bool arg)
    {
        if (m_flipped != arg) {
            m_flipped = arg;
            emit flippedChanged(arg);
        }
    }

    void setdealt(bool arg)
    {
        if (m_dealt != arg) {
            m_dealt = arg;
            emit dealtChanged(arg);
        }
    }

public:

    int m_card;
    bool m_flipped;
    bool m_dealt;
};

#endif // QMLCARD_H

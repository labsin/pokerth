#ifndef QMLGAME_H
#define QMLGAME_H

#include <QObject>
#include "manager.h"
#include "guiwrapper.h"
#include "player.h"

class PlayerModel;
class QmlCard;
class Manager;

class QmlGame : public QObject
{
    Q_OBJECT
    Q_ENUMS(Buttons)
    Q_ENUMS(QmlGameState)
    Q_ENUMS(PlayingMode)
    Q_PROPERTY(QString title READ title WRITE settitle NOTIFY titleChanged)
    Q_PROPERTY(int pot READ pot NOTIFY potChanged)
    Q_PROPERTY(int bets READ bets WRITE setbets NOTIFY betsChanged)
    Q_PROPERTY(QObject * myPlayer READ myPlayer NOTIFY myPlayerChanged)
    Q_PROPERTY(int highestSet READ highestSet WRITE sethighestSet NOTIFY highestSetChanged)
    Q_PROPERTY(int set READ boardSet WRITE setboardSet NOTIFY boardSetChanged)
    Q_PROPERTY(int minimumRaise READ minimumRaise WRITE setminimumRaise NOTIFY minimumRaiseChanged)
    Q_PROPERTY(int smallBlind READ smallBlind WRITE setsmallBlind NOTIFY smallBlindChanged)
    Q_PROPERTY(int gameNr READ gameNr NOTIFY gameNrChanged)
    Q_PROPERTY(int handNr READ handNr NOTIFY handNrChanged)
    Q_PROPERTY(QmlCard* flopCard1 READ flopCard1 NOTIFY flopCard1Changed)
    Q_PROPERTY(QmlCard* flopCard2 READ flopCard2 NOTIFY flopCard2Changed)
    Q_PROPERTY(QmlCard* flopCard3 READ flopCard3 NOTIFY flopCard3Changed)
    Q_PROPERTY(QmlCard* turnCard READ turnCard NOTIFY turnCardChanged)
    Q_PROPERTY(QmlCard* riverCard READ riverCard NOTIFY riverCardChanged)
    Q_PROPERTY(int gameSpeed READ getGameSpeed NOTIFY gameSpeedChanged)
    Q_PROPERTY(QObject* model READ getModel NOTIFY modelChanged)
    Q_PROPERTY(bool blinkStartButton READ blinkStartButton WRITE setblinkStartButton NOTIFY blinkStartButtonChanged)
    Q_PROPERTY(bool breakAfterCurrentHand READ breakAfterCurrentHand WRITE setbreakAfterCurrentHand NOTIFY breakAfterCurrentHandChanged)
    Q_PROPERTY(Buttons checkedButton READ checkedButton WRITE setcheckedButton NOTIFY checkedButtonChanged)
    Q_PROPERTY(PlayingMode playingMode READ playingMode WRITE setplayingMode NOTIFY playingModeChanged)
    Q_PROPERTY(QmlGameState gameState READ gameState WRITE setgameState NOTIFY gameStateChanged)
    Q_PROPERTY(bool buttonsCheckable READ buttonsCheckable WRITE setbuttonsCheckable NOTIFY buttonsCheckableChanged)
    Q_PROPERTY(bool fullBetRule READ fullBetRule WRITE setfullBetRule NOTIFY fullBetRuleChanged)
    Q_PROPERTY(bool myTurn READ myTurn WRITE setmyTurn NOTIFY myTurnChanged)

public:
    explicit QmlGame(QObject *parent = 0);
    ~QmlGame();

    enum Buttons {
        NoButton = 0,
        BetRaiseButton,
        CallButton,
        FoldButton,
        AllInButton
    };

    enum PlayingMode {
        ManualMode = 0,
        AutoCheckCallMode,
        AutoCheckFoldMode
    };

    enum QmlGameState {
        QML_GAME_STATE_PREFLOP = 0,
        QML_GAME_STATE_FLOP,
        QML_GAME_STATE_TURN,
        QML_GAME_STATE_RIVER,
        QML_GAME_STATE_POST_RIVER,
        QML_GAME_STATE_PREFLOP_SMALL_BLIND = 0xF0,
        QML_GAME_STATE_PREFLOP_BIG_BLIND = 0xF1
    };

    static void registerType();

    void setSpeeds();

    QString title() const
    {
        return m_title;
    }

    int getGameSpeed() const
    {
        return guiGameSpeed;
    }

    QObject *getModel() const;

    PlayerModel *getPlayerModel() const;

    int pot() const
    {
        return m_pot;
    }

    int bets() const
    {
        return m_bets;
    }

    int gameNr() const
    {
        return m_gameNr;
    }

    int handNr() const
    {
        return m_handNr;
    }

    QmlCard* flopCard1() const
    {
        return m_flopCard1;
    }

    QmlCard* flopCard2() const
    {
        return m_flopCard2;
    }

    QmlCard* flopCard3() const
    {
        return m_flopCard3;
    }

    QmlCard* turnCard() const
    {
        return m_turnCard;
    }

    QmlCard* riverCard() const
    {
        return m_riverCard;
    }
    void settitle(QString arg)
    {
        if (m_title != arg) {
            m_title = arg;
            emit titleChanged(arg);
        }
    }

    void setgameSpeed(int arg)
    {
        if (guiGameSpeed != arg) {
            guiGameSpeed = arg;
            setSpeeds();
            emit gameSpeedChanged(arg);
        }
    }

    void setpot(int arg)
    {
        if (m_pot != arg) {
            m_pot = arg;
            emit potChanged(arg);
        }
    }

    void setgameNr(int arg)
    {
        if (m_gameNr != arg) {
            m_gameNr = arg;
            emit gameNrChanged(arg);
        }
    }

    void sethandNr(int arg)
    {
        if (m_handNr != arg) {
            m_handNr = arg;
            emit handNrChanged(arg);
        }
    }

    void setflopCard1(QmlCard* arg);

    void setflopCard2(QmlCard* arg);

    void setflopCard3(QmlCard* arg);

    void setturnCard(QmlCard* arg);

    void setriverCard(QmlCard* arg);

    bool blinkStartButton() const
    {
        return m_blinkStartButton;
    }

    void nextRoundCleanGui();

    bool breakAfterCurrentHand() const
    {
        return m_breakAfterCurrentHand;
    }

    Buttons checkedButton() const
    {
        return m_checkedButton;
    }

    PlayingMode playingMode() const
    {
        return m_playingMode;
    }

    bool buttonsCheckable() const
    {
        return m_buttonsCheckable;
    }

    QObject * myPlayer() const
    {
        return m_myPlayer;
    }

    int highestSet() const
    {
        return m_highestSet;
    }

    int minimumRaise() const
    {
        return m_minimumRaise;
    }

    bool fullBetRule() const
    {
        return m_fullBetRule;
    }

    Q_INVOKABLE bool check();
    Q_INVOKABLE bool call();
    Q_INVOKABLE bool fold();
    Q_INVOKABLE bool allIn();
    Q_INVOKABLE bool set();

    void myActionDone();

    int smallBlind() const
    {
        return m_smallBlind;
    }

    QmlGameState gameState() const
    {
        return m_gameState;
    }

    bool myTurn() const
    {
        return m_myTurn;
    }

    int boardSet() const
    {
        return m_boardSet;
    }

public slots:

    void dealBeRoCards(int myBeRoID);
    void dealHoleCards();

    void dealFlopCards0();
    void dealFlopCards1();
    void dealFlopCards2();
    void dealFlopCards3();
    void dealFlopCards4();
    void dealFlopCards5();
    void dealFlopCards6();

    void dealTurnCards0();
    void dealTurnCards1();
    void dealTurnCards2();

    void dealRiverCards0();
    void dealRiverCards1();
    void dealRiverCards2();

    void nextPlayerAnimation();

    void beRoAnimation2(int myBeRoID);

    void preflopAnimation1();
    void preflopAnimation1Action();
    void preflopAnimation2();
    void preflopAnimation2Action();

    void flopAnimation1();
    void flopAnimation1Action();
    void flopAnimation2();
    void flopAnimation2Action();

    void turnAnimation1();
    void turnAnimation1Action();
    void turnAnimation2();
    void turnAnimation2Action();

    void riverAnimation1();
    void riverAnimation1Action();
    void riverAnimation2();
    void riverAnimation2Action();

    void postRiverAnimation1();
    void postRiverAnimation1Action();

    void postRiverRunAnimation1();
    void postRiverRunAnimation2();
    void postRiverRunAnimation3();
    void postRiverRunAnimation4();
    void postRiverRunAnimation5();
    void postRiverRunAnimation6();

    void handSwitchRounds();

    void startNewHand();

    void meInAction();

    void flipHolecardsAllIn();

    void setbets(int arg)
    {
        if (m_bets != arg) {
            m_bets = arg;
            emit betsChanged(arg);
        }
    }

    void setblinkStartButton(bool arg)
    {
        if (m_blinkStartButton != arg) {
            m_blinkStartButton = arg;
            emit blinkStartButtonChanged(arg);
        }
    }

    void setbreakAfterCurrentHand(bool arg)
    {
        if (m_breakAfterCurrentHand != arg) {
            m_breakAfterCurrentHand = arg;
            emit breakAfterCurrentHandChanged(arg);
        }
    }

    void setcheckedButton(Buttons arg)
    {
        qDebug() << "change button to " << arg ;
        if (m_checkedButton != arg) {
            m_checkedButton = arg;
            emit checkedButtonChanged(arg);
        }
    }

    void setplayingMode(PlayingMode arg)
    {
        if (m_playingMode != arg) {
            m_playingMode = arg;
            emit playingModeChanged(arg);
        }
    }

    void updateMyButtonsState(int mode=-1);

    void provideMyActions(int mode=-1);

    void setbuttonsCheckable(bool arg)
    {
        if (m_buttonsCheckable != arg) {
            m_buttonsCheckable = arg;
            setcheckedButton(NoButton);
            emit buttonsCheckableChanged(arg);
        }
    }

    void setmyPlayer(Player * arg)
    {
        QObject * tmpObj = dynamic_cast<QObject *>(arg);
        if (m_myPlayer != tmpObj) {
            m_myPlayer = tmpObj;
            emit myPlayerChanged(tmpObj);
        }
    }

    void sethighestSet(int arg)
    {
        if (m_highestSet != arg) {
            m_highestSet = arg;
            emit highestSetChanged(arg);
        }
    }

    void setminimumRaise(int arg)
    {
        if (m_minimumRaise != arg) {
            m_minimumRaise = arg;
            emit minimumRaiseChanged(arg);
        }
    }

    void setfullBetRule(bool arg)
    {
        if (m_fullBetRule != arg) {
            m_fullBetRule = arg;
            emit fullBetRuleChanged(arg);
        }
    }

    void setsmallBlind(int arg)
    {
        if (m_smallBlind != arg) {
            m_smallBlind = arg;
            emit smallBlindChanged(arg);
        }
    }

    void setgameState(QmlGameState arg)
    {
        if (m_gameState != arg) {
            m_gameState = arg;
            emit gameStateChanged(arg);
        }
    }

    void setmyTurn(bool arg)
    {
        if (m_myTurn != arg) {
            m_myTurn = arg;
            emit myTurnChanged(arg);
        }
    }

    void setboardSet(int arg)
    {
        if (m_boardSet != arg) {
            m_boardSet = arg;
            emit boardSetChanged(arg);
        }
    }

signals:
    void guiInitiated(int speed);

    void titleChanged(QString arg);

    void gameSpeedChanged(int arg);

    void modelChanged(QObject* arg);

    void potChanged(int arg);

    void betsChanged(int arg);

    void gameNrChanged(int arg);

    void handNrChanged(int arg);

    void flopCard1Changed(QmlCard* arg);

    void flopCard2Changed(QmlCard* arg);

    void flopCard3Changed(QmlCard* arg);

    void turnCardChanged(QmlCard* arg);

    void riverCardChanged(QmlCard* arg);

    void blinkStartButtonChanged(bool arg);

    void breakAfterCurrentHandChanged(bool arg);

    void nextPlayerAnimationSignal();

    void checkedButtonChanged(Buttons arg);

    void playingModeChanged(PlayingMode arg);

    void buttonsCheckableChanged(bool arg);

    void myPlayerChanged(QObject * arg);

    void highestSetChanged(int arg);

    void minimumRaiseChanged(int arg);

    void fullBetRuleChanged(bool arg);

    void smallBlindChanged(int arg);

    void gameStateChanged(QmlGameState arg);

    void myTurnChanged(bool arg);

    void boardSetChanged(int arg);

    void nextRound();

private:
    PlayerModel* myPlayerModel;
    Manager* myManager;

    QString m_title;
    int m_pot;
    int m_bets;
    int m_gameNr;
    int m_handNr;
    QmlCard* m_flopCard1;
    QmlCard* m_flopCard2;
    QmlCard* m_flopCard3;
    QmlCard* m_turnCard;
    QmlCard* m_riverCard;

    //Speed
    int guiGameSpeed;
    int mainGameSpeed;
    int dealCardsSpeed;
    int preDealCardsSpeed;
    int postDealCardsSpeed;
    int AllInDealCardsSpeed;
    int postRiverRunAnimationSpeed;
    int winnerBlinkSpeed;
    int newRoundSpeed;
    int nextPlayerSpeed1;
    int nextPlayerSpeed2;
    int nextPlayerSpeed3;
    int preflopNextPlayerSpeed;
    int nextOpponentSpeed;

    bool flipHolecardsAllInAlreadyDone;
    bool currentGameOver;
    bool m_breakAfterCurrentHand;
    int distributePotAnimCounter;

    //Timer
    QTimer *dealFlopCards0Timer;
    QTimer *dealFlopCards1Timer;
    QTimer *dealFlopCards2Timer;
    QTimer *dealFlopCards3Timer;
    QTimer *dealFlopCards4Timer;
    QTimer *dealFlopCards5Timer;
    QTimer *dealFlopCards6Timer;
    QTimer *dealTurnCards0Timer;
    QTimer *dealTurnCards1Timer;
    QTimer *dealTurnCards2Timer;
    QTimer *dealRiverCards0Timer;
    QTimer *dealRiverCards1Timer;
    QTimer *dealRiverCards2Timer;

    QTimer *nextPlayerAnimationTimer;
    QTimer *preflopAnimation1Timer;
    QTimer *preflopAnimation2Timer;
    QTimer *flopAnimation1Timer;
    QTimer *flopAnimation2Timer;
    QTimer *turnAnimation1Timer;
    QTimer *turnAnimation2Timer;
    QTimer *riverAnimation1Timer;
    QTimer *riverAnimation2Timer;

    QTimer *postRiverAnimation1Timer;
    QTimer *postRiverRunAnimation1Timer;
    QTimer *postRiverRunAnimation2Timer;
    QTimer *postRiverRunAnimation2_flipHoleCards1Timer;
    QTimer *postRiverRunAnimation2_flipHoleCards2Timer;
    QTimer *postRiverRunAnimation3Timer;
    QTimer *postRiverRunAnimation5Timer;
    QTimer *postRiverRunAnimation6Timer;

    QTimer *potDistributeTimer;
    QTimer *voteOnKickTimeoutTimer;
    bool m_blinkStartButton;
    Buttons m_checkedButton;
    PlayingMode m_playingMode;
    bool m_buttonsCheckable;
    QObject * m_myPlayer;
    int m_highestSet;
    int m_minimumRaise;
    bool m_fullBetRule;
    int m_smallBlind;
    QmlGameState m_gameState;
    bool m_myTurn;
    int m_boardSet;
};

#endif // QMLGAME_H

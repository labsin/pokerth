import QtQuick 2.0
import QtGraphicalEffects 1.0
import PokerTH 1.0

Flipable {
    id: container

    property int card: cardObj.card
    property bool flipped: cardObj.flipped
    property bool blurred: cardObj.blur
    property int xAxis: 0
    property int yAxis: 1
    property int angle: 0
    property Card cardObj: Card {}

    signal clicked()
    height: backImage.implicitHeight
    width: height*backImage.implicitWidth/backImage.implicitHeight
    implicitHeight: backImage.implicitHeight
    implicitWidth: backImage.implicitWidth

    front: Image {
        id: frontImage; smooth: true
        anchors.fill: parent
        source: card==-1?"":"image://PokerTH/cards/"+card+".png"
    }
    back: Image {
        id: backImage; smooth: true
        anchors.fill: parent
        source: "image://PokerTH/cards/flipside.png";
    }

    state: "back"

    MouseArea { anchors.fill: parent; onClicked: container.clicked() }

    ColorOverlay {
        id: blurObj
        anchors.fill: parent
        source: parent
        color: "#000000"
        opacity: 0.0
        Behavior on opacity {
            NumberAnimation { duration: 800 }
        }
    }

    onBlurredChanged: {
        if(blurred) {
            blurObj.opacity = 0.4
        }
        else {
            blurObj.opacity = 0.0
        }
    }

    transform: [
        Rotation {
            id: rotation; origin.x: container.width / 2; origin.y: container.height / 2
            axis.x: container.xAxis; axis.y: container.yAxis; axis.z: 0
            angle: container.angle
        }
    ]

    states: [
        State {
            name: "back"; when: !container.flipped
            PropertyChanges { target: rotation; angle: 180-container.angle }
        },
        State {
            name: "gone"; when: card==-1
            PropertyChanges { target: container; visible: false }
        }
    ]

    transitions: Transition {
        ParallelAnimation {
            NumberAnimation { target: rotation; properties: "angle"; duration: 800 }
            SequentialAnimation {
                NumberAnimation { target: container; property: "scale"; to: 0.75; duration: 300 }
                NumberAnimation { target: container; property: "scale"; to: 1.0; duration: 300 }
            }
        }
    }

}

import QtQuick 2.0

Row {
    property int stars: 0
    spacing: 2
    Repeater {
        model: 5
        delegate: Star {
            selected: index<stars
        }
    }
}

import QtQuick 2.0

Rectangle {
    property bool selected: false
    width: 10
    height: 10
    color: selected?"Yellow":"Gray"
}

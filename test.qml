import QtQuick 1.1
import "parser.js" as JS

Item {
    id:root
    width: 360
    height: 640

    Component.onCompleted: JS.load()


    ListModel {  id:listModel }

    Rectangle {
        id: btn
        color: "red"
        width: parent.width
        height: 50
        anchors.top: parent.top
        anchors.left: parent.left

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onClicked: JS.bench();
        }
    }

    ListView {
        id:view
        width: parent.width
        height: parent.height - 60
        anchors.top: btn.bottom
        anchors.topMargin: 10
        clip: true
        model : listModel
        delegate: Rectangle {
            width:parent.width
            height:80
            Text {
                anchors.centerIn: parent
                text: date
            }

        }
    }

}

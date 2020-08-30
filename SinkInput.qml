import QtQuick 2.0
import QtQuick.Controls 2.1
import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan

Qan.NodeItem {
    id: sinkInputNode
    property string sinkInputName: "<UNKNOWN>"
    property int index: 0
    function set(name, idx) {
        sinkInputName = name;
        index = idx;
    }

    width: 100
    height: 50
    Item {
        anchors.fill: parent
        Rectangle {
            id: background
            anchors.fill: parent
            color: "white"
            border.color: "black"
            border.width: 2
            Label {
                anchors.fill: parent
                text: sinkInputNode.sinkInputName
            }
        }
    }
}

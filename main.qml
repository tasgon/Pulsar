import QtQuick 2.12
import QtQuick.Window 2.12
import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Pulsar")


    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        graph: Qan.Graph {
            id: graph
            property var sinkInputNodeDelegate: Qt.createComponent("qrc:/SinkInput.qml")
            connectorEnabled: true
            onEdgeClicked: { graph.removeEdge(edge) }

            Component.onCompleted: {
                pulse.init()
            }

            Connections {
                target: pulse
                onUpdateSinkInput: {
                    var node = graph.insertNode(graph.sinkInputNodeDelegate);
                    node.item.set(name, index);

                    node.item.x = x;
                    node.item.y = y;
                }
            }
        }
    }
}

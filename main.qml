import QtQuick 2.12
import QtQuick.Window 2.12
import QuickQanava 2.0 as Qan
import "qrc:/QuickQanava" as Qan

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Pulsar")

    Qan.Graph {
        id: graph
        property var test: function (i) { console.log(i) }
        Component.onCompleted: {
            graph.test("wassup")
            var n3 = graph.insertNode()
            n3.label = "N3"; n3.item.x = 500; n3.item.y = 100
            var n3p1 = graph.insertInPort(n3, Qan.NodeItem.Left);
            n3p1.label = "IN #1"

            var n3p1 = graph.insertInPort(n3, Qan.NodeItem.Top);
            n3p1.label = "OUT #1"
            var n3p2 = graph.insertInPort(n3, Qan.NodeItem.Bottom);
            n3p2.label = "OUT #2"

            var e = graph.insertEdge(n2, n3)
            graph.bindEdgeDestination(e, n2p3)  // Bind our edge source to node N2 port P3 (OUT #1)
            graph.bindEdgeDestination(e, n3p1)  // Bind our edge destination to node N3 port P1 (IN #1)
        }
    }
}

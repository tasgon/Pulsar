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
            property var sinkNodeDelegate: Qt.createComponent("qrc:/Sink.qml")
            property var sinkInputNodeDelegate: Qt.createComponent("qrc:/SinkInput.qml")
            property var sinks: ({})
            property var sinkInputs: ({})
            property var allowedConnections: {
                "sinkInput": ["sink"],
            }

            function delNode(nodeList, index) {
                graph.removeNode(nodeList[index]);
                delete nodeList[index];
            }

            function getOrCreate(nodeList, index, delegate, x_offset = 0) {
                var node = nodeList[index];
                if (node === undefined) {
                    node = graph.insertNode(delegate);

                    var keys = Object.keys(nodeList);
                    var x, y;
                    if (keys.length === 0) {
                        x = x_offset; y = 0;
                    } else {
                        var lNode = nodeList[keys[keys.length - 1]];
                        x = lNode.item.x;
                        y = lNode.item.y + 80;
                    }

                    nodeList[index] = node;
                    node.item.x = x;
                    node.item.y = y;
                }
                return node;
            }

            connectorEnabled: true
            onEdgeClicked: { graph.removeEdge(edge) }
            onEdgeInserted: {
                // Establish our connection rules
                var src = edge.getSource().item;
                var dest = edge.getDestination().item;
                if (!allowedConnections.hasOwnProperty(src.nodeType)) {
                    console.log("Cannot connect", src.nodeType, "to anything");
                    graph.removeEdge(edge);
                }

                var allowed = allowedConnections[src.nodeType];
                if (!allowed.includes(dest.nodeType)) {
                    console.log("Cannot connect", src.nodeType, "to", dest.nodeType);
                    graph.removeEdge(edge);
                }
            }

            Component.onCompleted: {
                pulse.init();
            }

            Connections {
                target: pulse
                function onUpdateSink(name, index) {
                    var node = graph.getOrCreate(graph.sinks, index, graph.sinkNodeDelegate, 200);
                    node.item.set(name, index);
                }
                function onRemoveSink(index) { graph.delNode(graph.sinks, index); }
                function onUpdateSinkInput(name, index) {
                    var node = graph.getOrCreate(graph.sinkInputs, index, graph.sinkInputNodeDelegate);
                    node.item.set(name, index);
                }
                function onRemoveSinkInput(index) { graph.delNode(graph.sinkInputs, index); }
            }
        }
    }
}

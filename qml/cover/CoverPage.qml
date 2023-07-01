import QtQuick 2.6
import Sailfish.Silica 1.0
import RawViewQuick 1.0

CoverBackground {

    CoverPlaceholder {
        text: qsTr('Raw View')
        icon.source: 'image://theme/icon-l-image'
        visible: appModel.imageList.rowCount() === 0
    }

    PagedView {
        id: view

        anchors.fill: parent
        wrapMode: PagedView.NoWrap

        model: appModel.imageList

        currentIndex: appView.currentIndex

        delegate: Component {
            id: delegateComponent
            Loader {
                sourceComponent: Component {
                    Item {
                        width: 0.85 * view.width
                        height: 0.85 * view.height

                        ImageItem {
                            anchors.fill: parent
                            image: preview
                            scale: 1
                            scaleCenter: Qt.point(0, 0)
                        }
                    }
                }
            }
        }
    }

}

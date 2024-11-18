import QtQuick 2.6
import Sailfish.Silica 1.0
import RawViewQuick 1.0

CoverBackground {

    CoverPlaceholder {
        text: qsTr('Raw View')
        icon.source: 'image://theme/icon-l-image'
        visible: appView.imageListEmpty
    }

    property bool currentImageIsNotFirst: !appView.imageListEmpty && (appView.imageListCurrentIndex - 1 >= 0)
    property bool currentImageIsNotLast: !appView.imageListEmpty && (appView.imageListCurrentIndex + 1 < appView.imageListCount)

    CoverActionList {
        CoverAction {
            iconSource: 'image://theme/icon-cover-previous'
            onTriggered: appView.imageListCurrentIndex = currentImageIsNotFirst ? appView.imageListCurrentIndex - 1 : appView.imageListCurrentIndex
        }
        CoverAction {
            iconSource: 'image://theme/icon-cover-next'
            onTriggered: appView.imageListCurrentIndex = currentImageIsNotLast ? appView.imageListCurrentIndex + 1 : appView.imageListCurrentIndex
        }
        enabled: currentImageIsNotFirst && currentImageIsNotLast
    }
    CoverActionList {
        CoverAction {
            iconSource: 'image://theme/icon-cover-previous'
            onTriggered: appView.imageListCurrentIndex = currentImageIsNotFirst ? appView.imageListCurrentIndex - 1 : appView.imageListCurrentIndex
        }
        enabled: currentImageIsNotFirst && !currentImageIsNotLast
    }
    CoverActionList {
        CoverAction {
            iconSource: 'image://theme/icon-cover-next'
            onTriggered: appView.imageListCurrentIndex = currentImageIsNotLast ? appView.imageListCurrentIndex + 1 : appView.imageListCurrentIndex
        }
        enabled: !currentImageIsNotFirst && currentImageIsNotLast
    }

    PagedView {
        id: view
        cacheSize: 3

        anchors.fill: parent
        wrapMode: PagedView.NoWrap
        moveDuration: 400

        model: appModel.imageList

        currentIndex: appView.imageListCurrentIndex
        visible: !appView.imageListEmpty

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

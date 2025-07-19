import QtQuick 2.6
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import Sailfish.Share 1.0
import RawViewQuick 1.0

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    property bool currentImageIsNotFirst: !appView.imageListEmpty && (appView.imageListCurrentIndex - 1 >= 0)
    property bool currentImageIsNotLast: !appView.imageListEmpty && (appView.imageListCurrentIndex + 1 < appView.imageListCount)

    function formatSpeed(speed) {
        if (speed >= 1)
            return qsTr('%1"').arg(speed)
        return qsTr('1/%1"').arg(Math.round(1 / speed))
    }

    function labelText(cameraMaker, cameraModel, lensMaker, lensModel, aperture, focalLength, shutterSpeed, iso) {
        var result = ''
        if (cameraMaker !== '')
            result += ' ' + cameraMaker
        if (cameraModel !== '')
            result += ' ' + cameraModel
        else
            result += ' ' + qsTr('unknown camera')
        if (lensModel !== '' && lensMaker !== '')
            result += ' \u2022 ' + lensMaker + ' ' + lensModel
        else if (lensModel !== '')
            result += ' \u2022 ' + lensModel
        if (aperture > 0)
            result += ' \u2022 ' + 'f/' + aperture.toLocaleString(Qt.locale(),
                                                                  'f', 1)
        if (focalLength > 0)
            result += ' \u2022 ' + focalLength.toLocaleString(Qt.locale(), 'f',
                                                              1) + '\u202Fmm'
        if (shutterSpeed > 0)
            result += ' \u2022 ' + formatSpeed(shutterSpeed)
        if (iso > 0)
            result += ' \u2022 ' + 'ISO\u00A0' + iso
        return result
    }

    Component {
        id: quickSelector

        Page {
            id: quickSelectorPage

            allowedOrientations: Orientation.All

            SilicaListView {
                id: listView

                anchors.fill: parent

                model: appModel.imageList
                visible: !appView.imageListEmpty

                delegate: ListItem {
                    id: listItem
                    contentHeight: Theme.itemSizeLarge

                    highlighted: index === view.currentIndex

                    MouseArea {
                        width: listItem.contentWidth
                        height: listItem.contentHeight
                        onDoubleClicked: {
                            appView.imageListCurrentIndex = index
                            pageStack.pop()
                        }
                    }
                    ImageItem {
                        width: listItem.contentHeight
                        height: listItem.contentHeight
                        fillMode: ImageItem.PreserveAspectCrop
                        image: thumbNail
                        scale: 1
                    }
                    Column {
                        padding: Theme.paddingMedium
                        leftPadding: Theme.horizontalPageMargin + listItem.contentHeight
                        rightPadding: Theme.horizontalPageMargin
                        anchors.fill: parent
                        Label {
                            truncationMode: TruncationMode.Fade
                            text: fileName
                        }
                        Label {
                            truncationMode: TruncationMode.Fade
                            text: dateTimeOriginal.toLocaleString(Qt.locale())
                            font.pixelSize: Theme.fontSizeSmall
                            color: Theme.secondaryColor
                        }
                    }
                }

                VerticalScrollDecorator {
                    flickable: listView
                }

                Component.onCompleted: positionViewAtIndex(
                                           appView.imageListCurrentIndex,
                                           ListView.Beginning)
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: mainPage.height

        Component {
            id: folderPickerDialog

            FolderPickerPage {
                showNavigationIndicator: true
                showSystemFiles: true
                dialogTitle: qsTr('Show images in')
                onSelectedPathChanged: {
                    appModel.imageFolder = selectedPath
                    pageStack.replace('LoadImages.qml')
                }
            }
        }

        PullDownMenu {
            visible: appModel.showImageInfo || imageListEmpty

            MenuItem {
                text: qsTr('About Raw View')
                onClicked: pageStack.push(Qt.resolvedUrl('About.qml'))
            }

            MenuItem {
                text: qsTr('Choose image folder')
                onClicked: pageStack.push(folderPickerDialog)
            }

            MenuItem {
                text: qsTr('Quick selection')
                visible: !imageListEmpty
                onClicked: pageStack.push(quickSelector)
            }
        }
        PagedView {
            id: view
            cacheSize: 3

            property double p_scale: 1
            property point p_scale_origin: Qt.point(0, 0)

            anchors.fill: parent
            wrapMode: PagedView.NoWrap

            model: appModel.imageList
            visible: !appView.imageListEmpty

            dragThreshold: Theme.startDragDistance / 4
            moveDragThreshold: Theme.startDragDistance / 4
            moveDuration: 400

            delegate: Component {
                id: delegateComponent
                Loader {
                    asynchronous: true
                    visible: status === Loader.Ready
                    sourceComponent: Component {
                        Item {
                            width: view.width
                            height: view.height

                            ShareAction {
                                id: shareAction
                                mimeType: 'image/*'
                             }

                            ImageItem {
                                anchors.fill: parent
                                image: preview
                                scale: view.p_scale
                                scaleCenter: view.p_scale_origin
                            }
                            TextArea {
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: -Theme.paddingSmall
                                labelVisible: false
                                horizontalAlignment: TextEdit.AlignHCenter
                                verticalAlignment: TextEdit.AlignBottom
                                text: labelText(cameraMaker, cameraModel,
                                                lensMaker, lensModel, aperture,
                                                focalLength, shutterSpeed, iso)
                                color: Theme.primaryColor
                                backgroundStyle: TextEditor.NoBackground
                                readOnly: true
                                visible: appModel.showImageInfo
                            }
                            PinchArea {
                                id: pinchArea
                                anchors.fill: parent

                                MouseArea {
                                    anchors.fill: parent
                                    onDoubleClicked: appModel.showImageInfo
                                                     = !appModel.showImageInfo
                                }
                                onPinchUpdated: {
                                    view.p_scale = Math.max(
                                                Math.min(pinch.scale, 4), 1)
                                    view.p_scale_origin = pinch.center
                                }
                                onPinchFinished: {
                                    view.p_scale = 1
                                    view.p_scale_origin = Qt.point(0, 0)
                                }
                            }
                            IconButton {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.topMargin: Theme.paddingLarge
                                anchors.leftMargin: Theme.paddingMedium
                                icon.source: 'image://theme/icon-m-about'
                                visible: appModel.showImageInfo
                                enabled: appModel.showImageInfo
                                onClicked: pageStack.push(Qt.resolvedUrl(
                                                              'Metadata.qml'))
                            }
                            IconButton {
                                anchors.top: parent.top
                                anchors.right: parent.right
                                anchors.topMargin: Theme.paddingLarge
                                anchors.rightMargin: Theme.paddingMedium
                                icon.source: 'image://theme/icon-m-share'
                                visible: appModel.showImageInfo
                                enabled: appModel.showImageInfo
                                onClicked: {
                                    shareAction.resources = [
                                        appModel.imageList.get(index).share(true)
                                    ]
                                    shareAction.trigger()
                                }
                            }
                            IconButton {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: Theme.paddingSmall
                                icon.source: 'image://theme/icon-m-left'
                                visible: appModel.showImageInfo && currentImageIsNotFirst
                                enabled: appModel.showImageInfo && currentImageIsNotFirst
                                onClicked: appView.imageListCurrentIndex = currentImageIsNotFirst ? appView.imageListCurrentIndex - 1 : appView.imageListCurrentIndex
                            }
                            IconButton {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: Theme.paddingSmall
                                icon.source: 'image://theme/icon-m-right'
                                visible: appModel.showImageInfo && currentImageIsNotLast
                                enabled: appModel.showImageInfo && currentImageIsNotLast
                                onClicked: appView.imageListCurrentIndex = currentImageIsNotLast ? appView.imageListCurrentIndex + 1 : appView.imageListCurrentIndex
                            }
                        }
                    }
                }
            }

            currentIndex: appView.imageListCurrentIndex

            Binding {
                target: appView
                property: 'imageListCurrentIndex'
                value: view.currentIndex
            }

            Binding {
                target: appView
                property: 'imageListCount'
                value: view.count
            }
        }

        ColumnLayout {
            anchors.fill: parent
            visible: appView.imageListEmpty

            TextArea {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('No images found in chosen folder.')
                readOnly: true
                font.pixelSize: Theme.fontSizeLarge
                horizontalAlignment: TextEdit.AlignHCenter
                backgroundStyle: TextEditor.NoBackground
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: 'image://theme/icon-l-image'
                width: 2 * Theme.iconSizeExtraLarge
                height: 2 * Theme.iconSizeExtraLarge
                sourceSize.width: width
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    Component.onCompleted: pageStack.pushAttached(quickSelector)
}

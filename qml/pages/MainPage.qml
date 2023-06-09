import QtQuick 2.6
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import RawViewQuick 1.0

Page {
    id: mainPage

    function formatSpeed(speed) {
        if (speed >= 1)
            return qsTr('%1"').arg(speed)
        return qsTr('1/%1"').arg(Math.round(1 / speed))
    }

    function labelText(fileName, cameraMaker, cameraModel, lensMaker, lensModel, aperture, focalLength, shutterSpeed, iso) {
        var result = fileName + ':'
        if (cameraMaker !== '')
            result += ' ' + cameraMaker
        if (cameraModel !== '')
            result += ' ' + cameraModel
        else
            result += ' ' + qsTr('unknown camera')
        if (lensModel !== '' && lensMaker !== '')
            result += ', ' + lensMaker + ' ' + lensModel
        else if (lensModel !== '')
            result += ', ' + lensModel
        if (aperture !== 0)
            result += ', ' + 'f/' + aperture.toFixed(1)
        result += ', ' + focalLength.toFixed(1) + 'mm, ' + formatSpeed(
                    shutterSpeed) + ', ' + 'ISO ' + iso
        return result
    }

    Component {
        id: quickSelector

        Page {
            id: quickSelectorPage

            SilicaListView {
                id: listView

                anchors.fill: parent

                model: appModel.imageList
                visible: !appView.imageListEmpty

                delegate: ListItem {
                    contentHeight: Theme.itemSizeSmall

                    highlighted: index === view.currentIndex

                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr('Show image')
                            onClicked: {
                                view.currentIndex = index
                                pageStack.pop()
                            }
                        }
                    }
                    Item {
                        id: item
                        height: Theme.itemSizeSmall
                        width: parent.width
                        Label {
                            anchors.verticalCenter: item.verticalCenter
                            x: Theme.horizontalPageMargin
                            width: parent.width - 2 * x
                            truncationMode: TruncationMode.Fade
                            text: fileName
                        }
                    }
                }

                VerticalScrollDecorator {
                    flickable: listView
                }
            }
            onStatusChanged: {
                if (status === PageStatus.Active) {
                    listView.positionViewAtIndex(appView.imageListCurrentIndex,
                                                 ListView.Center)
                }
            }
        }
    }

    allowedOrientations: Orientation.All

    property bool show_image_information: false

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: mainPage.height

        Component {
            id: folderPickerDialog

            FolderPickerPage {
                showNavigationIndicator: true
                showSystemFiles: true
                dialogTitle: qsTr('Show images in')
                onSelectedPathChanged: appModel.imageFolder = selectedPath
            }
        }

        PullDownMenu {
            MenuItem {
                text: qsTr('About Raw View')
                onClicked: pageStack.push(Qt.resolvedUrl('About.qml'))
            }

            MenuItem {
                text: show_image_information ? qsTr('Hide image information') : qsTr(
                                                   'Show image information')
                onClicked: {
                    show_image_information = !show_image_information
                }
            }

            MenuItem {
                text: qsTr('Choose image folder')
                onClicked: pageStack.animatorPush(folderPickerDialog)
            }

            MenuItem {
                text: qsTr('Quick selection')
                onClicked: pageStack.animatorPush(quickSelector)
            }
        }
        PagedView {
            id: view

            property double p_scale: 1
            property point p_scale_origin: Qt.point(0, 0)

            anchors.fill: parent
            wrapMode: PagedView.NoWrap

            model: appModel.imageList
            visible: !appView.imageListEmpty

            delegate: Component {
                id: delegateComponent
                Loader {
                    sourceComponent: Component {
                        Item {
                            width: view.width
                            height: view.height

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
                                verticalAlignment: TextEdit.AlignBottom
                                text: labelText(fileName, cameraMaker,
                                                cameraModel, lensMaker,
                                                lensModel, aperture,
                                                focalLength, shutterSpeed, iso)
                                color: Theme.highlightColor
                                backgroundStyle: TextEditor.NoBackground
                                readOnly: true
                                visible: show_image_information
                            }
                        }
                    }
                }
            }

            Binding {
                target: appView
                property: 'imageListCurrentIndex'
                value: view.currentIndex
            }

            PinchArea {
                id: pinchArea
                anchors.fill: parent

                onPinchUpdated: {
                    view.p_scale = Math.max(Math.min(pinch.scale, 4), 1)
                    view.p_scale_origin = pinch.center
                }
                onPinchFinished: {
                    view.p_scale = 1
                    view.p_scale_origin = Qt.point(0, 0)
                }
            }
        }

        ColumnLayout {
            anchors.fill: parent
            visible: appView.imageListEmpty

            TextArea {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr('No raw-format images found in chosen image folder.')
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
}

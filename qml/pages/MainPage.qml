import QtQuick 2.6
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import RawViewQuick 1.0

Page {
    id: page

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

    allowedOrientations: Orientation.All

    property bool show_image_information: false

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: page.height

        Component {
            id: folderPickerDialog

            FolderPickerPage {
                showNavigationIndicator: true
                showSystemFiles: true
                dialogTitle: qsTr('Show images in')
                onSelectedPathChanged: application.imageFolder = selectedPath
            }
        }

        PullDownMenu {
            MenuItem {
                text: qsTr('Choose image folder')
                onClicked: pageStack.animatorPush(folderPickerDialog)
            }
            MenuItem {
                text: show_image_information ? qsTr('Hide image information') : qsTr(
                                                   'Show image information')
                onClicked: {
                    show_image_information = !show_image_information
                    console.log(show_image_information)
                }
            }
        }
        PagedView {
            id: view

            property double p_scale: 1
            property point p_scale_origin: Qt.point(0, 0)

            anchors.fill: parent
            wrapMode: PagedView.NoWrap

            model: application.imageList

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
                            Text {
                                anchors.bottom: parent.bottom
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: labelText(fileName, cameraMaker,
                                                cameraModel, lensMaker,
                                                lensModel, aperture,
                                                focalLength, shutterSpeed, iso)
                                color: Theme.highlightColor
                                visible: show_image_information
                            }
                        }
                    }
                }
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
    }
}

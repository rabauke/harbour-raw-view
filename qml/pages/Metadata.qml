import QtQuick 2.2
import Sailfish.Silica 1.0
import "../components"

Page {
    id: metadata_page

    allowedOrientations: Orientation.All

    function formatSpeed(speed) {
        if (speed >= 1)
            return qsTr('%1"').arg(speed)
        return qsTr('1/%1"').arg(Math.round(1 / speed))
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            width: parent.width
            spacing: 0
            PageHeader {
                title: qsTr('Image metadata')
            }
            SectionHeader {
                text: qsTr('File')
            }
            DetailItem {
                label: qsTr('File name')
                value: appModel.imageList.get(appView.imageListCurrentIndex).fileName
            }
            DetailItem {
                property real fileSize: appModel.imageList.get(appView.imageListCurrentIndex).fileSize
                label: qsTr('File size')
                value: (fileSize / 1024 / 1024).toLocaleString(Qt.locale(), 'f', 2) + '\u202FMB'
            }
            DetailItem {
                property int imageWidth: appModel.imageList.get(appView.imageListCurrentIndex).imageWidth
                property int imageHeight: appModel.imageList.get(appView.imageListCurrentIndex).imageHeight
                label: qsTr('Size')
                value: imageWidth.toString() + '\u202F×\u202F' + imageHeight.toString()
            }
            DetailItem {
                label: qsTr('Date and time')
                value: appModel.imageList.get(appView.imageListCurrentIndex).dateTimeOriginal.toLocaleString(Qt.locale())
            }
            SectionHeader {
                text: qsTr('Camera details')
            }
            DetailItem {
                label: qsTr('Camera manufacturer')
                value: appModel.imageList.get(appView.imageListCurrentIndex).cameraMaker
            }
            DetailItem {
                label: qsTr('Camera model')
                value: appModel.imageList.get(appView.imageListCurrentIndex).cameraModel
            }
            DetailItem {
                label: qsTr('Lens manufacturer')
                value: appModel.imageList.get(appView.imageListCurrentIndex).lensMaker
            }
            DetailItem {
                label: qsTr('Lens model')
                value: appModel.imageList.get(appView.imageListCurrentIndex).lensModel
            }
            DetailItem {
                property real focalLength: appModel.imageList.get(appView.imageListCurrentIndex).focalLength
                label: qsTr('Focal length')
                value: focalLength > 0 ? focalLength.toLocaleString(Qt.locale(), 'f', 1) + '\u202Fmm' : ''
            }
            DetailItem {
                property real aperture: appModel.imageList.get(appView.imageListCurrentIndex).aperture
                label: qsTr('Aperture')
                value: aperture > 0 ? 'f/' + aperture.toLocaleString(Qt.locale(), 'f', 1) : ''
            }
            DetailItem {
                property real shutterSpeed: appModel.imageList.get(appView.imageListCurrentIndex).shutterSpeed
                label: qsTr('Shutter speed')
                value: shutterSpeed > 0 ? formatSpeed(shutterSpeed) : ''
            }
            DetailItem {
                property real iso: appModel.imageList.get(appView.imageListCurrentIndex).iso
                label: qsTr('ISO')
                value: iso > 0 ? iso : ''
            }
        }
    }
}

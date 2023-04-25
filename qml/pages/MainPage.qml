import QtQuick 2.6
import QtQuick.Layouts 1.1
import Sailfish.Silica 1.0
import Sailfish.Pickers 1.0
import RawViewQuick 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

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
        }

        PagedView {
            id: view

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
                            }
                        }
                    }
                }
            }
        }
    }
}

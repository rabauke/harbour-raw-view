import QtQuick 2.6
import Sailfish.Silica 1.0
import RawViewQuick 1.0
import "pages"

ApplicationWindow {
    id: appView

    AppModel {
        id: appModel

        onReadingImagesFinished: {
            pageStack.clear()
            pageStack.push(Qt.resolvedUrl('pages/MainPage.qml'))
        }
    }

    property int imageListCurrentIndex: 0
    property int imageListCount
    property bool imageListEmpty: appModel.imageList.rowCount() === 0

    initialPage: Component {
        LoadImages {}
    }
    cover: Qt.resolvedUrl('cover/CoverPage.qml')
    allowedOrientations: defaultAllowedOrientations
}

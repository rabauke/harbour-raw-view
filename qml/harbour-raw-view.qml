import QtQuick 2.6
import Sailfish.Silica 1.0
import RawViewQuick 1.0
import "pages"

ApplicationWindow {
    id: appView

    AppModel {
        id: appModel
    }

    property int imageListCurrentIndex: 0
    property int imageListCount
    property bool imageListEmpty: appModel.imageList.rowCount() === 0

    initialPage: Component {
        MainPage {}
    }
    cover: Qt.resolvedUrl('cover/CoverPage.qml')
    allowedOrientations: defaultAllowedOrientations
}

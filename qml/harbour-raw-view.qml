import QtQuick 2.6
import Sailfish.Silica 1.0
import RawViewQuick 1.0
import "pages"

ApplicationWindow {
    id: appView

    property int currentIndex

    AppModel {
        id: appModel
    }

    initialPage: Component {
        MainPage {}
    }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}

import QtQuick 2.0
import Sailfish.Silica 1.0
import RawViewQuick 1.0
import "pages"

ApplicationWindow {

    AppModel {
        id: application
    }

    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
}

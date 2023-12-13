import QtQuick 2.2
import Sailfish.Silica 1.0

Item {
    property string componnet: ''
    property string version: ''
    property string license: ''
    property string url: ''

    width: parent.width
    height: label1.height + label2.height + Theme.paddingLarge

    Label {
        id: label1
        x: Theme.horizontalPageMargin
        width: parent.width - 2 * x
        text: componnet + (version !== '' ? ' ' + qsTr('version') + ' ' + version : '')
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeMedium
    }
    Label {
        id: label2
        x: Theme.horizontalPageMargin
        width: parent.width - 2 * x
        anchors.top: label1.bottom
        text: qsTr('license') + ': ' + license + '<br>' + qsTr('url') + ': <a href=\"' + url + '\">' + url + '</a>'
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeSmall
        textFormat: Text.StyledText
        linkColor: Theme.secondaryColor
        onLinkActivated: {
            Qt.openUrlExternally(link)
        }
    }
}

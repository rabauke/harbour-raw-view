import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
  id: about_page

  SilicaFlickable {
    anchors.fill: parent
    contentHeight: column.height

    Column {
      id: column

      width: parent.width
      spacing: Theme.paddingMedium
      PageHeader {
        title: "Raw View"
      }
      Text {
        x: Theme.horizontalPageMargin
        width: column.width-2*x
        color: Theme.primaryColor
        wrapMode: TextEdit.Wrap
        font.pixelSize: Theme.fontSizeMedium
        horizontalAlignment: TextEdit.AlignJustify
        text: qsTr("_description_")
        textFormat: Text.StyledText
        linkColor: Theme.highlightColor
        onLinkActivated: { Qt.openUrlExternally(link) }
      }
      Label {
        text: "<br>© Heiko Bauke, 2023<br><br>Fork me on github!<br><a href=\"https://github.com/rabauke/harbour-raw-view\">https://github.com/rabauke/harbour-raw-view</a>"
        textFormat: Text.StyledText
        width: column.width
        color: Theme.primaryColor
        linkColor: Theme.highlightColor
        wrapMode: TextEdit.Wrap
        font.pixelSize: Theme.fontSizeSmall
        horizontalAlignment: TextEdit.AlignHCenter
        onLinkActivated: { Qt.openUrlExternally(link) }
      }

    }

  }

}

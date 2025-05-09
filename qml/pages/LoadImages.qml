import QtQuick 2.2
import Sailfish.Silica 1.0
import RawViewQuick 1.0

Page {
  id: loadImagesPage

  BusyIndicator {
    id: busy
    size: BusyIndicatorSize.Large
    anchors.centerIn: parent
    running: true
  }
  onStatusChanged: {
    if (status === PageStatus.Active)
      appModel.loadImages(appModel.imageFolder)
  }
}

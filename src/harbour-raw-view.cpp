#ifdef QT_QML_DEBUG
  #include <QtQuick>
#endif
#include <QQuickView>
#include <sailfishapp.h>
#include "AppModel.hpp"
#include "ImagePropertiesModel.hpp"
#include "ImageItem.hpp"
#include "FolderListModel.hpp"


int main(int argc, char *argv[]) {
  QScopedPointer<QGuiApplication> app{SailfishApp::application(argc, argv)};
  app->setApplicationName("harbour-raw-view");
  app->setOrganizationDomain("rabauke");

  qmlRegisterType<ImageItem>("RawViewQuick", 1, 0, "ImageItem");
  qmlRegisterType<ImagePropertiesModel>("RawViewQuick", 1, 0, "ImagePropertiesModel");
  qRegisterMetaType<ImagePropertiesModel *>();
  qmlRegisterType<AppModel>("RawViewQuick", 1, 0, "AppModel");

  QScopedPointer<QQuickView> view{SailfishApp::createView()};
  view->setSource(SailfishApp::pathTo("qml/harbour-raw-view.qml"));
  view->show();
  return app->exec();
}

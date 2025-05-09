#pragma once

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QQmlListProperty>
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  #include <QtQmlIntegration>
#endif
#include <QStringListModel>
#include <QFileSystemWatcher>
#include "ImageListModel.hpp"
#include "Version.h"


class AppModel : public QObject {
  Q_OBJECT

public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  QML_NAMED_ELEMENT(AppModel)
#endif

  explicit AppModel(QObject* parent = nullptr);
  ~AppModel();

  Q_PROPERTY(QString version MEMBER m_version CONSTANT)
  Q_PROPERTY(
      QUrl imageFolder READ get_image_folder WRITE set_image_folder NOTIFY image_folder_changed)
  Q_PROPERTY(ImageListModel* imageList READ get_image_list NOTIFY image_list_changed)
  Q_PROPERTY(bool showImageInfo READ get_show_image_info WRITE set_show_image_info NOTIFY
                 show_image_info_changed)
  Q_INVOKABLE void loadImages(const QUrl& image_folder);

signals:
  void image_folder_changed();
  void image_list_changed();
  void show_image_info_changed();
  void readingImagesFinished();
  void readingImagesFailed();

private:
  [[nodiscard]] QUrl get_image_folder() const;
  void set_image_folder(const QUrl& new_image_folder);

  [[nodiscard]] ImageListModel* get_image_list() const;

  [[nodiscard]] bool get_show_image_info() const;
  void set_show_image_info(bool);

  QString m_version{QString::fromStdString(project_version)};
  QUrl m_image_folder;
  QScopedPointer<ImageListModel> m_image_list_model;
  bool m_show_image_info;
};

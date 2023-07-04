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
      QUrl imageFolder READ image_folder WRITE set_image_folder NOTIFY image_folder_changed)
  Q_PROPERTY(ImageListModel* imageList READ get_image_list NOTIFY image_list_changed)

signals:
  void image_folder_changed();
  void image_list_changed();

private:
  [[nodiscard]] QUrl image_folder() const;
  void set_image_folder(const QUrl& new_image_folder);

  [[nodiscard]] ImageListModel* get_image_list() const;

  void generate_image_file_names();

  QString m_version{QString::fromStdString(project_version)};
  QUrl m_image_folder;
  QScopedPointer<ImageListModel> m_image_list_model;
};

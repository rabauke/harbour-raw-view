#pragma once

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QUrl>
#include <QQmlListProperty>
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  #include <QtQmlIntegration>
#endif
#include <QStringListModel>
#include "Image.hpp"


class ImagePropertiesModel : public QObject {
  Q_OBJECT

public:
  explicit ImagePropertiesModel(QObject* parent = nullptr);
  explicit ImagePropertiesModel(const QFileInfo& file_info, QObject* parent = nullptr);

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  QML_NAMED_ELEMENT(ImagePropertiesModel)
#endif

  Q_PROPERTY(QString fileName READ file_name NOTIFY image_properties_changed)
  Q_PROPERTY(double fileSize READ file_size NOTIFY image_properties_changed)
  Q_PROPERTY(double imageWidth READ image_width NOTIFY image_properties_changed)
  Q_PROPERTY(double imageHeight READ image_height NOTIFY image_properties_changed)
  Q_PROPERTY(QString cameraMaker READ camera_maker NOTIFY image_properties_changed)
  Q_PROPERTY(QString cameraModel READ camera_model NOTIFY image_properties_changed)
  Q_PROPERTY(QString lensMaker READ lens_maker NOTIFY image_properties_changed)
  Q_PROPERTY(QString lensModel READ lens_model NOTIFY image_properties_changed)
  Q_PROPERTY(double focalLength READ focal_length NOTIFY image_properties_changed)
  Q_PROPERTY(double aperture READ aperture NOTIFY image_properties_changed)
  Q_PROPERTY(double shutterSpeed READ shutter_speed NOTIFY image_properties_changed)
  Q_PROPERTY(double iso READ iso NOTIFY image_properties_changed)
  Q_PROPERTY(QDateTime dateTimeOriginal READ date_time_original NOTIFY image_properties_changed)
  Q_PROPERTY(QImage preview READ preview NOTIFY image_properties_changed)

  [[nodiscard]] QString file_name();
  [[nodiscard]] double file_size();
  [[nodiscard]] double image_width();
  [[nodiscard]] double image_height();
  [[nodiscard]] QString camera_maker();
  [[nodiscard]] QString camera_model();
  [[nodiscard]] QString lens_maker();
  [[nodiscard]] QString lens_model();
  [[nodiscard]] double focal_length();
  [[nodiscard]] double aperture();
  [[nodiscard]] double shutter_speed();
  [[nodiscard]] double iso();
  [[nodiscard]] QDateTime date_time_original();
  [[nodiscard]] QImage preview();

signals:
  void image_properties_changed();

private:
  Image m_image;
};

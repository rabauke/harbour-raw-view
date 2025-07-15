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
  Q_PROPERTY(QPixmap preview READ preview NOTIFY image_properties_changed)
  Q_PROPERTY(QPixmap thumbNail READ thumb_nail NOTIFY image_properties_changed)
  Q_PROPERTY(QString absoluteFilePath READ absolute_file_path NOTIFY image_properties_changed)

  [[nodiscard]] const QFileInfo &file_info() const;
  [[nodiscard]] QString file_name() const;
  [[nodiscard]] double file_size() const;
  [[nodiscard]] double image_width() const;
  [[nodiscard]] double image_height() const;
  [[nodiscard]] QString camera_maker() const;
  [[nodiscard]] QString camera_model() const;
  [[nodiscard]] QString lens_maker() const;
  [[nodiscard]] QString lens_model() const;
  [[nodiscard]] double focal_length() const;
  [[nodiscard]] double aperture() const;
  [[nodiscard]] double shutter_speed() const;
  [[nodiscard]] double iso() const;
  [[nodiscard]] QDateTime date_time_original() const;
  [[nodiscard]] QPixmap preview();
  [[nodiscard]] QPixmap thumb_nail();
  [[nodiscard]] QString absolute_file_path();

signals:
  void image_properties_changed();

private:
  Image m_image;
};

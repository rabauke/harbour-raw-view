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

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  QML_NAMED_ELEMENT(ImagePropertiesModel)
#endif

  Q_PROPERTY(bool isValid READ is_valid NOTIFY image_properties_changed)
  Q_PROPERTY(QString fileName READ file_name NOTIFY image_properties_changed)
  Q_PROPERTY(QString maker READ camera_maker NOTIFY image_properties_changed)
  Q_PROPERTY(QString model READ camera_model NOTIFY image_properties_changed)
  Q_PROPERTY(QString lensMaker READ lens_maker NOTIFY image_properties_changed)
  Q_PROPERTY(QString lensModel READ lens_model NOTIFY image_properties_changed)
  Q_PROPERTY(float focalLength READ focal_length NOTIFY image_properties_changed)
  Q_PROPERTY(float aperture READ aperture NOTIFY image_properties_changed)
  Q_PROPERTY(float shutterSpeed READ shutter_speed NOTIFY image_properties_changed)
  Q_PROPERTY(float iso READ iso NOTIFY image_properties_changed)
  Q_PROPERTY(QImage preview READ preview NOTIFY image_properties_changed)

  void load(const QFileInfo& file_info);

  [[nodiscard]] bool is_valid() const;
  [[nodiscard]] QString file_name() const;
  [[nodiscard]] QString camera_maker() const;
  [[nodiscard]] QString camera_model() const;
  [[nodiscard]] QString lens_maker() const;
  [[nodiscard]] QString lens_model() const;
  [[nodiscard]] float focal_length() const;
  [[nodiscard]] float aperture() const;
  [[nodiscard]] float shutter_speed() const;
  [[nodiscard]] float iso() const;
  [[nodiscard]] QImage preview() const;

signals:
  void image_properties_changed();

private:

  Image m_image;
};

#pragma once

#include <QtCore>
#include <QFileInfo>
#include <QtGui>
#include <QAbstractListModel>
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  #include <QtQmlIntegration>
#endif
#include "Image.hpp"
#include "ImagePropertiesModel.hpp"


class ImageListModel : public QAbstractListModel {
  Q_OBJECT

public:
  explicit ImageListModel(QObject *other = nullptr);
  virtual ~ImageListModel() = default;

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  QML_NAMED_ELEMENT(ImageListModel)
#endif

  void set_file_names(const QFileInfoList &file_paths);
  void update_file_names(const QFileInfoList &file_paths);

  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
  Q_INVOKABLE ImagePropertiesModel *get(qint32 index) const;

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  static constexpr int file_name = Qt::UserRole;
  static constexpr int file_size = Qt::UserRole + 1;
  static constexpr int image_width = Qt::UserRole + 2;
  static constexpr int image_height = Qt::UserRole + 3;
  static constexpr int camera_maker = Qt::UserRole + 4;
  static constexpr int camera_model = Qt::UserRole + 5;
  static constexpr int lens_maker = Qt::UserRole + 6;
  static constexpr int lens_model = Qt::UserRole + 7;
  static constexpr int focal_length = Qt::UserRole + 8;
  static constexpr int aperture = Qt::UserRole + 9;
  static constexpr int shutter_speed = Qt::UserRole + 10;
  static constexpr int iso = Qt::UserRole + 11;
  static constexpr int date_time_original = Qt::UserRole + 12;
  static constexpr int preview = Qt::UserRole + 13;

  mutable QList<QSharedPointer<ImagePropertiesModel>> m_images;
};

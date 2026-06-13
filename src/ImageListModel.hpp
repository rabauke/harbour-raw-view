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

  void load_image_meta_data_async(const QFileInfoList &file_paths);

  [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
  Q_INVOKABLE ImagePropertiesModel *get(qint32 index) const;
  Q_INVOKABLE void addImage(const QString &file_path);

  friend class ImageListLoader;

signals:
  void readingFinished();
  void readingError();

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  void load_image_meta_data_async_impl(const QFileInfoList &file_paths);

  static constexpr int file_name = Qt::UserRole;
  static constexpr int is_raw_image = Qt::UserRole + 1;
  static constexpr int file_size = Qt::UserRole + 2;
  static constexpr int image_width = Qt::UserRole + 3;
  static constexpr int image_height = Qt::UserRole + 4;
  static constexpr int camera_maker = Qt::UserRole + 5;
  static constexpr int camera_model = Qt::UserRole + 6;
  static constexpr int lens_maker = Qt::UserRole + 7;
  static constexpr int lens_model = Qt::UserRole + 8;
  static constexpr int focal_length = Qt::UserRole + 9;
  static constexpr int aperture = Qt::UserRole + 10;
  static constexpr int shutter_speed = Qt::UserRole + 11;
  static constexpr int iso = Qt::UserRole + 12;
  static constexpr int date_time_original = Qt::UserRole + 13;
  static constexpr int preview = Qt::UserRole + 14;
  static constexpr int thumb_nail = Qt::UserRole + 15;
  static constexpr int absolute_file_path = Qt::UserRole + 16;

  mutable QList<QSharedPointer<ImagePropertiesModel>> m_images;
};


class ImageListLoader : public QObject {
  Q_OBJECT

public:
  ImageListLoader(ImageListModel &image_list_model, const QFileInfoList &file_paths);
  virtual ~ImageListLoader() = default;

signals:
  void finished();
  void error(QString err);

public slots:
  void load();

private:
  ImageListModel &m_image_list_model;
  QFileInfoList m_file_paths;
};

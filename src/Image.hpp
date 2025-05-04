#pragma once

#include <exception>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QImage>
#include <QDateTime>


class ImageException : public std::runtime_error {
  using base = std::runtime_error;

public:
  ImageException(const std::string &message) : base{message} {}
  ImageException(const char *message) : base{message} {}
};


enum class ImageOrientation {
  unknown = 0,
  original = 1,
  flip_horizontally = 2,
  rot_180 = 3,
  flip_verticallay = 4,
  rot_90_flip_horizontally = 5,
  rot_90 = 6,
  rot_90_flip_vertically = 7,
  rot_270 = 8
};


class Image {
public:
  Image() = default;
  Image(const QFileInfo &file_info);
  const QFileInfo &file_info() const;
  QString file_name() const;
  double file_size() const;
  double image_width() const;
  double image_height() const;
  QString camera_maker() const;
  QString camera_model() const;
  QString lens_maker() const;
  QString lens_model() const;
  double focal_length() const;
  double aperture() const;
  double shutter_speed() const;
  double iso() const;
  QDateTime date_time_original() const;
  QImage preview();

  static const QStringList &supported_file_extensions();

private:
  void load_raw(QImage &image);
  void load_nonraw(QImage &image);
  void load_metadata() const;
  void load_metadata_raw() const;
  void load_metadata_nonraw() const;

  static const QStringList &supported_raw_file_extensions();
  static const QStringList &supported_nonraw_file_extensions();

  QFileInfo m_file_info;
  mutable double m_image_width{0};
  mutable double m_image_height{0};
  mutable bool m_metadata_loaded{false};
  mutable QString m_camera_maker;
  mutable QString m_camera_model;
  mutable QString m_lens_maker;
  mutable QString m_lens;
  mutable double m_focal_length{0};
  mutable double m_aperture{0};
  mutable double m_shutter_speed{0};
  mutable double m_iso{0};
  mutable QDateTime m_date_time_original;
  mutable ImageOrientation m_image_orientation{ImageOrientation::unknown};
};

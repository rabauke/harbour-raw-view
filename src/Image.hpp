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
  QString file_name();
  double file_size();
  double image_width();
  double image_height();
  QString camera_maker();
  QString camera_model();
  QString lens_maker();
  QString lens_model();
  double focal_length();
  double aperture();
  double shutter_speed();
  double iso();
  QDateTime date_time_original();
  QImage preview();

  static const QStringList &supported_file_extensions();

private:
  void load_raw(QImage &image);
  void load_nonraw(QImage &image);
  void load_metadata();
  void load_metadata_raw();
  void load_metadata_nonraw();

  static const QStringList &supported_raw_file_extensions();
  static const QStringList &supported_nonraw_file_extensions();

  QFileInfo m_file_info;
  double m_image_width{0};
  double m_image_height{0};
  bool m_metadata_loaded{false};
  QString m_camera_maker;
  QString m_camera_model;
  QString m_lens_maker;
  QString m_lens;
  double m_focal_length{0};
  double m_aperture{0};
  double m_shutter_speed{0};
  double m_iso{0};
  QDateTime m_date_time_original;
  ImageOrientation m_image_orientation{ImageOrientation::unknown};
};

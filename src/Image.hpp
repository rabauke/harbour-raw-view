#pragma once

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QImage>


class Image {
public:
  Image() = default;
  void reset();
  void load(const QFileInfo &file_info);
  bool is_valid() const;
  QString file_name() const;
  QString camera_maker() const;
  QString camera_model() const;
  QString lens_maker() const;
  QString lens_model() const;
  float focal_length() const;
  float aperture() const;
  float shutter_speed() const;
  float iso() const;
  const QImage &preview() const;

  static const QStringList &supported_file_extensions();

private:
  void load_raw(const QFileInfo &file_info);
  void load_nonraw(const QFileInfo &file_info);

  static const QStringList &supported_raw_file_extensions();
  static const QStringList &supported_nonraw_file_extensions();

  bool m_is_valid{false};
  QString m_file_name;
  QString m_camera_maker;
  QString m_camera_model;
  QString m_lens_maker;
  QString m_lens;
  float m_focal_length{0};
  float m_aperture{0};
  float m_shutter_speed{0};
  float m_iso{0};
  QImage m_preview;
};

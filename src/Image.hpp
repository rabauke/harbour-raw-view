#pragma once

#include <QString>
#include <QImage>


class Image {
public:
  Image() = default;
  void reset();
  void load(const QString &file_path);
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

private:
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

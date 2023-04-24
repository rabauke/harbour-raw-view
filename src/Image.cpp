#include "Image.hpp"
#include <stdexcept>
#include <libraw/libraw.h>
#include <QFileInfo>


void Image::reset() {
  *this = Image();
}


void Image::load(const QString& file_path) {
  reset();
  LibRaw lib_raw;
  if (lib_raw.open_file(file_path.toUtf8().toStdString().c_str()) != LIBRAW_SUCCESS)
    throw std::runtime_error("Ooops");
  m_file_name = QFileInfo(file_path).fileName();
  m_camera_maker = lib_raw.imgdata.idata.make;
  m_camera_model = lib_raw.imgdata.idata.model;
  m_lens_maker = lib_raw.imgdata.lens.LensMake;
  m_lens = lib_raw.imgdata.lens.Lens;
  m_focal_length = lib_raw.imgdata.other.focal_len;
  m_aperture = lib_raw.imgdata.other.aperture;
  m_shutter_speed = lib_raw.imgdata.other.shutter;
  m_iso = lib_raw.imgdata.other.iso_speed;
  if (lib_raw.unpack_thumb() != LIBRAW_SUCCESS)
    throw std::runtime_error("Ooops");
  if (lib_raw.imgdata.thumbnail.tformat == LIBRAW_THUMBNAIL_JPEG) {
    m_preview.loadFromData(reinterpret_cast<const uchar*>(lib_raw.imgdata.thumbnail.thumb),
                           lib_raw.imgdata.thumbnail.tlength);
    if (lib_raw.imgdata.sizes.flip == 3)
      m_preview = m_preview.transformed(QTransform().rotate(180));
    else if (lib_raw.imgdata.sizes.flip == 5)
      m_preview = m_preview.transformed(QTransform().rotate(270));
    else if (lib_raw.imgdata.sizes.flip == 6)
      m_preview = m_preview.transformed(QTransform().rotate(90));
  }
  m_is_valid = true;
}


bool Image::is_valid() const {
  return m_is_valid;
}


QString Image::file_name() const {
  return m_file_name;
}


QString Image::camera_maker() const {
  return m_camera_maker;
}


QString Image::camera_model() const {
  return m_camera_model;
}


QString Image::lens_maker() const {
  return m_lens_maker;
}


QString Image::lens_model() const {
  return m_lens;
}


float Image::focal_length() const {
  return m_focal_length;
}


float Image::aperture() const {
  return m_aperture;
}


float Image::shutter_speed() const {
  return m_shutter_speed;
}


float Image::iso() const {
  return m_iso;
}


const QImage& Image::preview() const {
  return m_preview;
}

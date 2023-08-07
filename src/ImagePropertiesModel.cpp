#include "ImagePropertiesModel.hpp"


ImagePropertiesModel::ImagePropertiesModel(QObject* parent) : QObject{parent} {
}


ImagePropertiesModel::ImagePropertiesModel(const QFileInfo& file_info, QObject* parent)
    : QObject{parent}, m_file_info{file_info} {
}


QString ImagePropertiesModel::file_name() const {
  return m_file_info.fileName();
}


QString ImagePropertiesModel::camera_maker() {
  load();
  return m_image.camera_maker();
}


QString ImagePropertiesModel::camera_model() {
  load();
  return m_image.camera_model();
}


QString ImagePropertiesModel::lens_maker() {
  load();
  return m_image.lens_maker();
}


QString ImagePropertiesModel::lens_model() {
  load();
  return m_image.lens_model();
}


float ImagePropertiesModel::focal_length() {
  load();
  return m_image.focal_length();
}


float ImagePropertiesModel::aperture() {
  load();
  return m_image.aperture();
}


float ImagePropertiesModel::shutter_speed() {
  load();
  return m_image.shutter_speed();
}


float ImagePropertiesModel::iso() {
  load();
  return m_image.iso();
}


QImage ImagePropertiesModel::preview() {
  load();
  return m_image.preview();
}


void ImagePropertiesModel::load() {
  try {
    if (not m_image_loaded and not m_image_failed)
      m_image.load(m_file_info);
    m_image_loaded = true;
  } catch (...) {
    m_image_failed = true;
  }
  emit image_properties_changed();
}

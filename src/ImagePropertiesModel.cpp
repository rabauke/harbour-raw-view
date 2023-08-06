#include "ImagePropertiesModel.hpp"


ImagePropertiesModel::ImagePropertiesModel(QObject* parent) : QObject{parent} {
}


ImagePropertiesModel::ImagePropertiesModel(const QFileInfo& file_info, QObject* parent)
    : QObject{parent}, m_file_info{file_info} {
}


bool ImagePropertiesModel::is_valid() const {
  return m_image.is_valid();
}


QString ImagePropertiesModel::file_name() const {
  return m_image.file_name();
}


QString ImagePropertiesModel::camera_maker() const {
  return m_image.camera_maker();
}


QString ImagePropertiesModel::camera_model() const {
  return m_image.camera_model();
}


QString ImagePropertiesModel::lens_maker() const {
  return m_image.lens_maker();
}


QString ImagePropertiesModel::lens_model() const {
  return m_image.lens_model();
}


float ImagePropertiesModel::focal_length() const {
  return m_image.focal_length();
}


float ImagePropertiesModel::aperture() const {
  return m_image.aperture();
}


float ImagePropertiesModel::shutter_speed() const {
  return m_image.shutter_speed();
}


float ImagePropertiesModel::iso() const {
  return m_image.iso();
}


QImage ImagePropertiesModel::preview() const {
  return m_image.preview();
}


void ImagePropertiesModel::load() {
  try {
    m_image.load(m_file_info);
  } catch (...) {
  }
  emit image_properties_changed();
}

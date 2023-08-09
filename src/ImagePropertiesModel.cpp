#include "ImagePropertiesModel.hpp"


ImagePropertiesModel::ImagePropertiesModel(QObject* parent) : QObject{parent} {
}


ImagePropertiesModel::ImagePropertiesModel(const QFileInfo& file_info, QObject* parent)
    : QObject{parent}, m_image{file_info} {
}


QString ImagePropertiesModel::file_name() {
  return m_image.file_name();
}


QString ImagePropertiesModel::camera_maker() {
  return m_image.camera_maker();
}


QString ImagePropertiesModel::camera_model() {
  return m_image.camera_model();
}


QString ImagePropertiesModel::lens_maker() {
  return m_image.lens_maker();
}


QString ImagePropertiesModel::lens_model() {
  return m_image.lens_model();
}


float ImagePropertiesModel::focal_length() {
  return m_image.focal_length();
}


float ImagePropertiesModel::aperture() {
  return m_image.aperture();
}


float ImagePropertiesModel::shutter_speed() {
  return m_image.shutter_speed();
}


float ImagePropertiesModel::iso() {
  return m_image.iso();
}


QImage ImagePropertiesModel::preview() {
  return m_image.preview();
}

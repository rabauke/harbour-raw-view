#include "ImagePropertiesModel.hpp"


ImagePropertiesModel::ImagePropertiesModel(QObject* parent) : QObject{parent} {
}


ImagePropertiesModel::ImagePropertiesModel(const QFileInfo& file_info, QObject* parent)
    : QObject{parent}, m_image{file_info} {
}


QString ImagePropertiesModel::file_name() {
  return m_image.file_name();
}


double ImagePropertiesModel::file_size() {
  return m_image.file_size();
}


double ImagePropertiesModel::image_width() {
  return m_image.image_width();
}


double ImagePropertiesModel::image_height() {
  return m_image.image_height();
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


double ImagePropertiesModel::focal_length() {
  return m_image.focal_length();
}


double ImagePropertiesModel::aperture() {
  return m_image.aperture();
}


double ImagePropertiesModel::shutter_speed() {
  return m_image.shutter_speed();
}


double ImagePropertiesModel::iso() {
  return m_image.iso();
}


QDateTime ImagePropertiesModel::date_time_original() {
  return m_image.date_time_original();
}


QImage ImagePropertiesModel::preview() {
  return m_image.preview();
}

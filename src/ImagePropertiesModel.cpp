#include "ImagePropertiesModel.hpp"


ImagePropertiesModel::ImagePropertiesModel(QObject* parent) : QObject{parent} {
}


ImagePropertiesModel::ImagePropertiesModel(const QFileInfo& file_info, QObject* parent)
    : QObject{parent}, m_image{file_info} {
}


const QFileInfo& ImagePropertiesModel::file_info() const {
  return m_image.file_info();
}


QString ImagePropertiesModel::file_name() const {
  return m_image.file_name();
}


double ImagePropertiesModel::file_size() const {
  return m_image.file_size();
}


double ImagePropertiesModel::image_width() const {
  return m_image.image_width();
}


double ImagePropertiesModel::image_height() const {
  return m_image.image_height();
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


double ImagePropertiesModel::focal_length() const {
  return m_image.focal_length();
}


double ImagePropertiesModel::aperture() const {
  return m_image.aperture();
}


double ImagePropertiesModel::shutter_speed() const {
  return m_image.shutter_speed();
}


double ImagePropertiesModel::iso() const {
  return m_image.iso();
}


QDateTime ImagePropertiesModel::date_time_original() const {
  return m_image.date_time_original();
}


QPixmap ImagePropertiesModel::preview() {
  return m_image.preview();
}


QPixmap ImagePropertiesModel::thumb_nail() {
  return m_image.thumb_nail();
}

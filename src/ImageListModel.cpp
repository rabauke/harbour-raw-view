#include "ImageListModel.hpp"

ImageListModel::ImageListModel(QObject *other) : QAbstractListModel(other) {
}


int ImageListModel::rowCount([[maybe_unused]] const QModelIndex &parent) const {
  return m_images.count();
}


QVariant ImageListModel::data(const QModelIndex &index, int role) const {
  const int row{index.row()};
  if (0 <= row and row < m_images.count()) {
    auto &image{m_images[row]};
    if (not image->is_valid())
      image->load(m_file_paths.at(row));
    switch (role) {
      case file_name:
        return image->file_name();
      case camera_maker:
        return image->camera_maker();
      case camera_model:
        return image->camera_model();
      case lens_maker:
        return image->lens_maker();
      case lens_model:
        return image->lens_model();
      case focal_length:
        return image->focal_length();
      case aperture:
        return image->aperture();
      case shutter_speed:
        return image->shutter_speed();
      case iso:
        return image->iso();
      case preview:
        return image->preview();
    }
  }
  return {};
}


QHash<int, QByteArray> ImageListModel::roleNames() const {
  static const QHash<int, QByteArray> role_names{
      {file_name, "fileName"},   {camera_maker, "cameraMaker"},   {camera_model, "cameraModel"},
      {lens_maker, "lensMaker"}, {lens_model, "lensModel"},       {focal_length, "focalLength"},
      {aperture, "aperture"},    {shutter_speed, "shutterSpeed"}, {iso, "iso"},
      {preview, "preview"}};
  return role_names;
}


void ImageListModel::set_file_names(const QStringList &file_paths) {
  if (not m_images.empty()) {
    beginRemoveRows(QModelIndex(), 0, m_images.count() - 1);
    m_file_paths.clear();
    m_images.clear();
    endRemoveRows();
  }
  if (not file_paths.empty()) {
    beginInsertRows(QModelIndex(), 0, file_paths.count() - 1);
    m_file_paths = file_paths;
    for ([[maybe_unused]] const auto &file_path : file_paths) {
      QSharedPointer<ImagePropertiesModel> new_model{new ImagePropertiesModel()};
      m_images.push_back(new_model);
    }
    endInsertRows();
  }
}

#include "ImageListModel.hpp"


static bool compare_image_properties_models(
    const QSharedPointer<ImagePropertiesModel> &image_properties_model_1,
    const QSharedPointer<ImagePropertiesModel> &image_properties_model_2) {
  const auto time_1{image_properties_model_1->date_time_original()};
  const auto time_2{image_properties_model_2->date_time_original()};
  if (time_1 < time_2)
    return true;
  else if (time_1 == time_2) {
    const auto file_name_1{image_properties_model_1->file_name()};
    const auto file_name_2{image_properties_model_2->file_name()};
    return file_name_1.localeAwareCompare(file_name_2) < 0;
  }
  return false;
}


ImageListModel::ImageListModel(QObject *other) : QAbstractListModel(other) {
}


int ImageListModel::rowCount([[maybe_unused]] const QModelIndex &parent) const {
  return m_images.count();
}


QVariant ImageListModel::data(const QModelIndex &index, int role) const {
  const int row{index.row()};
  if (0 <= row and row < m_images.count()) {
    auto &image{m_images[row]};
    switch (role) {
      case file_name:
        return image->file_name();
      case file_size:
        return image->file_size();
      case image_width:
        return image->image_width();
      case image_height:
        return image->image_height();
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
      case date_time_original:
        return image->date_time_original();
      case preview:
        return image->preview();
    }
  }
  return {};
}


ImagePropertiesModel *ImageListModel::get(qint32 index) const {
  if (0 <= index and index < m_images.count())
    return m_images[index].data();
  return nullptr;
}


QHash<int, QByteArray> ImageListModel::roleNames() const {
  static const QHash<int, QByteArray> role_names{{file_name, "fileName"},
                                                 {file_size, "fileSize"},
                                                 {image_width, "imageWidth"},
                                                 {image_height, "imageHeight"},
                                                 {camera_maker, "cameraMaker"},
                                                 {camera_model, "cameraModel"},
                                                 {lens_maker, "lensMaker"},
                                                 {lens_model, "lensModel"},
                                                 {focal_length, "focalLength"},
                                                 {aperture, "aperture"},
                                                 {shutter_speed, "shutterSpeed"},
                                                 {iso, "iso"},
                                                 {date_time_original, "dateTimeOriginal"},
                                                 {preview, "preview"}};
  return role_names;
}


void ImageListModel::set_file_names(const QFileInfoList &file_infos) {
  if (not m_images.empty()) {
    beginRemoveRows(QModelIndex(), 0, m_images.count() - 1);
    m_images.clear();
    endRemoveRows();
  }
  if (not file_infos.empty()) {
    beginInsertRows(QModelIndex(), 0, file_infos.count() - 1);
    for (const auto &file_info : file_infos)
      m_images.push_back(
          QSharedPointer<ImagePropertiesModel>{new ImagePropertiesModel(file_info)});
    std::stable_sort(m_images.begin(), m_images.end(), compare_image_properties_models);
    endInsertRows();
  }
}


void ImageListModel::update_file_names(const QFileInfoList &file_infos) {
  if (not m_images.empty()) {
    QList<int> removed_images;
    for (int i{0}; i < m_images.count(); ++i)
      if (not file_infos.contains(m_images[i]->file_info()))
        removed_images.push_back(i);
    int num_removed{0};
    for (int i{0}; i < removed_images.count(); ++i) {
      const int index{removed_images[i] - num_removed};
      beginRemoveRows(QModelIndex(), index, index);
      m_images.removeAt(index);
      endRemoveRows();
      ++num_removed;
    }
  }
  if (not file_infos.empty()) {
    for (const auto &file_info : file_infos) {
      const auto iter{
          std::find_if(m_images.begin(), m_images.end(), [&file_info](const auto &image) {
            return image->file_info().absoluteFilePath() == file_info.absoluteFilePath();
          })};
      if (iter == m_images.end()) {
        QSharedPointer<ImagePropertiesModel> image_properties_model{
            new ImagePropertiesModel(file_info)};
        const auto insertion_pos_iter{std::upper_bound(m_images.begin(), m_images.end(),
                                                       image_properties_model,
                                                       compare_image_properties_models)};
        const auto insertion_pos{static_cast<int>(insertion_pos_iter - m_images.begin())};
        beginInsertRows(QModelIndex(), insertion_pos, insertion_pos);
        m_images.insert(insertion_pos_iter, image_properties_model);
        endInsertRows();
      }
    }
  }
}

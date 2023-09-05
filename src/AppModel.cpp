#include "AppModel.hpp"
#include "Image.hpp"
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>


#ifdef SAILJAIL

namespace {

  static QString settings_path() {
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
           QCoreApplication::applicationName() + ".conf";
  }

}  // namespace
#endif


AppModel::AppModel(QObject* parent)
    : QObject{parent},
      m_image_list_model{new ImageListModel()} {
#ifdef SAILJAIL
  QSettings settings(settings_path(), QSettings::NativeFormat);
#else
  QSettings settings;
#endif
  const QVariant image_folder{settings.value("image_folder")};
  if (image_folder.canConvert<QUrl>())
    set_image_folder(image_folder.toUrl());
  else {
    const QStringList folders{
        QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)};
    if (not folders.isEmpty())
      set_image_folder(QUrl::fromLocalFile(folders[0]));
  }
  const QVariant show_image_info{settings.value("show_image_info")};
  if (show_image_info.canConvert<bool>())
    set_show_image_info(show_image_info.toBool());
  else
    set_show_image_info(false);
}


AppModel::~AppModel() {
#ifdef SAILJAIL
  QSettings settings(settings_path(), QSettings::NativeFormat);
#else
  QSettings settings;
#endif
  settings.setValue("image_folder", get_image_folder());
  settings.setValue("show_image_info", get_show_image_info());
}


QUrl AppModel::get_image_folder() const {
  return m_image_folder;
}


void AppModel::set_image_folder(const QUrl& new_image_folder) {
  if (new_image_folder != m_image_folder) {
    m_image_folder = new_image_folder;
    emit image_folder_changed();
    generate_image_file_names();
  }
}


ImageListModel* AppModel::get_image_list() const {
  return m_image_list_model.data();
}


void AppModel::generate_image_file_names() {
  QDir dir{m_image_folder.toLocalFile()};
  auto file_list{dir.entryInfoList(QDir::Files)};
  QFileInfoList image_files;
  for (const auto& file : file_list) {
    if (Image::supported_file_extensions().contains(file.suffix(), Qt::CaseInsensitive))
      image_files.append(file.absoluteFilePath());
  }
  m_image_list_model->set_file_names(image_files);
  emit image_list_changed();
}


bool AppModel::get_show_image_info() const {
  return m_show_image_info;
}


void AppModel::set_show_image_info(bool new_show_image_info) {
  if (m_show_image_info != new_show_image_info) {
    m_show_image_info = new_show_image_info;
    emit show_image_info_changed();
  }
}

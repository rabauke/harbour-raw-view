#include "AppModel.hpp"
#include "Image.hpp"
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>


#ifdef SAILJAIL
static QString get_settings_path() {
  return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" +
         QCoreApplication::applicationName() + ".conf";
}
#endif


AppModel::AppModel(QObject* parent)
    : QObject{parent},
      m_image_list_model{new ImageListModel()} {
#ifdef SAILJAIL
  QSettings settings(get_settings_path(), QSettings::NativeFormat);
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
}


AppModel::~AppModel() {
#ifdef SAILJAIL
  QSettings settings(get_settings_path(), QSettings::NativeFormat);
#else
  QSettings settings;
#endif
  settings.setValue("image_folder", m_image_folder);
}


QUrl AppModel::image_folder() const {
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

#include "AppModel.hpp"
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
  static const QStringList supported_file_extensions{
      ".3fr", ".ari", ".arw", ".bay", ".braw", ".crw", ".cr2", ".cr3", ".cap", ".data", ".dcs",
      ".dcr", ".dng", ".drf", ".eip", ".erf",  ".fff", ".gpr", ".iiq", ".k25", ".kdc",  ".mdc",
      ".mef", ".mos", ".mrw", ".nef", ".nrw",  ".obm", ".orf", ".pef", ".ptx", ".pxn",  ".r3d",
      ".raf", ".raw", ".rwl", ".rw2", ".rwz",  ".sr2", ".srf", ".srw", ".tif", ".x3f"};

  QDir dir{m_image_folder.toLocalFile()};
  auto file_list{dir.entryList(QDir::Files)};
  QStringList image_file_names;
  for (const auto& file : file_list) {
    for (const auto& extension : supported_file_extensions)
      if (file.endsWith(extension, Qt::CaseInsensitive)) {
        image_file_names.append(m_image_folder.toLocalFile() + "/" + file);
        break;
      }
  }
  m_image_list_model->set_file_names(image_file_names);
  emit image_list_changed();
}

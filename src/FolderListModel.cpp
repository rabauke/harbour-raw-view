#include "FolderListModel.hpp"
#include <QDirIterator>

FolderListModel::FolderListModel(QObject *parent)
    : QAbstractListModel(parent), dir(""), fileInfoList(dir.entryInfoList()) {
}


QVariant FolderListModel::data(const QModelIndex &index, int role) const {
  if (not index.isValid())
    return QVariant();
  if (role == file_name_role)
    return QVariant(fileInfoList[index.row()].fileName());
  if (role == file_path_role)
    return QVariant(fileInfoList[index.row()].filePath());
  if (role == file_is_dir_role)
    return QVariant(fileInfoList[index.row()].isDir());
  return QVariant();
}

QHash<int, QByteArray> FolderListModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[file_name_role] = "fileName";
  roles[file_path_role] = "filePath";
  roles[file_is_dir_role] = "fileIsDir";
  return roles;
}


QString FolderListModel::folder() const {
  return dir.absolutePath();
}


void FolderListModel::set_folder(const QString &name) {
  beginResetModel();
  QFileInfo test(name);
  if (test.isDir())
    dir.cd(name);
  else
    dir.cd(dir.homePath());
  fileInfoList = dir.entryInfoList();
  endResetModel();
}

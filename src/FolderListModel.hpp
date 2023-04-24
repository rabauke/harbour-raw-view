#pragma once

#include <QAbstractListModel>
#include <QDirIterator>
#include <QVector>
#include <QDir>
#include <QFileInfo>


class FolderListModel : public QAbstractListModel {
  Q_OBJECT

public:
  explicit FolderListModel(QObject *parent = nullptr);
  virtual ~FolderListModel() = default;

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  QML_NAMED_ELEMENT(ImageListModel)
#endif

  Q_PROPERTY(QString folder READ folder WRITE set_folder)

  int rowCount(const QModelIndex &) const override { return dir.count(); }
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

private:
  enum FolderListModelRoles {
    file_name_role = Qt::UserRole + 1,
    file_path_role,
    file_is_dir_role
  };

  QString folder() const;
  void set_folder(const QString &name);

  QDir dir;
  QFileInfoList fileInfoList;
};

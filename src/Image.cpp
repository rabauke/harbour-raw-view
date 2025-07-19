#include "Image.hpp"
#include <stdexcept>
#include <random>
#include <libraw/libraw.h>
#include <exiv2/exiv2.hpp>
#include <sys/sysinfo.h>
#include <QFileInfo>
#include <QTimeZone>
#include <QImageReader>
#include <QDir>
#include <QDebug>


static QStringList to_string_list(const QByteArrayList& list) {
  QStringList new_list;
  for (const auto& item : list)
    new_list.push_back(QString::fromLatin1(item));
  return new_list;
}


static int random_int(int bound) {
  static std::mt19937_64 rand_engine;

  if (bound <= 1)
    return 0;
  std::uniform_int_distribution<int> distribution(0, bound - 1);
  return distribution(rand_engine);
}


static qint64 mem_available() {
  struct sysinfo info {};
  if (sysinfo(&info) != 0)
    return 0;
  const auto totalFreeMemory{info.freeram + info.bufferram};
  return static_cast<qint64>(totalFreeMemory * info.mem_unit);
}


QMap<QString, QPixmap> Image::s_preview_cache;
QDir Image::s_temp_dir;


Image::Image(const QFileInfo& file_info) : m_file_info{file_info} {
  load_metadata();
}


const QFileInfo& Image::file_info() const {
  return m_file_info;
}


QString Image::file_name() const {
  return m_file_info.fileName();
}


double Image::file_size() const {
  return m_file_info.size();
}


double Image::image_width() const {
  return m_image_width;
}


double Image::image_height() const {
  return m_image_height;
}


QString Image::camera_maker() const {
  return m_camera_maker;
}


QString Image::camera_model() const {
  return m_camera_model;
}


QString Image::lens_maker() const {
  return m_lens_maker;
}


QString Image::lens_model() const {
  return m_lens;
}


double Image::focal_length() const {
  return m_focal_length;
}


double Image::aperture() const {
  return m_aperture;
}


double Image::shutter_speed() const {
  return m_shutter_speed;
}


double Image::iso() const {
  return m_iso;
}


QDateTime Image::date_time_original() const {
  return m_date_time_original;
}


QPixmap Image::preview() {
  if (s_preview_cache.contains(m_file_info.absoluteFilePath()))
    return s_preview_cache[m_file_info.absoluteFilePath()];

  QPixmap image;
  try {
    if (is_supported_raw_file_type(m_file_info))
      load_raw(image);
    else if (is_supported_nonraw_file_type(m_file_info))
      load_nonraw(image);
    if (m_image_orientation == ImageOrientation::rot_180)
      image = image.transformed(QTransform().rotate(180));
    else if (m_image_orientation == ImageOrientation::rot_90)
      image = image.transformed(QTransform().rotate(90));
    else if (m_image_orientation == ImageOrientation::rot_270)
      image = image.transformed(QTransform().rotate(270));
  } catch (...) {
  }
  if (m_thumb_nail.isNull())
    m_thumb_nail = create_thumb_nail(image);
  int freed_images{0};
  while ((mem_available() < 256ll * 1024 * 1024 and freed_images < 4 and s_preview_cache.size() > 4)
         or s_preview_cache.size() > 64) {
    qDebug() << "freeing memory: " << mem_available();
    auto iter{s_preview_cache.keyBegin()};
    std::advance(iter, random_int(s_preview_cache.size()));
    s_preview_cache.remove(*iter);
    ++freed_images;
  }
  s_preview_cache.insert(m_file_info.absoluteFilePath(), image);
  qDebug() << "free memory: " << mem_available() << " cache size: " << s_preview_cache.size();
  return image;
}


QPixmap Image::thumb_nail() {
  if (m_thumb_nail.isNull())
    m_thumb_nail = create_thumb_nail(preview());
  return m_thumb_nail;
}


QString Image::absolute_file_path() const {
  return m_file_info.absoluteFilePath();
}


QString Image::share(bool share_raw_as_jpeg) const {
  if (is_supported_raw_file_type(m_file_info) and share_raw_as_jpeg) {
    QDir out_dir{s_temp_dir};
    out_dir.mkpath(".");
    QString filename{out_dir.filePath(m_file_info.baseName() + ".jpg")};
    QFile file{filename};
    file.open(QIODevice::WriteOnly);
    try {
      LibRaw lib_raw;
      if (lib_raw.open_file(m_file_info.absoluteFilePath().toUtf8().toStdString().c_str()) !=
          LIBRAW_SUCCESS)
        throw ImageException("unable to open file");
      if (lib_raw.unpack_thumb() != LIBRAW_SUCCESS)
        throw ImageException("unable to extract thumbnail image");
      if (lib_raw.imgdata.thumbnail.tformat == LIBRAW_THUMBNAIL_JPEG) {
        QByteArray data(reinterpret_cast<const char*>(lib_raw.imgdata.thumbnail.thumb),
                        lib_raw.imgdata.thumbnail.tlength);
        file.write(data);
      }
    }
    catch(...) {}
    return filename;
  }
  return absolute_file_path();
}


bool Image::is_supported_file_type(const QFileInfo& file_info) {
  return is_supported_raw_file_type(file_info) or is_supported_nonraw_file_type(file_info);
}


void Image::set_temp_dir(const QDir& temp_dir) {
  s_temp_dir = temp_dir;
}


bool Image::is_supported_raw_file_type(const QFileInfo &file_info) {
  static const QStringList extensions{
      "3fr", "ari", "arw", "bay", "braw", "crw", "cr2", "cr3", "cap", "data", "dcs",
      "dcr", "dng", "drf", "eip", "erf",  "fff", "gpr", "iiq", "k25", "kdc",  "mdc",
      "mef", "mos", "mrw", "nef", "nrw",  "obm", "orf", "pef", "ptx", "pxn",  "r3d",
      "raf", "raw", "rwl", "rw2", "rwz",  "sr2", "srf", "srw", "tif", "x3f"};
  return extensions.contains(file_info.suffix(), Qt::CaseInsensitive);
}


bool Image::is_supported_nonraw_file_type(const QFileInfo &file_info) {
  static const QStringList extensions{to_string_list(QImageReader::supportedImageFormats())};
  return extensions.contains(file_info.suffix(), Qt::CaseInsensitive);
}


QPixmap Image::create_thumb_nail(const QPixmap& image) {
  return image.scaled(128, 128, Qt::KeepAspectRatioByExpanding);
}


void Image::load_raw(QPixmap& image) {
  LibRaw lib_raw;
  if (lib_raw.open_file(m_file_info.absoluteFilePath().toUtf8().toStdString().c_str()) !=
      LIBRAW_SUCCESS)
    throw ImageException("unable to open file");
  if (lib_raw.unpack_thumb() != LIBRAW_SUCCESS)
    throw ImageException("unable to extract thumbnail image");
  if (lib_raw.imgdata.thumbnail.tformat == LIBRAW_THUMBNAIL_JPEG) {
    image.loadFromData(reinterpret_cast<const uchar*>(lib_raw.imgdata.thumbnail.thumb),
                       lib_raw.imgdata.thumbnail.tlength);
  }
}


void Image::load_nonraw(QPixmap& image) {
  if (not image.load(m_file_info.absoluteFilePath()))
    throw ImageException("unable to load image");
}


void Image::load_metadata() {
  if (not m_metadata_loaded) {
    m_metadata_loaded = true;
    try {
      if (is_supported_raw_file_type(m_file_info))
        load_metadata_raw();
      else if (is_supported_nonraw_file_type(m_file_info))
        load_metadata_nonraw();
    } catch (...) {
    }
  }
}


void Image::load_metadata_raw() {
  LibRaw lib_raw;
  if (lib_raw.open_file(m_file_info.absoluteFilePath().toUtf8().toStdString().c_str()) !=
      LIBRAW_SUCCESS)
    throw ImageException("unable to open file");
  m_image_width = lib_raw.imgdata.sizes.width;
  m_image_height = lib_raw.imgdata.sizes.height;
  m_camera_maker = lib_raw.imgdata.idata.make;
  m_camera_model = lib_raw.imgdata.idata.model;
  m_lens_maker = lib_raw.imgdata.lens.LensMake;
  m_lens = lib_raw.imgdata.lens.Lens;
  m_focal_length = lib_raw.imgdata.other.focal_len;
  m_aperture = lib_raw.imgdata.other.aperture;
  m_shutter_speed = lib_raw.imgdata.other.shutter;
  m_iso = lib_raw.imgdata.other.iso_speed;
  m_date_time_original = QDateTime::fromMSecsSinceEpoch(
      static_cast<qint64>(lib_raw.imgdata.other.timestamp) * 1000);
  switch (lib_raw.imgdata.sizes.flip) {
    case 3:
      m_image_orientation = ImageOrientation::rot_180;
      break;
    case 5:
      m_image_orientation = ImageOrientation::rot_270;
      break;
    case 6:
      m_image_orientation = ImageOrientation::rot_90;
      break;
    default:
      m_image_orientation = ImageOrientation::original;
      break;
  }
}


void Image::load_metadata_nonraw() {
  auto image{Exiv2::ImageFactory::open(m_file_info.absoluteFilePath().toLocal8Bit().data())};
  image->readMetadata();
  auto& exif_data{image->exifData()};
  m_image_width = image->pixelWidth();
  m_image_height = image->pixelHeight();
  m_camera_maker = QString::fromStdString(exif_data["Exif.Image.Make"].toString());
  m_camera_model = QString::fromStdString(exif_data["Exif.Image.Model"].toString());
  m_lens_maker = QString::fromStdString(exif_data["Exif.Photo.LensMake"].toString());
  m_lens = QString::fromStdString(exif_data["Exif.Photo.LensModel"].toString());
  m_focal_length = exif_data["Exif.Photo.FocalLength"].toFloat();
  m_aperture = exif_data["Exif.Photo.FNumber"].toFloat();
  m_shutter_speed = exif_data["Exif.Photo.ExposureTime"].toFloat();
  m_iso = exif_data["Exif.Photo.ISOSpeedRatings"].toFloat();
  m_date_time_original = QDateTime::fromString(
      QString::fromStdString(exif_data["Exif.Photo.DateTimeOriginal"].toString()),
      "yyyy:MM:dd hh:mm:ss");
  m_image_orientation =
      static_cast<ImageOrientation>(exif_data["Exif.Image.Orientation"].toInt64());
}

#include "Image.hpp"
#include <stdexcept>
#include <libraw/libraw.h>
#include <exiv2/exiv2.hpp>
#include <QFileInfo>
#include <QTimeZone>
#include <QImageReader>


static QStringList to_string_list(const QByteArrayList& list) {
  QStringList new_list;
  for (const auto& item : list)
    new_list.push_back(QString::fromLatin1(item));
  return new_list;
}


Image::Image(const QFileInfo& file_info) : m_file_info{file_info} {
}


QString Image::file_name() {
  return m_file_info.fileName();
}


double Image::file_size() {
  return m_file_info.size();
}


double Image::image_width() {
  return m_image_width;
}


double Image::image_height() {
  return m_image_height;
}


QString Image::camera_maker() {
  load_metadata();
  return m_camera_maker;
}


QString Image::camera_model() {
  load_metadata();
  return m_camera_model;
}


QString Image::lens_maker() {
  load_metadata();
  return m_lens_maker;
}


QString Image::lens_model() {
  load_metadata();
  return m_lens;
}


double Image::focal_length() {
  load_metadata();
  return m_focal_length;
}


double Image::aperture() {
  load_metadata();
  return m_aperture;
}


double Image::shutter_speed() {
  load_metadata();
  return m_shutter_speed;
}


double Image::iso() {
  load_metadata();
  return m_iso;
}


QDateTime Image::date_time_original() {
  return m_date_time_original;
}


QImage Image::preview() {
  load_metadata();
  QImage image;
  try {
    if (supported_raw_file_extensions().contains(m_file_info.suffix(), Qt::CaseInsensitive))
      load_raw(image);
    else if (supported_nonraw_file_extensions().contains(m_file_info.suffix(),
                                                         Qt::CaseInsensitive))
      load_nonraw(image);
    if (m_image_orientation == ImageOrientation::rot_180)
      image = image.transformed(QTransform().rotate(180));
    else if (m_image_orientation == ImageOrientation::rot_90)
      image = image.transformed(QTransform().rotate(90));
    else if (m_image_orientation == ImageOrientation::rot_270)
      image = image.transformed(QTransform().rotate(270));
  } catch (...) {
  }
  return image;
}


const QStringList& Image::supported_raw_file_extensions() {
  static const QStringList extensions{
      "3fr", "ari", "arw", "bay", "braw", "crw", "cr2", "cr3", "cap", "data", "dcs",
      "dcr", "dng", "drf", "eip", "erf",  "fff", "gpr", "iiq", "k25", "kdc",  "mdc",
      "mef", "mos", "mrw", "nef", "nrw",  "obm", "orf", "pef", "ptx", "pxn",  "r3d",
      "raf", "raw", "rwl", "rw2", "rwz",  "sr2", "srf", "srw", "tif", "x3f"};
  return extensions;
}


const QStringList& Image::supported_nonraw_file_extensions() {
  static const QStringList extensions{to_string_list(QImageReader::supportedImageFormats())};
  return extensions;
}


const QStringList& Image::supported_file_extensions() {
  static const QStringList extensions{supported_raw_file_extensions() +
                                      supported_nonraw_file_extensions()};
  return extensions;
}


void Image::load_raw(QImage& image) {
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


void Image::load_nonraw(QImage& image) {
  if (not image.load(m_file_info.absoluteFilePath()))
    throw ImageException("unable to load image");
}


void Image::load_metadata() {
  if (not m_metadata_loaded) {
    m_metadata_loaded = true;
    try {
      if (supported_raw_file_extensions().contains(m_file_info.suffix(), Qt::CaseInsensitive))
        load_metadata_raw();
      else if (supported_nonraw_file_extensions().contains(m_file_info.suffix(),
                                                           Qt::CaseInsensitive))
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

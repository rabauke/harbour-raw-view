#include "Image.hpp"
#include <stdexcept>
#include <libraw/libraw.h>
#include <exiv2/exiv2.hpp>
#include <QFileInfo>
#include <QImageReader>


static QStringList to_string_list(const QByteArrayList& list) {
  QStringList new_list;
  for (const auto& item : list)
    new_list.push_back(QString::fromLatin1(item));
  return new_list;
}


void Image::reset() {
  *this = Image();
}


void Image::load(const QFileInfo& file_info) {
  reset();
  if (supported_raw_file_extensions().contains(file_info.suffix(), Qt::CaseInsensitive))
    load_raw(file_info);
  else if (supported_nonraw_file_extensions().contains(file_info.suffix(), Qt::CaseInsensitive))
    load_nonraw(file_info);
}


bool Image::is_valid() const {
  return m_is_valid;
}


QString Image::file_name() const {
  return m_file_name;
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


float Image::focal_length() const {
  return m_focal_length;
}


float Image::aperture() const {
  return m_aperture;
}


float Image::shutter_speed() const {
  return m_shutter_speed;
}


float Image::iso() const {
  return m_iso;
}


const QImage& Image::preview() const {
  return m_preview;
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


void Image::load_raw(const QFileInfo& file_info) {
  LibRaw lib_raw;
  if (lib_raw.open_file(file_info.absoluteFilePath().toUtf8().toStdString().c_str()) !=
      LIBRAW_SUCCESS)
    throw ImageException("unable to open file");
  m_file_name = file_info.fileName();
  m_camera_maker = lib_raw.imgdata.idata.make;
  m_camera_model = lib_raw.imgdata.idata.model;
  m_lens_maker = lib_raw.imgdata.lens.LensMake;
  m_lens = lib_raw.imgdata.lens.Lens;
  m_focal_length = lib_raw.imgdata.other.focal_len;
  m_aperture = lib_raw.imgdata.other.aperture;
  m_shutter_speed = lib_raw.imgdata.other.shutter;
  m_iso = lib_raw.imgdata.other.iso_speed;
  if (lib_raw.unpack_thumb() != LIBRAW_SUCCESS)
    throw ImageException("unable to extract thumbnail image");
  if (lib_raw.imgdata.thumbnail.tformat == LIBRAW_THUMBNAIL_JPEG) {
    m_preview.loadFromData(reinterpret_cast<const uchar*>(lib_raw.imgdata.thumbnail.thumb),
                           lib_raw.imgdata.thumbnail.tlength);
    if (lib_raw.imgdata.sizes.flip == 3)
      m_preview = m_preview.transformed(QTransform().rotate(180));
    else if (lib_raw.imgdata.sizes.flip == 5)
      m_preview = m_preview.transformed(QTransform().rotate(270));
    else if (lib_raw.imgdata.sizes.flip == 6)
      m_preview = m_preview.transformed(QTransform().rotate(90));
  }
  m_is_valid = true;
}


void Image::load_nonraw(const QFileInfo& file_info) {
  m_file_name = file_info.fileName();
  if (not m_preview.load(file_info.absoluteFilePath()))
    throw ImageException("unable to load image");
  auto image{Exiv2::ImageFactory::open(file_info.absoluteFilePath().toLocal8Bit().data())};
  image->readMetadata();
  auto& exif_data{image->exifData()};
  m_camera_maker = QString::fromStdString(exif_data["Exif.Image.Make"].toString());
  m_camera_model = QString::fromStdString(exif_data["Exif.Image.Model"].toString());
  m_lens_maker = QString::fromStdString(exif_data["Exif.Photo.LensMake"].toString());
  m_lens = QString::fromStdString(exif_data["Exif.Photo.LensModel"].toString());
  m_focal_length = exif_data["Exif.Photo.FocalLength"].toFloat();
  m_aperture = exif_data["Exif.Photo.FNumber"].toFloat();
  m_shutter_speed = exif_data["Exif.Photo.ExposureTime"].toFloat();
  m_iso = exif_data["Exif.Photo.ISOSpeedRatings"].toFloat();
  m_is_valid = true;
}

#include "ImageItem.hpp"


ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem{parent} {
  m_current_image = QImage(1, 1, QImage::Format::Format_RGB888);
}


QImage ImageItem::get_image() const {
  return m_current_image;
}


void ImageItem::set_image(const QImage &image) {
  m_current_image = image;
  update();
}


double ImageItem::get_scale() const {
  return m_scale;
}


void ImageItem::set_scale(double scale) {
  m_scale = scale;
  update();
}


QPointF ImageItem::get_scale_center() const {
  return m_scale_center;
}


void ImageItem::set_scale_center(QPointF scale_center) {
  m_scale_center = scale_center;
}


void ImageItem::paint(QPainter *painter) {
  const QPointF screen_center{boundingRect().center()};
  const QSizeF image_size{m_current_image.size()};
  const QSizeF scaled_image_size{m_scale * image_size.scaled(boundingRect().size(), Qt::KeepAspectRatio)};
  QRectF drawing_area{QPointF{0, 0}, scaled_image_size};
  drawing_area.moveCenter(screen_center);
  drawing_area.translate(-(m_scale - 1) * (m_scale_center - screen_center));
  painter->drawImage(drawing_area, m_current_image);
}

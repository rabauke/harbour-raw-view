#include "ImageItem.hpp"
#include <cmath>


ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem(parent) {
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
  QRectF bounding_rect{boundingRect()};
  QRectF current_image_rect{m_current_image.rect()};
  const QSizeF size_scaled_to_width{
      bounding_rect.width(),
      bounding_rect.width() / current_image_rect.width() * current_image_rect.height()};
  const QSizeF size_scaled_to_height{
      bounding_rect.height() / current_image_rect.height() * current_image_rect.width(),
      bounding_rect.height()};

  QSizeF size_scaled{0, 0};
  QPointF upper_left_scale_1{0, 0};
  if (size_scaled_to_width.height() > bounding_rect.height()) {
    size_scaled.setWidth(m_scale * size_scaled_to_height.width());
    size_scaled.setHeight(m_scale * size_scaled_to_height.height());
    upper_left_scale_1.setX((bounding_rect.width() - size_scaled_to_height.width()) / 2);
    upper_left_scale_1.setY((bounding_rect.height() - size_scaled_to_height.height()) / 2);
  } else {
    size_scaled.setWidth(m_scale * size_scaled_to_width.width());
    size_scaled.setHeight(m_scale * size_scaled_to_width.height());
    upper_left_scale_1.setX((bounding_rect.width() - size_scaled_to_width.width()) / 2);
    upper_left_scale_1.setY((bounding_rect.height() - size_scaled_to_width.height()) / 2);
  }
  QSize size_scaled_int(std::round(size_scaled.width()), std::round(size_scaled.height()));
  QImage scaled{
      m_current_image.scaled(size_scaled_int, Qt::IgnoreAspectRatio, Qt::FastTransformation)};
  const QPointF scale_center{m_scale_center - upper_left_scale_1};
  const QPointF scaled_scale_center{scale_center * m_scale};
  const QPointF upper_left{upper_left_scale_1 - (scaled_scale_center - scale_center)};
  painter->drawImage(upper_left, scaled);
}

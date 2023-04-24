#include "ImageItem.hpp"


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


void ImageItem::paint(QPainter *painter) {
  QRectF bounding_rect{boundingRect()};
  QSize scaled_size(bounding_rect.width(), bounding_rect.height());
  QImage scaled{
      m_current_image.scaled(scaled_size, Qt::KeepAspectRatio, Qt::SmoothTransformation)};
  QPointF upper_left{bounding_rect.center() - scaled.rect().center()};
  if (upper_left.x() < 0)
    upper_left.setX(0);
  if (upper_left.y() < 0)
    upper_left.setY(0);
  painter->drawImage(upper_left, scaled);
}

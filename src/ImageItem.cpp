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
  emit imageChanged();
}


double ImageItem::get_scale() const {
  return m_scale;
}


void ImageItem::set_scale(double scale) {
  if (m_scale != scale) {
    m_scale = scale;
    update();
    emit scaleChanged();
  }
}


QPointF ImageItem::get_scale_center() const {
  return m_scale_center;
}


void ImageItem::set_scale_center(QPointF scale_center) {
  if (m_scale_center != scale_center) {
    m_scale_center = scale_center;
    emit scaleCenterChanged();
  }
}


ImageItem::Alignment ImageItem::get_alignment() const {
  return m_alignmnet;
}


void ImageItem::set_alignment(Alignment alignment) {
  if (m_alignmnet != alignment) {
    m_alignmnet = alignment;
    emit alignmentChanged();
  }
}


ImageItem::FillMode ImageItem::get_fill_mode() const {
  return m_fill_mode;
}


void ImageItem::set_fill_mode(FillMode fill_mode) {
  if (m_fill_mode != fill_mode) {
    m_fill_mode = fill_mode;
    emit fillModeChanged();
  }
}


void ImageItem::paint(QPainter *painter) {
  const QRectF bounding_box{boundingRect()};
  const QPointF screen_center{bounding_box.center()};
  const QSizeF image_size{m_current_image.size()};
  const QSizeF scaled_image_size{m_scale *
                                 image_size.scaled(bounding_box.size(),
                                                   m_fill_mode == FillMode::PreserveAspectFit ? Qt::KeepAspectRatio : Qt::KeepAspectRatioByExpanding)};
  QRectF drawing_area{QPointF{0, 0}, scaled_image_size};
  drawing_area.moveCenter(screen_center);
  switch (m_alignmnet) {
    case Alignment::AlignLeft:
      drawing_area.moveLeft(0);
      break;
    case Alignment::AlignRight:
      drawing_area.moveRight(bounding_box.width());
      break;
    case Alignment::AlignTop:
      drawing_area.moveTop(0);
      break;
    case Alignment::AlignBottom:
      drawing_area.moveBottom(bounding_box.height());
      break;
    case Alignment::AlignCenter:
      drawing_area.translate(-(m_scale - 1) * (m_scale_center - screen_center));
  }
  painter->drawImage(drawing_area, m_current_image);
}

#pragma once

#include <QtGlobal>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  #include <QtQmlIntegration>
#endif


class ImageItem : public QQuickPaintedItem {
  Q_OBJECT

public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
  QML_NAMED_ELEMENT(ImageItem)
#endif

  ImageItem(QQuickItem *parent = nullptr);

  Q_PROPERTY(QImage image READ get_image WRITE set_image NOTIFY imageChanged)
  Q_PROPERTY(double scale READ get_scale WRITE set_scale NOTIFY scaleChanged)
  Q_PROPERTY(QPointF scaleCenter READ get_scale_center WRITE set_scale_center NOTIFY
                 scaleCenterChanged)

  void paint(QPainter *painter) override;

signals:
  void imageChanged();
  void scaleChanged();
  void scaleCenterChanged();

private:
  QImage get_image() const;
  void set_image(const QImage &image);
  double get_scale() const;
  void set_scale(double scale);
  QPointF get_scale_center() const;
  void set_scale_center(QPointF scale_center);

  QImage m_current_image;
  double m_scale{1};
  QPointF m_scale_center{};
};

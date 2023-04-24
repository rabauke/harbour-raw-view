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

  void paint(QPainter *painter) override;

signals:
  void imageChanged();

private:
  QImage get_image() const;
  void set_image(const QImage &image);

  QImage m_current_image;
};

#pragma once

#include <QtGlobal>
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QPixmap>
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

  enum class Alignment : int { AlignCenter, AlignLeft, AlignRight, AlignTop, AlignBottom };
  Q_ENUM(Alignment)

  enum class FillMode : int { PreserveAspectFit, PreserveAspectCrop};
  Q_ENUM(FillMode)

  Q_PROPERTY(QPixmap image READ get_image WRITE set_image NOTIFY imageChanged)
  Q_PROPERTY(double scale READ get_scale WRITE set_scale NOTIFY scaleChanged)
  Q_PROPERTY(QPointF scaleCenter READ get_scale_center WRITE set_scale_center NOTIFY
                 scaleCenterChanged)
  Q_PROPERTY(Alignment alignment READ get_alignment WRITE set_alignment NOTIFY alignmentChanged)
  Q_PROPERTY(FillMode fillMode READ get_fill_mode WRITE set_fill_mode NOTIFY fillModeChanged)

  void paint(QPainter *painter) override;

signals:
  void imageChanged();
  void scaleChanged();
  void scaleCenterChanged();
  void alignmentChanged();
  void fillModeChanged();

private:
  QPixmap get_image() const;
  void set_image(const QPixmap &image);
  double get_scale() const;
  void set_scale(double scale);
  QPointF get_scale_center() const;
  void set_scale_center(QPointF scale_center);
  Alignment get_alignment() const;
  void set_alignment(Alignment alignment);
  FillMode get_fill_mode() const;
  void set_fill_mode(FillMode fill_mode);

  QPixmap m_current_image;
  double m_scale{1};
  QPointF m_scale_center{};
  Alignment m_alignmnet{Alignment::AlignCenter};
  FillMode m_fill_mode{FillMode::PreserveAspectFit};
};

#pragma once

#include "Useme.h"
#include <QWidget>
#include <QImage>
#include "ui_ImageView.h"

class ImageView : public QWidget
{
	Q_OBJECT

public:
	explicit ImageView(QImage qimage , QWidget *parent = Q_NULLPTR);
	~ImageView();

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	QImage m_Image;
	qreal m_ZoomValue = 1.0;
	int m_XPtInterval = 0;
	int m_YPtInterval = 0;
	QPoint m_OldPos;
	bool m_Pressed = false;

private slots:
	void onLoadImage(void);
	void onZoomInImage(void);
	void onZoomOutImage(void);
	void onPresetImage(void);

private:
	Ui::ImageView ui;
};

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QImage>
#include <QVector>

#include "types.h"

#include "YUVImage.h"

namespace Ui {
    class MainWindow;
}

class QGraphicsScene;
class QImage;

// MainWindow -------------------------------------------------

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

	YUVImage convertToYOV( const QImage & image);

private:
    Ui::MainWindow *ui;
    QGraphicsScene * sourceScene;
    QGraphicsScene * resultScene;
    QImage sourceImage;
    QImage resultImage;
	YUVImage yuvImage;


private slots:
    void on_pbLoad_clicked();
	void on_pbToYUV_clicked();
	void on_pbToRGB_clicked();

	void on_pbFitSource_clicked();
	void on_pbFitResult_clicked();
	void on_pbStretchSource_clicked();
	void on_pbStretchResult_clicked();


	void on_pbSourceZoomIn_clicked();
	void on_pbSourceZoomOut_clicked();
	void on_pbResultZoomIn_clicked();
	void on_pbResultZoomOut_clicked();

};

#endif // MAINWINDOW_H

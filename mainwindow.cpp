#include <QFileDialog>
#include <QGraphicsScene>
#include <QImage>
#include <QtDebug>
#include <QMessageBox>
#include <QGridLayout>
#include <QPixmap>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Console.h"

#include "ConvertYuvToRgb.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sourceScene = new QGraphicsScene( ui->gvSourcePicture);
    resultScene = new QGraphicsScene( ui->gvResultPicture);

	ui->gvSourcePicture->setScene( sourceScene);
	ui->gvResultPicture->setScene( resultScene);
	
	QGridLayout * lay = new QGridLayout( ui->consoleContainer );
	lay->addWidget( new MessageOutput::ConsoleWidget( MessageOutput::globalConsole(), ui->consoleContainer) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbLoad_clicked()
{
    QFileDialog d( this, "Choose picture.");
    d.setFileMode( QFileDialog::ExistingFile );
	//d.setFilter( QDir::Files );
    if ( d.exec() == QDialog::Accepted )
    {
        QStringList files = d.selectedFiles();
        if ( files.length() != 1 )
        {
            qCritical() << "MainWindow::on_pushButton_clicked(): files.length != 1.";
            return;
        }

        if ( ! sourceImage.load( files[0]) )
        {
            QMessageBox mb( QMessageBox::NoIcon, "Ошибка", "Не удалось открыть изображение.", QMessageBox::Ok);
			qWarning() << "MainWindow::on_pushButton_clicked(): sourceImage.load failed.";
            mb.exec();
        }

		qDebug() << "Source image loaded from: " << files[0];

		sourceScene->clear();
		sourceScene->addPixmap( QPixmap::fromImage( sourceImage) );
		//ui->gvSourcePicture->fitInView( sourceScene->sceneRect(), Qt::KeepAspectRatioByExpanding );
    }

}

void MainWindow::on_pbToYUV_clicked()
{
	yuvImage = YUVImage( sourceImage);
	qDebug() << "Loaded image converted to YUV format.";
}

void MainWindow::on_pbToRGB_clicked()
{
	int width = yuvImage.width();
	int height = yuvImage.height();
	QVector<BYTE> rgbArray( width*height*3*sizeof(BYTE) );
	CConvertYuvToRgb converter;

	converter.Convert(
		yuvImage.width(), yuvImage.height(),
		0, 0,
		yuvImage.y(), yuvImage.u(), yuvImage.v(),
		rgbArray.data()
	);

	qDebug() << "YUV image converted to RGB array.";

	resultImage = QImage( width, height, QImage::Format_RGB32);
	
	BYTE r, g, b;
	int i;
	int j;
	int index = 0;
	for ( i = 0; i < height; ++i)
	{
		for ( j = 0; j < width; ++j)
		{
			r =	rgbArray[index++];
			g =	rgbArray[index++];
			b =	rgbArray[index++];

			resultImage.setPixel( j, i, qRgb(r,g,b) );
		}
	}

	resultScene->clear();
	resultScene->addPixmap( QPixmap::fromImage( resultImage) );
	
	qDebug() << "RGB array converted to image and displayed.";
}

void MainWindow::on_pbFitSource_clicked()
{
	ui->gvSourcePicture->fitInView( sourceScene->sceneRect(), Qt::KeepAspectRatio );
}

void MainWindow::on_pbFitResult_clicked()
{
	ui->gvResultPicture->fitInView( resultScene->sceneRect(), Qt::KeepAspectRatio );
}

void MainWindow::on_pbStretchSource_clicked()
{
	ui->gvSourcePicture->fitInView( sourceScene->sceneRect() );
}
void MainWindow::on_pbStretchResult_clicked()
{
	ui->gvResultPicture->fitInView( resultScene->sceneRect() );
}

void MainWindow::on_pbSourceZoomIn_clicked()
{
	ui->gvSourcePicture->scale(2,2);
}

void MainWindow::on_pbSourceZoomOut_clicked()
{
	ui->gvSourcePicture->scale(0.5,0.5);
}

void MainWindow::on_pbResultZoomIn_clicked()
{
	ui->gvResultPicture->scale(2,2);
}

void MainWindow::on_pbResultZoomOut_clicked()
{
	ui->gvResultPicture->scale(0.5,0.5);
}

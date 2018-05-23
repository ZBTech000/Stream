#include "stdafx.h"
#include "Stream.h"
#include "zList.h"

Stream::Stream(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QGridLayout *layout = static_cast<QGridLayout *>(ui.centralWidget->layout());

	panel0 = new zList(parent);
	panel1 = new zList(parent);

	QString start0 = panel0->start;

	path0 = new QLineEdit(parent);
	path1 = new QLineEdit(parent);

	path0->setText(start0);
	path1->setText(start0);

	layout->addWidget(path0, 0, 0);
	layout->addWidget(path1, 0, 1);
	layout->addWidget(panel0,1,0);
	layout->addWidget(panel1,1,1);

	connect(panel0, SIGNAL(NewPath(QString)), this, SLOT(NewPath_L(QString)));
	connect(panel1, SIGNAL(NewPath(QString)), this, SLOT(NewPath_R(QString)));

	connect(path0, SIGNAL(returnPressed()), this, SLOT(Path_L_Enter()));
	connect(path1, SIGNAL(returnPressed()), this, SLOT(Path_R_Enter()));

}

void Stream::NewPath_L(QString path)
{
	path0->setText(path);
}

void Stream::NewPath_R(QString path)
{
	path1->setText(path);
}

void Stream::Path_L_Enter()
{
	panel0->GoToPath(path0->text());
}

void Stream::Path_R_Enter()
{
	panel1->GoToPath(path1->text());
}

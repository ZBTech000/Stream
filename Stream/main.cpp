#include "stdafx.h"
#include "Stream.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QApplication::setOrganizationName("ZBTech");
	QApplication::setApplicationName("Stream");
	QApplication::setApplicationVersion("2018");

	Stream w;
	w.show();
	return a.exec();
}

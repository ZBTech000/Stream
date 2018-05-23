#include "stdafx.h"
#include "Stream.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Stream w;
	w.show();
	return a.exec();
}

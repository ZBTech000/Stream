#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Stream.h"

class zList;

class Stream : public QMainWindow
{
	Q_OBJECT
private:
		zList *panel0 = nullptr, *panel1 = nullptr;
		QLineEdit *path0 = nullptr;
		QLineEdit *path1 = nullptr;
public:
	Stream(QWidget *parent = Q_NULLPTR);

private:
	Ui::StreamClass ui;
public:
	public slots :
		void NewPath_L(QString path);
	void NewPath_R(QString path);

	void Path_L_Enter();
	void Path_R_Enter();
};

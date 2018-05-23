#ifndef DIALOGRENAME_H
#define DIALOGRENAME_H

#include <QDialog>
#include "ui_DialogRename.h"

class DialogRename : public QDialog
{
	Q_OBJECT
public:
	QString name1;
private:
	QString base_name = "Untitled";
public:
	DialogRename(QWidget *parent = 0);
	~DialogRename();

private:
	Ui::DialogRename ui;
public:
	void SetCaption(QString caption);
	void SetStartName(QString t);
	public slots :
		void cmdOk();
	void cmdExit();
};

#endif // DIALOGRENAME_H
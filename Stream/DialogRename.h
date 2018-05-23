#ifndef DIALOGRENAME_H
#define DIALOGRENAME_H

#include <QDialog>
#include "ui_DialogRename.h"

class DialogRename : public QDialog
{
	Q_OBJECT
public:
	QString name1;
	QString ext;
	bool extmore = false;
	int extstage = 0;

	bool detect_contents = false;
private:
	QStringList list_origin;
	QString base_name = "Untitled";
	int minw = 0;
public:
	DialogRename(QWidget *parent = 0);
	~DialogRename();

private:
	Ui::DialogRename ui;
public:
	void SetCaption(QString caption);
	void SetStartName(QString t);
	void ShowExt(bool t);
	void SetExtList(QStringList t);
	public slots :
		void cmdOk();
		void cmdExtMore();
	void cmdExit();
};

#endif // DIALOGRENAME_H

#ifndef ZLIST_H
#define ZLIST_H

#include <QListWidget>
#include "ui_zList.h"

class DialogRename;

class zList : public QListWidget
{
	Q_OBJECT
private:
	QString cur_item;
	QStringList history;
	int history_id = 0;

	int simkey = 0;

	bool waitkey = false;

	bool rangeselected = false;

	int doubleclickstage = -1;
	int mx = 0, my = 0;
	QTime clicktime;
	QTime clicktime2;
public:
	QString start;
public:
	zList(QWidget *parent = 0);
	~zList();

	void ShowFolderContents(QString DirName);
	void Up();
	void Enter();
	void Prev();
	void Next();
	void SelectItemStarts(QString text);
	void Rename();
	void DeleteFolder(QString path);
	void NewFolder();
	void keyPressEvent(QKeyEvent *event);

	void Snapshot();
	void Back();
	void Forward();

	void GoToPath(QString path);
	void SelectSimilar();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void itemDoubleClicked(QListWidgetItem *item);
private:
	Ui::zList ui;
	QString path0;
	DialogRename* rename_dlg;
	QString active_letter;
signals:
	void NewPath(QString t);
};

#endif // ZLIST_H

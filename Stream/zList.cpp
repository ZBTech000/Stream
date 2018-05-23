#include "stdafx.h"
#include "zList.h"
#include <DialogRename.h>

zList::zList(QWidget *parent)
	: QListWidget(parent)
{
	ui.setupUi(this);
	start = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	if (start.isEmpty())
	{
		start = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
		if (start.isEmpty())
		{
			start = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
			if (start.isEmpty())
			{
				start = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
			}
		}
	}
	ShowFolderContents(start);
	path0 = start;

	rename_dlg = new DialogRename(this);
	rename_dlg->hide();

	history << start;
	history_id++;

	this->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
}

zList::~zList()
{

}

void zList::ShowFolderContents(QString DirName)
{
	this->clear();
	QDir Dir = QDir(DirName);

	while (!Dir.exists())
	{
		if (history_id == 1)
		{
			DirName = start;
			break;
		}
		Back();
	}

	QStringList EList = Dir.entryList(QDir::Filter::AllEntries | QDir::Filter::NoDotAndDotDot);
	for (auto &it : EList)
	{
		QString x = it;
		this->addItem(x);
	}

	if (this->count() == 0)
	{
		active_letter = "";
	}
	else
	{
		active_letter = this->item(0)->text().left(1);
		setCurrentRow(0);
	}
	emit NewPath(DirName);

}

void zList::Up()
{
	QStringList groups = path0.split("/");
	groups.removeLast();
	if (groups.isEmpty()) return;

	path0 = groups.join("/");
	ShowFolderContents(path0);
}

void zList::Enter()
{
	QStringList groups = path0.split("/");
	QString item = this->currentItem()->text();
	groups.append(item);
	path0 = groups.join("/");
	ShowFolderContents(path0);
}

void zList::Prev()
{
	int a = this->currentRow();
	if (a == 0)
	{
		a = this->count() - 1;
	}
	else
	{
		a = (a - 1) % this->count();
	}
	setCurrentRow(a);
}

void zList::Next()
{
	int a = this->currentRow();
	if (a == this->count() - 1)
	{
		a = 0;
	}
	else
	{
		a = (a + 1) % this->count();
	}
	setCurrentRow(a);
}

void zList::SelectItemStarts(QString text)
{
	text = text.left(1).toLower();

	if (text.compare(active_letter) == 0)
	{
		int x = currentRow() + 1;
		if (x > this->count() - 1)
		{
			x = 0;
		}
		QListWidgetItem *test = item(x);
		if (test->text().left(1).toLower().compare(active_letter) != 0)
		{
			for (int t = 0; t < this->count(); t++)
			{
				QListWidgetItem *b = this->item(t);

				QString text2 = b->text().toLower();

				if (text2.startsWith(text))
				{
					x = t;
					break;
				}
			}
		}
		setCurrentRow(x);
	}
	else
	{
		for (int t = 0; t < this->count(); t++)
		{
			QListWidgetItem *b = this->item(t);

			QString text2 = b->text().toLower();

			if (text2.startsWith(text))
			{
				setCurrentRow(t);
				active_letter = text;
				return;
			}
		}
	}
}

void zList::Rename()
{
	rename_dlg = new DialogRename(this);
	rename_dlg->hide();

	if (this->selectedItems().count() == 1)
	{
		rename_dlg->ShowExt(false);
		rename_dlg->SetCaption("Rename");
		rename_dlg->SetStartName("Untitled");
		int x = rename_dlg->exec();
		if (x != QDialog::Rejected)
		{
			QString name = rename_dlg->name1;
			QDir dir = QDir(path0);
			dir.setCurrent(path0);
			QString oldname = this->currentItem()->text();
			QString newname = name;

			dir.rename(oldname, newname);
			ShowFolderContents(path0);
		}
	}
	else
	{
		rename_dlg->ShowExt(true);
		QStringList exts;
		exts << "Existing";

		QList<QListWidgetItem *> listt = this->selectedItems();
		
		for (auto &it : listt)
		{
			QListWidgetItem *b = it;
			QString txt2 = b->text();
			QStringList g = txt2.split(".");

			if (!exts.contains(g.last().toLower()))
			{
				exts << g.last().toLower();
			}
		}

		exts << "Custom";

		rename_dlg->SetExtList(exts);

		int selcount = listt.count();

		int tv = selcount;
		int itercount = 0;
		while (tv > 2)
		{
			tv = tv / 10;
			itercount++;
		}

		itercount++;

		rename_dlg->SetCaption("Rename All");
		QString ix;
		for (int b2 = 0; b2 < itercount; b2++)
		{
			ix.append("0");
		}
		rename_dlg->SetStartName(QString("BaseName_") + ix);
		int x = rename_dlg->exec();
		if (x != QDialog::Rejected)
		{
			QString name = rename_dlg->name1;
			QString ext1 = rename_dlg->ext;
			
			bool ex0 = (ext1.compare("existing", Qt::CaseInsensitive)	== 0);
			bool ex1 = (ext1.compare("custom", Qt::CaseInsensitive)		== 0);
			
			if (rename_dlg->extmore)
			{
				QStringList bext = ext1.split(".");
				ext1 = bext.last();
				ext1 = ext1.replace(")", "");
				rename_dlg->extmore = false;
			}
			if (!ex1)
			{
				QStringList g2 = name.split(".");
				name = g2.first();
			}

			for (int tx = 0; tx < this->count(); tx++)
			{
				QListWidgetItem *b = this->item(tx);
				if (!b->isSelected()) continue;

				QString path3 = path0 + "/" + b->text();

				QDir testdir = QDir(path3);

				testdir.setCurrent(path3);
				if (testdir.exists())
				{
					return;
				}
			}

			int ncount = 0;
			for (int b = 6; b > 1; b--)
			{
				QString ix;
				for (int b2 = 0; b2 < b; b2++)
				{
					ix.append("0");
				}
				if (name.contains(ix))
				{
					ncount = b;
					break;
				}
			}
			if (ncount == 0) return;

			if (ncount < itercount);
			{
				ncount = itercount;
			}
			QString oldext;
			if (name.contains("."))
			{
				QStringList g2 = name.split(".");
				oldext = g2.last();
			}
			
			name = name.replace("0", "");

			int counter = 0;

			for (auto &it : listt)
			{
				QListWidgetItem *tx = it;
				QString oldname = path0 + "/" + tx->text();
				
				QString txtnum = QString::number(counter++);
				while (txtnum.count() < ncount)
				{
					txtnum.prepend("0");
				}

				QStringList extg = tx->text().split(".");
				QString ext;
				if (ex0)
				{
					ext = extg.last();
				}
				else if (ex1)
				{
					ext = oldext;
				}
				else
				{
					ext = ext1;
				}
				QStringList list2 = name.split(".");
				name = list2.first();

				QString newname = name + txtnum + "." + ext;

				QDir dir = QDir(path0);
				dir.setCurrent(path0);
				dir.rename(tx->text(), newname);
			}
			ShowFolderContents(path0);
		}
	}

	rename_dlg->deleteLater();
}
void zList::DeleteFolder(QString path)
{
	QDir test = QDir(path);
	test.setCurrent(path);
	test.removeRecursively();

	QStringList g = path.split("/");
	QString oldn = g.last();
	g.removeLast();
	QString p = g.join("/");

	QDir dir2 = QDir(p);
	dir2.setCurrent(p);
	dir2.rmdir(oldn);
}

void zList::NewFolder()
{
	rename_dlg = new DialogRename(this);
	rename_dlg->hide();

	QDir test = QDir(path0);
	rename_dlg->SetCaption("Create Folder");
	rename_dlg->ShowExt(false);
	int ok = rename_dlg->exec();
	if (ok != rename_dlg->Accepted) return;

	QString name = rename_dlg->name1;
	test.setCurrent(path0);
	bool ok2 = test.mkdir(name);
	ShowFolderContents(path0);

	rename_dlg->deleteLater();
}

void zList::keyPressEvent(QKeyEvent *event)
{
	bool ctrl = (event->modifiers() == Qt::KeyboardModifier::ControlModifier);
	if (event->key() == Qt::Key_Delete)
	{
		if (path0.contains("stream", Qt::CaseInsensitive)) return;
		if (this->currentItem()->text().contains("stream", Qt::CaseInsensitive)) return;

		if (this->currentRow() > -1)
		{
			int ok = QMessageBox::warning(nullptr, "Warning", "Do you want to delete a folder?", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel);
			if (ok == QMessageBox::Ok)
			{
				QStringList gt = path0.split("/");
				gt.append(this->currentItem()->text());

				QString path1 = gt.join("/");
				QDir test = QDir(path1);
				test.setCurrent(path1);

				QStringList EList = test.entryList(QDir::Filter::AllEntries);
				if (EList.count() > 200)
				{
					int ok2 = QMessageBox::warning(nullptr, "Warning", "There are more than 200 files in this folder. Continue?", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel);
					if (ok2 != QMessageBox::Ok)
					{
						return;
					}
				}
				for (auto &it : EList)
				{
					QString x = it;
					this->addItem(x);
				}
				cur_item = this->currentItem()->text();

				DeleteFolder(path0 + "/" + this->currentItem()->text());
				ShowFolderContents(path0);
			}
		}

	}
	else if (event->key() == Qt::Key_Up)
	{
		if (event->modifiers() == Qt::KeyboardModifier::AltModifier)
		{
			Up();
			Snapshot();
		}
		else
		{
			Prev();
		}
	}
	else if (event->key() == Qt::Key_Down)
	{
		Next();
	}
	else if (event->key() == Qt::Key_Left && event->modifiers() == Qt::KeyboardModifier::AltModifier)
	{
		Back();
	}
	else if (event->key() == Qt::Key_Right && event->modifiers() == Qt::KeyboardModifier::AltModifier)
	{
		Forward();
	}

	else if (event->key() == Qt::Key_Return)
	{
		QString txt = this->currentItem()->text();
		QDir d = QDir(path0 + "/" + txt);
		if (d.exists())
		{
			Enter();
			Snapshot();
		}
		else if (txt.endsWith(".exe"))
		{
			QProcess::startDetached(path0 + "/" + txt);
		}
		else
		{
			QDesktopServices::openUrl(path0 + "/" + txt);
		}
	}
	else if (event->key() == Qt::Key_R && ctrl)
	{
		if (waitkey)
		{
			simkey = 0;
			SelectSimilar();
			waitkey = false;
		}
		else
		{
			Rename();
		}
	}
	else if (event->key() == Qt::Key_T && ctrl)
	{
		if (waitkey)
		{
			simkey = 1;
			SelectSimilar();
			waitkey = false;
		}
	}
	else if (event->key() == Qt::Key_N && ctrl)
	{
		if (waitkey)
		{
			simkey = 2;
			SelectSimilar();
			waitkey = false;
		}
		else
		{
			NewFolder();
		}
	}
	else if (event->key() == Qt::Key_End)
	{
		int count0 = this->count();
		count0--;
		this->setCurrentRow(count0);
	}
	else if (event->key() == Qt::Key_Home)
	{
		this->setCurrentRow(0);
	}
	else if (event->key() == Qt::Key_G && ctrl)
	{
		waitkey = true;
	}
	else if (event->key() == Qt::Key_A)
	{
		this->selectAll();
	}
	else if (event->key() == Qt::Key_D && ctrl)
	{
		this->clearSelection();
	}
	else
	{
		QString text = event->text();
		if (text.isEmpty()) return;

		SelectItemStarts(text);
	}
}

void zList::Snapshot()
{
	while (history.count() > history_id)
	{
		history.removeLast();
	}
	history << path0;
	history_id++;
}

void zList::Back()
{
	if (history.isEmpty()) return;

	history_id--;

	if (history_id - 1 < 0)
	{
		history_id++;
		return;
	}

	QString t = history.at(history_id - 1);

	path0 = t;
	ShowFolderContents(t);
}

void zList::Forward()
{
	if (history_id > history.count() - 1) return;

	history_id++;

	if (history_id - 1 < 0) return;

	QString t = history.at(history_id - 1);

	path0 = t;

	ShowFolderContents(t);
}

void zList::GoToPath(QString path)
{
	path0 = path;
	ShowFolderContents(path);
	Snapshot();
}

void zList::SelectSimilar()
{
	QString txt = this->currentItem()->text();

	QString num = txt.replace(QRegExp("[^\\d]"), "");
	int count = num.count();
	if (simkey == 0)
	{
		QDir dir = QDir(path0);
		dir.setCurrent(path0);
		QFileInfoList list3 = dir.entryInfoList(QDir::Filter::AllEntries | QDir::Filter::NoDotAndDotDot);
		int row = this->currentRow();
		QFileInfo inf = list3.at(row);

		QDateTime time0 = inf.lastModified();

		int n = 0;
		for (auto &it : list3)
		{
			QFileInfo inf2 = it;

			QDate dt = inf2.lastModified().date();
			QDate dt2 = inf.lastModified().date();

			if (dt.year() == dt2.year() && dt.month() == dt2.month() && dt.day() == dt2.day())
			{
				QListWidgetItem *b = this->item(n);
				this->setItemSelected(b, true);
			}
			n++;
		}
	}
	else if (simkey == 1)
	{
		QDir dir = QDir(path0);
		dir.setCurrent(path0);
		QStringList list2 = dir.entryList(QDir::Filter::AllEntries | QDir::Filter::NoDotAndDotDot);

		QString reg2 = QString("[\\d]{") + QString::number(count) + "}";
		QString regf = QString("[\\d]{") + QString::number(count + 1) + "}";
		QRegExp rx = QRegExp(reg2);
		QRegExp rxf = QRegExp(regf);
		int n = 0;
		for (auto &it : list2)
		{
			QString txt2 = it;
			if (txt2.contains(rx) && !txt2.contains(rxf))
			{
				QListWidgetItem *b = this->item(n);
				this->setItemSelected(b, true);
			}
			n++;
		}
	}
	else if (simkey == 2)
	{
		QDir dir = QDir(path0);
		dir.setCurrent(path0);
		QFileInfoList list3 = dir.entryInfoList(QDir::Filter::AllEntries | QDir::Filter::NoDotAndDotDot);
		int row = this->currentRow();
		QFileInfo inf = list3.at(row);

		size_t size0 = inf.size();

		int n = 0;
		for (auto &it : list3)
		{
			QFileInfo inf2 = it;

			size_t size1 = inf2.size();

			bool xb = false;
			if (size1 > size0)
			{
				xb = (static_cast<double>(size0) / static_cast<double>(size1) > 0.75f);
			}
			else
			{
				xb = (static_cast<double>(size1) / static_cast<double>(size0) > 0.75f);
			}
			if (xb)
			{
				QListWidgetItem *b = this->item(n);
				this->setItemSelected(b, true);
			}
			n++;
		}
	}
}

void zList::mousePressEvent(QMouseEvent *event)
{
	if (this->selectedItems().count() > 1)
	{
		rangeselected = true;
		this->clearSelection();
		if (itemAt(event->x(), event->y()))
		{
			itemAt(event->x(), event->y())->setSelected(true);
		}
		return;
	}
	int t = clicktime2.elapsed();
	if (t > QApplication::doubleClickInterval())
	{
		clicktime = QTime();
		clicktime2 = QTime();
		doubleclickstage = -1;
	}

	if (doubleclickstage == -1)
	{
		clicktime.start();

		clearSelection();

		if (itemAt(event->x(), event->y()))
		{
			itemAt(event->x(), event->y())->setSelected(true);
		}
		mx = event->x();
		my = event->y();
	}
	else if (doubleclickstage == 0)
	{
		doubleclickstage = 1;
	}
}

void zList::mouseMoveEvent(QMouseEvent *e)
{
	if (doubleclickstage == 0)
	{
		if (abs(e->x() - mx) > 8 || abs(e->y() - my) > 8)
		{
			clicktime = QTime();
			clicktime2 = QTime();
			doubleclickstage = -1;
		}
	}


	if (e->buttons() == Qt::LeftButton)
	{
		this->itemAt(e->x(), e->y())->setSelected(true);
	}
}

void zList::mouseReleaseEvent(QMouseEvent *e)
{
	if (doubleclickstage == -1)
	{
		doubleclickstage = 0;
		clicktime2 = QTime();
		clicktime2.start();
	}
	else if (doubleclickstage == 1)
	{
		doubleclickstage = 2;
	}
	
	if (doubleclickstage == 2)
	{
		int t = clicktime.elapsed();
		if (t > QApplication::doubleClickInterval()) return;
		clicktime = QTime();
		doubleclickstage = -1;

		if (!itemAt(e->x(), e->y())) return;

		this->setCurrentItem(this->itemAt(e->x(), e->y()));
		itemDoubleClicked(this->itemAt(e->x(), e->y()));
	}
}

void zList::itemDoubleClicked(QListWidgetItem *item)
{
	if (!item) return;

	QDir xdir = QDir(path0 + "/" + item->text());
	if (xdir.exists())
	{
		Enter();
		Snapshot();
	}
	else if (item->text().endsWith(".exe"))
	{
		QProcess::startDetached(path0 + "/" + item->text());
	}
	else
	{
		QDesktopServices::openUrl(path0 + "/" + item->text());
	}
}


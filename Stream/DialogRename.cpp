#include "stdafx.h"
#include "DialogRename.h"

DialogRename::DialogRename(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.cmdApply, SIGNAL(pressed()), this, SLOT(cmdOk()));
	connect(ui.cmdClose, SIGNAL(pressed()), this, SLOT(cmdExit()));
	connect(ui.toolButtonExtMore, SIGNAL(pressed()), this, SLOT(cmdExtMore()));
}

DialogRename::~DialogRename()
{

}

void DialogRename::SetCaption(QString caption)
{
	this->setWindowTitle(caption);
}

void DialogRename::SetStartName(QString t)
{
	ui.lineEditName->setText(t);
}

void DialogRename::ShowExt(bool t)
{
	ui.comboBoxExtension->setVisible(t);
}

void DialogRename::SetExtList(QStringList t)
{
	ui.comboBoxExtension->clear();
	ui.comboBoxExtension->addItems(t);
}

void DialogRename::cmdOk()
{
	name1 = ui.lineEditName->text();
	this->setWindowTitle("Rename");
	ext = ui.comboBoxExtension->currentText();
	extstage = 0;
	ui.comboBoxExtension->setMinimumWidth(0);

	list_origin.clear();
	accept();
}

void DialogRename::cmdExtMore()
{
	extmore = true;

	QStringList items;
	for (int t = 0; t < ui.comboBoxExtension->count(); t++)
	{
		items << ui.comboBoxExtension->itemText(t);
	}
	ui.comboBoxExtension->clear();
	items.removeFirst();
	items.removeLast();
	items.removeLast();

	QStringList b;

	if (extstage == 0)
	{
		b << "Bitmap (*.bmp)";
		b << "JPEG (*.jpg)";
		b << "Truevision (*.tga)";
		b << "Portable Graphics (*.png)";
		b << "Graphics Interchange (*.gif)";
		b << "Tagged Image (*.tif)";
		b << "Photoshop (*.psd)";
		b << "Google (*.webp)";
		b << "Direct3D (*.dds)";
		b << "Text (*.txt)";

		list_origin << items;
	}
	else if (extstage == 1)
	{
		b << "Bitmap (*.bmp)";
		b << "JPEG (*.jpg)";
		b << "Truevision (*.tga)";
		b << "Portable Graphics (*.png)";
		b << "Graphics Interchange (*.gif)";
		b << "Tagged Image (*.tif)";
		b << "Photoshop (*.psd)";
		b << "Google (*.webp)";
		b << "Direct3D (*.dds)";
		b << "Text (*.txt)";

		b << "Word (*.docx)";
		b << "OpenOffice (*.odt)";
		b << "Video (*.mp4)";
		b << "QuickTime (*.mov)";
		b << "AVI (*.avi)";

		items.clear();
		items << list_origin;
	}
	else if (extstage == 2)
	{
		b << "Bitmap (*.bmp)";
		b << "JPEG (*.jpg)";
		b << "Truevision (*.tga)";
		b << "Portable Graphics (*.png)";
		b << "Graphics Interchange (*.gif)";
		b << "Tagged Image (*.tif)";
		b << "Photoshop (*.psd)";
		b << "Google (*.webp)";
		b << "Direct3D (*.dds)";
		b << "Text (*.txt)";

		b << "Word (*.docx)";
		b << "OpenOffice (*.odt)";
		b << "Video (*.mp4)";
		b << "QuickTime (*.mov)";
		b << "AVI (*.avi)";

		b << "Sound(*.wav)";
		b << "Sound(*.mp3)";
		b << "Music(*.ogg)";
		b << "Music(*.flac)";

		b << "Blender (*.blend)";
		b << "3dsMax (*.max)";
		b << "Maya (*.mb)";
		b << "Lightwave (*.lwo)";
		b << "ZBrush (*.ztl)";

		items.clear();
		items << list_origin;

	}

	extstage++;
	QStringList b2;

	for (auto &it : b)
	{
		QString xa = it;
		bool bok = false;
		for (auto &it2 : items)
		{
			QString xa2 = it2;
			if (xa.contains(xa2))
			{
				bok = true;
				break;
			}
		}
		if (bok) continue;

		b2 << xa;
	}

	items.clear();
	items << b2;

	items.prepend("Existing");
	items.append("Custom");
	
	ui.comboBoxExtension->addItems(items);
	minw = ui.comboBoxExtension->minimumWidth();
	ui.comboBoxExtension->setMinimumWidth(200);
}

void DialogRename::cmdExit()
{
	this->setWindowTitle("Rename");
	extstage = 0;
	extmore = false;
	ui.comboBoxExtension->setMinimumWidth(0);
	list_origin.clear();

	reject();
}

#include "stdafx.h"
#include "DialogRename.h"

DialogRename::DialogRename(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.cmdApply, SIGNAL(pressed()), this, SLOT(cmdOk()));
	connect(ui.cmdClose, SIGNAL(pressed()), this, SLOT(cmdExit()));

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

void DialogRename::cmdOk()
{
	name1 = ui.lineEditName->text();
	this->setWindowTitle("Rename");
	accept();
}

void DialogRename::cmdExit()
{
	this->setWindowTitle("Rename");

	reject();
}

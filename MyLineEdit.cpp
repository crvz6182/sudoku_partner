#include "MyLineEdit.h"
#include <qmenu.h>
#include <qmessagebox.h>

void MyLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
	connect(action, SIGNAL(triggered()), this, SLOT(hintCliked()));
	hintMenu->exec(QCursor::pos());
}

void MyLineEdit::hintCliked()
{
	emit hint();
	//QMessageBox::information(this, "2", "Try it again!");
}

void MyLineEdit::setRead(bool a)
{
	if (a)
	{
		this->setReadOnly(true);
		this->setStyleSheet("background-color:#D2E9FF");
		action->setEnabled(false);
	}
	else
	{
		this->setReadOnly(false);
		this->setStyleSheet("background-color:#ffffff");
		action->setEnabled(true);
	}
}
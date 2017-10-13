#pragma once
#include <QtWidgets/QMainWindow>
#include <qlineedit.h>
#include <qmenu.h>

class MyLineEdit :public QLineEdit
{
	Q_OBJECT

public:
	void setRead(bool a);

protected:
	QMenu * hintMenu = new QMenu();
	QAction * action = hintMenu->addAction("hint");
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	void hint();

	public slots :
	void hintCliked();

};
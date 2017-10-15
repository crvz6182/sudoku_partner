#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_sudokuGui1.h"
#include "Core.h"
#include <qtextedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "MyLineEdit.h"

class sudokuGui1 : public QMainWindow
{
	Q_OBJECT

public:
	sudokuGui1(QWidget *parent = Q_NULLPTR);

public slots:
	void restartButtonCliked();
	void clearButtonClicked();
	void submitButtonClicked();
	void modeChooseCliked();
	void updateTime();
	void hintCliked();
	void helpClicked();

private:
	Ui::sudokuGui1Class ui;
	int sudo[1][81];
	int mode;
	QTimer * timer;      //定时器 每秒更新时间
	QTime * TimeRecord;  //记录时间
	QLineEdit *bestRecord;  // 显示最好记录的时间
	QLineEdit *nowRecord;  // 显示现在的时间
	QString record;
	MyLineEdit * blocks[81];
	QAction * subMenuEasy;
	QAction * subMenuGeneral;
	QAction * subMenuHard;
	Core * core;
	QPushButton * submitButton;
	QPushButton *clearButton;
};

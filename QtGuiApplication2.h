#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication2.h"
#include <qtextedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "MyLineEdit.h"
#include "Core.h"

class QtGuiApplication2 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication2(QWidget *parent = Q_NULLPTR);

public slots:
void restartButtonCliked();
void clearButtonClicked();
void submitButtonClicked();
void modeChooseCliked();
void updateTime();
void hintCliked();
void helpClicked();

private:
	Ui::QtGuiApplication2Class ui;
	int sudo[1][81];
	int mode;
	QTimer * timer;      //��ʱ�� ÿ�����ʱ��
	QTime * TimeRecord;  //��¼ʱ��
	QLineEdit *bestRecord;  // ��ʾ��ü�¼��ʱ��
	QLineEdit *nowRecord;  // ��ʾ���ڵ�ʱ��
	QString record;
	MyLineEdit * blocks[81];
	QAction * subMenuEasy;
	QAction * subMenuGeneral;
	QAction * subMenuHard;
	Core * core;
	QPushButton * submitButton;
	QPushButton *clearButton;
};

#include "sudokuGui.h"
#include <qtablewidget.h>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qmenu.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qtimezone.h>
#include <qtimer.h>
#include <qevent.h>

sudokuGui::sudokuGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	// ��ʼ��
	mode = 1;
	core = new Core();
	// 9*9�ķ���
	core->generate(1, mode, sudo);  //init ���ú���
	QGridLayout *layoutSudo = new QGridLayout;
	QGridLayout *layoutBlocks[9];  // 9��3*3
	for (int i = 0; i < 9; i++) {
		layoutBlocks[i] = new QGridLayout;
		layoutBlocks[i]->setHorizontalSpacing(0);
		layoutBlocks[i]->setVerticalSpacing(0);
	}
	int nowNum = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			nowNum = i * 9 + j;
			blocks[nowNum] = new MyLineEdit();
			blocks[nowNum]->setObjectName(QString::number(nowNum));  // �趨�ÿؼ�������
			blocks[nowNum]->setAlignment(Qt::AlignCenter | Qt::AlignBottom | Qt::AlignHCenter);  // ����
			blocks[nowNum]->setMinimumSize(70, 70);  // ��ʼ��С
			QRegExp regExp("[1-9]");  // ���������ֻ����1-9������
			blocks[nowNum]->setValidator(new QRegExpValidator(regExp, this));
			connect(blocks[nowNum], SIGNAL(hint()), this, SLOT(hintCliked()));  // ��ʾ���¼�
			if (sudo[0][nowNum] != 0)  // �ж��Ƿ�Ϊ��Ҫ��д��
			{
				blocks[nowNum]->setText(QString::number(sudo[0][nowNum]));  // ��ǰС�������
				blocks[nowNum]->setRead(true);  // ����Ϊֻ��
			}
			layoutBlocks[i / 3 * 3 + j / 3]->addWidget(blocks[nowNum], i % 3, j % 3, 0);  // ���ؼ�����3*3��
		}
	}
	// ��������9*9�Ĳ���
	for (int i = 0; i < 9; i++) {
		layoutSudo->addLayout(layoutBlocks[i], i / 3, i % 3, 0);
	}
	layoutSudo->setHorizontalSpacing(5);  // ���ÿؼ�֮��ľ���
	layoutSudo->setVerticalSpacing(5);

	// �˵�
	QMenu* mainMenu = this->menuBar()->addMenu("Game");
	subMenuEasy = mainMenu->addAction("easy");
	subMenuGeneral = mainMenu->addAction("general");
	subMenuHard = mainMenu->addAction("hard");
	subMenuEasy->setCheckable(true);
	subMenuEasy->setChecked(true);  // ��ʼ���Ѷ�ѡ��
	subMenuGeneral->setCheckable(true);
	subMenuHard->setCheckable(true);
	// �󶨵���¼�
	connect(subMenuEasy, SIGNAL(triggered()), this, SLOT(modeChooseCliked()));
	connect(subMenuGeneral, SIGNAL(triggered()), this, SLOT(modeChooseCliked()));
	connect(subMenuHard, SIGNAL(triggered()), this, SLOT(modeChooseCliked()));
	// Help�˵�����Ϸ�������
	QAction* helpMenu = this->menuBar()->addAction("Help");
	connect(helpMenu, SIGNAL(triggered()), this, SLOT(helpClicked()));

	//��ť
	QPushButton *restartButton = new QPushButton(this);  // ���¿�ʼ
	clearButton = new QPushButton(this);  // ȫ�����
	submitButton = new QPushButton(this);  // �ύ
	restartButton->setText(tr("restart"));
	clearButton->setText(tr("clear all"));
	submitButton->setText("Submit");
	connect(restartButton, SIGNAL(clicked(bool)), this, SLOT(restartButtonCliked()));
	connect(clearButton, SIGNAL(clicked(bool)), this, SLOT(clearButtonClicked()));
	connect(submitButton, SIGNAL(clicked(bool)), this, SLOT(submitButtonClicked()));

	//ʱ���¼
	record = "00:00:00";
	timer = new QTimer;  //��ʼ����ʱ��
	TimeRecord = new QTime(0, 0, 0); //��ʼ��ʱ��
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	timer->start(1000);
	// ��ʾʱ��
	bestRecord = new QLineEdit();
	nowRecord = new QLineEdit();
	bestRecord->setStyleSheet("background-color:#00ffffff; border:1px solid#00ffffff");
	nowRecord->setStyleSheet("background-color:#00ffffff; border:1px solid#00ffffff");
	bestRecord->setAlignment(Qt::AlignRight);
	nowRecord->setAlignment(Qt::AlignRight);
	bestRecord->setContextMenuPolicy(Qt::NoContextMenu);
	nowRecord->setContextMenuPolicy(Qt::NoContextMenu);
	bestRecord->setText(record);
	bestRecord->setReadOnly(true);
	nowRecord->setText(TimeRecord->toString("hh:mm:ss"));
	nowRecord->setReadOnly(true);

	//����
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(restartButton, 0, 0, 2, 1, 0); // ���¿�ʼ��ť
	layout->addWidget(clearButton, 0, 1, 2, 1, 0); // ��հ�ť
	layout->addWidget(bestRecord, 0, 8, 1, 1, Qt::AlignRight); //��Ѽ�¼ʱ��
	layout->addWidget(nowRecord, 1, 8, 1, 1, Qt::AlignRight); //�Ѿ���ʱ
	layout->addLayout(layoutSudo, 2, 0, 1, 9, Qt::AlignCenter);  // 9*9�ķ���
	layout->addWidget(submitButton, 3, 0, 1, 9, Qt::AlignCenter);  // �ύ��ť

	layout->setContentsMargins(65, 15, 65, 20);  // left, top, right, bottom
	this->centralWidget()->setLayout(layout);
}

// ������ʵ��
void sudokuGui::restartButtonCliked()
{
	submitButton->setEnabled(true);
	clearButton->setEnabled(true);
	core->generate(1, mode, sudo);
	for (int i = 0; i < 81; i++)
	{
		blocks[i]->setRead(false);
		if (sudo[0][i] != 0)  // �ж��Ƿ�Ϊ��Ҫ��д��
		{
			blocks[i]->setText(QString::number(sudo[0][i]));  // ��ǰС�������
			blocks[i]->setRead(true);  // ����Ϊֻ��
		}
		else
		{
			blocks[i]->setText("");
		}
	}
	timer->stop();    // ��ʱ��ֹͣ
	TimeRecord->setHMS(0, 0, 0); // ʱ����Ϊ0���¿�ʼ
	timer->start(1000);
}

void sudokuGui::clearButtonClicked()
{
	for (int i = 0; i < 81; i++)
	{
		if (sudo[0][i] == 0)  // �ж��Ƿ�Ϊ��Ҫ���
		{
			blocks[i]->setText("");  // ��ǰС�������
		}
	}
}

void sudokuGui::submitButtonClicked()
{
	timer->stop();  // ��ʱ��ֹͣ
					// ������Ѽ�¼
	if (record.compare("00:00:00") == 0)
	{
		record = TimeRecord->toString("hh:mm:ss");
	}
	else
	{
		record = record.compare(record, TimeRecord->toString("hh:mm:ss")) < 0 ? record : TimeRecord->toString("hh:mm:ss");
	}
	int resultSudo[81];
	int rSudo[81] = { 0 };
	bool result;
	bool filled = true;
	for (int i = 0; i < 81; i++)
	{
		//QMessageBox::information(this, QString::number(i), blocks[i]->text());
		if (blocks[i]->text().compare("") == 0)
		{
			resultSudo[i] = 0;
			filled = false;
		}
		else
		{
			resultSudo[i] = (blocks[i]->text()).toInt();
		}
	}
	result = core->solve(resultSudo, rSudo);
	if (result && filled)
	{
		QMessageBox::information(this, "success!", "You are smart!");
		bestRecord->setText(record);
		for (int i = 0; i < 81; i++)
		{
			if (sudo[0][i] == 0)
			{
				blocks[i]->setRead(true);  // ����Ϊֻ��
				blocks[i]->setStyleSheet("background-color:#ffffff");
			}
		}
	}
	else
	{
		core->solve(sudo[0], rSudo);
		QMessageBox::information(this, "failed!", "Try it again!");
		for (int i = 0; i < 81; i++)
		{
			if (sudo[0][i] == 0)
			{
				blocks[i]->setText(QString::number(rSudo[i]));  // ��ǰС�������
				blocks[i]->setRead(true);  // ����Ϊֻ��
				blocks[i]->setStyleSheet("background-color:#ffffff");
			}
		}
	}
	submitButton->setEnabled(false);
	clearButton->setEnabled(false);
}

void sudokuGui::modeChooseCliked()
{
	// �ж��ǵ��������ģʽ
	QAction * action = qobject_cast<QAction*>(sender());
	action->setChecked(true);
	QString actionName = action->text();
	if (actionName.compare("easy") == 0)
	{
		mode = 1;
		subMenuGeneral->setChecked(false);
		subMenuHard->setChecked(false);
	}
	else if (actionName.compare("general") == 0)
	{
		mode = 2;
		subMenuEasy->setChecked(false);
		subMenuHard->setChecked(false);
	}
	else
	{
		mode = 3;
		subMenuEasy->setChecked(false);
		subMenuGeneral->setChecked(false);
	}
	submitButton->setEnabled(true);
	clearButton->setEnabled(true);
	core->generate(1, mode, sudo);
	for (int i = 0; i < 81; i++)
	{
		blocks[i]->setRead(false);
		if (sudo[0][i] != 0)  // �ж��Ƿ�Ϊ��Ҫ��д��
		{
			blocks[i]->setText(QString::number(sudo[0][i]));  // ��ǰС�������
			blocks[i]->setRead(true);  // ����Ϊֻ��
		}
		else
		{
			blocks[i]->setText("");
		}
	}
	timer->stop();    // ��ʱ��ֹͣ
	TimeRecord->setHMS(0, 0, 0); // ʱ����Ϊ0���¿�ʼ
	timer->start(1000);
}

void sudokuGui::updateTime()
{
	*TimeRecord = TimeRecord->addSecs(1);
	nowRecord->setText(TimeRecord->toString("hh:mm:ss"));
}

void sudokuGui::hintCliked()
{
	int resultSudo[81];
	int rSudo[81];
	bool result;
	// �ж��ĸ�λ�ñ����
	QString actionName = qobject_cast<MyLineEdit*>(sender())->objectName();
	int pos = actionName.toInt();
	// ��ȡ�û���ǰ������
	for (int i = 0; i < 81; i++)
	{
		if (blocks[i]->text().compare("") == 0)
		{
			resultSudo[i] = 0;
		}
		else
		{
			resultSudo[i] = (blocks[i]->text()).toInt();
		}
	}
	result = core->solve(resultSudo, rSudo);
	if (result)
	{
		blocks[pos]->setText(QString::number(rSudo[pos]));
	}
	else
	{
		QMessageBox::information(this, "Hint Error", "Sorry, there are some mistakes in this sudoku, please check it!");
	}
}

void sudokuGui::helpClicked()
{
	QMessageBox::information(this, "Help", "HOW TO PALY THE GAME?\n enter 1-9 in the blank block \n HOW TO CHOSSE MODE?\n click Game in Menu, then choose the mode\n HOW TO GET HINT?\n click the block with right button");
}

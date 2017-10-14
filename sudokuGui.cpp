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
	// 初始化
	mode = 1;
	core = new Core();
	// 9*9的方正
	core->generate(1, mode, sudo);  //init 调用函数
	QGridLayout *layoutSudo = new QGridLayout;
	QGridLayout *layoutBlocks[9];  // 9块3*3
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
			blocks[nowNum]->setObjectName(QString::number(nowNum));  // 设定该控件的名字
			blocks[nowNum]->setAlignment(Qt::AlignCenter | Qt::AlignBottom | Qt::AlignHCenter);  // 居中
			blocks[nowNum]->setMinimumSize(70, 70);  // 初始大小
			QRegExp regExp("[1-9]");  // 限制输入的只能是1-9的数字
			blocks[nowNum]->setValidator(new QRegExpValidator(regExp, this));
			connect(blocks[nowNum], SIGNAL(hint()), this, SLOT(hintCliked()));  // 提示绑定事件
			if (sudo[0][nowNum] != 0)  // 判断是否为需要填写的
			{
				blocks[nowNum]->setText(QString::number(sudo[0][nowNum]));  // 当前小块的数字
				blocks[nowNum]->setRead(true);  // 设置为只读
			}
			layoutBlocks[i / 3 * 3 + j / 3]->addWidget(blocks[nowNum], i % 3, j % 3, 0);  // 将控件插入3*3中
		}
	}
	// 设置整个9*9的布局
	for (int i = 0; i < 9; i++) {
		layoutSudo->addLayout(layoutBlocks[i], i / 3, i % 3, 0);
	}
	layoutSudo->setHorizontalSpacing(5);  // 设置控件之间的距离
	layoutSudo->setVerticalSpacing(5);

	// 菜单
	QMenu* mainMenu = this->menuBar()->addMenu("Game");
	subMenuEasy = mainMenu->addAction("easy");
	subMenuGeneral = mainMenu->addAction("general");
	subMenuHard = mainMenu->addAction("hard");
	subMenuEasy->setCheckable(true);
	subMenuEasy->setChecked(true);  // 初始化难度选项
	subMenuGeneral->setCheckable(true);
	subMenuHard->setCheckable(true);
	// 绑定点击事件
	connect(subMenuEasy, SIGNAL(triggered()), this, SLOT(modeChooseCliked()));
	connect(subMenuGeneral, SIGNAL(triggered()), this, SLOT(modeChooseCliked()));
	connect(subMenuHard, SIGNAL(triggered()), this, SLOT(modeChooseCliked()));
	// Help菜单，游戏规则介绍
	QAction* helpMenu = this->menuBar()->addAction("Help");
	connect(helpMenu, SIGNAL(triggered()), this, SLOT(helpClicked()));

	//按钮
	QPushButton *restartButton = new QPushButton(this);  // 重新开始
	clearButton = new QPushButton(this);  // 全部清空
	submitButton = new QPushButton(this);  // 提交
	restartButton->setText(tr("restart"));
	clearButton->setText(tr("clear all"));
	submitButton->setText("Submit");
	connect(restartButton, SIGNAL(clicked(bool)), this, SLOT(restartButtonCliked()));
	connect(clearButton, SIGNAL(clicked(bool)), this, SLOT(clearButtonClicked()));
	connect(submitButton, SIGNAL(clicked(bool)), this, SLOT(submitButtonClicked()));

	//时间记录
	record = "00:00:00";
	timer = new QTimer;  //初始化定时器
	TimeRecord = new QTime(0, 0, 0); //初始化时间
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	timer->start(1000);
	// 显示时间
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

	//布局
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(restartButton, 0, 0, 2, 1, 0); // 重新开始按钮
	layout->addWidget(clearButton, 0, 1, 2, 1, 0); // 清空按钮
	layout->addWidget(bestRecord, 0, 8, 1, 1, Qt::AlignRight); //最佳纪录时间
	layout->addWidget(nowRecord, 1, 8, 1, 1, Qt::AlignRight); //已经用时
	layout->addLayout(layoutSudo, 2, 0, 1, 9, Qt::AlignCenter);  // 9*9的方正
	layout->addWidget(submitButton, 3, 0, 1, 9, Qt::AlignCenter);  // 提交按钮

	layout->setContentsMargins(65, 15, 65, 20);  // left, top, right, bottom
	this->centralWidget()->setLayout(layout);
}

// 函数的实现
void sudokuGui::restartButtonCliked()
{
	submitButton->setEnabled(true);
	clearButton->setEnabled(true);
	core->generate(1, mode, sudo);
	for (int i = 0; i < 81; i++)
	{
		blocks[i]->setRead(false);
		if (sudo[0][i] != 0)  // 判断是否为需要填写的
		{
			blocks[i]->setText(QString::number(sudo[0][i]));  // 当前小块的数字
			blocks[i]->setRead(true);  // 设置为只读
		}
		else
		{
			blocks[i]->setText("");
		}
	}
	timer->stop();    // 计时器停止
	TimeRecord->setHMS(0, 0, 0); // 时间设为0重新开始
	timer->start(1000);
}

void sudokuGui::clearButtonClicked()
{
	for (int i = 0; i < 81; i++)
	{
		if (sudo[0][i] == 0)  // 判断是否为需要清空
		{
			blocks[i]->setText("");  // 当前小块的数字
		}
	}
}

void sudokuGui::submitButtonClicked()
{
	timer->stop();  // 计时器停止
					// 更新最佳纪录
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
				blocks[i]->setRead(true);  // 设置为只读
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
				blocks[i]->setText(QString::number(rSudo[i]));  // 当前小块的数字
				blocks[i]->setRead(true);  // 设置为只读
				blocks[i]->setStyleSheet("background-color:#ffffff");
			}
		}
	}
	submitButton->setEnabled(false);
	clearButton->setEnabled(false);
}

void sudokuGui::modeChooseCliked()
{
	// 判断是点击了哪种模式
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
		if (sudo[0][i] != 0)  // 判断是否为需要填写的
		{
			blocks[i]->setText(QString::number(sudo[0][i]));  // 当前小块的数字
			blocks[i]->setRead(true);  // 设置为只读
		}
		else
		{
			blocks[i]->setText("");
		}
	}
	timer->stop();    // 计时器停止
	TimeRecord->setHMS(0, 0, 0); // 时间设为0重新开始
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
	// 判断哪个位置被点击
	QString actionName = qobject_cast<MyLineEdit*>(sender())->objectName();
	int pos = actionName.toInt();
	// 获取用户当前的数独
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

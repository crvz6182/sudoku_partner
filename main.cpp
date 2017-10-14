#include "sudokuGui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sudokuGui w;
	w.setWindowFlags((w.windowFlags())& ~Qt::WindowMaximizeButtonHint);
	w.show();
	return a.exec();
}

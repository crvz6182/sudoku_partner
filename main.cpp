#include "sudokuGui1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sudokuGui1 w;
	w.show();
	return a.exec();
}

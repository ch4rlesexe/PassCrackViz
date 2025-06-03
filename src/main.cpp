#include "UIManager.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	UIManager w;
	w.show();
	return a.exec();
}

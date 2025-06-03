#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UIManager; }
QT_END_NAMESPACE

class DictionaryCracker;
class BruteForceCracker;

class UIManager : public QMainWindow {
	Q_OBJECT

public:
	explicit UIManager(QWidget *parent = nullptr);
	~UIManager() override;

private slots:
	void on_buttonStartDictionary_clicked();
	void on_buttonStartBruteForce_clicked();
	void on_buttonStop_clicked();
	void updateProgress(const QString &currentGuess, int attempts);
	void crackingFinished(const QString &found, bool success);

private:
	void resetUI();

	DictionaryCracker *dictCracker;
	BruteForceCracker *bfCracker;
};

#endif 

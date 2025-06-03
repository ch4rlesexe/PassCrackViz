#ifndef DICTIONARYCRACKER_H
#define DICTIONARYCRACKER_H

#include <QThread>
#include <QString>

class DictionaryCracker : public QThread {
	Q_OBJECT
public:
	DictionaryCracker(const QString &hash, const QString &wordlistPath);
	~DictionaryCracker() override { }

	void run() override;
	void stop();

signals:
	void progressUpdated(const QString &currentGuess, int attempts);
	void finishedCracking(const QString &foundPassword, bool success);

private:
	QString targetHash;
	QString wordlist;
	bool stopped;
};

#endif

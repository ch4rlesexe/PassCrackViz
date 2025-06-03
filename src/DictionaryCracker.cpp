#include "DictionaryCracker.h"
#include "HashUtils.h"

#include <QFile>
#include <QTextStream>

DictionaryCracker::DictionaryCracker(const QString &hash, const QString &wordlistPath)
    : targetHash(hash.trimmed().toLower()),
      wordlist(wordlistPath),
      stopped(false)
{
}

void DictionaryCracker::stop() {
    stopped = true;
}

void DictionaryCracker::run() {
    QFile file(wordlist);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit finishedCracking("", false);
        return;
    }

    QTextStream in(&file);
    int attempts = 0;
    while (!in.atEnd() && !stopped) {
        QString word = in.readLine().trimmed();
        if (word.isEmpty()) continue;

        std::string md5hex = HashUtils::md5(word.toStdString());
        QString hashAttempt = QString::fromStdString(md5hex);

        ++attempts;
        emit progressUpdated(word, attempts);

        if (hashAttempt.toLower() == targetHash) {
            emit finishedCracking(word, true);
            return;
        }
    }

    emit finishedCracking("", false);
}


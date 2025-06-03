#ifndef BRUTEFORCECRACKER_H
#define BRUTEFORCECRACKER_H

#include <QThread>
#include <QString>
#include <atomic>

/*
 A QThread that iteratively enumerates all strings over
 “abcdefghijklmnopqrstuvwxyz” of length 1..maxLen,
 hashes each with MD5, and compares to targetHash.
 Emits progressUpdated(candidate, attempts) on each try,
 and finishedCracking(foundPassword, success) when done or stopped.
*/

class BruteForceCracker : public QThread {
    Q_OBJECT
public:
    BruteForceCracker(const QString &hash, int maxLength);
    ~BruteForceCracker() override { }


    void run() override;

    void stop();

signals:

    void progressUpdated(const QString &currentGuess, int attempts);

    void finishedCracking(const QString &foundPassword, bool success);

private:
    QString    targetHash;    // lowercase, 32‑char MD5
    int        maxLen;        // maximum length to try (1..maxLen)
    std::atomic<bool> stopped; 
    long long  attempts;      // total attempts made

    const std::string charset = "abcdefghijklmnopqrstuvwxyz";
};

#endif 


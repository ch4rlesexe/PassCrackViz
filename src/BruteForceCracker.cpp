#include "BruteForceCracker.h"
#include "HashUtils.h"

#include <QString>
#include <vector>

BruteForceCracker::BruteForceCracker(const QString &hash, int maxLength)
    : targetHash(hash.trimmed().toLower()),
      maxLen(maxLength),
      stopped(false),
      attempts(0)
{
}

void BruteForceCracker::stop() {
    stopped = true;
}

void BruteForceCracker::run() {
    // for each length from 1 to maxLen
    for (int length = 1; length <= maxLen && !stopped; ++length) {
        std::vector<int> indices(length, 0);

        std::string candidate(length, charset[0]);

        bool doneForThisLength = false;
        while (!doneForThisLength && !stopped) {
            // 1) check this candidate:
            ++attempts;
            QString guess = QString::fromStdString(candidate);
            emit progressUpdated(guess, static_cast<int>(attempts));

            // compute MD5 and compare:
            std::string md5hex = HashUtils::md5(candidate);
            if (QString::fromStdString(md5hex).toLower() == targetHash) {
                stopped = true;
                emit finishedCracking(QString::fromStdString(candidate), true);
                return;
            }

            // 2) increment the “digits” (indices) like a counter
            for (int pos = length - 1; pos >= 0; --pos) {
                if (stopped) {
                    // If stop was requested, just bail out
                    return;
                }
                if (indices[pos] + 1 < static_cast<int>(charset.size())) {
                    indices[pos] += 1;
                    candidate[pos] = charset[ indices[pos] ];
                    for (int k = pos + 1; k < length; ++k) {
                        indices[k] = 0;
                        candidate[k] = charset[0];
                    }
                    break;
                } else {
                    if (pos == 0) {
                        doneForThisLength = true;
                    }
                    indices[pos] = 0;
                    candidate[pos] = charset[0];
                }
            }
        }
    }

    if (!stopped) {
        emit finishedCracking("", false);
    }
}


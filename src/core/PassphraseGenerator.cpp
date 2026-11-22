/*
 *  Copyright (C) 2025 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PassphraseGenerator.h"

#include <QFile>
#include <QRegularExpression>
#include <QSet>
#include <QTextStream>
#include <cmath>

#include "core/Resources.h"
#include "crypto/Random.h"

const int PassphraseGenerator::DefaultWordCount = 7;
const char* PassphraseGenerator::DefaultSeparator = " ";
const char* PassphraseGenerator::DefaultWordList = "eff_large.wordlist";
// Symbols most sites accept
const char* PassphraseGenerator::ExtraSymbolSet = "!#$%&*+-=?@^_";

PassphraseGenerator::PassphraseGenerator()
    : m_wordCount(DefaultWordCount)
    , m_wordCase(LOWERCASE)
    , m_separator(DefaultSeparator)
{
    setDefaultWordList();
}

double PassphraseGenerator::estimateEntropy(int wordCount)
{
    if (m_wordlist.isEmpty()) {
        return 0.0;
    }
    if (wordCount < 1) {
        wordCount = m_wordCount;
    }

    // Extra characters are counted for their value only, not their position, so this stays conservative
    return std::log2(m_wordlist.size()) * wordCount + std::log2(10.0) * m_extraDigits
           + std::log2(static_cast<double>(qstrlen(ExtraSymbolSet))) * m_extraSymbols;
}

void PassphraseGenerator::setWordCount(int wordCount)
{
    m_wordCount = qMax(1, wordCount);
}

void PassphraseGenerator::setWordCase(PassphraseWordCase wordCase)
{
    m_wordCase = wordCase;
}

void PassphraseGenerator::setWordList(const QString& path)
{
    m_wordlist.clear();
    // Initially load wordlist into a set to avoid duplicates
    QSet<QString> wordset;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't load passphrase wordlist: %s", qPrintable(path));
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    bool isSigned = line.startsWith("-----BEGIN PGP SIGNED MESSAGE-----");
    if (isSigned) {
        while (!line.isNull() && !line.trimmed().isEmpty()) {
            line = in.readLine();
        }
    }
    QRegularExpression rx("^[0-9]+(-[0-9]+)*\\s+([^\\s]+)$");
    while (!line.isNull()) {
        if (isSigned && line.startsWith("-----BEGIN PGP SIGNATURE-----")) {
            break;
        }
        // Handle dash-escaped lines (if the wordlist is signed)
        if (isSigned && line.startsWith("- ")) {
            line.remove(0, 2);
        }
        line = line.trimmed();
        line.replace(rx, "\\2");
        if (!line.isEmpty()) {
            wordset.insert(line);
        }
        line = in.readLine();
    }

    m_wordlist = wordset.values();

    if (!isWordListValid()) {
        qWarning("Wordlist is less than minimum acceptable size: %s", qPrintable(path));
    }
}

void PassphraseGenerator::setDefaultWordList()
{
    const QString path = resources()->wordlistPath(PassphraseGenerator::DefaultWordList);
    setWordList(path);
}

void PassphraseGenerator::setWordSeparator(const QString& separator)
{
    m_separator = separator;
}

void PassphraseGenerator::setExtraDigits(int count)
{
    m_extraDigits = qMax(0, count);
}

void PassphraseGenerator::setExtraSymbols(int count)
{
    m_extraSymbols = qMax(0, count);
}

QString PassphraseGenerator::generatePassphrase() const
{
    if (m_wordlist.isEmpty()) {
        return {};
    }

    QStringList words;
    int randomIndex = randomGen()->randomUInt(static_cast<quint32>(m_wordCount));
    for (int i = 0; i < m_wordCount; ++i) {
        int wordIndex = randomGen()->randomUInt(static_cast<quint32>(m_wordlist.size()));
        auto tmpWord = m_wordlist.at(wordIndex);

        // convert case
        switch (m_wordCase) {
        case UPPERCASE:
            tmpWord = tmpWord.toUpper();
            break;
        case TITLECASE:
            tmpWord = tmpWord.replace(0, 1, tmpWord.left(1).toUpper());
            break;
        case MIXEDCASE:
            tmpWord = i == randomIndex ? tmpWord.toUpper() : tmpWord.toLower();
            break;
        case LOWERCASE:
            tmpWord = tmpWord.toLower();
            break;
        }
        words.append(tmpWord);
    }

    // Stick each extra digit or symbol onto the end of a randomly chosen word
    const QString symbols = QString::fromLatin1(ExtraSymbolSet);
    auto appendToRandomWord = [&words](QChar c) {
        auto i = randomGen()->randomUInt(static_cast<quint32>(words.size()));
        words[static_cast<int>(i)].append(c);
    };
    for (int i = 0; i < m_extraDigits; ++i) {
        appendToRandomWord(QChar('0' + static_cast<int>(randomGen()->randomUInt(10))));
    }
    for (int i = 0; i < m_extraSymbols; ++i) {
        appendToRandomWord(symbols.at(static_cast<int>(randomGen()->randomUInt(static_cast<quint32>(symbols.size())))));
    }

    return words.join(m_separator);
}

bool PassphraseGenerator::isWordListValid() const
{
    return m_wordlist.size() >= m_minWordListSize;
}

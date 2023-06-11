#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QFile>
#include <QTextStream>

class WordFileParser
{
public:
    WordFileParser(const QString &filename);
    QString getNextWord();
    double getProcessPercentage() const;
    bool atEnd();
private:
    size_t totalBytes_;
    QFile file_;
    QTextStream txtStream_;
};

#endif // FILEPARSER_H

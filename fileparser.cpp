#include "fileparser.h"

#include <QUrl>
#include <QRegularExpression>

WordFileParser::WordFileParser(const QString &filename)
    : file_(QUrl(filename).toLocalFile())
    , txtStream_(&file_)
{
    if(!file_.open(QIODevice::ReadOnly))
    {
        return;
    }

    totalBytes_ = file_.size();
}

QString WordFileParser::getNextWord()
{
    static QRegularExpression rexp("[^[:punct:],[:space:]]"); // словом считаю любой символ, но не разрывы и пунктуацию
    if(!file_.isOpen() || totalBytes_ == 0)
    {
        return {};
    }

    QString word;
    while (!txtStream_.atEnd())
    {
        QChar ch;
        txtStream_ >> ch;
        if(rexp.match(ch).hasMatch())
            word.append(QStringLiteral("%1").arg(ch));
        else
            break;
    }

    return word;
}

double WordFileParser::getProcessPercentage() const
{
    if(!file_.isOpen() || totalBytes_ == 0)
    {
        return 0;
    }

    return static_cast<double>(txtStream_.pos()) / (static_cast<double>(totalBytes_) / 100.0);
}

bool WordFileParser::atEnd()
{
    if(!file_.isOpen() || totalBytes_ == 0)
    {
        return true;
    }

    return txtStream_.atEnd();
}

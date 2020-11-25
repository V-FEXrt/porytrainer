#include "parser_util.h"

#include "parser.h"

ParserUtil::ParserUtil(QString root): root_(root) {}

QStringList ParserUtil::ReadDefines(QString filename, QString prefix)
{
    if (filename.isEmpty()) {
        return QStringList();
    }

    QString filepath = root_ + "/" + filename;

    fex::Parser parser;

    std::vector<std::string> match_list = { prefix.toStdString() + ".*" };
    std::map<std::string, int> defines = parser.ReadDefines(filepath.toStdString(), match_list);

    QStringList out;
    for(auto const& define : defines) {
        out.append(QString::fromStdString(define.first));
    }

    return out;
}

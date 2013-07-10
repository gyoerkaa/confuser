#include "texcode.h"
#include <QStringList>
#include <QStringBuilder>

TexCode::TexCode(const ConfMat &cmat,
                       QObject *parent):
    QObject(parent)
{
    m_Table.clear();
    m_Preamble.clear();
    this->generateTable(cmat);
    this->generatePreamble();
}


TexCode::~TexCode()
{

}


void TexCode::generateTable(const ConfMat &cmat)
{
    QString line;
    int colCount = cmat.getColCount();
    int rowCount = cmat.getRowCount();

    m_Table.clear();
    m_Table.append(QString("\\begin{table}[H]%"));
    m_Table.append(QString("    \\centering"));

    line = QString("    \\begin{tabular}{|cc|");
    for (int c = 0; c < colCount-1; ++c)
        line += QString("c|");
    line += QString("}%");
    m_Table.append(line);

    line = QString("    \\cline{1-") %
           QString::number(colCount+1) %
           QString("}%");
    m_Table.append(line);

    line = QString("    & & \\multicolumn{") %
           QString::number(colCount-1) %
           QString("}{c|}{") %
           QString("Column Caption") %
           QString("} \\\\");
    m_Table.append(line);

    line = QString("    \\cline{3-") %
           QString::number(colCount+1) %
           QString("}%");
    m_Table.append(line);

    cmat.getItem(0,0).toString();
    line = QString("    &");
    for (int c = 1; c < colCount; ++c)
        line += QString(" & ") % cmat.getItem(0,c).toString();
    line += QString("\\\\");
    m_Table.append(line);

    line = QString("    \\cline{1-") %
           QString::number(colCount+1) %
           QString("}%");
    m_Table.append(line);

    line = QString("    \\multicolumn{1}{|c}{\\multirow{") %
           QString::number(colCount) %
           QString("}{*}{\\begin{sideways}") %
           QString("Row Caption") %
           QString("\\end{sideways}}} & \\multicolumn{1}{|c|}{") %
           QString(cmat.getItem(1, 0).toString()) %
           QString("}");
    for (int c = 1; c < colCount; ++c)
        line = line % QString(" & ") % cmat.getItem(1, c).toString();
    line = line % QString("\\\\");
    m_Table.append(line);

    for (int r = 2; r < rowCount; ++r)
    {
        line = QString("    \\cline{2-") %
               QString::number(colCount+1) %
               QString("}%");
        m_Table.append(line);

        line = QString("    \\multicolumn{1}{|c}{} & \\multicolumn{1}{|c|}{") %
               cmat.getItem(r, 0).toString() %
               QString("}");
        for (int c = 1; c < colCount; ++c)
            line += QString(" & ") % cmat.getItem(r, c).toString();

        line += QString("\\\\");
        m_Table.append(line);
    }

    m_Table.append(QString("    \\cline{1-") %
                       QString::number(colCount+1) %
                       QString("}%") );
    m_Table.append(QString("    \\end{tabular}%"));
    m_Table.append(QString("    \\caption[Short Caption]{Long Caption}%"));
    m_Table.append(QString("    \\label{tab:Label}"));
    m_Table.append(QString("\\end{table}"));
}


void TexCode::generatePreamble()
{
    m_Preamble.clear();
    m_Preamble.append(QString("\\usepackage{multirow}"));
    m_Preamble.append(QString("\\usepackage{rotating}"));
}


QStringList TexCode::getPreamble() const
{
    return m_Preamble;
}


QStringList TexCode::getTable() const
{
    return m_Table;
}


QStringList TexCode::getDocument(QString documentoptions,
                                 QString documentclass) const
{
    QStringList document;

    document.clear();
    document.append(QString("\\documentclass[" +
                             documentoptions +
                            "]{" +
                            documentclass +
                            "}"));
    document.append(QString(""));
    document.append(QString("\\usepackage[latin1]{inputenc}"));
    document.append(m_Preamble);
    document.append(QString(""));
    document.append(QString("\\begin{document}"));
    document.append(QString(""));
    document.append(m_Table);
    document.append(QString(""));
    document.append(QString("\\end{document}"));

    return document;
}

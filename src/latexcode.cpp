#include "latexcode.h"

LatexCode::LatexCode(const ConfMat &cmat,
                               QObject *parent):
    QObject(parent)
{
    m_latexCode = new QStringList();
    this->generate(cmat);
}


LatexCode::~LatexCode()
{
    delete m_latexCode;
}


int LatexCode::getMatLineCount() const
{
    return m_latexCode->size();
}


QString LatexCode::getMatLine(int line) const
{
    return m_latexCode->at(line);
}


void LatexCode::generate(const ConfMat &cmat) const
{
    QString line;
    int colCount = cmat.getColCount();
    int rowCount = cmat.getRowCount();

    m_latexCode->clear();
    m_latexCode->append(QString("\\begin{table}[H]%\n"));
    m_latexCode->append(QString("    \\centering\n"));

    line = QString("    \\begin{tabular}{|cc|c|");
    for (int c = 0; c < colCount; ++c)
        line += QString("c|");
    line += QString("}%\n");
    m_latexCode->append(line);

    line = QString("    \\cline{1-") %
           QString::number(colCount+2) %
           QString("}%\n");
    m_latexCode->append(line);

    line = QString("    & & \\multicolumn{") %
           QString::number(colCount) %
           QString("}{c|}{") %
           QString("Column Caption") %
           QString("} \\\\\n");
    m_latexCode->append(line);

    line = QString("    \\cline{3-") %
           QString::number(colCount+2) %
           QString("}%\n");
    m_latexCode->append(line);

    cmat.getItem(0,0).toString();
    line = QString("    &");
    for (int c = 1; c < colCount; ++c)
        line += QString(" & ") % cmat.getItem(0,c).toString();
    line += QString("\\\\\n");
    m_latexCode->append(line);

    line = QString("    \\cline{1-") %
           QString::number(colCount+2) %
           QString("}%\n");
    m_latexCode->append(line);

    line = QString("    \\multicolumn{1}{|c}{\\multirow{7}{*}{\\begin{sideways}") %
           QString("Row Caption") %
           QString("\\end{sideways}}} & \\multicolumn{1}{|c|}{") %
           QString(cmat.getItem(1, 0).toString()) %
           QString("}");
    for (int c = 1; c < colCount; ++c)
        line = line % QString(" & ") % cmat.getItem(1, c).toString();
    line = line % QString("\\\\\n");
    m_latexCode->append(line);

    for (int r = 2; r < rowCount; ++r)
    {
        line = QString("    \\cline{2-") %
               QString::number(colCount+2) %
               QString("}%\n");
        m_latexCode->append(line);

        line = QString("    \\multicolumn{1}{|c}{} & \\multicolumn{1}{|c|}{") %
               cmat.getItem(r, 0).toString() %
               QString("}");
        for (int c = 1; c < colCount; ++c)
            line += QString(" & ") % cmat.getItem(r, c).toString();

        line += QString("\\\\\n");
        m_latexCode->append(line);
    }

    m_latexCode->append(QString("    \\cline{1-") %
                       QString::number(colCount+2) %
                       QString("}%\n") );
    m_latexCode->append(QString("    \\end{tabular}%\n"));
    m_latexCode->append(QString("    \\caption[Short Caption]{Long Caption}%\n"));
    m_latexCode->append(QString("    \\label{tab:Label}\n"));
    m_latexCode->append(QString("\\end{table}\n"));
}

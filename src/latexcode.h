#ifndef LATEXCODE_H
#define LATEXCODE_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QStringBuilder>
#include "confmat.h"

//! @brief Generate
//!
//!
//!
//! @author Attila Györkös
//!
class LatexCode : public QObject
{
    Q_OBJECT
public:
    explicit LatexCode(const ConfMat& cmat,
                       QObject* parent = 0);
    ~LatexCode();

    int getMatLineCount() const;
    QString getMatLine(int line) const;


private:
    //! @brief Set the size of the matrix
    QStringList* m_latexCode;

    void generate(const ConfMat& cmat) const;

signals:
    
public slots:
    
};

#endif // LATEXCODE_H

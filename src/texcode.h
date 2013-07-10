#ifndef TEXCODE_H
#define TEXCODE_H

#include <QObject>
#include <QVariant>
#include "confmat.h"

class QStringList;

//! @brief Generate
//!
//!
//!
//! @author Attila Györkös
//!
class TexCode : public QObject
{
    Q_OBJECT
public:
    explicit TexCode(const ConfMat& cmat,
                     QObject* parent = 0);
    ~TexCode();


    QStringList getPreamble() const;
    QStringList getTable() const;
    QStringList getDocument(QString documentoptions = "a4paper",
                            QString documentclass = "article") const;

private:

    QStringList m_Table;
    QStringList m_Preamble;

    void generateTable(const ConfMat& cmat);
    void generatePreamble();

signals:

public slots:
    
};

#endif // TEXCODE_H

#ifndef CONFMAT_H
#define CONFMAT_H

#include <QList>
#include <QVariant>
#include <QModelIndex>
#include <QStringList>

class ConfMat
{
public:
    explicit ConfMat(int rowCount = 1, int colCount = 1);
    ConfMat(QList<QList<QVariant> > values);

    QVariant getItem(int row, int col) const;
    bool setItem(int row, int col, QVariant value);
    QStringList toStringList() const;

    int  getColCount() const;
    bool insertCols(int col, int count);
    bool removeCols(int col, int count);
    void setColCount(int newColCount);

    int  getRowCount() const;
    bool insertRows(int row, int count);
    bool removeRows(int row, int count);
    void setRowCount(int newRowCount);

private:
    QList<QList<QVariant> > m_cmat;
    int m_rowCount;
    int m_colCount;

    void init(int rowCount, int colCount);

};

#endif // CONFMAT_H

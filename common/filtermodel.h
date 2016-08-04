#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

namespace Gibrievida {

class FilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool inOperation READ isInOperation NOTIFY inOperationChanged)
    Q_PROPERTY(QString search READ getSearch WRITE setSearch NOTIFY searchChanged)
public:
    explicit FilterModel(QObject *parent = nullptr);
    ~FilterModel();

    bool isInOperation() const;
    QString getSearch() const;

    void setSearch(const QString &search);

public slots:
    void setInOperation(bool inOperation);

signals:
    void inOperationChanged(bool inOperation);
    void searchChanged(const QString &search);

private:
    bool m_inOperation;
    QString m_search;

    Q_DISABLE_COPY(FilterModel)
};

}

#endif // FILTERMODEL_H

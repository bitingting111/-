#ifndef PATIENTMODEL_H
#define PATIENTMODEL_H

#include <QSqlQueryModel>

class PatientModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    PatientModel(QObject *parent = nullptr,const QSqlDatabase& db = QSqlDatabase());

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setName(int id, QString name);
    bool setSex(int id, QString sex);
    bool setAge(int id, int age);

private:
    QSqlDatabase db_;

};

#endif // PATIENTMODEL_H


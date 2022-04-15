#ifndef MODELS_H
#define MODELS_H
#include<QString>

class DataItem
{
public:
    DataItem();
    DataItem(QString n, QString p, bool c, QString d, QString i, QString pa);

public:
    QString StrName;
    QString StrPath;
    QString StrDesc;
    QString StrIcon;
    QString StrParant;
    bool Check;
};

#endif // MODELS_H

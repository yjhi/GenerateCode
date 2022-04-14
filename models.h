#ifndef MODELS_H
#define MODELS_H
#include<QString>

class DataItem
{
public:
    DataItem();
    DataItem(QString n,QString p,QString d,QString i);

public:
    QString StrName;
    QString StrPath;
    QString StrDesc;
    QString StrIcon;
};

#endif // MODELS_H

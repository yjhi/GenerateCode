#include "models.h"

DataItem::DataItem()
{
    StrName="";
    StrPath="";
    StrDesc="";
    StrIcon="";
}

DataItem::DataItem(QString n, QString p, QString d, QString i)
{
    StrName=n;
    StrPath=p;
    StrDesc=d;
    StrIcon=i;
}

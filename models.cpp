#include "models.h"

DataItem::DataItem()
{
    StrName="";
    StrPath="";
    StrDesc="";
    StrIcon="";
    Check=false;
}

DataItem::DataItem(QString n, QString p, bool c, QString d, QString i,QString pa)
{
    StrName=n;
    StrPath=p;
    StrDesc=d;
    StrIcon=i;
    Check=c;
    StrParant=pa;
}

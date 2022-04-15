#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSplitter>
#include<QMessageBox>
#include<QList>
#include<QDir>
#include <QTextStream>
#include<QFile>
#include<QStringList>
#include<QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setStyleSheet("QScrollBar:vertical {\
                        width: 15px;\
                        background: #DDD;\
                        padding-bottom: 8px;\
                    }\
                    \
                    QScrollBar:horizontal {\
                        height: 15px;\
                        background: #DDD;\
                    }\
                    QScrollBar::handle:vertical,\
                    QScrollBar::handle:horizontal {\
                        background: #AAA;\
                    }\
                    QScrollBar::handle:vertical:hover,\
                    QScrollBar::handle:horizontal:hover {\
                        background: #888;\
                    }\
                    \
                    QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical,\
                    QScrollBar::sub-line:horizontal, QScrollBar::add-line:horizontal {\
                        width: 0;\
                        height: 0;\
                    }");
    initUI();

    showMsg("加载成功");

}


void MainWindow::AddRow()
{
    int rowcount=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowcount);
    ui->tableWidget->setCellWidget(rowcount,2,new QCheckBox());
    showMsg("添加行成功");

}

void MainWindow::DelRow(int t)
{

    if(t==0)
    {
        if(selRow>-1)
        {
            ui->tableWidget->removeRow(selRow);

        }
    }else{

        if(ui->tableWidget->currentIndex().isValid())
        {
             ui->tableWidget->removeRow(ui->tableWidget->currentIndex().row());
        }
    }

    showMsg("删除行成功");


}

void MainWindow::DelAllRow()
{

    while(ui->tableWidget->rowCount()>0)
    {
        ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
    }

    showMsg("清空行成功");

}
#include<QCloseEvent>
void MainWindow::closeEvent(QCloseEvent *event)
{
    SaveFile();


    if(QMessageBox::Yes==QMessageBox::question(this,"退出","数据已保存,正在退出,是否继续?",QMessageBox::Yes,QMessageBox::No))
    {

        event->accept();
    }else{
        event->ignore();
    }
}

void MainWindow::showMsg(QString msg)
{
    ui->statusBar->showMessage(msg);
}

void MainWindow::initUI()
{


    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        ui->tableWidget->setCellWidget(i,2,new QCheckBox());
    }


    this->centralWidget()->setLayout(ui->gridLayout);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"菜单名称*"<<"路径*"<<"是否编辑"<<"描述"<<"图标"<<"父级菜单");


    connect(ui->quit,&QAction::triggered,this,[&](){

          SaveFile();

          exit(0);

    });




    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);



    connect(ui->tableWidget,&QTableWidget::customContextMenuRequested,this,&MainWindow::slotRightContext);


    rightMenu=new QMenu(this);


    rightMenu->addAction(QIcon(":/png/add.png"),"添加行",[&](){

        AddRow();

    });


    rightMenu->addAction(QIcon(":/png/del.png"),"删除行",[&](){

        DelRow();
    });

    rightMenu->addSeparator();


    rightMenu->addAction(QIcon(":/png/run.png"),"生成",[&](){

        DoMakeCode();

    });

     ui->mainToolBar->addAction(QIcon(":/png/add.png"),"添加行",[&](){

         AddRow();

     });


      ui->mainToolBar->addAction(QIcon(":/png/del.png"),"删除行",[&](){

         DelRow(1);
     });

      ui->mainToolBar->addSeparator();


      ui->mainToolBar->addAction(QIcon(":/png/run.png"),"生成",[&](){

         DoMakeCode();

     });


      ui->mainToolBar->addAction(QIcon(":/png/delete.png"),"清空",[&](){

          if(QMessageBox::Yes==QMessageBox::question(this,"清空","此操作将清空表格以及生成结果,是否继续?",QMessageBox::Yes,QMessageBox::No))
          {

            while(ui->tableWidget->rowCount()>0)
            {
                ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
            }

            ui->txtMenu->setText("");
            ui->txtCode->setText("");
            ui->txtZh->setText("");
          }

      });



      ReadFile();

}


void MainWindow::ReadFile()
{
    QString path=QDir::currentPath();

    QString state=path+"/state.dat";
    QString data=path+"/data.dat";

    {
        QFile f(state);

        if(f.open(QFile::ReadOnly))
        {

            QRect rect;
            QTextStream stream(&f);

            int x;
            int y;
            int w;
            int h;

            stream>>x>>y>>w>>h;

            if(rect.isValid()){
                this->setGeometry(x,y,w,h);
            }

            f.close();
        }
    }


    {
               QFile f(data);


               if(f.open(QFile::ReadOnly))
               {

                   QStringList list;

                   QTextStream stream(&f);


                   while(!stream.atEnd())
                   {
                       QString strline=stream.readLine();

                       QString strRaw=QString(QByteArray::fromBase64(strline.toUtf8()));

                       if(strRaw.trimmed().length()==0) continue;

                        list.append(strRaw);

                   }
                    f.close();


                    if(list.count()>0)
                    {
                        DelAllRow();
                    }




                    for(int i=0;i<list.count();i++)
                    {

                        QString strRaw= list[i];

                        QStringList ll=strRaw.split(";");

                        if(ll.count()==0)continue;


                        ui->tableWidget->insertRow(ui->tableWidget->rowCount());

                        for(int j=0;j<ll.count();j++)
                        {

                            if(j==2)
                            {
                                QCheckBox *ck=new QCheckBox();
                                if(QString(ll[j]).length()>0)
                                {
                                    if(QString(ll[j])=="1")
                                    {

                                        ck->setChecked(true);

                                    }else{
                                        ck->setChecked(false);

                                    }
                                }
                                 ui->tableWidget->setCellWidget(i,j,ck);
                            }else{
                                if(QString(ll[j]).length()>0)
                                {
                                    ui->tableWidget->setItem(i,j,new QTableWidgetItem(ll[j]));

                                }
                            }


                        }
                    }

               }

           }



}

void MainWindow::SaveFile()
{
       QString path=QDir::currentPath();

       QString state=path+"/state.dat";
       QString data=path+"/data.dat";


       {
           QFile f(state);

           if(f.open(QFile::WriteOnly))
           {
               QTextStream stream(&f);

               stream<<this->geometry().x()<<"\n"<<this->geometry().y()<<"\n"<<this->geometry().width()<<"\n"<<this->geometry().height()<<"\n";

           }

           f.close();
       }


       {

           int rows=ui->tableWidget->rowCount();
           int cols=ui->tableWidget->columnCount();


           if(rows<1)return;


           QFile f(data);

           if(f.open(QFile::WriteOnly))
           {
               QTextStream stream(&f);

               for(int i=0;i<rows;i++)
               {

                   QStringList list;
                   for(int j=0;j<cols;j++)
                   {
                       if(j==2)
                       {
                             QCheckBox*ck= (QCheckBox*)(ui->tableWidget->cellWidget(i,j));
                              if(ck!=nullptr)
                              {
                                  if(ck->isChecked())
                                  {
                                      list.append("1");
                                  }else{
                                      list.append("0");
                                  }
                              }else{
                                   list.append("0");
                              }

                       }else{
                           QTableWidgetItem *item= ui->tableWidget->item(i,j);

                           if(item&&item->text().length()>0)
                           {
                               list.append(item->text());
                           }else{
                               if(j==0)
                               {
                                    break;
                               }

                           }
                       }


                   }

                    if(list.count()>0)
                    {
                       QString strjoin=list.join(";");

                       QString strLine=QString::fromUtf8(strjoin.toUtf8().toBase64());

                       stream<<strLine<<"\n";
                    }


               }
                f.close();
           }


       }



}

void MainWindow::DoMakeCode()
{
    int rows=ui->tableWidget->rowCount();


    if(rows<1)return;

    int cols=ui->tableWidget->columnCount();


    QString strErr="";

    for(int i=0;i<rows;i++)
    {

        for(int j=0;j<cols;j++)
        {

            if(j==0||j==1)
            {
                QTableWidgetItem *item= ui->tableWidget->item(i,j);

                if(item&&item->text().length()>0)
                {

                }else{
                      strErr+=QString("第%1行%2列数据无效\n").arg(i+1).arg(j+1);
                }
            }else if(j==2){



            }else if(j==3){
                QTableWidgetItem *item= ui->tableWidget->item(i,j);
                QTableWidgetItem *item0= ui->tableWidget->item(i,0);

                if(item&&item->text().length()>0)
                {

                }else{
                    if(item0&&item0->text().length()>0)
                    {
                        ui->tableWidget->setItem(i,j,new QTableWidgetItem(item0->text()));
                    }

                }
            }else if(j==4){
                QTableWidgetItem *item= ui->tableWidget->item(i,j);

                if(item&&item->text().length()>0)
                {

                }else{

                      ui->tableWidget->setItem(i,j,new QTableWidgetItem("iconzhongduanguanli"));
                }
            }else if(j==5)
            {
                QTableWidgetItem *item= ui->tableWidget->item(i,j);

                if(item&&item->text().length()>0)
                {

                }else{

                      ui->tableWidget->setItem(i,j,new QTableWidgetItem("info_basic"));
                }
            }
        }
    }


    if(strErr.length()>0)
    {
        QMessageBox::warning(this,"数据校验失败请修改",strErr);
        return ;
    }


    QList<DataItem> datas;

    for(int i=0;i<rows;i++)
    {

            QTableWidgetItem *item0= ui->tableWidget->item(i,0);
            QTableWidgetItem *item1= ui->tableWidget->item(i,1);
            QCheckBox *item2=(QCheckBox*)ui->tableWidget->cellWidget(i,2);

            QTableWidgetItem *item3= ui->tableWidget->item(i,3);
            QTableWidgetItem *item4= ui->tableWidget->item(i,4);
            QTableWidgetItem *item5= ui->tableWidget->item(i,5);

            datas.append(DataItem(item0->text(),item1->text(),item2==nullptr?false:item2->isChecked(),item3->text(),item4->text(),item5->text()));

    }



    if(datas.count()==0) return;



    ui->txtMenu->setText(strMenuTmpl(datas));
    ui->txtCode->setText(strCodeTmpl(datas));
    ui->txtZh->setText(strZhTmpl(datas));


    showMsg("生成成功");

//    QString strCount=QString("%1").arg(rows);

//    QMessageBox::information(this,"T",strCount);

}


QString MainWindow::strMenuTmpl(QList<DataItem> &datas)
{
    QString str="";

    for(int i=0;i<datas.count();i++)
    {
             DataItem d=datas[i];


            str+="        <!--"+d.StrName+"-->\n";
            str+="        <subItems>\n";


            str+="          <menu name=\"info_"+d.StrPath+"Show\" displayName=\"info_"+d.StrPath+"Show\" icon=\""+d.StrIcon+"\" url=\""+d.StrPath+"\"   requiredPermissionName=\"info_"+d.StrPath+"Show\" description=\""+d.StrDesc+"\">\n";

            str+="          </menu>\n";
            str+="        </subItems>\n";

    }

    return str;
}


QString MainWindow::strZhTmpl(QList<DataItem> &datas)
{

    QString str="";

    for(int i=0;i<datas.count();i++)
    {
         DataItem d=datas[i];
         str+="    <text name=\"info_"+d.StrPath+"Show\" value=\""+d.StrName+"\"></text>\n";

    }

    return str;
}

QString MainWindow::strCodeTmpl(QList<DataItem> &datas)
{

    QString  str="";

    str+="            \n";
    for(int i=0;i<datas.count();i++)
    {

        DataItem d=datas[i];
        str+="            //"+d.StrName+"\n";
        str+="            var info_"+d.StrPath+"List = "+d.StrParant+".CreateChildPermission(\"info_"+d.StrPath+"_parent\", L(\"info_"+d.StrPath+"Show\"));//"+d.StrName+"\n";
        str+="            info_"+d.StrPath+"List.CreateChildPermission(\"info_"+d.StrPath+"Show\", L(\"info_"+d.StrPath+"Show\"), properties: Show);//"+d.StrName+"\n";

        if(d.Check)
        {
            str+="            info_"+d.StrPath+"List.CreateChildPermission(\"info_"+d.StrPath+"Edit\", L(\"info_"+d.StrPath+"Show\"), properties: Edit);//"+d.StrName+"\n";
        }
    }

    return str;

}

void MainWindow::slotRightContext(const QPoint &pos){

    QModelIndex inx=ui->tableWidget->indexAt(pos);
    selRow=-1;
    if(inx.isValid()){
        selRow=inx.row();
    }

    rightMenu->exec(this->mapToGlobal(pos));


}

MainWindow::~MainWindow()
{
    delete ui;
}

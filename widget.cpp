#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //Real time display time
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

/*********************************************************************************************
* sun 20200613
* By sunguiyu96@gmail.com
* Display webpage and update TLE
* input：
* output：
* progress：
* 2020/06/13 sun 18:33:Currently, web pages can be displayed and tle can be saved；
* 2020/06/13 sun 18:55:Optimized information output；
**********************************************************************************************/
void Widget::on_Iridium_clicked()
{
    QString path = "https://www.celestrak.com/NORAD/elements/table.php?tleFile=iridium&title=Iridium%20Satellites&orbits=40&pointsPerRev=90&frame=1";
    QString pathIridium = "https://www.celestrak.com/NORAD/elements/iridium.txt";

    //Read current content of text from web
    QString Currunt_textEdit = ui->textEdit->toPlainText();
    QString text = "Loading https://www.celestrak.com/NORAD/elements/table.php?tleFile=iridium ...";
    Currunt_textEdit += text;
    ui->textEdit->setText(Currunt_textEdit);

    QUrl url(path);
    QUrl urlIridium(pathIridium);

    ui->TLEVIew->load(url);
    ui->TLEVIew->show();

    //The following is the source code of the saved web page
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply;

    qDebug() << "Reading code from " << pathIridium;

    reply = manager.get(QNetworkRequest(urlIridium));
    //After the request is completed and the download is completed, exit the sub event loop
    QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    //Open sub event cycle
    loop.exec();

    //write file
    QString currentpath = "C:/BIOS'sGithub/TLEUpdateTool_QT/TLEDownload/";
    QString FILE_NAME = currentpath + "Iridium.tle";
    QFile file(FILE_NAME);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        qDebug() << "Cannot open the file: " << FILE_NAME;
    }
    QTextStream out(&file);
    QString codeContent = reply->readAll();

    //qDebug() << FILE_NAME << endl;

    out << codeContent;

    //update display
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_dt =current_date_time.toString("\nyyyy.MM.dd hh:mm:ss.zzz");
    text = current_dt + ": Iridium.tle has updated successfully!\n\n";
    Currunt_textEdit += text;
    ui->textEdit->setText(Currunt_textEdit);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);

    file.close();

}



/*********************************************************************************************
* sun 20200613
* By sunguiyu96@gmail.com
* Display webpage and update TLE
* input：
* output：
* progress：
* 2020/06/13 sun 18:36:Currently, web pages can be displayed and tle can be saved；
* 2020/06/13 sun 18:55:Optimized information output；
**********************************************************************************************/
void Widget::on_IridiumNext_clicked()
{
    QString path = "https://www.celestrak.com/NORAD/elements/table.php?tleFile=iridium-NEXT&title=Iridium%20NEXT%20Satellites&orbits=0&pointsPerRev=90&frame=1";
    QString pathIridium = "https://www.celestrak.com/NORAD/elements/iridium-NEXT.txt";

    //Read current content of text from web
    QString Currunt_textEdit = ui->textEdit->toPlainText();
    QString text = "Loading https://www.celestrak.com/NORAD/elements/table.php?tleFile=iridium-NEXT ...";
    Currunt_textEdit += text;
    ui->textEdit->setText(Currunt_textEdit);

    QUrl url(path);
    QUrl urlIridium(pathIridium);

    ui->TLEVIew->load(url);
    ui->TLEVIew->show();

    //The following is the source code of the saved web page
    QNetworkAccessManager manager;
    QEventLoop loop;
    QNetworkReply *reply;

    qDebug() << "Reading code from " << pathIridium;

    reply = manager.get(QNetworkRequest(urlIridium));
    //After the request is completed and the download is completed, exit the sub event loop
    QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    //Open sub event cycle
    loop.exec();

    //write file
    QString currentpath = "C:/BIOS'sGithub/TLEUpdateTool_QT/TLEDownload/";
    QString FILE_NAME = currentpath + "IridiumNEXT.tle";
    QFile file(FILE_NAME);

    if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        qDebug() << "Cannot open the file: " << FILE_NAME;
    }
    QTextStream out(&file);
    QString codeContent = reply->readAll();

    //qDebug() << FILE_NAME << endl;
    out << codeContent;

    //update display
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_dt =current_date_time.toString("\nyyyy.MM.dd hh:mm:ss");
    text = current_dt + ": IridiumNEXT.tle has updated successfully!\n\n";
    Currunt_textEdit += text;
    ui->textEdit->setText(Currunt_textEdit);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);

    file.close();
}

/*********************************************************************************************
* sun 20200613
* By sunguiyu96@gmail.com
* Update to file xxxxxxxx.tle
* input：
* output：
* progress：
* 2020/06/13 sun 18:59:start；
* 2020/06/13 sun 19:55:end；
* 2020/06/13 sun 20:17:Basically, but two blank lines occured in the stream output for unknown reasons;
* 2020/06/13 sun 22:29:Two line breaks can be deleted directly;
* 2020/06/13 sun 23:46:\r\n are considered newline characters, need to be deleted;
* 2020/06/14 sun 01:34:Basically achieve the desired effect;
**********************************************************************************************/
void Widget::on_UpdateTLE_clicked()
{
    ui->Iridium->setEnabled(false);
    ui->IridiumNext->setEnabled(false);
    ui->UpdateTLE->setEnabled(false);

    QString currentpath = "C:/BIOS'sGithub/TLEUpdateTool_QT/TLEDownload/";
    QString IriTLE = currentpath + "Iridium.tle";
    QString NextTLE = currentpath + "IridiumNEXT.tle";

    QFileInfo fileI(IriTLE);
    QFileInfo fileN(NextTLE);
    if((fileI.exists() == false) ||(fileN.exists() == false))
    {
        ui->textEdit_2->setText("Please check if Iridium/IridiumNEXT.tle exists in " + currentpath);
    }
    else
    {
        QFile fileIri(IriTLE);
        bool isok = fileIri.open(QIODevice::ReadOnly);//Open file read-only
        QString Current_textEdit = ui->textEdit->toPlainText();
        QString Current_textEdit_2 = ui->textEdit_2->toPlainText();
        QByteArray array;
        QByteArray line;
        QString TLE;
        while(fileIri.atEnd() == false)
        {
            line = fileIri.readLine();      //Reading line
            array += line;      //the file in the byte array with the end as \r\r\n, change three lines
            line.remove(line.length() - 3,3);       //Delete line break at the end of"\r\r\n"
//            int length = line.length();
//            qDebug() << length;
//            if(length > 4)
//            {
//                qDebug("%d %d %d %d\n",  line[length - 1], line[length - 2] , line[length - 3] , line[length - 4]);
//            }
            QString locline;
            locline.prepend(line);
//            if(length > 4)
//            {
//                qDebug("%d %d %d %d\n",  locline[length - 1] == "\n", locline[length - 2] == "\r", locline[length - 3] == "\r", locline[length - 4]== "7");
//            }
            //Read by format
            if((locline[0]=="I" && locline[1]=="R" && locline[2]=="I") || (locline[0]=="D" && locline[1]=="U" && locline[2]=="M"))
            {
                Current_textEdit_2 = Current_textEdit_2 + line;

            }
            else if((locline[0] == "1") || (locline[0] == "2"))
            {
                QString str(line);
                TLE = TLE + str + "\n";

                if(locline[0] == "1")
                {
                    QString SatNum = line.mid(2,5);
                    QString TLE_DT = CalculateTimeVSTLE(line);

                    //set information display
                    Current_textEdit_2 = Current_textEdit_2 + "   " + SatNum + "    " + TLE_DT + "   OK!\n";
                    ui->textEdit_2->setText(Current_textEdit_2);
                    QTextCursor cursor = ui->textEdit_2->textCursor();
                    cursor.movePosition(QTextCursor::End);
                    ui->textEdit_2->setTextCursor(cursor);
                }
            }
        }
        //Current_textEdit += array;
        fileIri.close();

        QFile fileNext(NextTLE);
        isok = fileNext.open(QIODevice::ReadOnly);//Open file read-only

        while(fileNext.atEnd() == false)
        {
            line = fileNext.readLine();      //Reading line
            array += line;         //the file in the byte array with the end as \r\r\n, change three lines
            line.remove(line.length() - 3,3);       //Delete line break at the end of"\r\r\n"
            QString locline;
            locline.prepend(line);
            //Read by format
            if(locline[0]=="I" && locline[1]=="R" && locline[2]=="I")
            {
                Current_textEdit_2 = Current_textEdit_2 + line;

            }
            else if((locline[0] == "1") || (locline[0] == "2"))
            {
                QString str(line);
                TLE = TLE + str + "\n";

                if(locline[0] == "1")
                {
                    QString SatNum = line.mid(2,5);
                    QString TLE_DT = CalculateTimeVSTLE(line);

                    //set information display
                    Current_textEdit_2 = Current_textEdit_2 + "   " + SatNum + "    " + TLE_DT + "   OK!\n";
                    ui->textEdit_2->setText(Current_textEdit_2);
                    QTextCursor cursor = ui->textEdit_2->textCursor();
                    cursor.movePosition(QTextCursor::End);
                    ui->textEdit_2->setTextCursor(cursor);
                }
            }
        }
        //ui->textEdit->setText(Current_textEdit + array);
        //Move cursor to end
        QTextCursor cursor = ui->textEdit->textCursor();
//        cursor.movePosition(QTextCursor::End);
//        ui->textEdit->setTextCursor(cursor);

        fileNext.close();

        //Write to required xxxxxxxx.TLE
        //update display
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyyMMdd");
        QString outfileName = currentpath + current_date + ".tle";

        QFile file(outfileName);
        if( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            qDebug() << "Cannot open the file: " << outfileName;
        }
        QTextStream out(&file);

        out << TLE;

        Current_textEdit = ui->textEdit->toPlainText();
        QString current_dt =current_date_time.toString("\nyyyy.MM.dd hh:mm:ss");
        ui->textEdit->setText(Current_textEdit + current_dt + ": " +current_date + ".tle has updated successfully!\n\n");
        //Move cursor to end
        cursor.movePosition(QTextCursor::End);
        ui->textEdit->setTextCursor(cursor);
    }

    ui->Iridium->setEnabled(true);
    ui->IridiumNext->setEnabled(true);
    ui->UpdateTLE->setEnabled(true);

}

/*********************************************************************************************
* sun 20200614
* By sunguiyu96@gmail.com
* Calculate the time of collecting data through TLE
* input：fisrt line of TLE
* output：date and time
* progress：
* 2020/06/14 sun 00:08:star;
* 2020/06/14 sun 01:32:Achieve the desired effect;
**********************************************************************************************/
QString Widget::CalculateTimeVSTLE(QString line)
{
    QString TLE_DT;

    QString year = line.mid(18,2);
    QString day = line.mid(20,12);
    int TLEyear = year.toInt();
    double TLEday = day.toDouble();
    int UTCyear = 0;
    double UTCday = 0.0;
    if(TLEyear < 57)
    {
        UTCyear = TLEyear + 2000;
        UTCday = TLEday;
    }
    else
    {
        UTCyear = TLEyear + 1900;
        UTCday = TLEday;
    }
    //Convert UTC based on TLE days to true UTC
    DateTime UTC = days2ymdhms(UTCyear,UTCday);
    DateTime BJT = UTC2BJT(UTC);

    //Convert date format
    QString DT_s,year_s,month_s,day_s,hour_s,minute_s,second_s;      //prepare for conversion to time format
    //year
    if(BJT.Y >= 1000 && BJT.Y <= 9999)
    {
        year_s = QString::number(BJT.Y);        //the year of consideration must be four
    }
    else
    {
        ui ->textEdit_2 ->setText("Year Error");  //report errors
    }
    //month
    if(BJT.M < 10 && BJT.M > 0)
    {
        month_s = QString::number(BJT.M);
        month_s = "0" + month_s;//complement 0
    }
    else if(BJT.M < 13 && BJT.M > 9)
    {
        month_s = QString::number(BJT.M);
    }
    else
    {
        ui ->textEdit_2 ->setText("Month Error");  //report errors
    }
    //day
    if(BJT.D < 10 && BJT.D > 0)
    {
        day_s = QString::number(BJT.D);
        day_s = "0" + day_s;//complement 0
    }
    else if(BJT.D < 32 && BJT.D > 9)
    {
        day_s = QString::number(BJT.D);
    }
    else
    {
        ui ->textEdit_2 ->setText("Day Error");  //report errors
    }
    //hour
    if(BJT.h < 10 && BJT.h >= 0)
    {
        hour_s = QString::number(BJT.h);
        hour_s = "0" + hour_s;//complement 0
    }
    else if(BJT.h < 25 && BJT.h > 9)
    {
        hour_s = QString::number(BJT.h);
    }
    else
    {
         ui ->textEdit_2 ->setText("Hour Error");  //report errors
    }
    //minute
    if(BJT.m < 10 && BJT.m >= 0)
    {
        minute_s = QString::number(BJT.m);
        minute_s = "0" + minute_s;//complement 0
    }
    else if(BJT.m < 61 && BJT.m > 9)
    {
        minute_s = QString::number(BJT.m);
    }
    else
    {
        ui ->textEdit_2 ->setText("Minute Error");  //report errors
    }
    //second
    int second = (int)BJT.s;
    if(second < 10 && second >= 0)
    {
        second_s = QString::number(second);
        second_s = "0" + second_s;//complement 0
    }
    else if(second <= 60 && second >= 10)
    {
        second_s = QString::number(second);
    }
    else
    {
        ui ->textEdit_2 ->setText("Second Error");  //report errors
    }

    TLE_DT = year_s + "/" + month_s + "/" + day_s + " " + hour_s + ":" + minute_s + ":" + second_s;

    return TLE_DT;
}

/*********************************************************************************************
* sun 20200614
* By sunguiyu96@gmail.com
* Launch time from year and days
* input：year , days
* output：date and time
* progress：
* 2020/06/14 sun 00:25:start;
* 2020/06/14 sun 00:43:end;
**********************************************************************************************/
DateTime Widget::days2ymdhms(int y,double d)
{
    DateTime dt;
    dt.Y = y;
    //set month
    int l_M[13] = {0};
    for(int i = 1;i < 12;i++)
    {
        l_M[i] = 31;
        if(i == 2)
        {
            l_M[i] = 28;
        }
        else if(i == 4 || i == 6 || i == 9 || i == 11)
        {
            l_M[i] = 30;
        }
    }
    int day = floor(d);

    //find month and day
    if((y - 1900) % 4 == 0)
    {
        l_M[2] = 29;
    }

    int i = 1;
    int temp = 0;
    while((day > temp + l_M[i]) && (i < 12))
    {
        temp += l_M[i];
        i++;
    }

    dt.M = i;
    dt.D = day - temp;

    double time  = (d - day) * 24.0;
    dt.h = fix(time);
    time = (time - double(dt.h)) * 60.0;
    dt.m = fix(time);
    dt.s = (time - double(dt.m)) * 60.0;

    return dt;
}

/*********************************************************************************************
* sun 20190819/20200614
* By sunguiyu96@gmail.com
* Function：round to 0
* input：double
* output：int
* other：copy from C
* 2019/8/20 sun 16:41：completed;
**********************************************************************************************/
int Widget::fix(double a)
{
    int b = 0;
    if (a > 0)
    {
        b = floor(a);
    }
    if (a == 0.0)
    {
        b = 0;
    }
    if (a < 0)
    {
        a = -a;
        b = floor(a);
        b = -b;
    }

    return b;
}

/*********************************************************************************************
* sun 20200614
* By sunguiyu96@gmail.com
* UTC转为BJT
* input：
* output：
* progress：
* 2020/06/14 sun 00:27:start;
* 2020/06/14 sun 01:03:end;
**********************************************************************************************/
DateTime Widget::UTC2BJT(DateTime U)
{
    DateTime B;
    B.Y = U.Y;B.M = U.M;B.D = U.D;B.h = U.h;B.m = U.m;B.s = U.s;

    int UTCHourMinute = U.h * 100 + U.m;

    int MIN = 60;
    int HOUR = 24;

    if((UTCHourMinute < 0) || UTCHourMinute > 2359)
    {
        return B;
    }

    int UTCMinute = U.h * MIN + U.m;

    int BJTMinute = UTCMinute + 8 * MIN;
    if(BJTMinute <= 1439)
    {
        B.h = fix(double(BJTMinute/MIN));
        B.m = BJTMinute - B.h * MIN;
    }
    else
    {
        BJTMinute -= HOUR * MIN;
        B.h = fix(double(BJTMinute/MIN));
        B.m = BJTMinute - B.h * MIN;
        B.D++;
    }

    return B;
}

/*********************************************************************************************
* sun 20200102/20200614
* By sunguiyu96@gmail.com
* update current time
* 输入：
* 输出：
* 进程：
* 2020/06/14 sun 01:08:finish;
**********************************************************************************************/
void Widget::timerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss");   //set display format
    ui->dateTimeEdit->setDateTime(time);
}

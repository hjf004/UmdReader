#include<QtGui>
#include"../include/MainWindow.h"
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QLocale locale=QLocale::system();
    QTranslator trans;
    if(locale.language()==QLocale::Chinese)
    {
        trans.load(":/resource/locale/zh_CN.qm");
        app.installTranslator(&trans);
    }
    MainWindow *mainWnd=new MainWindow();
    mainWnd->showMaximized();
    return app.exec();
}

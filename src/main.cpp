#include<QtGui>
#include"../include/MainWindow.h"
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    app.setApplicationName("UmdReader");
    app.setOrganizationName("SMTNT");
#if defined(Q_OS_LINUX)
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    }
#elif defined(Q_OS_WIN32)
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("System"));
    }
#endif
    QLocale locale=QLocale::system();
    QTranslator trans;
    if(locale.language()==QLocale::Chinese)
    {
        trans.load(":/resource/locale/zh_CN.qm");
        app.installTranslator(&trans);
    }
    MainWindow *mainWnd=new MainWindow();
    for(int i=0;i<argc-1;i++)
    {
        mainWnd->loadFile(argv[i+1]);       //open files from command line
    }
    mainWnd->showMaximized();
    return app.exec();
}

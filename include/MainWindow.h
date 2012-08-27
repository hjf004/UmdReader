#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QtGui>
#include"../include/UmdReader.h"
class MainWindow:public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
private:
    void creatWidget();
    void creatAction();
    void creatMenubar();
    void creatToolbar();
    void creatConnection();

    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    enum { MaxRecentFiles = 5 };

    QTabBar *tabBar;
    QStackedWidget *stackedWidget;
    QGroupBox *box;
    QMenuBar *menuBar;
    QToolBar *toolBar;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QAction *openAction;
    QAction *saveAsAction;
    QAction *atrAction;
    QAction *printAction;
    QAction *exitAction;
    QAction *separatorAct;
    QAction *recentFileActs[MaxRecentFiles];
    QAction *showOrHideAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *findAction;
    QAction *findNextAction;
    QAction *copyAction;
    QAction *selectAllAction;
    QAction *toolBarAction;

    bool show;
    QString bookName;
    QString searchString;
    QVector<UmdReader*> tabs;
private slots:
    void openFile();
    void openRecentFile();
    void showAttribute();
    void saveAs();
    void closeTab(int index);
    void loadFile(const QString &);
    void zoomIn();
    void zoomOut();
    void findString();
    void findNext();
    void showOrHideList(bool);
    void print();
    void selectAll();
    void onCurrentChanged(int);
    void copy();
};

#endif // MAINWINDOW_H

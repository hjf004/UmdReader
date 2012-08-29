#include"../include/MainWindow.h"
#include"../include/Additions.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    show=true;
    creatAction();
    creatMenubar();
    creatToolbar();
    creatWidget();
    creatConnection();
    setWindowFilePath(QString());
}

void MainWindow::creatAction()
{
    openAction=new QAction(QIcon(":/resource/pixmap/document_open.png"),
                           QObject::tr("&Open File"),this);
    openAction->setShortcut(QKeySequence::Open);
    saveAsAction=new QAction(QIcon(":/resource/pixmap/filesaveas.png"),
                             QObject::tr("&Save As..."),this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setEnabled(false);
    atrAction=new QAction(QObject::tr("File &Attribute"),this);
    printAction=new QAction(QIcon(":/resource/pixmap/document_print.png"),
                            QObject::tr("&Print"),this);
    printAction->setEnabled(false);
    atrAction->setEnabled(false);
    exitAction=new QAction(QObject::tr("&Quit"),this);
    exitAction->setShortcut(QKeySequence::Quit);

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
    }

    showOrHideAction=new QAction(QObject::tr("SlideBar"),this);
    showOrHideAction->setEnabled(false);
    showOrHideAction->setCheckable(true);
    showOrHideAction->setChecked(false);
    zoomInAction=new QAction(QIcon(":/resource/pixmap/zoom_in.png"),
                             QObject::tr("Zoom In"),this);
    zoomInAction->setEnabled(false);
    zoomOutAction=new QAction(QIcon(":/resource/pixmap/zoom_out.png"),
                              QObject::tr("Zoom Out"),this);
    zoomOutAction->setEnabled(false);
    findAction=new QAction(QIcon(":/resource/pixmap/search.png"),
                           QObject::tr("&Search"),this);
    findAction->setShortcut(QKeySequence::Find);
    findAction->setEnabled(false);
    findNextAction=new QAction(QObject::tr("Search Next"),this);
    findNextAction->setShortcut(QKeySequence::FindNext);
    findNextAction->setEnabled(false);

    selectAllAction=new QAction(QIcon(":/resource/pixmap/select_all.png"),
                                QObject::tr("Select All"),this);
    selectAllAction->setEnabled(false);
    copyAction=new QAction(QIcon(":/resource/pixmap/document_copy.png"),
                           QObject::tr("&Copy"),this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setEnabled(false);

    toolBarAction=new QAction(QIcon(":/resource/pixmap/settings.png"),
                              QObject::tr("&ToolBar"),this);
    toolBarAction->setCheckable(true);
    toolBarAction->setChecked(true);
}
void MainWindow::creatMenubar()
{
    menuBar=new QMenuBar(this);
    fileMenu=new QMenu(QObject::tr("&File"),menuBar);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(atrAction);
    fileMenu->addSeparator();
    fileMenu->addAction(printAction);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    updateRecentFileActions();
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu=new QMenu(QObject::tr("&Edit"),menuBar);
    editMenu->addAction(copyAction);
    editMenu->addAction(selectAllAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(findNextAction);

    viewMenu=new QMenu(QObject::tr("&View"),menuBar);
    viewMenu->addAction(showOrHideAction);
    viewMenu->addAction(toolBarAction);

    helpMenu=new QMenu(QObject::tr("&Help"),menuBar);
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(viewMenu);
    menuBar->addMenu(helpMenu);
    setMenuBar(menuBar);
}

void MainWindow::creatToolbar()
{
    toolBar=addToolBar(QObject::tr("ToolBar"));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAsAction);
    toolBar->addAction(printAction);
    toolBar->addSeparator();
    toolBar->addAction(findAction);
    toolBar->addSeparator();
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);
}

void MainWindow::creatWidget()
{
    box=new QGroupBox(this);
    QVBoxLayout *layout=new QVBoxLayout(box);
    tabBar=new QTabBar(box);
    tabBar->setTabsClosable(true);
    stackedWidget=new QStackedWidget(box);
    layout->addWidget(tabBar);
    layout->addWidget(stackedWidget);
    box->setLayout(layout);
    setCentralWidget(box);
}

void MainWindow::creatConnection()
{
    connect(openAction,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(atrAction,SIGNAL(triggered()),this,SLOT(showAttribute()));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
    connect(tabBar,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(tabBar,SIGNAL(currentChanged(int)),this,SLOT(onCurrentChanged(int)));
    for(int i=0;i<MaxRecentFiles;i++)
    {
        connect(recentFileActs[i],SIGNAL(triggered()),this,SLOT(openRecentFile()));
    }
    connect(zoomInAction,SIGNAL(triggered()),this,SLOT(zoomIn()));
    connect(zoomOutAction,SIGNAL(triggered()),this,SLOT(zoomOut()));
    connect(findAction,SIGNAL(triggered()),this,SLOT(findString()));
    connect(findNextAction,SIGNAL(triggered()),this,SLOT(findNext()));
    connect(showOrHideAction,SIGNAL(toggled(bool)),this,SLOT(showOrHideList(bool)));
    connect(printAction,SIGNAL(triggered()),this,SLOT(print()));
    connect(selectAllAction,SIGNAL(triggered()),this,SLOT(selectAll()));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));
    connect(toolBarAction,SIGNAL(toggled(bool)),toolBar,SLOT(setVisible(bool)));
    connect(toolBar,SIGNAL(visibilityChanged(bool)),toolBarAction,SLOT(setChecked(bool)));
}

void MainWindow::openFile()
{
    QString book=QFileDialog::getOpenFileName(this,QObject::tr("Open a umd book"),QDir::homePath(),QObject::tr("Documents(*.umd)"));
    if(!book.isEmpty())
        loadFile(book);
}

void MainWindow::openRecentFile()
{
    QAction *act=qobject_cast<QAction*>(sender());
    QString book=act->data().toString();
    if(act&&(!book.isEmpty()))
        loadFile(book);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    bookName = fileName;
    setWindowFilePath(bookName);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = QObject::tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::closeTab(int index)
{
    tabBar->removeTab(index);
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->widget(index));
    stackedWidget->removeWidget(qobject_cast<QWidget*>(reader));
    tabs.remove(index);
    delete reader;
    if(stackedWidget->count())
    {
        reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
        QString title("UmdReader--");
        title.append(reader->getTitle());
        setWindowTitle(title);
        saveAsAction->setEnabled(true);
        atrAction->setEnabled(true);
        printAction->setEnabled(true);
        zoomInAction->setEnabled(true);
        zoomOutAction->setEnabled(true);
        findAction->setEnabled(true);
        selectAllAction->setEnabled(true);
        showOrHideAction->setEnabled(true);
    }
    else
    {
        setWindowTitle("UmdReader");
        saveAsAction->setEnabled(false);
        atrAction->setEnabled(false);
        printAction->setEnabled(false);
        zoomInAction->setEnabled(false);
        zoomOutAction->setEnabled(false);
        findAction->setEnabled(false);
        selectAllAction->setEnabled(false);
        showOrHideAction->setEnabled(false);
    }
}

void MainWindow::loadFile(const QString &book)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    tabs.append(new UmdReader(book,this));
    UmdReader *reader=tabs.last();
    tabBar->addTab(reader->getTitle());
    stackedWidget->addWidget(qobject_cast<QWidget*>(reader));
    int count=tabBar->count();
    tabBar->setCurrentIndex(count-1);
    stackedWidget->setCurrentIndex(count-1);
    reader->showOrHideList(show);
    QString title("UmdReader--");
    title.append(reader->getTitle());
    setWindowTitle(title);
    setCurrentFile(book);
    QApplication::restoreOverrideCursor();

    atrAction->setEnabled(true);
    saveAsAction->setEnabled(true);
    printAction->setEnabled(true);
    zoomInAction->setEnabled(true);
    zoomOutAction->setEnabled(true);
    findAction->setEnabled(true);
    selectAllAction->setEnabled(true);
    showOrHideAction->setEnabled(true);
    showOrHideAction->setChecked(show);
    connect(reader->getEdit(),SIGNAL(copyAvailable(bool)),
            copyAction,SLOT(setEnabled(bool)));
    connect(reader->getEdit(),SIGNAL(copyAvailable(bool)),
            findNextAction,SLOT(setEnabled(bool)));
}

void MainWindow::showAttribute()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
    {
        UMD::Head *head=reader->getParser()->getHead();
        QString title=(char*)head->title;
        QString author=(char*)head->author;
        QString year=(char*)head->year;
        QString month=(char*)head->month;
        QString day=(char*)head->day;
        QString gender=(char*)head->gender;
        QString publisher=(char*)head->publisher;
        QString vendor=(char*)head->vendor;
        QString info(QObject::tr("Book Title:\t%1\nAuthor:\t%2\nYear:\t%3\nMonth:\t%4\n"
                                 "Day:\t%5\nGender:\t%6\nPublisher:\t%7\nVendor:\t%8")
                     .arg(title).arg(author).arg(year).arg(month).arg(day).arg(gender)
                     .arg(publisher).arg(vendor));
        QMessageBox::information(parentWidget(),QObject::tr("File Attribute"),info);
    }
}

void MainWindow::saveAs()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
    {
        QString book=QFileDialog::getSaveFileName(parentWidget(),QObject::tr("Save File As..."),
                                                  QDir::homePath(),QObject::tr("Documents (*.txt)"));
        QFile file(book);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream out(&file);
            out<<reader->getDocument()->toPlainText();
            file.close();
        }
    }
}

void MainWindow::zoomIn()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
        reader->getEdit()->zoomIn();
}

void MainWindow::zoomOut()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
        reader->getEdit()->zoomOut();
}

void MainWindow::findString()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
    {
        bool ok;
        QString str=QInputDialog::getText(parentWidget(),QObject::tr("Search A String"),
                                          QObject::tr("Input the string to search:"),
                                          QLineEdit::Normal,QString(),&ok);
        if(ok&&(!str.isEmpty()))
        {
            searchString=str;
            findNextAction->setEnabled(true);
            //        QTextCursor cur=doc->find(str);
            reader->getEdit()->find(str);
        }
    }
}

void MainWindow::findNext()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
    {
        QTextEdit *edit=reader->getEdit();
        QTextCursor cur=edit->textCursor();
        QString str=cur.selectedText();
        if(!str.isEmpty())
            edit->find(str);
        if(!searchString.isEmpty())
            edit->find(searchString);
    }
}

void MainWindow::showOrHideList(bool b)
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
    {
        show=b;
        reader->showOrHideList(b);
    }
}

void MainWindow::print()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
    {
        QPrinter printer;
        QPrintDialog dialog(&printer,this);
        if(dialog.exec()==QAbstractPrintDialog::Accepted)
        {
            reader->getDocument()->print(&printer);
        }
    }
}

void MainWindow::selectAll()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
        reader->getEdit()->selectAll();
}

void MainWindow::onCurrentChanged(int index)
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());   //last widget
    if(reader)
    {
        disconnect(reader->getEdit(),SIGNAL(copyAvailable(bool)),
                   copyAction,SLOT(setEnabled(bool)));
        disconnect(reader->getEdit(),SIGNAL(copyAvailable(bool)),
                   findNextAction,SLOT(setEnabled(bool)));
    }
    if(stackedWidget->count())
    {
        QString title("UmdReader--");
        stackedWidget->setCurrentIndex(index);
        reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());        //current widget
        reader->showOrHideList(show);
        title.append(reader->getTitle());
        setWindowTitle(title);                                                  //change window title when read another document
        connect(reader->getEdit(),SIGNAL(copyAvailable(bool)),
                copyAction,SLOT(setEnabled(bool)));
        connect(reader->getEdit(),SIGNAL(copyAvailable(bool)),
                copyAction,SLOT(setEnabled(bool)));
    }
}

void MainWindow::copy()
{
    UmdReader *reader=qobject_cast<UmdReader*>(stackedWidget->currentWidget());
    if(reader)
        reader->getEdit()->copy();
}

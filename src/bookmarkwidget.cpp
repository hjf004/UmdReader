#include"../include/bookmarkwidget.h"
BookMarkWidget::BookMarkWidget(const QString &name, QWidget *parent):QListWidget(parent)
{
    bookTitle=name;
    setWindowTitle(QObject::tr("BookMarks--").append(bookTitle));

    bookMarkTitle=QStringList();
    bookMarkPosition=QList<int>();
    //    bookMarksList=new QListWidget(this);
    createMenu();
    initList();
    //    setWidget(bookMarksList);

    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onBookMarkSelected(QModelIndex)));
    connect(addBookMarkAction,SIGNAL(triggered()),this,SLOT(addBookMark()));
    connect(delBookMarkAction,SIGNAL(triggered()),this,SLOT(delBookMark()));
    connect(renameBookMarkAction,SIGNAL(triggered()),this,SLOT(renameBookMark()));
}

void BookMarkWidget::saveBookMarks()            //save all bookmarks
{
    QSettings settings(QSettings::IniFormat,QSettings::UserScope,
                       "SMTNT",QApplication::applicationName());
    QString keyTitle(bookTitle);
    QString keyPos(bookTitle);
    keyTitle.append("-BookMarkTitle");
    keyPos.append("-BookMarkPosition");
    QList<QVariant> posList;
    int i;
    foreach(i,bookMarkPosition)
    {
        posList.append(QVariant(i));
    }

    settings.setValue(keyTitle,bookMarkTitle);
    settings.setValue(keyPos,posList);
    settings.sync();
}

void BookMarkWidget::saveBookMark(const QString &name)
{
    readBookMarks();

    QTextCursor cur=textEdit->textCursor();
    int pos=cur.position();
    bookMarkTitle.append(name);
    bookMarkPosition.append(pos);
    this->addItem(name);
    saveBookMarks();
}

bool BookMarkWidget::gotoBookMark(const QString &name, int index)
{
    bool result;
    if(bookMarkTitle.at(index)==name)
    {
        int pos=bookMarkPosition.at(index);
        QTextCursor cur=textEdit->textCursor();
        cur.setPosition(pos);
        textEdit->setTextCursor(cur);
        result=true;
    }
    else
        result=false;
    return result;
}

void BookMarkWidget::readBookMarks()
{
    QSettings settings(QSettings::IniFormat,QSettings::UserScope,
                       "SMTNT",QApplication::applicationName());
    QString keyTitle(bookTitle);
    QString keyPos(bookTitle);
    keyTitle.append("-BookMarkTitle");
    keyPos.append("-BookMarkPosition");
    bookMarkTitle.clear();
    bookMarkPosition.clear();
    bookMarkTitle.append(settings.value(keyTitle).toStringList());
    QList<QVariant> posList=settings.value(keyPos).toList();
    QVariant var;
    foreach(var,posList)
    {
        bookMarkPosition.append(var.toInt());
    }
}

void BookMarkWidget::initList()
{
    readBookMarks();
    this->clear();
    this->addItems(bookMarkTitle);
}

void BookMarkWidget::onBookMarkSelected(const QModelIndex & index)
{
    int row=index.row();
    QListWidgetItem *item=this->item(row);
    gotoBookMark(item->text(),row);
}

bool BookMarkWidget::removeBookMark(const QString &name,int index)
{
    bool result=false;
    if(name==bookMarkTitle.at(index))
    {
        bookMarkTitle.removeAt(index);
        bookMarkPosition.removeAt(index);
        QListWidgetItem *item=this->takeItem(index);
        if(!item)
            delete item;
        saveBookMarks();
        result=true;
    }
    return result;
}

void BookMarkWidget::createMenu()
{
    bookMarkManagerMenu=new QMenu(this);
    addBookMarkAction=new QAction(QObject::tr("Add a &BookMark"),this);
    delBookMarkAction=new QAction(QObject::tr("Remove BookMark"),this);
    renameBookMarkAction=new QAction(QObject::tr("Rename BookName"),this);
    bookMarkManagerMenu->addAction(addBookMarkAction);
    bookMarkManagerMenu->addAction(delBookMarkAction);
    bookMarkManagerMenu->addAction(renameBookMarkAction);
}

void BookMarkWidget::contextMenuEvent(QContextMenuEvent *event)
{
    bookMarkManagerMenu->exec(event->globalPos());
}

void BookMarkWidget::addBookMark()
{
    QString bookMarkName=QInputDialog::getText(this,QObject::tr("Add A BookMark"),
                                               QObject::tr("BookMark Name"));
    if(!bookMarkName.isEmpty())
    {

        saveBookMark(bookMarkName);
    }
}

void BookMarkWidget::delBookMark()
{
    int index= currentRow();
    removeBookMark(bookMarkTitle.at(index),index);

}

void BookMarkWidget::renameBookMark()
{
    int index= currentRow();
    QString bookMarkName=QInputDialog::getText(this,QObject::tr("Add A BookMark"),
                                               QObject::tr("BookMark Name"));
    if(!bookMarkName.isEmpty())
    {
        bookMarkTitle.removeAt(index);
        bookMarkTitle.insert(index,bookMarkName);
        QListWidgetItem *item=this->item(index);
        item->setText(bookMarkName);
        saveBookMarks();
    }
}


#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H
#include<QtGui>

class BookMarkWidget:public QListWidget
{
    Q_OBJECT
public:
    explicit BookMarkWidget(const QString &bookName,QWidget *parent=0);

    void setTextEdit(QTextEdit *te)
    {
        textEdit=te;
    }

    void saveBookMark(const QString &name);
    void saveBookMarks();
    void readBookMarks();
    bool gotoBookMark(const QString &name,int index);
    bool removeBookMark(const QString &name,int index);
private:
    void initList();
    void createMenu();
    QTextEdit *textEdit;
    QString bookTitle;
    QList<int> bookMarkPosition;
    QStringList bookMarkTitle;

    QMenu *bookMarkManagerMenu;
    QAction *addBookMarkAction;
    QAction *delBookMarkAction;
    QAction *renameBookMarkAction;

protected:
    void contextMenuEvent(QContextMenuEvent * event);
private slots:
    void onBookMarkSelected(const QModelIndex & index);
    void addBookMark();
    void delBookMark();
    void renameBookMark();
};

#endif // BOOKMARKWIDGET_H

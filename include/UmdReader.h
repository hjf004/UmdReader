#ifndef UMDREADER_H
#define UMDREADER_H
#include<QtGui>
#include"../include/UmdParser.h"
#include"../include/bookmarkwidget.h"

class UmdReader:public QSplitter
{
    Q_OBJECT
public:
    UmdReader(const QString name,QWidget *parent=0);
    ~UmdReader();
    QString getTitle();
    QTextDocument* getDocument();
    QTextEdit* getEdit();
    BookMarkWidget* getBookMarkManager();
    UMD::UmdParser *getParser();
    void showOrHideList(bool);
    void showChapterList(bool);
    void showBookMarkList(bool);

private:
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout;
    QTreeWidget *treeWidget;
    BookMarkWidget *bookMarkWidget;
    QTextEdit *textView;
    QTextDocument *doc;

    QString bookName;
    QMap<QString,unsigned int> index;
    UMD::UmdParser *parser;

    int creatParser();
    void creatWidget();
    void creatConnection();
    void readChapters();
    void readDataBlocks();
private slots:
    void findChapter(QTreeWidgetItem * current, QTreeWidgetItem * previous);
};

#endif // UMDREADER_H

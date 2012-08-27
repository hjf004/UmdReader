#ifndef UMDREADER_H
#define UMDREADER_H
#include<QtGui>
#include"../include/UmdParser.h"
class UmdReader:public QSplitter
{
    Q_OBJECT
public:
    UmdReader(const QString name,QWidget *parent=0);
    ~UmdReader();
    QString getTitle();
    QTextDocument* getDocument();
    QTextEdit* getEdit();
    UMD::UmdParser *getParser();
    void showOrHideList(bool);

private:
    QTreeWidget *treeWidget;
    QTextEdit *textView;
    QTextDocument *doc;

    QString bookName;
    QMap<QString,unsigned int> index;
    UMD::UmdParser *parser;

    void creatWidget();
    void creatConnection();
    void readChapters();
    void readDataBlocks();
private slots:
    void findChapter(QTreeWidgetItem * current, QTreeWidgetItem * previous);
};

#endif // UMDREADER_H

#include"../include/UmdReader.h"
#include"../include/UmdParser.h"
#include"../include/Additions.h"
#include<stdlib.h>
#include<zlib.h>

UmdReader::UmdReader(const QString name, QWidget *parent):QSplitter(parent)
{
    bookName=name;
    creatWidget();
    readChapters();
    readDataBlocks();
    creatConnection();
}

UmdReader::~UmdReader()
{
    delete treeWidget;
    delete textView;
    delete doc;
    delete parser;
}

void UmdReader::creatWidget()
{
        treeWidget=new QTreeWidget(this);
        treeWidget->setHeaderLabel(QObject::tr("Chapter List"));
        treeWidget->setMaximumWidth(240);

        textView=new QTextEdit(this);
        textView->setReadOnly(true);

        this->addWidget(treeWidget);
        this->addWidget(textView);
}

void UmdReader::creatConnection()
{
    connect(treeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this,SLOT(findChapter(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void UmdReader::readChapters()
{
    if(!bookName.isEmpty())
    {
        parser=new UMD::UmdParser();
        QByteArray byteStr(bookName.toAscii());
        parser->Parse(byteStr.data());
    }
    else
        return;
    std::vector<UMD::Chapter *> chapters=parser->getChapters();
    QTreeWidgetItem *item=0;
    for(int i=0;i<chapters.size();i++)
    {
        QString chapterName=chapters[i]->title;
        QStringList chapter(chapterName);
        index.insertMulti(chapterName,chapters[i]->offset);
        item=new QTreeWidgetItem(chapter);
        treeWidget->addTopLevelItem(item);
    }
}

void UmdReader::readDataBlocks()
{
    const int MAXBUFFERSIZE=1024*32;
    unsigned long deslen=MAXBUFFERSIZE;
    unsigned char buffer_data[2*MAXBUFFERSIZE];
    unsigned char buffer[MAXBUFFERSIZE];

    doc=new QTextDocument();
    QTextCursor cur(doc);
    if(!bookName.isEmpty())
    {
        std::vector<UMD::Content *> blocks=parser->getBlocks();
        cur.beginEditBlock();
        for(int i=0;i<blocks.size();i++)
        {
            QString content;
            memset(buffer,0,MAXBUFFERSIZE);
            memset(buffer_data,0,MAXBUFFERSIZE*2);
            unsigned long length=blocks[i]->length;
            if(Z_OK==uncompress(buffer,&deslen,blocks[i]->data,length))
            {
                UMD::EnterFilter(buffer,MAXBUFFERSIZE);
                UMD::WideCharToMultiByte((char*)buffer_data,buffer,MAXBUFFERSIZE,2*MAXBUFFERSIZE);
                content.append((char*)buffer_data);
                cur.insertText(content);
            }
        }
        cur.endEditBlock();
        doc->setTextWidth(800);
        textView->setDocument(doc);
        textView->setFont(QFont("Sans",14));
    }
}

void UmdReader::findChapter(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString title=current->text(0);
    unsigned int position=index[title]/2;
    QTextCursor cur=textView->textCursor();
    cur.setPosition(position);
    textView->setTextCursor(cur);
}

QString UmdReader::getTitle()
{
    if(parser)
    {
        UMD::Head *head=parser->getHead();
        return QString((char*)head->title);
    }
    return QString();
}

void UmdReader::showOrHideList()
{
    treeWidget->setVisible(!(treeWidget->isVisible()));
}

QTextDocument* UmdReader::getDocument()
{
    return doc;
}

QTextEdit* UmdReader::getEdit()
{
    return textView;
}

UMD::UmdParser* UmdReader::getParser()
{
    return parser;
}

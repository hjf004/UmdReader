#include "../include/Additions.h"
#include<iconv.h>
#include<assert.h>
namespace UMD
{
int WideCharToMultiByte(char* des,unsigned char *sr,unsigned long in_byte,unsigned long out_byte)
{
    iconv_t icv=iconv_open("UTF-8","UTF-16");
    int res=iconv(icv,(char**)(&sr),(size_t*)(&in_byte),&des,(size_t*)(&out_byte));
    iconv_close(icv);
    return res;
}

void EnterFilter(unsigned char* p,unsigned int len)
{
    unsigned short* A = (unsigned short*)p;
    for (int i=0; i<len/2; i++)
    {
        if (*(A+i) == 0x2029)
        {
            *(A+i) =(unsigned short)'\n';
        }
    }
}
Head::Head()
{
    title=NULL;
    author=NULL;
    year=NULL;
    month=NULL;
    day=NULL;
    gender=NULL;
    publisher=NULL;
    vendor=NULL;
}

Head::Head(Head &other)
{
    int tlen=strlen((char*)other.title)+1;
    int alen=strlen((char*)other.author)+1;
    int ylen=strlen((char*)other.year)+1;
    int mlen=strlen((char*)other.month)+1;
    int dlen=strlen((char*)other.day)+1;
    int glen=strlen((char*)other.gender)+1;
    int plen=strlen((char*)other.publisher)+1;
    int vlen=strlen((char*)other.vendor)+1;
    title=(unsigned char*)malloc(tlen);
    author=(unsigned char*)malloc(alen);
    year=(unsigned char*)malloc(ylen);
    month=(unsigned char*)malloc(mlen);
    day=(unsigned char*)malloc(dlen);
    gender=(unsigned char*)malloc(glen);
    publisher=(unsigned char*)malloc(plen);
    vendor=(unsigned char*)malloc(vlen);
    assert(title&&author&&year&&month&&day&&gender&&publisher&&vendor);
    memcpy(title,other.title,tlen);
    memcpy(author,other.author,alen);
    memcpy(year,other.year,ylen);
    memcpy(month,other.month,mlen);
    memcpy(day,other.day,dlen);
    memcpy(gender,other.gender,glen);
    memcpy(publisher,other.publisher,plen);
    memcpy(vendor,other.vendor,vlen);
}

Head::~Head()
{
    free(title);
    free(author);
    free(year);
    free(month);
    free(day);
    free(gender);
    free(publisher);
    free(vendor);
}

void Head::setAttribute(Attribute a,unsigned char*atr,int len)
{
    switch(a)
    {
    case Title:
        title=(unsigned char*)malloc(len*2+1);
        memcpy(title,atr,len*2);
        title[len*2]='\0';
        break;
    case Author:
        author=(unsigned char*)malloc(len*2+1);
        memcpy(author,atr,len*2);
        author[len*2]='\0';
        break;
    case Year:
        year=(unsigned char*)malloc(len*2+1);
        memcpy(year,atr,len*2);
        year[len*2]='\0';
        break;
    case Month:
        month=(unsigned char*)malloc(len+1);
        memcpy(month,atr,len);
        month[len]='\0';
        break;
    case Day:
        day=(unsigned char*)malloc(len*2+1);
        memcpy(day,atr,len*2);
        day[len*2]='\0';
        break;
    case Gender:
        gender=(unsigned char*)malloc(len*2+1);
        memcpy(gender,atr,len*2);
        gender[len*2]='\0';
        break;
    case Publisher:
        publisher=(unsigned char*)malloc(len*2+1);
        memcpy(publisher,atr,len*2);
        publisher[len*2]='\0';
        break;
    case Vendor:
        vendor=(unsigned char*)malloc(len*2+1);
        memcpy(vendor,atr,len*2);
        vendor[len*2]='\0';
        break;
    default:
        printf("Invalid Call of this function!\n");
        exit(1);

    }
}

Chapter::Chapter(char *t,int len,unsigned int _offset)
{
    offset=_offset;
    title=(char*)malloc(len*2+1);
    memcpy(title,t,len*2);
    title[len*2]='\0';
}
Chapter::Chapter(Chapter& other)
{
    offset=other.offset;
    title=(char*)malloc(strlen(other.title)+1);
    strcpy(title,other.title);
}
Chapter::~Chapter()
{
    free(title);
}

Cover::Cover(TYPE t,unsigned char *img,unsigned int len)
{
    type=t;
    length=len;
    content=(unsigned char*)calloc(1,length);
    memcpy(content,img,length);
}
Cover::Cover(Cover& other)
{
    type=other.type;
    length=other.length;
    content=(unsigned char*)calloc(1,length);
    memcpy(content,other.content,length);
}
Cover::~Cover()
{
    free(content);
}
Content::Content(Content &other)
{
    length=other.length;
    data=other.data;
}
}

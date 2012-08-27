#ifndef ADDITIONS_H
#define ADDITIONS_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
namespace UMD
{
    int WideCharToMultiByte(char* des,unsigned char* sr,unsigned long in_byte,unsigned long out_byte);
    void EnterFilter(unsigned char* p,unsigned int len);
    struct Head
    {
        enum Attribute
        {
            Title=0,
            Author,
            Year,
            Month,
            Day,
            Gender,
            Publisher,
            Vendor
        };
        Head();
        Head(Head& other);
        ~Head();
        unsigned char *title;
        unsigned char *author;
        unsigned char *year;
        unsigned char *month;
        unsigned char *day;
        unsigned char *gender;
        unsigned char *publisher;
        unsigned char* vendor;
        void setAttribute(Attribute a,unsigned char*atr,int len);
    };
     struct Chapter
    {
        Chapter(char *t,int _len,unsigned int _offset);
        Chapter(Chapter& other);
        ~Chapter();
        char *title;
        unsigned int offset;
    };
    struct Cover             //book
    {
        enum TYPE
        {
            bmp=0,
            jpg,
            gif
        } type;
        Cover(TYPE t,unsigned char *img,unsigned int len);
        Cover(Cover& other);
        ~Cover();
        unsigned char *content;
        unsigned int length;
    };

    struct Content          //book body
    {
        Content(unsigned char* d,unsigned int len)
        {
            length=len;
            data=d;
        }
        Content(Content& other);
        unsigned char* data;
        unsigned int length;
    };
}
#endif

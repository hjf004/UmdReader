#include "../include/UmdParser.h"
#include <zlib.h>
const unsigned long MAXBUFFERSIZE=1024*32;
unsigned char BUFFER[MAXBUFFERSIZE];

namespace UMD
{
UmdParser::UmdParser()
{
    m_pUMDFile = NULL;
    m_nUMDFileSize = 0;
    m_pUMDBuffer = NULL;
    m_pChapOffset = NULL;
    m_pChapOffset = NULL;

    cover=NULL;
    head=new Head();
    m_nPkgSeed = 0;
    m_nCid = 0;
    m_nAdditionalCheck = 0;
    m_nContentLength = 0;
    m_nChapLen = 0;

}

UmdParser::~UmdParser()
{
    FreeMemory();
}

void UmdParser::FreeMemory()
{
    if (m_pUMDFile)
    {
        fclose(m_pUMDFile);
        m_pUMDFile = NULL;
    }

    if (m_pUMDBuffer)
    {
        free(m_pUMDBuffer);
        m_pUMDBuffer = NULL;
    }

    if (m_pChapOffset)
    {
        free(m_pChapOffset);
        m_pChapOffset = NULL;
    }

    if(cover)
        delete cover;
    for(int i=0; i<data_blocks.size(); i++)
    {
        delete data_blocks[i];
    }

    for(int i=0; i<chapters.size(); i++)
    {
        delete chapters[i];
    }
}

void UmdParser::ReadAdditional(short id, unsigned int check, unsigned int length)
{
    switch(id)
    {
    case 0x81:
        break;
    case 0x82:
    {
        char *imagepath[3] = {"Cover.bmp","Cover.jpg","Cover.gif"};
        int type = 0;
        unsigned short flag = *(unsigned short*)m_pCurrent;
        if (flag == 0xD8FF)
        {
            type = 1;
            cover=new Cover(Cover::bmp,m_pCurrent,length);
        }
        else if (flag == 0x4947)
        {
            type = 2;
            cover=new Cover(Cover::jpg,m_pCurrent,length);
        }
        else if (flag == 0x4D42)
        {
            type = 0;
            cover=new Cover(Cover::gif,m_pCurrent,length);
        }
        FILE *image = fopen(imagepath[type], "wb");
        fwrite(m_pCurrent,1,length,image);
        fclose(image);
    }
        break;
    case 0x83:
    {
        m_nChapLen = length/4;
        m_pChapOffset = (unsigned int*)calloc(m_nChapLen,sizeof(unsigned int));
        for (int i=0; i<m_nChapLen; i++)
        {
            m_pChapOffset[i] = *((unsigned int*)m_pCurrent + i);
        }
//        printf("Chapter Count:",m_nChapLen);
    }
        break;
    case 0x84:
    {
        if (m_nAdditionalCheck != check)
        {
            data_blocks.push_back(new Content(m_pCurrent,length));
        }
        else
        {
            unsigned int index = 0;
            int i = 0;
            while (index < length)
            {
                unsigned char count = *(m_pCurrent+index);
                memset(BUFFER,0,MAXBUFFERSIZE);
                index++;
                WideCharToMultiByte((char*)BUFFER,m_pCurrent+index,count,MAXBUFFERSIZE);
                //                PrintLog("章节名称%s:|章节偏移:%d\n",BUFFER,m_pChapOffset[i++]);
                chapters.push_back(new Chapter((char*)BUFFER,count,m_pChapOffset[i++]));
                index += count;
            }
        }
    }
        break;
    }

    m_pCurrent += length;
}

void UmdParser::ReadSection(short id, unsigned char b, unsigned char length)
{
    memset(BUFFER,0,MAXBUFFERSIZE);
    switch(id)
    {
    case 1:
        m_nPkgSeed = *(unsigned short*)m_pCurrent;
        break;
    case 2:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Title,BUFFER,length);
        break;
    case 3:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Author,BUFFER,length);
        break;
    case 4:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Year,BUFFER,length);
        break;
    case 5:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Month,BUFFER,length);
        break;
    case 6:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Day,BUFFER,length);
        break;
    case 7:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Gender,BUFFER,length);
        break;
    case 8:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Publisher,BUFFER,length);
        break;
    case 9:
        WideCharToMultiByte((char*)BUFFER,m_pCurrent,length,MAXBUFFERSIZE);
        head->setAttribute(Head::Vendor,BUFFER,length);
        break;
    case 10:
        m_nCid = *(unsigned int*)m_pCurrent;
        break;
    case 11:
        m_nContentLength = *(unsigned int*)m_pCurrent;
        break;
    case 12:
        break;
    case 0x81:
    case 0x82:
    case 0x83:
    case 0x84:
        m_nAdditionalCheck = *(unsigned int*)m_pCurrent;
        break;
    }

    m_pCurrent += length;
}


int UmdParser::ParseHeader()
{
    //检查文件头
    unsigned int magicint = *(unsigned int*)m_pCurrent;
    if (magicint != 0xde9a9b89)
    {
        printf("Not a umd file!\n");
        return -1;
    }
    m_pCurrent += 4;

    char ch = *m_pCurrent;
    while (ch == '#')
    {
        m_pCurrent += 1;

        short id = *(short*)m_pCurrent;
        m_pCurrent += 2;
        unsigned char b = *m_pCurrent;
        m_pCurrent += 1;
        unsigned char len = *m_pCurrent - 5;
        m_pCurrent += 1;
        ReadSection(id, b, len);

        ch = (char)*m_pCurrent;
        switch (id)
        {
        case 0xf1:
        case 10:
            id = 0x84;
            break;
        }

        while (ch == '$')
        {
            m_pCurrent += 1;
            unsigned long check = *(unsigned long*)m_pCurrent;
            m_pCurrent+=4;
            unsigned long num6 = *(unsigned long*)m_pCurrent - 9;
            m_pCurrent+=4;
            ReadAdditional(id,check,num6);
            ch = *m_pCurrent;
        }
    }

    return 0;
}

int UmdParser::Parse(char *pFileName)
{
    if (pFileName == NULL) return -1;
    FreeMemory();

    if((m_pUMDFile = fopen(pFileName, "rb")) == NULL)
    {
        printf("Failed to Open file!\n");
        return -1;
    }

    //因为是测试代码，所以直接全部都读取到内存中
    fseek(m_pUMDFile, 0, SEEK_END);
    m_nUMDFileSize = ftell(m_pUMDFile);
    m_pUMDBuffer = (unsigned char*)calloc(m_nUMDFileSize, 1);
    m_pCurrent = m_pUMDBuffer;
    fseek(m_pUMDFile, 0, SEEK_SET);
    if (fread((void*)m_pUMDBuffer, sizeof(unsigned char), m_nUMDFileSize, m_pUMDFile) != m_nUMDFileSize)
    {
        printf("Failed to read file!\n");
        return -1;
    }

    if (ParseHeader()) return -1;
    return 0;
}

Cover* UmdParser::getCover()
{
    return cover;
}

Head* UmdParser::getHead()
{
    return head;
}

std::vector<Chapter*> UmdParser::getChapters()
{
    return chapters;
}

std::vector<Content*> UmdParser::getBlocks()
{
    return data_blocks;
}
}           //namespace UMD;

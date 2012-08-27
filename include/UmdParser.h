#ifndef UMDPARSER_H
#define UMDPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <string.h>
#include <vector>
#include "../include/Additions.h"
namespace UMD
{
class UmdParser
{
public:
    UmdParser();
    ~UmdParser();
public:
    int Parse(char *pFileName);
    Head  *getHead();
    Cover *getCover();
    std::vector<Chapter*> getChapters();
    std::vector<Content*> getBlocks();
private:
    void ReadSection(short id, unsigned char b, unsigned char length);
    void ReadAdditional(short id, unsigned int check, unsigned int length);
    int ParseHeader();
//    int ParseContent();

    void FreeMemory();
    FILE *m_pUMDFile;
    unsigned long m_nUMDFileSize;

    unsigned char *m_pUMDBuffer;
    unsigned char *m_pCurrent;
    unsigned short m_nPkgSeed;
    unsigned int m_nCid;
    unsigned int m_nAdditionalCheck;
    unsigned int m_nContentLength;

    int m_nChapLen;
    unsigned int *m_pChapOffset;
    Head *head;
    Cover *cover;
    std::vector<Content*> data_blocks;
    std::vector<Chapter*> chapters;
};
}
#endif  // UMDPARSER_H


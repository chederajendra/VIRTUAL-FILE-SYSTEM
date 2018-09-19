#define _CRT_SECURE_NO_WARNINGS
#define MAXINODE 50
#define READ 1
#define WRITE 2
#define MAXFILESIZE 5
#define REGULAR 1
#define SPECIAL 2
#define START 0
#define CURRENT 1
#define END 2
#define MAXDATABLOCKS 3

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<io.h>

typedef struct superblock
{
	int TotalInodes;
	int FreeInode;
}SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;
	char * Data[MAXDATABLOCKS];
	int LinkCount;
	int ReferenceCount;
	int Permission;
	struct inode * next;
}INODE, *PINODE, **PPINODE;

typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int count;
	int mode;
	PINODE ptrinode;
}FILETABLE, *PFILETABLE;

typedef struct ufdt
{
	PFILETABLE ptrfiletable;
}UFDT;

void man(char*);
void DisplayHelp();
int GetFDFromName(char*);
PINODE GetInode(char*);
void CreateDILB();
void InitialiseSuperBlock();
int CreateFile(char*, int);
int rm_File(char*);
int ReadFile(int, char*, int);
int WriteFile(int, char*, int);
int OpenFile(char*, int);
void CloseFileByFD(int);
int CloseFileByName(char*);
void CloseAllFiles();
int LseekFile(int, int, int);
void ls_File();
int fstat_file(int);
int stat_file(char*);
int TruncateFile(char*);
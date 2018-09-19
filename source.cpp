#include "Header.h"

UFDT UFDTArr[MAXINODE];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;

void Backup()
{
	int fd = create("Backup.txt",0777);
	write( fd , UFDTArr , sizeof(UFDT)*MAXINODE);
	write( fd , SUPERBLOCKobj , sizeof(SUPERBLOCK) );
	
	for(int i = 0 ; i < MAXINODE ; i++)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
			write(fd , UFDTArr[i].ptrfiletable , sizeof(FILETABLE));
	}
	
	PINODE temp = head ;
	while(temp != NULL)
	{
		write(fd , temp , sizeof(INODE));
		write( fd , temp->Buffer , temp->FileActualSize );
	}
}

void ReadBackup()
{
	int fd  = open("Backup.txt" , 0777);
	
	read(fd , UFDTArr , sizeof(UFDT)*MAXINODE);
	read(fd , SUPERBLOCKobj , sizeof(SUPERBLOCK));
	
	for(int i = 0 ; i < MAXINODE ; i++)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
			read(fd , UFDTArr[i].ptrfiletable , sizeof(FILETABLE));
	}

}
*/

void man(char *name)
{
	if (name == NULL)
		return;
	if (_stricmp(name, "create") == 0)
	{
		printf("Descriptiom : Used to create new regular file\n");
		printf("Usage : Create file_name permission\n");
	}
	else if (_stricmp(name, "read") == 0)
	{
		printf("Descriptiom : Used to read data from regular file\n");
		printf("Usage : read File_name No_Of_Bytes_To_Read\n");
	}
	else if (_stricmp(name, "write") == 0)
	{
		printf("Descriptiom : Used to write into regular file\n");
		printf("Usage : Write File_name\nAfter this enter the data to write\n");
	}
	else if (_stricmp(name, "ls") == 0)
	{
		printf("Descriptiom : Used to list all information of files\n");
		printf("Usage : ls\n");
	}
	else if (_stricmp(name, "stat") == 0)
	{
		printf("Descriptiom : Used to display information of file\n");
		printf("Usage : stat File_Name\n");
	}
	else if (_stricmp(name, "fstat") == 0)
	{
		printf("Descriptiom : Used to display information of file\n");
		printf("Usage : fstat File_Descriptor\n");
	}
	else if (_stricmp(name, "truncate") == 0)
	{
		printf("Descriptiom : Used to remove data from  file\n");
		printf("Usage : truncate File_name \n");
	}
	else if (_stricmp(name, "open") == 0)
	{
		printf("Descriptiom : Used to open existing file\n");
		printf("Usage : open File_name mode\n");
	}
	else if (_stricmp(name, "close") == 0)
	{
		printf("Descriptiom : Used to close opened file\n");
		printf("Usage : close File_name \n");
	}
	else if (_stricmp(name, "closeall") == 0)
	{
		printf("Descriptiom : Used to close all opened file\n");
		printf("Usage : closeall \n");
	}
	else if (_stricmp(name, "lseek") == 0)
	{
		printf("Descriptiom : Used to change file offset\n");
		printf("Usage : lseek File_name Change_in_Offset Starting_Point\n");
	}
	else if (_stricmp(name, "rm") == 0)
	{
		printf("Descriptiom : Used to delete file \n");
		printf("Usage : rm File_Name \n");
	}
	else
	{
		printf("ERROR : No manual entry found\n");
	}

}

void DisplayHelp()
{
	printf("ls			: To List out all files\n");
	printf("clear		: To clear console\n");
	printf("open		: To open the file\n");
	printf("close		: To close the file\n");
	printf("closeall	: To close all opened files\n");
	printf("read		: To read contents from files\n");
	printf("write		: To write contents from files\n");
	printf("stat		: To display file information using name\n");
	printf("fstat		: To display file information using file descriptor\n");
	printf("truncate	: To remove all data from file\n");
	printf("rm			: To delete the file\n");
	printf("exit		: To Terminate the system\n");
}

int GetFDFromName(char * name)
{
	int i = 0;

	while (i < MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable != NULL)
		{
			if (_stricmp((UFDTArr[i].ptrfiletable->ptrinode->FileName), name) == 0)
			{
				break;
			}
			i++;
		}
	}
	if (i == 50)
		return -1;
	else
		return i;

}

PINODE GetInode(char * name)
{
	PINODE temp = head;

	if (name == NULL)
	{
		return NULL;
	}

	while (temp != NULL)
	{
		if ((strcmp(name, temp->FileName) == 0) && (temp->FileType != 0))
			break;
		temp = temp->next;
	}

	return temp;
}

void CreateDILB()
{
	PINODE newn = NULL;
	PINODE temp = head;
	int i = 1;

	while (i <= MAXINODE)
	{
		newn = (PINODE)malloc(sizeof(INODE));
		newn->LinkCount = newn->ReferenceCount = 0;
		newn->FileType = newn->FileSize = 0;
		newn->next = NULL;
		newn->InodeNumber = i;

		for (int j = 0; j < MAXDATABLOCKS ; j++)
			newn->Data[j] = NULL;

		if (temp == NULL)
		{
			head = newn;
			temp = head;
		}
		else
		{
			temp->next = newn;
			temp = temp->next;
		}
		i++;
	}
}

void InitialiseSuperBlock()
{
	int i = 0;
	while (i < MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}

	SUPERBLOCKobj.TotalInodes = MAXINODE;
	SUPERBLOCKobj.FreeInode = MAXINODE;
}

int CreateFile(char *name, int permission)
{
	int i = 0;
	PINODE temp = head;

	if ((name == NULL) || (permission <= 0) || (permission > 3))
	{
		return -1;
	}

	if (SUPERBLOCKobj.FreeInode == 0)
	{
		return -2;
	}

	if (GetInode(name) != NULL)
	{
		return -3;
	}

	(SUPERBLOCKobj.FreeInode)--;

	while (temp != NULL)
	{
		if (temp->FileType == 0)
			break;
		temp = temp->next;
	}

	while (i < MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable == NULL)
			break;
		i++;
	}

	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].ptrfiletable == NULL)
		return -4;

	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = permission;
	UFDTArr[i].ptrfiletable->readoffset = 0;
	UFDTArr[i].ptrfiletable->writeoffset = 0;

	UFDTArr[i].ptrfiletable->ptrinode = temp;
	strcpy_s(UFDTArr[i].ptrfiletable->ptrinode->FileName, name);
	UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->FileSize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->Permission = permission;
	memset(UFDTArr[i].ptrfiletable->ptrinode->Data, 0, MAXDATABLOCKS);

	return i;
}

int rm_File(char * name)
{
	int fd = 0;
	fd = GetFDFromName(name);

	if (fd == -1)
		return -1;

	(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

	if (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
		free(UFDTArr[fd].ptrfiletable);

		for(int i = 0 ; i < MAXDATABLOCKS ; i++)
			free(UFDTArr[fd].ptrfiletable->ptrinode->Data[i]);
	}

	UFDTArr[fd].ptrfiletable = NULL;
	(SUPERBLOCKobj.FreeInode)++;

	return 0;
}

int ReadFile(int fd, char * arr, int isize)
{
	if (UFDTArr[fd].ptrfiletable == NULL)
		return -1;

	if ((UFDTArr[fd].ptrfiletable->mode != READ) && (UFDTArr[fd].ptrfiletable->mode != (READ + WRITE)))
		return -2;

	if ((UFDTArr[fd].ptrfiletable->ptrinode->Permission != READ) && (UFDTArr[fd].ptrfiletable->ptrinode->Permission != (READ + WRITE)))
		return -2;

	if (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize == UFDTArr[fd].ptrfiletable->readoffset)
		return -3;

	if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
		return -4;

	int read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);
	int no = (UFDTArr[fd].ptrfiletable->readoffset) / MAXFILESIZE;
	int temp1 = 0;
	int temp2 = 0;

	if (read_size < isize)
	{
		while (read_size != 0)
		{
			temp1 = MAXDATABLOCKS * MAXFILESIZE - (UFDTArr[fd].ptrfiletable->readoffset);
			temp1 = temp1 % MAXFILESIZE;
			if (temp1 == 0)
				temp1 = MAXFILESIZE;

			if(temp1 > read_size)
				temp1 = read_size;

			strncpy(arr + temp2, (UFDTArr[fd].ptrfiletable->ptrinode->Data[no]) + (UFDTArr[fd].ptrfiletable->readoffset) % MAXFILESIZE, temp1);
			UFDTArr[fd].ptrfiletable->readoffset += temp1;
			read_size -= temp1;
			temp2 += temp1;
			no++;
		}
	}
	else
	{
		while (isize != 0)
		{
			temp1 = MAXDATABLOCKS * MAXFILESIZE - (UFDTArr[fd].ptrfiletable->readoffset);
			temp1 = temp1 % MAXFILESIZE;
			if (temp1 == 0)
				temp1 = MAXFILESIZE;

			if(temp1 > isize)
				temp1 = isize;

			strncpy(arr + temp2, (UFDTArr[fd].ptrfiletable->ptrinode->Data[no]) + (UFDTArr[fd].ptrfiletable->readoffset) % MAXFILESIZE, temp1);
			UFDTArr[fd].ptrfiletable->readoffset += temp1;
			isize -= temp1;
			temp2 += temp1;
			no++;
		}
	}

	return temp2;
}

int WriteFile(int fd, char * arr, int isize)
{
	if ((UFDTArr[fd].ptrfiletable->mode != WRITE) && (UFDTArr[fd].ptrfiletable->mode != (READ + WRITE)))
		return -1;

	if ((UFDTArr[fd].ptrfiletable->ptrinode->Permission != WRITE) && (UFDTArr[fd].ptrfiletable->ptrinode->Permission != (READ + WRITE)))
		return -1;

	if (UFDTArr[fd].ptrfiletable->writeoffset == MAXFILESIZE*MAXDATABLOCKS)
		return -2;

	if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
		return -3;

//aszie = availableSize , temp3 = AvailableSizeInBlock and later on temp3 is used as offset teller
	int asize = MAXDATABLOCKS*MAXFILESIZE - UFDTArr[fd].ptrfiletable->writeoffset;
	int no = UFDTArr[fd].ptrfiletable->writeoffset / MAXFILESIZE;
	int temp1 = isize < asize ? isize : asize , temp3 = (asize%MAXFILESIZE);

	if (temp3 == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->Data[no] = (char*)malloc(MAXFILESIZE);
		(UFDTArr[fd].ptrfiletable->ptrinode->FileSize) += MAXFILESIZE;
		temp3 = MAXFILESIZE;
	}
//IF NO DATA CAN BE WRITTEN IN AN EXISTING BLOCK
	if ((temp3 >= isize))
	{
		strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Data[no]) + ((UFDTArr[fd].ptrfiletable->writeoffset)%MAXFILESIZE), arr, isize);
		UFDTArr[fd].ptrfiletable->writeoffset = UFDTArr[fd].ptrfiletable->writeoffset + isize;
		(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) += isize;
	}
	else 
	{
		// FIRSTLY FILL THE REMAINING BLOCK
		strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Data[no]) + ((UFDTArr[fd].ptrfiletable->writeoffset) % MAXFILESIZE), arr, temp3);
		UFDTArr[fd].ptrfiletable->writeoffset += temp3;
		(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) += temp3;
		isize -= temp3;

		//THEN FILL THE DATA
		while ((no < MAXDATABLOCKS - 1) && (isize > 0) )
		{
			no++;
			UFDTArr[fd].ptrfiletable->ptrinode->Data[no] = (char*)malloc(MAXFILESIZE);
			(UFDTArr[fd].ptrfiletable->ptrinode->FileSize) += MAXFILESIZE;
			
			if (isize >= MAXFILESIZE)
			{
				strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Data[no]), arr + temp3, MAXFILESIZE);
				UFDTArr[fd].ptrfiletable->writeoffset = UFDTArr[fd].ptrfiletable->writeoffset + MAXFILESIZE;
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) += MAXFILESIZE;
				temp3 += MAXFILESIZE;
				isize -= MAXFILESIZE;
			}
			else
			{
				strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Data[no]), arr + temp3, isize);
				UFDTArr[fd].ptrfiletable->writeoffset = UFDTArr[fd].ptrfiletable->writeoffset + isize;
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) += isize;
				break;
			}
		}
	}
	return temp1;
}

int OpenFile(char * name, int mode)
{
	int i = 0;
	PINODE temp = NULL;

	if (name == NULL || mode <= 0)
		return -1;

	temp = GetInode(name);
	if (temp == NULL)
		return -2;

	if (temp->Permission < mode)
		return -3;

	while (i < 50)
	{
		if (UFDTArr[i].ptrfiletable == NULL)
			break;
		i++;
	}

	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].ptrfiletable == NULL)
		return -2;
	UFDTArr[i].ptrfiletable->count = 1;
	UFDTArr[i].ptrfiletable->mode = mode;

	if (mode == READ + WRITE)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
		UFDTArr[i].ptrfiletable->writeoffset = 0;
	}
	else if (mode == READ)
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
	}
	else if (mode == WRITE)
	{
		UFDTArr[i].ptrfiletable->writeoffset = 0;
	}
	else
	{
		return -1;
	}

	UFDTArr[i].ptrfiletable->ptrinode = temp;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

	return i;
}

void CloseFileByFD(int fd)
{
	UFDTArr[fd].ptrfiletable->readoffset = 0;
	UFDTArr[fd].ptrfiletable->writeoffset = 0;
	if( ( UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount) != 0 )
		( UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)-- ;

	}

int CloseFileByName(char * name)
{
	int fd = GetFDFromName(name);
	if (fd == -1)
		return -1;

	CloseFileByFD(fd);

	return 0;
}

void CloseAllFiles()
{
	int i = 0;

	while (i < MAXINODE)
	{
		if (UFDTArr[i].ptrfiletable != NULL)
		{
			CloseFileByFD(i);
		}

		i++;
	}
}

int LseekFile(int fd, int size, int from)
{
	if ((fd < 0) || (fd > MAXINODE) || (from > 2))
		return -1;

	if (UFDTArr[fd].ptrfiletable == NULL)
		return -1;

	if ((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ + WRITE))
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->readoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
				return -1;

			if ((UFDTArr[fd].ptrfiletable->readoffset) + size < 0)
				return -1;

			UFDTArr[fd].ptrfiletable->readoffset += size;
		}
		else if (from == START)
		{
			if (size >(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
				return -1;

			if (size < 0)
				return -1;

			(UFDTArr[fd].ptrfiletable->readoffset) = size;
		}
		else if (from == END)
		{
			if (((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size) > MAXFILESIZE)
				return -1;

			if (((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0)
				return -1;

			(UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
		}
	}
	else if ((UFDTArr[fd].ptrfiletable->mode) == WRITE)
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
			{
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = ((UFDTArr[fd].ptrfiletable->writeoffset) + size);
			}

			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > MAXFILESIZE)
				return -1;

			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
				return -1;

			UFDTArr[fd].ptrfiletable->writeoffset += size;
		}
		else if (from == START)
		{
			if (size >MAXFILESIZE)
				return -1;

			if (size < 0)
				return -1;

			if (size > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
				UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = size;

			(UFDTArr[fd].ptrfiletable->writeoffset) = size;
		}
		else if (from == END)
		{
			if (((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size) > MAXFILESIZE)
				return -1;

			if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
				return -1;

			(UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
		}
	}
	
	return 0;
}

void ls_File()
{
	PINODE temp = head;

	if (SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("ERROR : There are no files");
		return;
	}

	printf("\nFile Name\tInode Number\tFile Size\tLink Count \n");
	printf("-----------------------------------------------\n");
	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			printf("%s\t\t%d\t\t%d\t\t%d\n", temp->FileName, temp->InodeNumber, temp->FileActualSize, temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("-----------------------------------------------\n");
}

int fstat_file(int fd)
{
	PINODE temp = head;

	if (fd < 0 || fd > MAXINODE)
		return -1;

	if (UFDTArr[fd].ptrfiletable == NULL)
		return -2;

	temp = UFDTArr[fd].ptrfiletable->ptrinode;

	printf("\n-------------STATISTICAL INFORMATION ABOUT FILE----------------\n");
	printf("File name			: %s\n", temp->FileName);
	printf("Inode number		: %d\n", temp->InodeNumber);
	printf("File size			: %d\n", temp->FileSize);
	printf("File actual size	: %d\n", temp->FileActualSize);
	printf("Link count			: %d\n", temp->LinkCount);
	printf("Reference count		: %d\n", temp->ReferenceCount);
	if (temp->Permission == 1)
		printf("File Permission		: Read Only\n");
	else if (temp->Permission == 2)
		printf("File Permission		: Write Only\n");
	else if (temp->Permission == 3)
		printf("File Permission		: Read & Write\n");
	printf("\n---------------------------------------------------------------\n");

	return 0;
}

int stat_file(char * name)
{
	int fd = 0;

	if (name == NULL)
		return -1;

	fd = GetFDFromName(name);
	if (fd == -1)
		return -2;

	fstat_file(fd);

	return 0;

}

int TruncateFile(char *name)
{
	int fd = GetFDFromName(name);
	if (fd == -1)
		return -1;

	for(int i = 0 ; i < MAXDATABLOCKS ; i++)
		free(UFDTArr[fd].ptrfiletable->ptrinode->Data[i]);

	memset(UFDTArr->ptrfiletable->ptrinode->Data, 0, MAXDATABLOCKS);
	UFDTArr->ptrfiletable->ptrinode->FileActualSize = 0;
	UFDTArr->ptrfiletable->readoffset = 0;
	UFDTArr->ptrfiletable->writeoffset = 0;

	return 0;
}

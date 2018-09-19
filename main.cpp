#include"Header.h"

extern UFDT UFDTArr[MAXINODE];
extern SUPERBLOCK SUPERBLOCKobj;
extern PINODE head;

int main()
{
	char* ptr = NULL;
	int ret = 0, fd = 0, count = 0;
	char command[4][80], str[80], arr[MAXFILESIZE];

	InitialiseSuperBlock();
	CreateDILB();

	while (1)
	{
		fflush(stdin);
		strcpy_s(str, "");
		printf("\nVirtual_File_System :> ");
		fgets(str, 80, stdin);

		count = sscanf(str, "%s %s %s %s", command[0], command[1], command[2], command[3]);

		if (count == 1)
		{
			if (_stricmp(command[0], "ls") == 0)
			{
				ls_File();
			}

			else if (_stricmp(command[0], "closeall") == 0)
			{
				CloseAllFiles();
				printf("\nAll files closed succesfully\n");
				continue;
			}

			else if (_stricmp(command[0], "clear") == 0)
			{
				system("cls");
				continue;
			}

			else if (_stricmp(command[0], "help") == 0)
			{
				DisplayHelp();
				continue;
			}

			else if (_stricmp(command[0], "exit") == 0)
			{
				printf("\nTerminating File System \n");
				break;
			}

			else
			{
				printf("\nERROR : Command not found !! \n");
				continue;
			}
		}

		else if (count == 2)
		{

			if (_stricmp(command[0], "stat") == 0)
			{
				ret = stat_file(command[1]);

				if (ret == -1)
					printf("ERROR : Incorrect parameters\n");
				if (ret == -2)
					printf("ERROR : There is no such file\n");
				continue;
			}

			else if (_stricmp(command[0], "fstat") == 0)
			{
				ret = fstat_file(atoi(command[1]));

				if (ret == -1)
					printf("ERROR : Incorrect parameters\n");
				if (ret == -2)
					printf("ERROR : There is no such file\n");
				continue;
			}

			else if (_stricmp(command[0], "close") == 0)
			{
				ret = CloseFileByName(command[1]);

				if (ret == -1)
					printf("ERROR : There is no such file\n");
				continue;
			}

			else if (_stricmp(command[0], "rm") == 0)
			{
				ret = rm_File(command[1]);

				if (ret == -1)
					printf("ERROR : There is no such file\n");
				continue;
			}

			else if (_stricmp(command[0], "man") == 0)
			{
				man(command[1]);
			}

			else if (_stricmp(command[0], "write") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR : Incorrect parameters\n");
					continue;
				}

				printf("Enter data :\n");
				scanf("%[^\n]", arr);

				ret = strlen(arr);
				if (ret == 0)
				{
					printf("ERROR : Incorrect parameters\n");
					continue;
				}

				ret = WriteFile(fd, arr, ret);
				if (ret == -1)
					printf("ERROR : Premission denied\n");
				if (ret == -2)
					printf("ERROR : There is no suffficent space\n");
				if (ret == -3)
					printf("ERROR : It is not regular file\n");
			}

			else if (_stricmp(command[0], "truncate") == 0)
			{
				ret = TruncateFile(command[1]);

				if (ret == -1)
					printf("ERROR : Incorrect parameters\n");
				continue;
			}

			else
			{
				printf("\nERROR : Command not found !! \n");
				continue;
			}
		}

		else if (count == 3)
		{
			if (_stricmp(command[0], "create") == 0)
			{
				ret = CreateFile(command[1], atoi(command[2]));
				if (ret >= 0)
					printf("File is succesfully created with file descriptor : %d\n", ret);
				if (ret == -1)
					printf("ERROR : Incorrect parameters\n");
				if (ret == -2)
					printf("ERROR : There is no inodes\n");
				if (ret == -3)
					printf("ERROR : File already exists\n");
				if (ret == -4)
					printf("ERROR : Memory allocation failure\n");
				continue;
			}

			else if (_stricmp(command[0], "open") == 0)
			{
				ret = OpenFile(command[1], atoi(command[2]));
				if (ret >= 0)
					printf("File is succesfully created with file descriptor : %d\n", ret);
				if (ret == -1)
					printf("ERROR : Incorrect parameters\n");
				if (ret == -2)
					printf("ERROR : File not present\n");
				if (ret == -3)
					printf("ERROR : Permission denied\n");
			}

			else if (_stricmp(command[0], "read") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR : Incorrect parameters\n");
					continue;
				}

				ptr = (char*)malloc(sizeof(atoi(command[2])) + 1);
				if (ptr == NULL)
				{
					printf("ERROR : Insufficient memory\n");
					continue;
				}

				ret = ReadFile(fd, ptr, atoi(command[2]));
				if (ret == -1)
					printf("ERROR : File does not exist\n");
				if (ret == -2)
					printf("ERROR : Permission denied\n");
				if (ret == -3)
					printf("ERROR : Reached at the end of file\n");
				if (ret == -4)
					printf("ERROR : It is not regular file\n");
				if (ret == 0)
					printf("ERROR : File empty\n");
				if (ret > 0)
				{
					_write(2, ptr, ret);
					continue;
				}
			}

			else
			{
				printf("\nERROR : Command not found !! \n");
				continue;
			}
		}

		else if (count == 4)
		{
			if (_stricmp(command[0], "lseek") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR : Incorrect parameters\n");
					continue;
				}

				ret = LseekFile(fd, atoi(command[2]), atoi(command[3]));
				if (ret == -1)
					printf("ERROR : Unable to perform lseek\n");
			}

			else
			{
				printf("\nERROR : Command not found !! \n");
				continue;
			}
		}

		else
		{
			printf("\nERROR : Command not found !! \n");
			continue;
		}
	}


	return 0;
}
//////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
//////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<io.h>

//////////////////////////////////////////////////////////
//
//  User Defined Macros
//
//////////////////////////////////////////////////////////

// Maximum file size that we allow in the project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

//////////////////////////////////////////////////////////
//
//  User Defined Macros for Error Handling
//
//////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

//////////////////////////////////////////////////////////
//
//  User Defined Structures
//
//////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock
//  Description :       Holds the information to boot the OS
//
//////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

//////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       Holds the information about the file system
//
//////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

//////////////////////////////////////////////////////////
//
//  Structure Name :    Inode
//  Description :       Holds the information about file
//
//////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int FileType;
    int ActualFileSize;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode ** PPINODE;

//////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       Holds the information about opened file
//
//////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

//////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       Holds the information about process file
//
//////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

//////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
//
//////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

//////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseUAREA
//  Description :       It is used to initialise UAREA members
//  Author :            Raj Samir Jadhav
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void InitialiseUAREA()
{
   strcpy(uareaobj.ProcessName,"Myexe");
   
   int i = 0;

   for(i = 0; i < MAXOPENFILES; i++)
   {
        uareaobj.UFDT[i] = NULL;
   }
    printf("Marvellous CVFS : UAREA gets initialised succesfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseSuperBlock
//  Description :       It is used to initialise Super block members
//  Author :            Raj Samir Jadhav
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super block gets initialised succesfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is used to create Linkedlist of inodes
//  Author :            Raj Samir Jadhav
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn-> FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)    // LL is empty
        {
            head = newn;
            temp = head;
        }
        else                // LL contains atleast 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB created succesfully\n");
}

//////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitilisation
//  Description :       It is used to call all such functions which are
//                      used to initialise auxillary data
//  Author :            Raj Samir Jadhav
//  Date :              13/01/2026
//
//////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation()
{
    strcpy(bootobj.Information,"Booting process of Marvellous CVFS is done");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary data initialised succesfully\n");
}
//////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp
//  Description :       It is used to display the help page
//  Author :            Raj Samir Jadhav
//  Date :              14/01/2026
//
//////////////////////////////////////////////////////////
void DisplayHelp()
{
    printf("-----------------------------------------------\n");
    printf("----- Marvellous CVFS Help Page ---------------\n");
    printf("-----------------------------------------------\n");

    printf("man    : It is used to display a manual page\n");
    printf("clear  : It is Useed to clear the terminal\n");
    printf("creat  : It is Useed to Create a new file\n");
    printf("write  : It is used to write data into the file\n");
    printf("read   : It is used to read the data from the file\n");
    printf("stat   : It is used to display Statistical Information\n");
    printf("unlink : It is used to delete the file\n");
    printf("exit   : It is used to terminate Marvellous CVFS\n");

    printf("-----------------------------------------------\n");

}

//////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay
//  Description :       It is used to display the Man Page
//  Author :            Raj Samir Jadhav
//  Date :              14/01/2026
//
//////////////////////////////////////////////////////////

void ManPageDisplay( char Name [])
{
    if(strcmp("ls",Name) == 0)
    {
        printf("About : It is used to list the names of all files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp("man",Name) == 0)
     {
        printf("About : It is used to display manual page\n");
        printf("Usage : man command_name\n");
        printf("command_name : It is the name of Command\n");
    }
    else if(strcmp("exit",Name) == 0)
    {
         printf("About : It is used to terminate the shell\n");
        printf("Usage : exit\n");
    }
    else if(strcmp("clear",Name) == 0)
    {
         printf("About : It is used to clear the shell\n");
        printf("Usage : clear\n");
    }
    else
    {
        printf("No manual entery for %s\n",Name);
    }
}
//////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description :       It is used to chech whether file is already exist or not
//  Input  :            It accepts file name 
//  output :            It returns the True or False
//  Author :            Raj Samir Jadhav
//  Date :              16/01/2026
//
//////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name            // File Name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }
    return bFlag;

}



//////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description :       It is used to create new regular file
//  Input  :            It accepts file name and permission.
//  output :            it returns the file descriptor. 
//  Author :            Raj Samir Jadhav
//  Date :              16/01/2026
//
//////////////////////////////////////////////////////////

int CreateFile(
                    char *name,                     //Name of new file
                    int permission                  // permission for that file

)
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n",superobj.FreeInodes);

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if the permission value is wrong
    //permission -> 1 -> READ
    //permission -> 2 -> WRITE
    //per,ission -> -> READ + WRITE

    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }
    /// If the nodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }
    // If File is alreeady present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty Inode
    while(temp != NULL)
    {
        if(temp-> FileType == 0)
        {
            break;
        }
        temp = temp-> next;
    }

    if(temp == NULL)
    {
        printf("There is no Inode\n");
        return ERR_NO_INODES;
    }

    // Search for Empty Inode
    // Note : 0,1,2 are reserved
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }
    // UFDT is Full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory or File Table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    //Intialise the File Table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    // Connect File table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initaialise elements of Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    //Allocate memory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i; /// file Descriptor
}

//////////////////////////////////////////////////////////
//
//  Function Name :     LsFile()
//  Description :       It is used to list all Files
//  Input  :            Nothing
//  output :            nothing
//  Author :            Raj Samir Jadhav
//  Date :              16/01/2026
//
//////////////////////////////////////////////////////////

// ls -l
void LsFile()
{   
    PINODE temp = head;
    printf("-----------------------------------------------\n");
    printf("----- Marvellous CVFS Files Information -----\n");
    printf("-----------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
        printf("%d\t%s\t%d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        temp = temp->next;
    }
    printf("-----------------------------------------------\n");


}

//////////////////////////////////////////////////////////
//
//  Function Name :     UnlinkFile()
//  Description :       It is used to delete the file.
//  Input  :            File Name
//  output :            nothing
//  Author :            Raj Samir Jadhav
//  Date :              22/01/2026
//
//////////////////////////////////////////////////////////

int UnlinkFile(
                char *name
)
{   int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }
    // Travel the UFDT
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {

                // Deallocate the memory of buffer 
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // Rest all values of node
                // Dont deallocate the memory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0',sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                //Deallocate memory of File Table
                free(uareaobj.UFDT[i]);

                //Set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                 // Increment free inodes count
                 superobj.FreeInodes++;

                 break; //IMP
                 // End of if
                 //End of if
                 // End of for
                 //end of function
            }

        }
    }
     return EXECUTE_SUCCESS;
}


//////////////////////////////////////////////////////////
//
//  Function Name :     WriteFile()
//  Description :       It is used to write the data into the file.
//  Input  :            File descriptor
//                      Address of Buffer which contains data.
//                      Size of data that we want to write.
//  output :            Numbe of bytes successfully written.
//  Author :            Raj Samir Jadhav
//  Date :              22/01/2026
//
//////////////////////////////////////////////////////////

int WriteFile(int fd, char *data, int size)
{
    printf("File descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Number of bytes that we want to write : %d\n",size);

    // Invalid FD
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // FD points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // There is no permission to write
    if((uareaobj.UFDT[fd]->ptrinode->Permission & WRITE) == 0)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer +
            uareaobj.UFDT[fd]->WriteOffset,
            data, size);

    // Update the write offset
    uareaobj.UFDT[fd]->WriteOffset += size;

    // Update the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize += size;

    return size;
}

//////////////////////////////////////////////////////////
//
//  Function Name :     ReadFile()
//  Description :       It is used to read the data into the file.
//  Input  :            File descriptor
//                      Address of Empty Buffer which contains data.
//                      Size of data that we want to read.
//  output :            Number of bytes successfully read.
//  Author :            Raj Samir Jadhav
//  Date :              22/01/2026
//
//////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
            )
{
    // Invalid FD
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // filter for permission
    if((uareaobj.UFDT[fd]->ptrinode->Permission & READ) == 0)
    {
        return ERR_PERMISSION_DENIED;
    }


    // insufficient data
    if((uareaobj.UFDT[fd]->ptrinode->ActualFileSize - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }


    //Read the Data
    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    // updatevthe read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}




//////////////////////////////////////////////////////////
//
//  Entry Point function of the project
//
//////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    int iCount = 0;
    int iRet = 0;
    char InputBuffer[MAXFILESIZE] = {'\0'};
    char *EmptyBuffer = NULL;

    StartAuxillaryDataInitilisation();

    printf("-----------------------------------------------\n");
    printf("----- Marvellous CVFS started succesfully -----\n");
    printf("-----------------------------------------------\n");

    while(1)
    {
        fflush(stdin);
        strcpy(str,"");

        printf("\nMarvellous CVFS : > ");
        fgets(str,sizeof(str),stdin);

        iCount = sscanf(str,"%s %s %s %s %s",
                        Command[0],Command[1],Command[2],
                        Command[3],Command[4]);

        fflush(stdin);

        // -------- 1 WORD COMMANDS --------
        if(iCount == 1)
        {
            if(strcmp("exit",Command[0]) == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                break;
            }
            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();
            }
            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }
            else if(strcmp("clear",Command[0]) == 0)
            {
                system("clear");
            }
            else
            {
                printf("Command not Found\n");
            }
        }

        // -------- 2 WORD COMMANDS --------
        else if(iCount == 2)
        {
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }
            else if(strcmp("unlink",Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                    printf("Error: Invalid Parameter\n");
                else if(iRet == ERR_FILE_NOT_EXIST)
                    printf("Error: File does not exist\n");
                else
                    printf("File deleted successfully\n");
            }
            else if(strcmp("write",Command[0]) == 0)
            {
                printf("Enter data:\n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = WriteFile(atoi(Command[1]),
                                 InputBuffer,
                                 strlen(InputBuffer)-1);

                if(iRet == ERR_INVALID_PARAMETER)
                    printf("Error: Invalid parameter\n");
                else if(iRet == ERR_FILE_NOT_EXIST)
                    printf("Error: File does not exist\n");
                else if(iRet == ERR_PERMISSION_DENIED)
                    printf("Error: Permission denied\n");
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                    printf("Error: No sufficient space\n");
                else
                    printf("%d bytes written successfully\n",iRet);
            }
            else
            {
                printf("Command not Found\n");
            }
        }

        // -------- 3 WORD COMMANDS --------
        else if(iCount == 3)
        {
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                    printf("Error: Invalid parameter\n");
                else if(iRet == ERR_NO_INODES)
                    printf("Error: No inodes available\n");
                else if(iRet == ERR_FILE_ALREADY_EXIST)
                    printf("Error: File already exists\n");
                else if(iRet == ERR_MAX_FILES_OPEN)
                    printf("Error: Max file limit reached\n");
                else
                    printf("File created successfully with FD %d\n",iRet);
            }
            else if(strcmp("read",Command[0]) == 0)
            {
                EmptyBuffer = (char *)malloc(atoi(Command[2]) + 1);

                iRet = ReadFile(atoi(Command[1]),
                                EmptyBuffer,
                                atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                    printf("Error: Invalid parameter\n");
                else if(iRet == ERR_FILE_NOT_EXIST)
                    printf("Error: File does not exist\n");
                else if(iRet == ERR_PERMISSION_DENIED)
                    printf("Error: Permission denied\n");
                else if(iRet == ERR_INSUFFICIENT_DATA)
                    printf("Error: Insufficient data\n");
                else
                {
                    EmptyBuffer[iRet] = '\0';
                    printf("Data : %s\n",EmptyBuffer);
                }

                free(EmptyBuffer);
            }
            else
            {
                printf("Command not Found\n");
            }
        }
        else
        {
            printf("Command not Found\n");
        }
    }

    return 0;
}

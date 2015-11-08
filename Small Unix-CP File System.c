/*
	////////////////////////////////////////////////////////////////
	// Author name: Francisco Martinez		//
	// Class: 		  CSE 3320					//
	// Description:   File System      	    //
	// Date: 	  November 26, 2014	 	    //   
	////////////////////////////////////////////////////////////////
*/

//Declarations of the required header files to reference "libraries" used for this program.
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//Define a unique variables to use during the execution of my program
#define BLOCKS_SIZE        256
#define NEW_DIRECTORY     "DIR"
#define DISK_1            "DISK_1"
#define EXTERNAL_FILE     "EXTERNAL_FILE"
#define USER_ID           "USER_ID_"
#define PARENT_DIRECTORY  ".."

//Declaration of all my variables used.
char  InitialFile[BLOCKS_SIZE];
char  LinkName[BLOCKS_SIZE];
char  InitialDirectory[BLOCKS_SIZE] = ".";
char  Buffer[32];
int   FilDes = -1;
int   BytesRead;
int   BytesWritten;
uid_t UserID;
 
//Declarations of functions used with their respectives attributes
void CleanUpOnError(int level)
{
   printf("Error encountered, cleaning up.\n");
   switch ( level ) {

       case 1:   printf("Could not get current working directory.\n");
                 break;

       case 2:   printf("Could not create file %s.\n", DISK_1);
                 break;

       case 3:   printf("Could not write to file %s.\n", DISK_1);
                 close(FilDes);
                 unlink(DISK_1);
                 break;

       case 4:   printf("Could not close file %s.\n", DISK_1);
                 close(FilDes);
                 unlink(DISK_1);
                 break;

       case 5:
                 printf("Could not make directory %s.\n", NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 6:   printf("Could not change to directory %s.\n", NEW_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 7:   printf("Could not create link %s to %s.\n", LinkName,InitialFile);
                 chdir(PARENT_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 8:   printf("Could not open link %s.\n", LinkName);
                 unlink(LinkName);
                 chdir(PARENT_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 9:
                 printf("Could not read link %s.\n", LinkName);
                 close(FilDes);
                 unlink(LinkName);
                 chdir(PARENT_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 10:  printf("Could not close link %s.\n", LinkName);
	         close(FilDes);
	       	 unlink(LinkName);
      	         chdir(PARENT_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 11:  printf("Could not unlink link %s.\n", LinkName);
                 unlink(LinkName);
                 chdir(PARENT_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 12:  printf("Could not change to directory %s.\n", PARENT_DIRECTORY);
                 chdir(PARENT_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 13:  printf("Could not remove directory %s.\n", NEW_DIRECTORY);
                 rmdir(NEW_DIRECTORY);
                 unlink(DISK_1);
                 break;

       case 14:  printf("Could not unlink file %s.\n", DISK_1);
           	 unlink(DISK_1);
           	 break;

       default:  break;
    }
    printf("Program ended with Error.\n"\
           "All test files and directories may not have been removed.\n");
}

//Beginning of the program.
int main ( void ) 
{
	printf("\n================== Small Unix-CP File System ===================\n");
	
	printf("\n => Starting Testing Operations\n\n");
    /* Get the current working directory and store it in InitialDirectory. */
    if ( NULL == getcwd(InitialDirectory,BLOCKS_SIZE) ) {
      	perror("getcwd Error");
      	CleanUpOnError(1);
      	return 0;
    }
    printf(" => The Working Directory is %s\n",InitialDirectory);

    /* Create the DISK_0 of 256 BLOCKS_SIZE for writing, if it does not exist.
       Give the owner authority to read, write, and execute. 
    ----------------------------------------------------------------*/
    FilDes = open(DISK_1, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU);
    if ( -1 == FilDes ) {
        perror("open Error");
        CleanUpOnError(2);
      	return 0;
    }
    printf("\n => Created %s of 256 BLOCKS_SIZE with R/W/X Permissions\n",DISK_1);

    /* Write EXTERNAL_FILE to DISK_1 via FilDes */
    BytesWritten = write(FilDes,EXTERNAL_FILE,strlen(EXTERNAL_FILE));
    if ( -1 == BytesWritten ) {
        perror("write Error");
        CleanUpOnError(3);
        return 0;
    }
    printf("\n => PUT %s to %s\n", EXTERNAL_FILE, DISK_1); 

    /* Close DISK_1 via FilDes */
    if ( -1 == close(FilDes) ) {
        perror("close Error");
        CleanUpOnError(4);
        return 0;
    }
    FilDes = -1;
    printf("\n => SaveFS %s\n", DISK_1);

    /* Make a new directory in the current working directory and
       grant the owner read, write and execute authority 
    ------------------------------------------------------------*/
	
    if ( -1 == mkdir(NEW_DIRECTORY, S_IRWXU) ) {
        perror("mkdir Error");
        CleanUpOnError(5);
        return 0;
    }
    printf("\n => OpenFS %s\n", DISK_1);
 
    /* Change the current working directory to the
       directory NEW_DIRECTORY just created. 
    ---------------------------------------------*/
    if ( -1 == chdir(NEW_DIRECTORY) ) {
        perror("chdir Error");
        CleanUpOnError(6);
        return 0;
    }
    //printf("Changed to directory %s/%s.\n", InitialDirectory, NEW_DIRECTORY);
 
    /* Copy PARENT_DIRECTORY to InitialFile and
      append  "/" and DISK_1 to InitialFile. 
    -------------------------------------------*/
    strcpy(InitialFile, PARENT_DIRECTORY);
    strcat(InitialFile,"/");
    strcat(InitialFile, DISK_1);
 
    /* Copy USER_ID to LinkName then append the
       UserID as a string to LinkName. 
    -------------------------------------------*/
    strcpy(LinkName, USER_ID);
    sprintf(Buffer, "%d", (int)UserID);
    strcat(LinkName, Buffer);
 
    /* Create a link to the InitialFile name with the LinkName. */
    if ( -1 == link(InitialFile, LinkName) ) {
        perror("link Error");
        CleanUpOnError(7);
        return 0;
    }
    printf("\n => LINK %s to %s\n", LinkName, InitialFile);
 
    /* Open the LinkName file for reading only. */
    if ( -1 == (FilDes = open(LinkName, O_RDONLY)) ) {
        perror("open Error");
        CleanUpOnError(8);
        return 0;
    }
    printf("\n => Opened %s for reading\n", LinkName);
 
    /* Read from the LinkName file, via FilDes, into Buffer. */
    BytesRead = read(FilDes, Buffer, sizeof(Buffer));
    if ( -1 == BytesRead ) {
        perror("read Error");
        CleanUpOnError(9);
        return 0;
    }
    printf("\n => Read %s from %s\n", Buffer, LinkName);
    if ( BytesRead != BytesWritten ) {
        printf("WARNING: the number of bytes read is "\
             "not equal to the number of bytes written.\n");
    }
 
    /* Close the LinkName file via FilDes. */
    if ( -1 == close(FilDes) ) {
        perror("close Error");
        CleanUpOnError(10);
        return 0;
    }
    FilDes = -1;
    printf("\n => Closed %s\n",LinkName);
 
    /* Unlink the LinkName link to InitialFile. */
    if ( -1 == unlink(LinkName) ) {
        perror("unlink Error");
        CleanUpOnError(11);
        return 0;
    }
    printf("\n => %s is unlinked.\n", LinkName);
 
    /* Change the current working directory
      back to the starting directory. 
    --------------------------------------------*/
    if ( -1 == chdir(PARENT_DIRECTORY) ) {
        perror("chdir Error");
        CleanUpOnError(12);
        return 0;
    }
    //printf("changing directory to %s.\n", InitialDirectory);
 
    /* Remove the directory NEW_DIRECTORY */
    if ( -1 == rmdir(NEW_DIRECTORY) ) {
        perror("rmdir Error");
        CleanUpOnError(13);
        return 0;
    }
    //printf("Removing directory %s.\n", NEW_DIRECTORY);
 
    /* Unlink the file DISK_1 */
    if ( -1 == unlink(DISK_1) ) {
        perror("unlink Error");
        CleanUpOnError(14);
        return 0;
    }
    printf("\n => Unlinking %s\n",DISK_1);
 
    printf("\n => Testing Finished Successfully.\n");
    return 0; //End of the program with return 0.
}


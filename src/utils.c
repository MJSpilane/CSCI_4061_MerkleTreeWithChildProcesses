#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder) {
    // Find file length and set up file reading
    FILE *inputPtr = fopen(input_file, "r");
    fseek(inputPtr, 0, SEEK_END);
    int inputFileLength = ftell(inputPtr);

    // Reset file pointer to start of file
    fseek(inputPtr, 0, SEEK_SET); 

    // Size of all but the last partitioned file
    int partitionedFileSize = inputFileLength/n; 
    
    // Initialize variables for file creation
    FILE *newFilePtr;
    char *txt = ".txt";
    char iAsChars[4];
    char fileName[9];
    char finalFileName[21] = "";
    char blockFileContents[partitionedFileSize];

    // Create and populate block files for all but final leaf
    for (int i = 0; i < (n-1); i++){
        // Clear and set up variables that change each iteration
        blockFileContents[0] = '\0';
        fileName[0] = '/';
        fileName[1] = '\0';
        finalFileName[0] = '\0';

        // Create file name
        strcat(finalFileName, blocks_folder);
        sprintf(iAsChars, "%d", i);
        strcat(fileName, iAsChars);
        strcat(fileName, txt);

        // Create and write file <i.txt>
        newFilePtr = fopen(strcat(finalFileName, fileName), "w");
        fread(blockFileContents, partitionedFileSize, 1, inputPtr);
        fwrite(blockFileContents, partitionedFileSize, 1, newFilePtr);

        fclose(newFilePtr);
    }
    
    // Clear variables and set up length of final data block
    char finalFileContents[partitionedFileSize+(inputFileLength%n)];
    fileName[0] = '/';
    fileName[1] = '\0';
    finalFileName[0] = '\0';

    // Create file name
    strcat(finalFileName, blocks_folder);
    sprintf(iAsChars, "%d", (n-1));
    strcat(fileName, iAsChars);
    strcat(fileName, txt);

    // Create and write <n-1.txt>
    newFilePtr = fopen(strcat(finalFileName, fileName), "w"); // Create nth file
    fread(finalFileContents, partitionedFileSize+(inputFileLength%n), 1, inputPtr);
    fwrite(finalFileContents, partitionedFileSize+(inputFileLength%n), 1, newFilePtr);

    fclose(newFilePtr);
    fclose(inputPtr);
}


// ##### DO NOT MODIFY THIS FUNCTION #####
void setup_output_directory(char *block_folder, char *hash_folder) {
    // Remove output directory
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"rm", "-rf", "./output/", NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed\n");
            exit(1);
        }
        exit(0);
    } else {
        wait(NULL);
    }

    sleep(1);

    // Creating output directory
    if (mkdir("output", 0777) < 0) {
        printf("ERROR: mkdir output failed\n");
        exit(1);
    }

    sleep(1);

    // Creating blocks directory
    if (mkdir(block_folder, 0777) < 0) {
        printf("ERROR: mkdir output/blocks failed\n");
        exit(1);
    }

    // Creating hashes directory
    if (mkdir(hash_folder, 0777) < 0) {
        printf("ERROR: mkdir output/hashes failed\n");
        exit(1);
    }
}

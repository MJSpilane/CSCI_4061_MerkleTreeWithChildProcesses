#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"

#define PATH_MAX 1024

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id>\n");
        return 1;
    }

    // Instantiate variables to store block folders and file paths
    char blocks_folder[30];
    strcat(blocks_folder, argv[1]);
    char hashes_folder[30];
    strcat(hashes_folder, argv[2]);
    
    char *txt = ".txt";
    char *out = ".out";

    int idNum = atoi(argv[4]);
    int nNum = atoi(argv[3]);

    // Non-leav node
    if (idNum < (nNum - 1)){
        // Fork to produce left child
        pid_t pid1 = fork();
        if (pid1 == 0){                                                                          
            char idStr[4];                                                              
            sprintf(idStr, "%d", ((idNum * 2) + 1));                                    
            char *args[] = {"./child_process", argv[1], argv[2], argv[3], idStr, NULL}; 
            execvp(args[0], args);                                                      
        }

        // Fork to produce right child
        pid_t pid2 = fork();
        if (pid2 == 0){                                                                        
            char idStr[4];                                                              
            sprintf(idStr, "%d", ((idNum * 2) + 2));                                    
            char *args[] = {"./child_process", argv[1], argv[2], argv[3], idStr, NULL}; 
            execvp(args[0], args);                                                      
        }

        // Wait for children
        waitpid(pid1, NULL, 0); 
        waitpid(pid2, NULL, 0); 

        // initialize char buffers for file contents
        char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
        char right_hash[SHA256_BLOCK_SIZE * 2 + 1];
        char result_hash[SHA256_BLOCK_SIZE * 2 + 1];
        char leftFileName[9];
        char rightFileName[9];
        char finalFileName[9];
        char nodeID[4];

        // Read .out file for left child (<currentId * 2 + 1>.out)
        hashes_folder[0] = '\0';
        strcat(hashes_folder, argv[2]);
        leftFileName[0] = '/';
        sprintf(nodeID, "%d", (idNum * 2 + 1));
        strcat(leftFileName, nodeID);
        strcat(leftFileName, out);
        strcat(hashes_folder, leftFileName);
        FILE *leftPtr = fopen(hashes_folder, "r");
        fread(left_hash, SHA256_BLOCK_SIZE * 2 + 1, 1, leftPtr);

        // Read .out file for right child (<currentId * 2 + 2>.out)
        hashes_folder[0] = '\0';
        nodeID[0] = '\0';
        strcat(hashes_folder, argv[2]);
        rightFileName[0] = '/';
        sprintf(nodeID, "%d", (idNum * 2 + 2));
        strcat(rightFileName, nodeID);
        strcat(rightFileName, out);
        strcat(hashes_folder, rightFileName);
        FILE *rightPtr = fopen(hashes_folder, "r");
        fread(right_hash, SHA256_BLOCK_SIZE * 2 + 1, 1, rightPtr);

        fclose(leftPtr);
        fclose(rightPtr);

        // Compute hash of Left and Right children
        compute_dual_hash(result_hash, left_hash, right_hash);
        
        // Create .out file for currentNode (<currentID>.out)
        hashes_folder[0] = '\0';
        nodeID[0] = '\0';
        strcat(hashes_folder, argv[2]);
        finalFileName[0] = '/';
        sprintf(nodeID, "%d", idNum);
        strcat(finalFileName, nodeID);
        strcat(finalFileName, out);
        strcat(hashes_folder, finalFileName);
        FILE *parentPtr = fopen(hashes_folder, "w");
        fwrite(result_hash, SHA256_BLOCK_SIZE * 2 + 1, 1, parentPtr);

        fclose(parentPtr);

    }
    else{ // Leaf node
        // Set up file names
        int fileNum = idNum - (nNum - 1);
        char fileStr[9];
        char tempFileStr[4];
        fileStr[0] = '/';
        char block_hash[SHA256_BLOCK_SIZE * 2 + 1];

        // Read block associated with leaf
        blocks_folder[0] = '\0';
        strcat(blocks_folder, argv[1]);
        sprintf(tempFileStr, "%d", fileNum);
        strcat(fileStr, tempFileStr);
        strcat(fileStr, txt);
        strcat(blocks_folder, fileStr);

        // Hash block data
        hash_data_block(block_hash, blocks_folder);

        // write to ~/output/hashes/<ID>.out
        fileStr[0] = '/';
        fileStr[1] = '\0';
        tempFileStr[0] = '\0';
        hashes_folder[0] = '\0';
        strcat(hashes_folder, argv[2]);
        sprintf(tempFileStr, "%d", idNum);
        strcat(fileStr, tempFileStr);
        strcat(fileStr, out);
        strcat(hashes_folder, fileStr);
        FILE *newFilePtr = fopen(hashes_folder, "w");
        fwrite(block_hash, SHA256_BLOCK_SIZE * 2 + 1, 1, newFilePtr); 

        fclose(newFilePtr);

        exit(0);

    }

    return 0;
}

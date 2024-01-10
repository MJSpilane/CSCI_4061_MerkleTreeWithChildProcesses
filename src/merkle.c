#include "utils.h"
#include "print_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// ##### DO NOT MODIFY THESE VARIABLES #####
char *blocks_folder = "output/blocks";
char *hashes_folder = "output/hashes";
char *visualization_file = "output/visualization.txt";


int main(int argc, char* argv[]) {
    if (argc != 3) {
        // N is the number of data blocks to split <file> into (should be a power of 2)
        // N will also be the number of leaf nodes in the merkle tree
        printf("Usage: ./merkle <file> <N>\n");
        return 1;
    }

    char *input_file;
    int n;

    // Ensure file exists
    input_file = argv[1];
    FILE *inputPtr = fopen(input_file, "r"); 
    if (inputPtr == NULL){
        printf("Incorrect input, file path does not exist\n");
        return -1;
    }
    
    // Ensure N is an integer
    for (int i = 0; i < strlen(argv[2]); i++){
        if (!isdigit(argv[2][i])){
            printf("Incorrect input, N is not a valid positive integer power of 2\n");
            return -1;
        }
    }

    // Check N is a power of 2
    int tempN = atoi(argv[2]);
    while (tempN != 1){
        // Ensure even and not 1
        if ((tempN % 2 == 1) && (tempN != 1)){ 
            printf("Incorrect input, N is not a valid positive integer power of 2\n");
            return -1;
        }
        tempN /= 2;
    }

    // N is valid, so convert to int
    n = atoi(argv[2]);
    
    // Check file length
    fseek(inputPtr, 0, SEEK_END);
    int inputFileLength = ftell(inputPtr);
    if (inputFileLength < n){ 
        printf("Incorrect input, file length must be at least equal to N value\n");
        return -1;
    }

    fclose(inputPtr);

    // ##### DO NOT REMOVE #####
    setup_output_directory(blocks_folder, hashes_folder);

    // Partition file data
    partition_file_data(input_file, n, blocks_folder);

    // Spawn root process for Merkle Tree creation
    pid_t pid = fork();
    if (pid == 0){
        char *args[] = {"./child_process", blocks_folder, hashes_folder, argv[2], "0", NULL};
        int test = execvp(args[0], args);

        // Check for failed root exec
        if (test == -1){
            printf("Error, exec failed at root node\n");
            return -1;
        }
    }

    // Ensure hashing is complete before visualizing Merkle Tree
    waitpid(pid, NULL, 0);

    // ##### DO NOT REMOVE #####
    #ifndef TEST_INTERMEDIATE
        // Visually display the merkle tree using the output in the hashes_folder
        print_merkle_tree(visualization_file, hashes_folder, n);
    #endif

    return 0;
}

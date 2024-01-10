# CSCI 4061 Fall 2023 Programming Assignment 1

Canvas PA Group 60

### Group Members:
- Matthew Breach (breac001) 
- Matthew Johnson (joh18723)
- Harrison Wallander (walla875)

### CSELabs Computer Used for Testing:
- csel-kh1260-14.cselabs.umn.edu

### Makefile Changes:
- None

### Additional Assumptions:
- None

### AI Code:
- None

### Individual Contributions:
- breac001
    - child_process.c
        - Leaf-node hashing
    - Exec research
    - debugging
    - code linting/styling
- joh18723
    - child_process.c
        - Non-leaf node hashing
    - Hashing research
    - debugging
- walla875
    - input validation
    - partition_file_data
    - initial fork/exec call
    - child_process.c
        - Algorithm
    - debugging

### Our Merkle Tree Algorithm:
1. In merkle.c:
```
    pid = fork()

    if (pid == 0):
        exec(./child_process with ID 0)
        Check for failed exec // Different from intermediate, added

    waitpid(pid) // Different from intermediate, added
```
 
2. In child_process.c:
```
/*
    Note: We have adjusted the formatting of
    our pseudocode to better reflect our
    final algorithm
*/
    if (ID < (n-1)): //Non-leaf process
        pid1 = fork()
        if (pid1 == 0)
            exec(./child_process with ID = (currentID*2+1))

        pid2 = fork()
        if (pid2 == 0)
            exec(./child_process with ID = (currentID*2+2))

        waitpid(pid1) // Different from intermediate, updated
        waitpid(pid2) // Different from intermediate, updated

        leftData = read(leftChild)
        rightData = read(rightChild)
        hashedData = hash(leftData, rightData)
        write hashedData to currentNode

    else: //Leaf process
        blockContents = read(bock_(ID-(n-1)))
        hashedBlock = hash(blockContents)
        write hashedBlock to currentNode
        exit()
```

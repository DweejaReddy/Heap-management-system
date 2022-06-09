//  BT20CSE032_DWEEJA REDDY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_SIZE 999        // Maximum Heap SIize
#define NAME_SIZE 100        // Maximum Variable name length

struct Node                 // Node Structure
{
    int start;              // Start index of the variable
    int end;                // End index of the variable
    char var_name[NAME_SIZE];  // Variable name
    struct Node *prev;         // Previous node
    struct Node *next;         // Next node
    int isFree;             // Is the node free or not
};
struct heap{
    struct Node *lptr;    // Pointer to the leftmost node
};


struct Node *allocatedNode(int start, int end, char name[]);  // Function to Allocate a node
struct Node *freeNode(int start, int end);    // Function to free a node
int isFull(struct heap *heap);                // Function to check if the heap is full
int isEmpty(struct heap *heap);               // Function to check if the heap is empty
int isVarPresent(struct heap *heap, char name[]);    // Function to check if the block with given variable is present in the heap
void allocateNodeByFirstFit(struct heap *heap, int allocatedSize, char name[]);       // Function to allocate a node by first fit
void freeSpace(struct heap *heap, char name[]);            // Function to free a node by name
void printAllocatedNode(struct heap *heap);                // Function to print the allocated nodes
void printFreeNode(struct heap *heap);                     // Function to print the free nodes
void printAllNodes(struct heap *heap);                     // Function to print all the nodes
void deleteNode(struct heap *heap);                        // Function to delete the heap


struct Node *allocatedNode(int start, int end, char name[])
{
    struct Node *ptr;                   // Pointer to the node
	ptr = (struct Node*)malloc(sizeof(struct Node));       // Allocate memory for the node
    if(ptr!=NULL) {            // Check if the memory is allocated
        ptr->start = start;    // Assign the start index
    	ptr->end = end;        // Assign the end index
    	strcpy(ptr->var_name, name);     // Assign the variable name
        ptr->isFree = 0;                 // Assign the node as allocated
    	ptr->next = NULL;               // Assign the next node as NULL
        ptr->prev = NULL;               // Assign the previous node as NULL
    }
	return ptr;
}

struct Node *freeNode(int start, int end)
{
    struct Node *nptr;       
    nptr = (struct Node*)malloc(sizeof(struct Node));
    if(nptr!=NULL) {
        nptr->start = start;
        nptr->end = end;
        nptr->isFree = 1;               // Assign the node as free
        nptr->next = NULL;
        nptr->prev = NULL;
    }
    return nptr;
}

int isFull(struct heap *heap)
{
    int retVal = 1;             // Assume the heap is full
    struct Node *lptr = heap->lptr;    // Get the leftmost node
    while(lptr!=NULL && !(lptr->isFree)) {       // Check if the node is free
        lptr = lptr->next;               
    }
    if(lptr!=NULL)
        retVal=0;                                
    return retVal;
}

int isEmpty(struct heap *heap)
{
    int retVal = 1;                     // Assume the heap is empty
    struct Node *lptr = heap->lptr;             
    while(lptr!=NULL && lptr->isFree) {         // Check if the node is free
        lptr = lptr->next;
    }
    if(lptr!=NULL)                              
        retVal=0;
    return retVal;
}

int isVarPresent(struct heap *heap, char name[])
{
    struct Node *lptr = heap->lptr;             
    int retVal = 0;                           // Assume the variable is not present
    while(lptr!=NULL) {                       // Check if the variable is present
        if(!(lptr->isFree) && strcmp(name, lptr->var_name)==0)      // If the variable is present
            retVal = 1;                       // Assign the return value as 1
        lptr = lptr->next;  
    }
    return retVal;
}

void allocateNodeByFirstFit(struct heap *heap, int allocatedSize, char name[])
{
    struct Node *lptr = heap->lptr;
    if(lptr == NULL) {             
        printf("No free space is created.\n");
    }
    else {
        struct Node *nptr = lptr, *ptr = NULL;  // Initialize the pointers
        int done = 0;
        while(nptr!=NULL && !done) {            // Check if the node is free
            if(nptr->isFree && (nptr->end - nptr->start + 1) >= allocatedSize) { // If the node is free and the size is greater than the allocated size
                ptr = nptr;
                done = 1;
            }
            nptr = nptr->next;
        }
        int index;              
        if(ptr!=NULL){                                  // If the node is found
            index = ptr->start + allocatedSize -1;  //index of last element of allocated node
        } 
        if(ptr == NULL || ( ptr->next==NULL && (index > HEAP_SIZE) ) ) {       // If the node is not free or the allocated node is out of heap size
            printf("Sorry!! No free space is available.\n");
        }
        else {
            struct Node *aptr = allocatedNode(ptr->start, index, name);       
            if(aptr==NULL) {                                // If the memory is not allocated
                printf("Heap is full.\n");
            }
            else {
                if(index == ptr->end) {                    // If the allocated node is the last node
                    lptr = aptr;
                    if(ptr->prev!=NULL) {                  // If the allocated node is not the first node
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    if(ptr->next!=NULL) {                 // If the allocated node is not the last node
                        ptr->next->prev = lptr;
                        lptr->next = ptr->next;
                    }
                    free(ptr);
                }
                else {
                    ptr->start = index+1;               // If the allocated node is not the last node
                    lptr = aptr;
                    if(ptr->prev!=NULL) {               // If the allocated node is not the first node
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    lptr->next = ptr;                   // If the allocated node is not the last node
                    ptr->prev = lptr;
                }
                printf("Space is allocated.\n");
                while(lptr->prev != NULL) {             // Move the pointer to the leftmost node
                    lptr = lptr->prev;
                }
                heap->lptr = lptr;
            }
        }
    }
}

void freeSpace(struct heap *heap, char name[])
{
    struct Node *lptr = heap->lptr, *aptr = NULL;       
    if(isEmpty(heap)) {                         // If the heap is empty
        printf("No space is allocated.\n");
    }
    else {
        int done = 0;
        while (lptr!=NULL && !done) {
            if(!(lptr->isFree) && strcmp(name,lptr->var_name)==0) {   /// If the variable is present
                aptr = lptr;            // aptr is the allocated node
                done = 1;
            }
            lptr = lptr->next;
        }
        if(aptr==NULL) {            // If the variable is not present
            printf("There is no element with the given variable name.\n");
        }
        else {
            struct Node *ptr;
            aptr->isFree = 1;
            if(aptr->prev!=NULL && aptr->prev->isFree) {        // If the previous node is free
                ptr = aptr->prev;   //ptr is previous node of aptr
                aptr->start = ptr->start;
                if(ptr->prev != NULL)
                {
                    ptr->prev->next = aptr;         // If the previous node is not the first node
                }   
                aptr->prev = ptr->prev;
                free(ptr);
            }
            if(aptr->next!=NULL && aptr->next->isFree) {        // If the next node is free
                ptr = aptr->next;
                aptr->end = ptr->end;
                if(ptr->next != NULL)
                {
                    ptr->next->prev = aptr;         //  If the next node is not the last node
                }
                aptr->next = ptr->next;
                free(ptr);
            }
            lptr = aptr;
            while(lptr->prev != NULL) {            // Move the pointer to the leftmost node
                lptr = lptr->prev;
            }
            printf("Space is freed.\n");
            heap->lptr = lptr;                     
        }
    }
}

void printAllocatedNode(struct heap *heap)
{
    if(isEmpty(heap)) {         // If the heap is empty
        printf("No allocated space.\n");
    }
    else {
        struct Node *ptr = heap->lptr;      
        printf("Start\tEnd\tSize\tVariableName\n");
		while(ptr!=NULL) {
			if(!(ptr->isFree))
				printf("%d\t%d\t%d\t%s\n", ptr->start, ptr->end, ptr->end-ptr->start+1, ptr->var_name);
			ptr = ptr->next;
		}
    }
}

void printFreeNode(struct heap *heap)
{
    if(isFull(heap)) {          // If the heap is full
        printf("No free space.\n");
    }
    else {
        struct Node *ptr = heap->lptr;
        printf("Start\tEnd\tSize\n");
		while(ptr!=NULL) {
			if(ptr->isFree)
				printf("%d\t%d\t%d\n", ptr->start, ptr->end, ptr->end-ptr->start+1);
			ptr = ptr->next;
		}
    }
}

void printAllNodes(struct heap *heap)
{
    printf("---------------------------------------------------------------");
    printf("\nAllocated Nodes Details: \n");
    printAllocatedNode(heap);
    printf("---------------------------------------------------------------");
    printf("\nFree Nodes Details: \n");
    printFreeNode(heap);
    printf("---------------------------------------------------------------");
    printf("\n");
}

void deleteNode(struct heap *heap)
{
    struct Node *ptr;
    ptr = heap->lptr;       // ptr is the pointer to the leftmost node
    while(heap->lptr!=NULL) {       // Delete the leftmost node
        ptr = heap->lptr;           
        heap->lptr = heap->lptr->next;
        free(ptr);
    }
    heap->lptr = NULL;
}

int main()
{
    struct heap mainHeap;              // mainHeap is the heap
    struct heap *ptr1 = &mainHeap;    // ptr1 is the pointer to the heap
	struct Node *fptr = freeNode(0,HEAP_SIZE);   // fptr is the pointer to the first free node
    if(fptr == NULL) {              // If the heap is full
        printf("No space in the Heap.\n");
    }
    else {          // If the heap is not full
        ptr1->lptr = fptr;
    	int choice, allocatedSize=0; // size (in bytes)
    	char var_name[NAME_SIZE];
        printf("Total heap space available is %d bytes\n", fptr->end-fptr->start+1); 

        while(1) {
            printf("------------------------------------------");
    		printf("\n==> Enter your choice number as below:\n==> 1) Allocate Space\n==> 2) Free a Node Space\n==> 3) Print the Nodes\n==> 4) Exit the program\n");
            printf("------------------------------------------\n");
            printf("Enter your choice: ");            
    		scanf("%d", &choice);
            
            
            if(choice==1){
                printf("Enter the variable name : ");
                scanf("%s", var_name);
                printf("Enter the size (in bytes) you want to allocate : ");	
                scanf("%d", &allocatedSize);  
                printf("\n");	
                if(isVarPresent(ptr1, var_name))
                    printf("Sorry!! Variable name already exists. Try with other variable name.\n");
                else
                    allocateNodeByFirstFit(ptr1, allocatedSize, var_name);
            }
            else if(choice==2){
                printf("Enter the variable name : ");
                scanf("%s", var_name);
                printf("\n");
                if(!isVarPresent(ptr1, var_name))
                    printf("Variable name does not exist!!\n");
                else
                    freeSpace(ptr1, var_name);
            }
            else if(choice==3){
                printAllNodes(ptr1);
            }
            else if(choice==4){
                printf("EXITED PROGRAM SUCCESSFULLY!!\n");
                return 0;
            }
            else
                printf("Invalid choice!! Try again!!\n");
        }
        deleteNode(ptr1); // to free space at the end
    }
    return 0;
}
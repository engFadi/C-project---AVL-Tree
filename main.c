//Fadi Bassous
//1221005
//sec 2

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WORD_SIZE 100

typedef struct AVLnode *AVLNode;

struct AVLnode {
    char Word[WORD_SIZE];
    AVLNode Left;
    AVLNode Right;
    int Height; // Balance information
    int frequency;
};

typedef struct HashTable *Hash;

struct HashTable {
    char WordH[WORD_SIZE];
    int frequencyH;
};


 int size = 10; // Initial size of hash table
int countW = 0; // Count the number of words


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode MakeEmpty(AVLNode T);

AVLNode Find(char *x, AVLNode T);

AVLNode FindMin(AVLNode T);

AVLNode FindMax(AVLNode T);

int Height(AVLNode P);

int Max(int Lhs, int Rhs);

AVLNode SingleRotateWithLeft(AVLNode K2);

AVLNode SingleRotateWithRight(AVLNode K1);

AVLNode DoubleRotateWithLeft(AVLNode K3);

AVLNode DoubleRotateWithRight(AVLNode K1);

void toLower(char *ch);// Convert character to lowercase if uppercase

AVLNode Insert(char *X, AVLNode T);

void PrintInOrder(AVLNode t);

AVLNode readFromFile(AVLNode T);

AVLNode Delete(char *X, AVLNode T);

AVLNode BalanceAfterDelete(char *X, AVLNode T); // Balance AVL tree after deletion

Hash createHash();

Hash insertHash(char *word, int frequency, Hash H);

int hash(char *key, int TableSize);

void AVLtoHash(AVLNode avl, Hash hash);// Function to insert AVL data  to Hash Table

void Rehashing(Hash H);

int search(char * word,Hash H);//search for a word and retrieve the frequency

Hash DeleteHash(char *word,Hash H);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    AVLNode T = NULL;
    AVLNode tree = readFromFile(T);
    printf("Test to print the tree (In-Order):\n");
    PrintInOrder(tree);
    printf("\nTotal words: %d\n", countW);

    Hash hashTable = createHash();
    AVLtoHash(tree, hashTable);
   // insertHash("fadi",1,hashTable);

    //DeleteHash("notebook",hashTable);

    // Printing hash table for testing
    printf("Hash Table Contents:\n");
    for (int i = 0; i < size; i++) {
        if (hashTable[i].frequencyH > 0) {
            printf("Index %d: Word = %s, Frequency = %d\n", i, hashTable[i].WordH, hashTable[i].frequencyH);
        }
    }
  //  int s=search("notebook",hashTable);
  //  printf("%d",s);

    return 0;
}

// AVL-------------------------------------------------------------------------------------------------------------------
AVLNode readFromFile(AVLNode T) {
    FILE *fp;
    char line[100];

    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, " \n"); // Split the words by spaces
        while (token != NULL) {
            T = Insert(token, T);
            countW++;
            token = strtok(NULL, " \n");
        }
    }

    fclose(fp);
    return T;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode MakeEmpty(AVLNode T) {
    if (T != NULL) {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode Find(char *x, AVLNode T) {
    if (T == NULL)
        return NULL;

    if (strcmp(x, T->Word) < 0)
        return Find(x, T->Left);
    else if (strcmp(x, T->Word) > 0)
        return Find(x, T->Right);
    else
        return T;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode FindMin(AVLNode T) {
    if (T == NULL)
        return NULL;
    else if (T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode FindMax(AVLNode T) {
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;

    return T;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Height(AVLNode P) {
    if (P == NULL)
        return -1;
    else
        return P->Height;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Max(int Lhs, int Rhs) {
    return Lhs > Rhs ? Lhs : Rhs; // Find the max value then return it
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode SingleRotateWithLeft(AVLNode K2) {
    AVLNode K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1; // Edited

    return K1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode SingleRotateWithRight(AVLNode K1) {
    AVLNode K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Right), Height(K2->Left)) + 1; // Edited

    return K2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode DoubleRotateWithLeft(AVLNode K3) {
    /* Rotate between K1 and K2 */
    K3->Left = SingleRotateWithRight(K3->Left);

    /* Rotate between K3 and K2 */
    return SingleRotateWithLeft(K3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode DoubleRotateWithRight(AVLNode K1) {
    /* Rotate between K3 and K2 */
    K1->Right = SingleRotateWithLeft(K1->Right);

    /* Rotate between K1 and K2 */
    return SingleRotateWithRight(K1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void toLower(char *ch) {
    if (ch == NULL) return;

    for (int i = 0; ch[i] != '\0'; i++)
        if (ch[i] >= 'A' && ch[i] <= 'Z') {
            ch[i] = ch[i] + 32;
        }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode Insert(char *X, AVLNode T) {
    toLower(X);
    if (T == NULL) {
        /* Create and return a one-node tree */
        T = malloc(sizeof(struct AVLnode));
        if (T == NULL)
            printf("Out of space!!!");
        else {
            // Initialize the new node
            strcpy(T->Word, X);
            T->frequency = 1;
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    } else if (strcmp(X, T->Word) < 0) {
        T->Left = Insert(X, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
            if (strcmp(X, T->Left->Word) < 0)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
    } else if (strcmp(X, T->Word) > 0) {
        T->Right = Insert(X, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
            if (strcmp(X, T->Right->Word) > 0)
                T = SingleRotateWithRight(T);
            else
                T = DoubleRotateWithRight(T);
    } else {
        T->frequency++; // Word is already in the tree then increment frequency
        countW--; // Word is already in the tree then don't count
    }

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintInOrder(AVLNode t) { // In-order print to see the elements
    if (t != NULL) {
        PrintInOrder(t->Left);
        printf("%s frequency= %d\t", t->Word, t->frequency);
        PrintInOrder(t->Right);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode Delete(char *X, AVLNode T) {
    AVLNode TmpCell;
    if (T == NULL) {
        printf("Element not found");
        return T;
    } else if (strcmp(X, T->Word) < 0) /* Go left */
        T->Left = Delete(X, T->Left);
    else if (strcmp(X, T->Word) > 0) /* Go right */
        T->Right = Delete(X, T->Right);
    else { /* Found element to be deleted */
        if (T->Left && T->Right) { /* Two children */
            // Replace with smallest in right subtree
            TmpCell = FindMin(T->Right);
            strcpy(T->Word, TmpCell->Word);
            T->Right = Delete(T->Word, T->Right);
        } else { /* One child */
            TmpCell = T;
            if (T->Left == NULL) /* Also handles 0 children */
                T = T->Right;
            else if (T->Right == NULL)
                T = T->Left;
            free(TmpCell);
        }
    }
    return T;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AVLNode BalanceAfterDelete(char *X, AVLNode T) {
    T = Delete(X, T);
    countW--; // Increase a counter by 1
    if (T == NULL) {
        return T;
    }
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;

    if (Height(T->Left) - Height(T->Right) == 2) {
        if (Height(T->Left->Left) >= Height(T->Left->Right)) {
            T = SingleRotateWithLeft(T);
        } else {
            T = DoubleRotateWithLeft(T);
        }
    } else if (Height(T->Right) - Height(T->Left) == 2) {
        if (Height(T->Right->Right) >= Height(T->Right->Left)) {
            T = SingleRotateWithRight(T);
        } else {
            T = DoubleRotateWithRight(T);
        }
    }
    return T;
}

//Hash------------------------------------------------------------------------------------------------------------------
Hash createHash() {
    Hash H = (Hash) malloc(size * sizeof(struct HashTable));
    if (H == NULL) {
        printf("Out of memory\n");
        return NULL;
    }
    for (int i = 0; i < size; i++) {// Initialize word and frequencyH
        H[i].WordH[0] = '\0';
        H[i].frequencyH = 0;
    }
    return H;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int hash(char *key, int TableSize) {
    unsigned int hashValue = 0;
    while (*key != '\0') {
        hashValue = (hashValue << 5) + *key++;
    }
    return (hashValue % TableSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Hash insertHash(char *word, int frequency, Hash H) {
    if (countW > size) {
        Rehashing(H);
        return H;
    }
    int index = hash(word, size);
    while (H[index].frequencyH != 0) { //Linear Hashing, if frequency not equal 0 then there is a Collision
        index = (index + 1) % size;
    }
    strcpy(H[index].WordH, word);
    H[index].frequencyH = frequency;
    return H;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AVLtoHash(AVLNode avl, Hash hash) {
    if (avl != NULL) {
        AVLtoHash(avl->Left, hash);
        insertHash(avl->Word, avl->frequency, hash); // Insert each node into hash table
        AVLtoHash(avl->Right, hash);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int isPrime(int n) {
    if (n == 0 || n == 1)
        return 0;
    if (n == 2 || n == 3)
        return 1;

    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Rehashing(Hash H) {
    Hash temp=H;
   int oldSize=size;
    size = size * 2;
    while (isPrime(size) == 0) {
        size++;
    }
    Hash newHash  = (Hash) malloc(size * sizeof(struct HashTable));
    if (newHash == NULL) {
        printf("Out of memory\n");
        return;
    }

    for (int i = 0; i < size; i++) {// Initialize word and frequencyH
        newHash[i].WordH[0] = '\0';
        newHash[i].frequencyH = 0;
    }

    for (int i = 0; i < oldSize; i++) {
            int newIndex = hash(temp[i].WordH, size);
            while (newHash[newIndex].frequencyH != 0) { //Linear Hashing, if frequency not equal 0 then there is a Collision
                newIndex = (newIndex + 1) % size;
            }
            strcpy(newHash[newIndex].WordH, temp[i].WordH);
            newHash[newIndex].frequencyH = temp[i].frequencyH;
    }
    free(H);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int search(char * word,Hash H)
{

    int index = hash(word,size);
    return H[index].frequencyH;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Hash DeleteHash(char *word,Hash H)
{
    int index = hash(word,size);
    H[index].WordH[0] = '\0';
    H[index].frequencyH = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void WordCount(Hash H)
//{
//    int count=0;
//    for(int i=0;i<size;i++)
//    {
//        if(H[i].frequencyH==1)
//        {
//            count++;
//        }
//    }
//    printf("total number of unique words:%d\n",count);
//    int max=H.frequencyH
//}

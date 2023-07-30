#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10 // You can choose an appropriate size for the hash table

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType* next;
};

// Fill out this structure
struct HashType
{
	struct RecordType* records;
};

// Compute the hash function
int hash(int x)
{
	return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct RecordType *currRecord;

	for (i = 0; i < hashSz; ++i)
	{
		if (pHashArray[i].records != NULL)
		{
			printf("Index %d ->", i);
			currRecord = pHashArray[i].records;
			while (currRecord != NULL)
			{
				printf(" %d, %c, %d ->", currRecord->id, currRecord->name, currRecord->order);
				currRecord = currRecord->next;
			}
			printf(" NULL\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation
	struct HashType hashArray[HASH_SIZE];

	// Initialize the hashArray with NULL pointers
	for (int i = 0; i < HASH_SIZE; ++i)
	{
		hashArray[i].records = NULL;
	}

	// Populate the hash table with chaining
	struct RecordType *currRecord;
	int hashIndex;
	for (int i = 0; i < recordSz; ++i)
	{
		currRecord = &pRecords[i];
		hashIndex = hash(currRecord->id);

		// Add the record to the hash table using chaining
		currRecord->next = hashArray[hashIndex].records;
		hashArray[hashIndex].records = currRecord;
	}

	printf("\nHash Table:\n");
	displayRecordsInHash(hashArray, HASH_SIZE);

	// Free allocated memory
	free(pRecords);

	return 0;
}

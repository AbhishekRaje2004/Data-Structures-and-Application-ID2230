#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CreateArray(const char *filename, int *array, int count)
{
    FILE *file = fopen(filename, "r");

    int numRead = 0;
    for (int i = 0; i < count; i++)
    {
        if (fscanf(file, "%d", &array[i]) == 1)
        {
            numRead++;
        }
        else
        {
            break;
        }
    }

    fclose(file);
}

int hashfunction1(int key)
{
    return 9 * key + 1;
}

int hashfunction2(int key)
{
    return 13 * key + 5;
}

void Insert(int HashTable[], int size, int element)
{
    int u = hashfunction1(element) % size;
    if (HashTable[u] == 0)
    {
        HashTable[u] = element;
        return;
    }
    int v = hashfunction2(element) % size;
    for (int i = 0; i < size; i++)
    {
        u += v;
        u = u % size;
        if (HashTable[u] == 0)
        {
            HashTable[u] = element;
            break;
        }
    }
}

void SubArray(int *originalArray, int *shrunkenArray, int originalSize, int newSize)
{
    srand(time(NULL));

    for (int i = 0; i < newSize; i++)
    {
        int randomIndex = rand() % originalSize;
        shrunkenArray[i] = originalArray[randomIndex];
    }
}

int Search(int HashTable[], int size, int element, int *probes)
{
    int u = hashfunction1(element) % size;
    if (HashTable[u] == element)
    {
        (*probes)++;
        return 1;
    }
    int v = hashfunction2(element) % size;
    int i;
    for (i = 0; i < size; i++)
    {
        u += v;
        u = u % size;
        (*probes)++;
        if (HashTable[u] == element)
        {
            return 1;
        }
    }
    return 0;
}

void DeleteElement(int HashTable[], int size, int element)
{
    int u = hashfunction1(element) % size;
    if (HashTable[u] == element)
    {
        HashTable[u] = 0;
        return;
    }
    int v = hashfunction2(element) % size;
    for (int i = 0; i < size; i++)
    {
        u += v;
        u = u % size;
        if (HashTable[u] == element)
        {
            HashTable[u] = 0;
            return;
        }
    }
}

int main()
{
    const char *filename = "RandomNumbers.txt";
    int count = 1000;
    int searchcount = 100;
    int size = 1001;
    int HashTable[size];
    for (int i = 0; i < size; i++)
    {
        HashTable[i] = 0;
    }
    int RandomArray[count];
    int SearchArray[count];
    CreateArray(filename, RandomArray, count);
    for (int i = 0; i < count; i++)
    {
        Insert(HashTable, size, RandomArray[i]);
    }
    SubArray(RandomArray, SearchArray, count, searchcount);

    FILE *outputFile = fopen("output size=1001.txt", "w");

    int probes = 0;
    float sum_probes = 0;

    fprintf(outputFile, "Random Array Elements:\n");
    for (int i = 0; i < count; i++)
    {
        fprintf(outputFile, "%d ", RandomArray[i]);
    }
    fprintf(outputFile, "\n");

    fprintf(outputFile, "Search Array Elements:\n");
    for (int i = 0; i < searchcount; i++)
    {
        fprintf(outputFile, "%d ", SearchArray[i]);
    }
    fprintf(outputFile, "\n");

    for (int i = 0; i < searchcount; i++)
    {
        int x = Search(HashTable, size, SearchArray[i], &probes);
        if (x == 1)
        {
            fprintf(outputFile, "Element %d found. Probes: %d\n", SearchArray[i], probes);
            sum_probes += probes;
        }
        probes = 0;
    }
    fprintf(outputFile, "Average Number of Probes for Successful Probes %f\n", sum_probes / searchcount);

    for (int i = 0; i < searchcount; i++)
    {
        DeleteElement(HashTable, size, SearchArray[i]);
    }

    int newSize = 0;
    for (int i = 0; i < count; i++)
    {
        int found = 0;
        for (int j = 0; j < searchcount; j++)
        {
            if (RandomArray[i] == SearchArray[j])
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            RandomArray[newSize] = RandomArray[i];
            newSize++;
        }
    }
    count = newSize;
    SubArray(RandomArray, SearchArray, count, searchcount);

    probes = 0;
    sum_probes = 0;

    for (int i = 0; i < searchcount; i++)
    {
        int x = Search(HashTable, size, SearchArray[i], &probes);
        if (x == 1)
        {
            fprintf(outputFile, "Element %d found. Probes: %d\n", SearchArray[i], probes);
            sum_probes += probes;
        }
        probes = 0;
    }
    fprintf(outputFile, "Average Number of Sucessful Probes for Modiefied Array %f\n", sum_probes / searchcount);

    sum_probes = 0;
    int t = 0;
    while (t != 100)
    {
        int key = rand() % count;
        int element = RandomArray[key];
        int x = Search(HashTable, size, element, &probes);
        if (x == 0) {
            fprintf(outputFile, "Element %d not found. Probes: %d\n", element, probes);
            t++;
        }

        sum_probes += probes;
        probes = 0;
    }
    fprintf(outputFile, "Average Number of Probes for Unsucessful Probes  %f\n", sum_probes / searchcount);

    printf("Files Generated");
    fclose(outputFile);
    return 0;
}

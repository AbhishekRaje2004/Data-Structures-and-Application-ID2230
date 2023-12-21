#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateNumbersToFile(const char* filename, int count) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening the file");
        return;
    }

    srand(time(NULL)); 

    for (int i = 0; i < count; i++) {
        int randomNum = rand() % 1000000 + 1; 
        fprintf(file,"%d\n",randomNum);
    }

    fclose(file);
}

int main() {
    const char* filename = "RandomNumbers.txt";
    int count = 1000;

    generateNumbersToFile(filename, count);

    return 0;
}

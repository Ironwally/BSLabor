#include <stdio.h>

int main() {

    char input[100];
    printf("Awaiting string input...\n");
    scanf("%s", input);

    int i = 0;
    while (input[i]!='\0') {
        printf("%c", input[i]);
        i++;
        if (input[i]==' ') {
            printf("\n");
            i++;
        }
    }

    return(0);
}
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *ptr;
    char buf[100];
    char fifo_name[100];
    // bierzemy nazwe do fifo
    char *name = argv[1];
    char *first_word;
    char *second_word;
    char *third_word;
    printf("nazwa: %s\n", name);
    // otweiramy  isprawdzamt plik
    ptr = fopen("fifo.txt", "r");
    if (NULL == ptr)
    {
        printf("error with opening a file \n");
    }
    while (fgets(buf, 20, ptr) != NULL)
    {
        first_word = strtok(buf, " ");
        second_word = strtok(NULL, " ");
        third_word = strtok(NULL, "\n");
        // printf("%s \n", first_word);
        // printf("%s \n", name);
        // printf("%s \n", third_word);
        // szukamy jaka nazwe wprowadzul uzytkownik
        if (strcmp(first_word, name) == 0)
        {
            strcpy(fifo_name, third_word);
            printf("%s \n", fifo_name);
        }
    }
    int status_main;
    int fifo_queue;
    // tutaj jakiegos scanf, zeby miec duzo procesow
    char name_client[100] = "example";
    char buf_message[500];
    // status_main = mkfifo(fifo_name, 0640);
    fifo_queue = open(fifo_name, O_RDWR);
    int status = mkfifo(name_client, 0640);
    printf("status klienta%d \n", status);
    int client = open(name_client, O_RDWR);
    printf("status %d", status_main);
    write(fifo_queue, name_client, 100);
    read(client, buf_message, 500);
    printf("%s\n", buf_message);
    close(client);

    return 0;
}
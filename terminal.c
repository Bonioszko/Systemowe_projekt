#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    FILE *ptr;
    char buf[100];
    char buf_child[100];
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
        perror("blad przy otwieraniu pliku");
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

    int status;
    int fifo_queue;
    int fifo_queue_child;
    int id;
    int id_main;
    int status_proccess_to_create;
    int name_proccess_to_create;
    char message[100];
    char name_to_proccess[100];
    char command[100];
    char fifo_to_proccess[100];
    char fifo_to_write[100];
    char buf_message[500];
    int rs;

    id_main = fork();
    if (id_main == 0)
    {
        // tworzenie glownej kolejki danego procesu
        status = mkfifo(fifo_name, 0640);
        printf("dd%sdd \n", fifo_name);
        printf("Status kplejki %d \n", status);
        perror("Blad");
        if (status == -1)
        {
            perror("blad orzy tworzeniu koljki");
        }
        printf("kolejka %d \n", fifo_queue);
        // otweiranie glownej kolejki
        fifo_queue = open(fifo_name, O_RDWR);
        if (fifo_queue == -1)
        {
            perror("blad orzy otwieraniu koljki");
        }
        while (1)
        {
            // zczytywanie czy cos przyszlo do kolejki
            rs = read(fifo_queue, buf_message, 500);
            if (rs == -1)
            {
                perror("blad przy zczytaniu z kolejki");
            }

            printf("odczytano: %d\n", rs);
            // tutaj wstawic rs>0
            if (rs > 0)
            {
                printf("dostalełem %s\n", buf_message);
                id = fork();
                if (id == 0)
                {
                    // wykonywanie danego polecenia
                    printf("polo\n");
                    fifo_queue_child = open(buf_message, O_WRONLY);
                    if (fifo_queue_child == -1)
                    {
                        perror("blad przy otwieraniu kolejki");
                    }

                    close(STDOUT_FILENO);
                    dup(fifo_queue_child);
                    close(fifo_queue_child);
                    execlp("ls", "ls", NULL);
                    unlink(buf_message);
                }
            }

            sleep(1);
        }
    }
    else
    {
        while (1)
        {
            printf("podaj komende: ");
            /// tutaj bedzie czekal na uczytkownika i zczytuje z lini kilka danych
            scanf("%s %s %s", name_to_proccess, command, fifo_to_proccess);
            printf("koko %s %s %s\n", name_to_proccess, command, fifo_to_proccess);
            // ronienei kolejki o nazwie wprowadzonej
            status_proccess_to_create = mkfifo(fifo_to_proccess, 0640);
            if (status_proccess_to_create == -1)
            {
                perror("Blad w tworzeniu kolejki:");
            }
            // // tutaj zmienic na fifo_to_proccess
            // name_proccess_to_create = open(fifo_to_proccess, O_RDWR);
            if (name_proccess_to_create == -1)
            {
                perror("otwarcie kolejki");
            }
            // z pliku musimy przerobić usr na kolejke
            ptr = fopen("fifo.txt", "r");
            if (NULL == ptr)
            {
                perror("blad przy otwieraniu pliku");
                exit(1);
            }
            while (fgets(buf_child, 20, ptr) != NULL)
            {

                first_word = strtok(buf_child, " ");
                second_word = strtok(NULL, " ");
                third_word = strtok(NULL, "\n");
                printf("%s \n", first_word);
                printf("%s \n", third_word);
                // szukamy jaka nazwe wprowadzul uzytkownik
                if (strcmp(first_word, name_to_proccess) == 0)
                {
                    strcpy(fifo_to_write, third_word);
                    printf("nazwa %s \n", fifo_to_write);
                }
            }
            printf(" fifo:name %s \n", fifo_to_write);
            // otweiramy glowna kolejke
            fifo_queue = open(fifo_to_write, O_WRONLY);
            // na razie na sztywno, ale musze zmienic
            if (fifo_queue == -1)
            {
                perror("blad przy otwarciu pliku do pisania");
                exit(1);
            }

            if (write(fifo_queue, fifo_to_proccess, 100) == -1)
            {
                perror("blad w zapisie do kolejki");
                exit(1);
            }

            // odbieramy z kolejki wiadomosc i ja wyswietlamy
            name_proccess_to_create = open(fifo_to_proccess, O_RDONLY);
            if (name_proccess_to_create == -1)
            {
                perror("blad przy zapsiie pliku do odczytu");
                exit(1);
            }

            if (read(name_proccess_to_create, buf_message, 500) == -1)
            {
                perror("blad przy odczycie z kolejki");
                exit(1);
            }

            printf("%s\n", buf_message);
            close(name_proccess_to_create);
            unlink(fifo_to_proccess);
            close(fifo_queue);
        }
    }

    close(fifo_queue);
    unlink(fifo_to_write);
    printf("1 \n");
    return 0;
}

// int pdesk;
// int status;

// status = mkfifo("serwer2", 0640);
// printf("Status kolejki: %d\n", status);
// pdesk = open("serwer2", O_RDONLY);
// printf("Polaczono %d\n", pdesk);
// char buf[100];
// int cdesk;
// int id;
// int rs;
// while (1)
// {
//     rs = read(pdesk, buf, 100);
//     printf("Odczytano: %d\n", rs);
//     if (rs > 0)
//     {
//         printf("Message received! %s\n", buf);
//         id = fork();
//         if (id == 0)
//         {
//             cdesk = open(buf, O_WRONLY);
//             close(STDOUT_FILENO);
//             dup(cdesk);
//             close(cdesk);
//             execlp("ls", "ls", NULL);
//         }
//         close(cdesk);
//     }
//     printf("working...\n");
//     sleep(1);
// }
// close(pdesk);

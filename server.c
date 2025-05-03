#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 4

// Structure to hold client information
typedef struct {
    int socket;
    struct sockaddr_in address;
} client_info_t;

// Function to handle client connection
void* handle_client(void* arg) {
    client_info_t* client = (client_info_t*)arg;
    char buffer[1024] = {0};
    FILE *fptr;

    printf("Yeni istemci baglandi! Thread ID: %lu\n", pthread_self());

    while(1) {
        memset(buffer, 0, sizeof(buffer));  // Clear buffer before reading
        int bytes_read = read(client->socket, buffer, 1024);
        
        if (bytes_read <= 0) {
            printf("İstemci bağlantıyı kapattı. Thread ID: %lu\n", pthread_self());
            break;
        }
        
        if (strcmp(buffer, "q") == 0) {
            printf("İstemci çıkış yapıyor. Thread ID: %lu\n", pthread_self());
            break;
        }

        // Read file
        fptr = fopen(buffer, "r");
        if(fptr != NULL) {
            char ch;
            while ((ch = fgetc(fptr)) != EOF) {
                send(client->socket, &ch, 1, 0); 
            }
            // Send null character to indicate end of file
            ch = '\0';
            send(client->socket, &ch, 1, 0);
        } else {
            printf("Not able to open the file.");
            char error_msg[] = "File not found";
            send(client->socket, error_msg, strlen(error_msg), 0);
        }

        fclose(fptr);
    }

    close(client->socket);
    free(client);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t threads[MAX_CLIENTS];
    int thread_count = 0;

    // 1. Soket oluştur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket oluşturulamadı");
        exit(EXIT_FAILURE);
    }

    // 2. Adres bilgilerini ayarla
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3. Soketi bağla
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bağlantı hatası");
        exit(EXIT_FAILURE);
    }

    // 4. Dinleme moduna al
    if (listen(server_fd, 3) < 0) {
        perror("Dinleme hatası");
        exit(EXIT_FAILURE);
    }

    printf("Sunucu baslatildi. Baglantilar bekleniyor\n");

    while(1) {
        // 5. Bağlantıyı kabul et
        client_info_t* client = malloc(sizeof(client_info_t));
        client->socket = accept(server_fd, (struct sockaddr*)&client->address, (socklen_t*)&addrlen);
        
        if (client->socket < 0) {
            perror("Bağlantı kabul hatası");
            free(client);
            continue;
        }

        // Create new thread for client
        if (pthread_create(&threads[thread_count], NULL, handle_client, (void*)client) != 0) {
            perror("Thread oluşturulamadı");
            close(client->socket);
            free(client);
            continue;
        }

        // Detach thread so it can run independently
        pthread_detach(threads[thread_count]);
        thread_count++;
    }

    // 7. Server socket'i kapat
    close(server_fd);
    return 0;
}
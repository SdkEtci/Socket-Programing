#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char dosyaAdi[100];

    // 1. Soket oluştur
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        perror("Soket oluşturulamadı");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. Sunucuya bağlan
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        perror("Geçersiz adres / Adrese erişilemiyor");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        perror("Bağlantı hatası");
        return -1;
    }
    else{
        printf("Sunucuya baglandi.\n");
    }

    // 3. Veri gönder ve cevap al
    while (1){
        printf("Okunacak dosya ismini giriniz (q to quit): ");
        scanf("%s", dosyaAdi);
        
        if (strcmp(dosyaAdi, "q") == 0) {
            send(sock, "q", strlen("q"), 0);
            break;
        }
        
        send(sock, dosyaAdi, strlen(dosyaAdi), 0);
        
        char ch;
        while (read(sock, &ch, 1) > 0){
            if (ch == '\0') break;
            putchar(ch);
        }
        printf("\n");
    }

    close(sock);
    return 0;
}
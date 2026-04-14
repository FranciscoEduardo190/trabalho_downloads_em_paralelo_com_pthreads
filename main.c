#include <stdio.h>  
#include <pthread.h>
#include "downloads.h"

pthread_mutex_t lock;

int main() {
    // URLs para download
    char *urls[] = {
        "https://example.com",
        "https://www.w3.org/WAI/ER/tests/xhtml/testfiles/resources/pdf/dummy.pdf",
        "https://www.learningcontainer.com/wp-content/uploads/2020/04/sample-text-file.txt",
        "https://siteinexistente12345.com/teste.txt",
        "https://www.w3schools.com/python/python_variables_names.asp",
        "https://www.w3schools.com/html/mov_bbb.mp4"
    };
    // Calcula o número de URLs a partir do tamanho do array
    int num_urls = sizeof(urls) / sizeof(urls[0]);
    // Define o número de downloads simultâneos
    int simultaneos = 2;

    // Array para armazenar os IDs das threads
    pthread_t threads[num_urls];
    // Array para armazenar os argumentos das threads
    DownloadArgs args[num_urls];
    
    // Preenche os argumentos para cada thread
    for (int i = 0; i < num_urls; i++) {
    args[i].url = urls[i];
    args[i].id = i;
}

    // Inicializa o mutex
    pthread_mutex_init(&lock, NULL);
    
    // Cria threads para baixar os arquivos
    for(int i = 0; i < num_urls; i+= simultaneos) {
        int fim = i + simultaneos;

        if(fim > num_urls) {
            fim = num_urls;
        }
        // Cria threads para o lote atual
        for(int j = i; j < fim; j++) {
            pthread_create(&threads[j], NULL, baixar_arquivo, (void *)&args[j]);
        }

        // Aguarda as threads do lote atual terminarem
        for (int j = i; j < fim; j++) {
            pthread_join(threads[j], NULL);
        }

    }

    printf("Todos os downloads foram finalizados.\n");
    // Libera o mutex
    pthread_mutex_destroy(&lock);
    return 0;
}
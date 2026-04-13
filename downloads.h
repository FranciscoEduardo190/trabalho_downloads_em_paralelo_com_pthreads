#ifndef DOWNLOADS_H
#define DOWNLOADS_H

#include <pthread.h>

extern pthread_mutex_t lock;

// Estrutura para passar os argumentos para a função de download
typedef struct {
    char *url;
    int id;
} DownloadArgs;

// Função para baixar o arquivo, a ser executada em uma thread
void *baixar_arquivo(void *arg);

#endif

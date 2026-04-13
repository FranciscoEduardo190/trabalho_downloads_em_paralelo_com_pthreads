#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "downloads.h"
#include <pthread.h>

    void *baixar_arquivo(void *arg) {
        // Converte o argumento para o tipo correto
        DownloadArgs *args = (DownloadArgs *)arg;
        char *url = args->url;
        int id = args->id;

        // Configurações para o download
        int max_tentativas = 3;
        int tentativas = 0;
        CURL *curl;
        CURLcode res;
        FILE *fp;

        // Inicializa o CURL
        curl = curl_easy_init();
        // Tenta baixar o arquivo com um número máximo de tentativas
        if(curl) {
            // Gera um nome de arquivo único para cada download
            char nome_arquivo[100];
            sprintf(nome_arquivo, "downloads/download_%d.bin", id);
            // Tenta baixar o arquivo, se falhar, tenta novamente até atingir o limite de tentativas
            for(tentativas = 0; tentativas < max_tentativas; tentativas++) {
                fp = fopen(nome_arquivo, "wb");
                if(fp == NULL) {
                    pthread_mutex_lock(&lock);
                    curl_easy_cleanup(curl);
                    fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
                    pthread_mutex_unlock(&lock);
                    return NULL;
                }
                // Configura as opções do CURL para o download
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

                // Realiza o download
                res = curl_easy_perform(curl);
                fclose(fp);

                // Verifica o resultado do download
                if(res == CURLE_OK) {
                    pthread_mutex_lock(&lock);
                    printf("Download bem-sucedido: %s\n", url);
                    pthread_mutex_unlock(&lock);
                    break;
                } 
                // Se o download falhar, remove o arquivo parcialmente baixado e tenta novamente    
                else {
                    pthread_mutex_lock(&lock);
                    fprintf(stderr, "Erro no download (%d): %s, tentativa %d\n", res, curl_easy_strerror(res), tentativas + 1);
                    pthread_mutex_unlock(&lock);
                    remove(nome_arquivo); 
                }
            }   
            // Limpa os recursos do CURL
            curl_easy_cleanup(curl);
        }
        return NULL;
        
    }

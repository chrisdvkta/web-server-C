#include <stdlib.h>
#include "sys/socket.h"
#include <sys/stat.h>
#include <unistd.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#include "./lib/file_helper.h"
#include "./lib/http_helper.h"

#define BUFFER_SIZE 104857600
#define SRC_DIR "./src/"



void build_http_response(const char *file_name, const char *file_ext, char *response, size_t *response_len){
    printf("considered FILE NAME : %s \n", file_name); 



    const char *mime_type  = get_mime_type(file_ext); 

    char *header = (char *) malloc(BUFFER_SIZE * sizeof(char)); 
    snprintf(header, BUFFER_SIZE, 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: %s\r\n"
    "\r\n",
    mime_type
    );


    int file_fd = get_file_descriptor(SRC_DIR, file_name); 
    if (file_fd==-1)
{ 
    snprintf(response, BUFFER_SIZE,
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "404 Not Found");
    *response_len = strlen(response); 
    return ;
}


struct stat file_stat; 
fstat(file_fd, &file_stat); //get file size
off_t file_size = file_stat.st_size;  
printf("FILE DESC : %d size : %li \n", file_fd, file_size); 

//copy the header to response buffer
*response_len = 0; 
memcpy(response,header, strlen(header)); 
*response_len += strlen(header);

ssize_t bytes_read;
while ((bytes_read = read(file_fd, response + *response_len, BUFFER_SIZE - *response_len))>0){
    *response_len += bytes_read;
}

free(header); 
close(file_fd);
}

void *handle_client(void *arg){
    int client_fd = *((int *) arg); 
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char)); 

    //req data from client and store inside buffer
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0); 

    if (bytes_received > 0){
        regex_t regex; 
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED); 
        regmatch_t matches[2]; 

        if(regexec(&regex, buffer, 2, matches, 0)==0){
            buffer[matches[1].rm_so] = '\0'; 
            const char *url_encoded_file_name = buffer + matches[1].rm_so; 
            char *file_name = url_decode(url_encoded_file_name); 

        if (strlen(file_name)==0){
            strcpy(file_name, "index.html"); 
        }

        char file_ext[32]; 
        strcpy(file_ext, get_file_extension(file_name));

        char *response = (char *) malloc(BUFFER_SIZE *2*sizeof(char));
        size_t response_len; 
        build_http_response(file_name, file_ext,response, &response_len); 

        send(client_fd, response, response_len, 0); 

        free(response);
        free(file_name);
        }
        regfree(&regex);
    }
    close(client_fd); 
    free(arg); 
    free(buffer); 
    return NULL ;
}
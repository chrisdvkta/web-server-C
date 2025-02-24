#include <stdlib.h>
#include "sys/socket.h"
#include <sys/stat.h>
#include <unistd.h>
#include <regex>
#include <stdio.h>
#include <string.h>

#include "./lib/file_helper.h"
#include "./lib/http_helper.h"

#define BUFFER_SIZE 104857600
#define SRC_DIR "./src/"



void build_http_response(const char *file_name, cosnt char *file_ext, char *response, size_t *response_len){
    print("considered FILE NAME : %s \n", file_name); 



    const char *mime_type  = get_mime_type(file_ext); 

    char *header = (char *) malloc(BUFFER_SIZE * sizeof(char)); 
    snprintf(header, BUFFER_SIZE, 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: %s\r\n"
    "\r\n",
    mime_type
    )


    int file_df = get_file_descriptor(SRC_DIR, file_name); 
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
fstat(file_fd, &file_stat); 
off_t file_size = file_stat.st_size; 
printf("FILE DESC : %d size : %li \n", file_fd, file_size); 

*response_len = 0; 


}


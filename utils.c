#include <stdlib.h>
#include "sys/socket.h"
#include <sys/stat.h>
#include <unistd.h>
// #include <regex>
#include <stdio.h>
#include <string.h>

#include "./lib/file_helper.h"
#include "./lib/http_helper.h"

#define BUFFER_SIZE 104857600
#define SRC_DIR "./src/"
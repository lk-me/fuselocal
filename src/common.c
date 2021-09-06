#include "common.h"

#include <openssl/md5.h>
#include <uuid/uuid.h>

#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * \brief Backtrace buffer size
 */
#define BT_BUF_SIZE 100

void exit_failure(void)
{
    int nptrs;
    void *buffer[BT_BUF_SIZE];
    nptrs = backtrace(buffer, BT_BUF_SIZE);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
    exit(EXIT_FAILURE);
}

void *CALLOC(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (!ptr) {
        exit_failure();
    }
    return ptr;
}



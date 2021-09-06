#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_FILENAME_LEN    255
#define MAX_PATH_LEN        4096


typedef struct Link Link;

typedef enum {
    LINK_HEAD = 'H',
    LINK_DIR = 'D',
    LINK_FILE = 'F',
    LINK_INVALID = 'I',
    LINK_UNINITIALISED_FILE = 'U'
} LinkType;

struct Link;

typedef struct LinkTable LinkTable;

struct Link {
    /** \brief The link name in the last level of the URL */
    char linkname[MAX_FILENAME_LEN+1];
    /** \brief The link path in the last level of the URL */
    char linkpath[MAX_FILENAME_LEN+1];
    /** \brief The full URL of the file */
    char f_url[MAX_PATH_LEN+1];
    /** \brief The type of the link */
    LinkType type;
    /** \brief CURLINFO_CONTENT_LENGTH_DOWNLOAD of the file */
    size_t content_length;
    /** \brief The next LinkTable level, if it is a LINK_DIR */
    LinkTable *next_table;
    /** \brief The content for file*/
    char *content;
     /** \brief The mode for file*/
    mode_t link_mode;
};

struct LinkTable {
    int num;
    Link **links;
};

void *CALLOC(size_t nmemb, size_t size);
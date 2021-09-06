#include "link.h"

#include <gumbo.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>


/**
 * \brief find the link associated with a path
 */
Link *path_to_Link(const char *path, LinkTable *ROOT_LINK_TBL)
{
    char *new_path = strndup(path, MAX_PATH_LEN);
    if (!new_path) {
        printf("path_to_Link(): cannot allocate memory\n");
    }
    Link *link = LinkTable_exist(ROOT_LINK_TBL, new_path);
    free(new_path);
    return link;
}

/**
 * \brief create a new Link
 */
Link *Link_new(const char *linkname, LinkType type, mode_t mode)
{
    Link *link = (Link*)CALLOC(1, sizeof(Link));
    link->link_mode = mode | S_IFREG;
    strncpy(link->linkpath, linkname, MAX_FILENAME_LEN);
    char *pName = &(strrchr(linkname, '/')[1]);
    strncpy(link->linkname, pName, MAX_FILENAME_LEN);
    link->type = type;
    /* remove the '/' from linkname if it exists */
    char *c = &(link->linkname[strnlen(link->linkname, MAX_FILENAME_LEN) - 1]);
    if ( *c == '/') {
        *c = '\0';
    }
    return link;
}

/**
 * \brief create a new LinkTable
 */
void LinkTable_add(LinkTable *linktbl, Link *link)
{
    linktbl->num++;
    linktbl->links = realloc(linktbl->links, linktbl->num * sizeof(Link *));
    if (!linktbl->links) {
        return;
    }
    linktbl->links[linktbl->num - 1] = link;
}


LinkTable *LinkTable_alloc()
{
    LinkTable *linktbl = CALLOC(1, sizeof(LinkTable));

    /* populate the base URL */
    Link *head_link = Link_new("/", LINK_HEAD, 0);
    LinkTable_add(linktbl, head_link);
    assert(linktbl->num == 1);
    return linktbl;
}

Link* LinkTable_exist(LinkTable *linktbl, const char *linkpath)
{
    Link *link = NULL;
    for (int i = 0; linktbl != NULL &&i < linktbl->num; i++) {
        link = linktbl->links[i];
        if (0 == strcmp(link->linkpath, linkpath)) {

            return linktbl->links[i];
        }
        if(link->next_table != NULL) {
            link =  LinkTable_exist(link->next_table, linkpath);
            if(NULL != link) {
                return link;
            }
        }
    }
    return NULL;
}

/**
 * \brief unlink a LinkTable
 */
LinkTable* LinkTable_unlink(LinkTable *linktbl, const char *path)
{
    Link *link = NULL;
    int unLink = 0;
    int nTable = linktbl->num;
    for (int i = 0; linktbl != NULL &&i < linktbl->num; i++) {
        link = linktbl->links[i];
        if (0 == strcmp(link->linkpath, path)) {
            int nTmp = i;
            free(linktbl->links[i]);
            linktbl->links[i] = NULL;

            while (NULL != linktbl->links[nTmp+1] && nTmp < nTable - 1) {
                int nCount = nTmp+1;
                linktbl->links[nTmp] = linktbl->links[nCount];
                nTmp = nCount;
            }
            linktbl->num--;
            // 最后一个为NULL
            linktbl->links[linktbl->num] = NULL;
        }
        if(link->next_table != NULL) {
            LinkTable_unlink(link->next_table, path);
        }
    }
    return linktbl;
}

LinkTable* LinkTable_insert(const char *path, LinkType type, LinkTable *ROOT_LINK_LOCAL, mode_t mode)
{
    LinkTable *linktbl = NULL;
    Link *pLink;
    bool bRoot = false;
    char *ret = strrchr(path, '/');
    char szTmpPath[MAX_PATH_LEN] = "";
    if (NULL != ret) {
        strncpy(szTmpPath, path, strlen(path) - strlen(ret));
    }else{
        strcpy(szTmpPath, path);
    }
    if (NULL != ROOT_LINK_LOCAL && strcmp("", szTmpPath)) {
        pLink = LinkTable_exist(ROOT_LINK_LOCAL, szTmpPath);
        linktbl = pLink->next_table;
        if (NULL == pLink) {
            return ROOT_LINK_LOCAL;
        }
    }else{
        bRoot = true;
        linktbl = ROOT_LINK_LOCAL;
    }
    Link* pLinkNew = Link_new(path, type, mode);
    pLinkNew->next_table = NULL;
    if(ROOT_LINK_LOCAL == NULL) {
        ROOT_LINK_LOCAL = LinkTable_alloc();
        ROOT_LINK_LOCAL->links[0] = pLinkNew;
        ROOT_LINK_LOCAL->num = 1;
    } else if (bRoot){
        LinkTable_add(ROOT_LINK_LOCAL, pLinkNew);
    }else{
         if(pLink->next_table == NULL) {
            pLink->next_table = LinkTable_alloc();
            pLink->next_table->links[0] = pLinkNew;
            pLink->next_table->num = 1;
        }else{
            LinkTable_add(pLink->next_table, pLinkNew);
        }
    }
    return ROOT_LINK_LOCAL;
}

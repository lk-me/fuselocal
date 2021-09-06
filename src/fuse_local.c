/* must be included before including <fuse.h> */
#define FUSE_USE_VERSION 26

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fuse.h>

#include "link.h"
#include "fuse_local.h"

LinkTable *ROOT_LINK_TBL = NULL;

static void *fs_init(struct fuse_conn_info *conn)
{
    (void) conn;
    return NULL;
}

/** \brief release an opened file */
static int fs_release(const char *path, struct fuse_file_info *fi)
{
    return 0;
}

/** \brief return the attributes for a single file indicated by path */
static int fs_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));
    if (0 == strcmp(path, "/")) {
        stbuf->st_mode = 0755 | S_IFDIR;
        stbuf->st_nlink = 1;
    } else {
        Link *link = path_to_Link(path, ROOT_LINK_TBL);
        if (!link) {
            return -ENOENT;
        }
        switch (link->type) {
            case LINK_DIR:
                stbuf->st_mode =  0755 | S_IFDIR;
                stbuf->st_nlink = 1;
                break;
            case LINK_FILE:
                stbuf->st_mode = link->link_mode;
                stbuf->st_nlink = 1;
                break;
            default:
                return -ENOENT;
        }
    }
    return 0;
}

/** \brief read a file */
static int fs_read(const char *path, char *buf, size_t size, off_t offset,
                   struct fuse_file_info *fi)
{
    return 0;
}

/** \brief open a file indicated by the path */
static int fs_open(const char *path, struct fuse_file_info *fi)
{
    return 0;
}

/** \brief make a dir by the path */
static int fs_mkdir(const char *path, mode_t mode)
{
    ROOT_LINK_TBL = LinkTable_insert(path, LINK_DIR, ROOT_LINK_TBL, mode);
    return 0;
}

/** \brief rename a path file name */
static int fs_rename(const char *dest,const char *src)
{
    Link *pLink = LinkTable_exist(ROOT_LINK_TBL, dest);
    if(NULL != pLink && NULL != src) {
        char *pLinkName = strrchr(src, '/');
        strcpy(pLink->linkname, &pLinkName[1]);
        strcpy(pLink->linkpath, src);
    }
    return 0;
}

/** \brief write a file by the path */
static int fs_write(const char *path, const char *buf, size_t size, off_t offset,
                   struct fuse_file_info *fi)
{
    return 0;
}

/** \brief read the directory indicated by the path*/
static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t dir_add,
                      off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;
    LinkTable *linktbl = NULL;
    Link *link;
    Link *pLink = LinkTable_exist(ROOT_LINK_TBL, path);
    if (!strcmp(path, "/")) {
        linktbl = ROOT_LINK_TBL;
    }else if(pLink->next_table != NULL) {
        linktbl = pLink->next_table;
    }
    /* start adding the links */
    dir_add(buf, ".", NULL, 0);
    dir_add(buf, "..", NULL, 0);
    if(NULL != linktbl) {
        printf("int nNum = %d\n", linktbl->num);
    }

    for (int i = 0; linktbl != NULL &&i < linktbl->num; i++) {
        link = linktbl->links[i];
        if (link->type != LINK_INVALID) {
            dir_add(buf, link->linkname, NULL, 0);
        }
    }
    return 0;
}

/** \brief create a file info by the path */
static int fs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    ROOT_LINK_TBL = LinkTable_insert(path, LINK_FILE, ROOT_LINK_TBL, mode);
    return 0;
}

/** \brief remove a file info by the path */
static int fs_unlink(const char *path)
{
    if (NULL == ROOT_LINK_TBL) {
        return 0;
    }
    ROOT_LINK_TBL = LinkTable_unlink(ROOT_LINK_TBL, path);
    return 0;
}

/** \brief remove a dir info by the path */
static int fs_rmdir(const char *path)
{
    if (NULL == ROOT_LINK_TBL) {
        return 0;
    }
    ROOT_LINK_TBL = LinkTable_unlink(ROOT_LINK_TBL, path);
    return 0;
}

static struct fuse_operations fs_oper = {
    .getattr	= fs_getattr,
    .readdir	= fs_readdir,
    .open		= fs_open,
    .read		= fs_read,
    .init       = fs_init,
    .release    = fs_release,
    .create     = fs_create,
    .write      = fs_write,
    .rename     = fs_rename,
    .mkdir      = fs_mkdir,
    .unlink     = fs_unlink,
    .rmdir      = fs_rmdir
};

int fuse_local_init(int argc, char **argv)
{
    return fuse_main(argc, argv, &fs_oper, NULL);
}


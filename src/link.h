#ifndef _LINK_H_
#define _LINK_H_

#include "common.h"

/**
 * \brief find the link associated with a path
 */
Link *path_to_Link(const char *path, LinkTable *ROOT_LINK_TBL);

/**
 * \brief create a new Link
 */
Link *Link_new(const char *linkname, LinkType type, mode_t mode);

/**
 * \brief create a new LinkTable
 */
LinkTable *LinkTable_alloc();

/**
 * \brief find link does it exist
 */
Link* LinkTable_exist(LinkTable *linktbl, const char *linkname);

/**
 * \brief add a new LinkTable
 */
void LinkTable_add(LinkTable *linktbl, Link *link);

/**
 * \brief insert a new LinkTable
 */
LinkTable* LinkTable_insert(const char *path, LinkType type, LinkTable *linktbl, mode_t mode);

/**
 * \brief unlink a LinkTable
 */
LinkTable* LinkTable_unlink(LinkTable *linktbl, const char *path);

#endif
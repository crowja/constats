/**
 *  @file seqinfo.h
 *  @brief FIXME
 *  @details FIXME
 */

#ifndef _SEQINFO_H_
#define _SEQINFO_H_

#include "varstr.h"

struct seq;

/**
 *
 *  @brief Create and return a new seqinfo object.
 *  @details FIXME longer description here ...
 */

struct seqinfo *seqinfo_new( void );

/**
 *
 *  @brief Clean up and free a seqinfo structure.
 *  @details FIXME longer description here ...
 */

void        seqinfo_free( struct seqinfo *p );

/**
 *
 *  @brief Return the version of this package.
 *  @details FIXME longer description here ...
 */

const char *seqinfo_version( void );

/**
 *
 *  @brief Setter-getter for the description attribute.
 *  @details FIXME longer description here ...
 *
 *  @param p Pointer to a seqinfo object
 *
 *  @param x FIXME
 */

char       *seqinfo_desc( struct seqinfo *p, char *x );

/**
 *
 *  @brief Setter-getter for the identifier attribute.
 *  @details FIXME longer description here ...
 *
 *  @param p Pointer to a seqinfo object
 *
 *  @param x FIXME
 */

char       *seqinfo_id( struct seqinfo *p, char *x );

void        seqinfo_increment_pair_counts( struct seqinfo *p, unsigned from, unsigned to, unsigned is_correct );

/**
 *
 *  @brief Setter-getter for the sequence text attribute.
 *  @details FIXME longer description here ...
 *
 *  @param p Pointer to a seqinfo object
 *
 *  @param x FIXME
 */

char       *seqinfo_text( struct seqinfo *p, char *x );

/**
 *
 *  @brief Initialize a seqinfo object.
 *  @details FIXME longer description here ...
 *
 *  @param p Pointer to a seqinfo object
 *
 *  @param x FIXME
 */

int         seqinfo_reset( struct seqinfo *p );

#endif

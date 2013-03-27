/**
 *  @file fareader.h
 *  @brief FIXME
 *  @details FIXME
 */

#ifndef _FAREADER_H_
#define _FAREADER_H_

#ifdef  _PACKAGE_NAME
#undef  _PACKAGE_NAME
#endif
#define _PACKAGE_NAME "fareader"

struct fareader;

/**
 *
 *  @brief Create and return a new fareader object.
 *  @details FIXME longer description here ...
 */

struct fareader *fareader_new( char *fname );

/**
 *
 *  @brief Clean up and free a fareader structure.
 *  @details FIXME longer description here ...
 */

void        fareader_free( struct fareader *p );

/**
 *
 *  @brief Return the version of this package.
 *  @details FIXME longer description here ...
 */

const char *fareader_version( void );

int         fareader_buffersize( struct fareader *p, unsigned size, unsigned extend );

/**
 *
 *  @brief Initialize a fareader object.
 *  @details FIXME longer description here ...
 *
 *  @param p Pointer to a fareader object
 *
 *  @param x FIXME
 */

int         fareader_init( struct fareader *p, void *x );

int         fareader_next( struct fareader *p, char **h, char **s );

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "seqinfo.h"

#ifdef  _IS_NULL
#undef  _IS_NULL
#endif
#define _IS_NULL(p)   ((NULL == (p)) ? (1) : (0))

#ifdef  _FREE
#undef  _FREE
#endif
#define _FREE(p)      ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

static const char version[] = "20120211";

struct seqinfo {
   struct varstr *id;
   struct varstr *desc;
   struct varstr *text;
   unsigned   *correct_pair_counts;
   unsigned   *incorrect_pair_counts;
   double      self_score;
   unsigned    count_at;
   unsigned    count_gc;
   unsigned    count_others;
};


/*** seqinfo_new() ***/

struct seqinfo *
seqinfo_new( void )
{
   struct seqinfo *tp;

   tp = ( struct seqinfo * ) malloc( sizeof ( struct seqinfo ) );
   if ( _IS_NULL( tp ) )
      return NULL;

   tp->id = varstr_new(  );
   tp->desc = varstr_new(  );
   tp->text = varstr_new(  );
   tp->correct_pair_counts = NULL;
   tp->incorrect_pair_counts = NULL;

   return tp;
}


/*** seqinfo_free() ***/

void
seqinfo_free( struct seqinfo *p )
{
   varstr_free( p->id );
   varstr_free( p->desc );
   varstr_free( p->text );
   _FREE( p->correct_pair_counts );
   _FREE( p->incorrect_pair_counts );
   _FREE( p );
}


/*** seqinfo_version() ***/

const char *
seqinfo_version( void )
{
   return version;
}


/*** seqinfo_desc() ***/

char       *
seqinfo_desc( struct seqinfo *p, char *x )
{
   if ( !_IS_NULL( x ) ) {
      varstr_empty( p->desc );
      varstr_cat( p->desc, x );
      varstr_lrtrim( p->desc );
   }

   return varstr_str( p->desc );
}


/*** seqinfo_id() ***/

char       *
seqinfo_id( struct seqinfo *p, char *x )
{
   if ( !_IS_NULL( x ) ) {
      varstr_empty( p->id );
      varstr_cat( p->id, x );
      varstr_lrtrim( p->id );
   }

   return varstr_str( p->id );
}


/*** seqinfo_increment_pair_counts ***/

void
seqinfo_increment_pair_counts( struct seqinfo *p, unsigned from, unsigned to, unsigned is_correct )
{
   unsigned    len = strlen( varstr_str( p->text ) );

   if ( len == 0 )                               /* avoid a problem with len - 1 below */
      return;

   else {
      unsigned    i;

      to = to < len ? to : len - 1;

      if ( is_correct )
         for ( i = from; i <= to; i++ )
            ( p->correct_pair_counts )[i] += 1;

      else
         for ( i = from; i <= to; i++ )
            ( p->incorrect_pair_counts )[i] += 1;
   }
}

/*** seqinfo_reset() ***/

int
seqinfo_reset( struct seqinfo *p )
{
   unsigned    i;
   unsigned    len = strlen( varstr_str( p->text ) );

   varstr_empty( p->id );
   varstr_empty( p->desc );
   varstr_empty( p->text );

   for ( i = 0; i < len; i++ ) {
      ( p->correct_pair_counts )[i] = 0;
      ( p->incorrect_pair_counts )[i] = 0;
   }

   return 0;
}


/*** seqinfo_text() ***/

char       *
seqinfo_text( struct seqinfo *p, char *x )
{

   if ( !_IS_NULL( x ) ) {
      unsigned    i, len;

      varstr_empty( p->text );
      varstr_cat( p->text, x );
      varstr_compact( p->text );
      len = strlen( varstr_str( p->text ) );
      p->correct_pair_counts = realloc( p->correct_pair_counts, len * sizeof ( unsigned ) );
      p->incorrect_pair_counts = realloc( p->incorrect_pair_counts, len * sizeof ( unsigned ) );

      for ( i = 0; i < len; i++ ) {
         ( p->correct_pair_counts )[i] = 0;
         ( p->incorrect_pair_counts )[i] = 0;
      }
   }

   return varstr_str( p->text );
}

#undef _IS_NULL
#undef _FREE

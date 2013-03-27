#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "options.h"
#include "fareader.h"
#include "seqinfo.h"
#include "sequtils.h"
#include "tokenset.h"
#include "utils.h"
#include "bam.h"

#ifdef  _IS_NULL
#undef  _IS_NULL
#endif
#define _IS_NULL(p)              ((NULL == (p)) ? (1) : (0))

#ifdef  _FREE
#undef  _FREE
#endif
#define _FREE(p)                 ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

static struct seqinfo **seqs = NULL;
unsigned    seqs_extend = 1024;
unsigned    seqs_size = 0;
static struct tokenset *seqids = NULL;
static struct fareader *r = NULL;
static char *tmpstr = NULL;                 /* character workspace */
static double *wrk = NULL;                  /* double workspace */


static int
_seqinfo_init( char *id, char *s )
{
   unsigned    i = tokenset_add( seqids, id );
   static char *revcom = NULL;

   /* Convert s to uppercase DNA */
   sequtils_clean( s, 1 );
   sequtils_to_rna( s );

   if ( i >= seqs_size ) {                       /* need to extend */
      unsigned    need = i + seqs_extend;
      unsigned    j;

      seqs = realloc( seqs, need * sizeof ( struct seqinfo * ) );

      for ( j = seqs_size; j < need; j++ )
         seqs[j] = seqinfo_new(  );

      seqs_size = need;
   }

   seqinfo_id( seqs[i], id );
   seqinfo_text( seqs[i], s );


   /* Align s to its reverse complement */
   revcom = realloc( revcom, ( 1 + strlen( s ) ) * sizeof ( char ) );
   wrk = realloc( wrk, 2 * ( 2 + strlen( s ) ) * sizeof ( double ) );
   strcpy( revcom, s );
   sequtils_revcom( revcom );

   {
      unsigned    at, gc, others;
      double      swscore = sequtils_swscore( s, revcom, 2, 200, 1, 0, wrk );

      sequtils_count_gc( s, &gc, &at, &others );
      printf( "%s\t%d\t%d\t%d\t%d\t%0.1f\n", id, ( int ) strlen( s ), at, gc, others, swscore );
   }
}


/*** main() ***/

int
main( int argc, char *argv[] )
{
   unsigned    i;
   char       *id = NULL;
   char       *h = NULL, *s = NULL;
   struct options *o = options_new(  );

   /* Get the command line options */
   options_cmdline( o, argc, argv );

   /* Initialize the sequence id maps */
   seqids = tokenset_new(  );

   /* Open a fareader based on a file or stdin */
   if ( o->optind == argc )                      /* read sequences from stdin */
      r = fareader_new( NULL );

   else {                                        /* read sequences from input files */
      r = fareader_new( argv[o->optind] );

      if ( _IS_NULL( r ) ) {
         fprintf( stderr, "[ERROR] %s: Cannot open input file \"%s\"\n", _I_AM, argv[o->optind] );
         exit( 1 );
      }
   }

   /* Read the sequences */

   fareader_buffersize( r, 1024, 1024 );

   while ( fareader_next( r, &h, &s ) ) {
      utils_extract_id( &id, h );
      _seqinfo_init( id, s );
   }

   fareader_free( r );

   /* Read the bamfile if available */

   if ( !_IS_NULL( o->bamfn ) ) {
      bamFile     bam_in;
      bam_header_t *hp;
      bam1_t     *bp = bam_init1(  );

      bam_in = bam_open( o->bamfn, "r" );
      if ( _IS_NULL( bam_in ) ) {
         fprintf( stderr, "[ERROR] %s: Cannot open BAM file \"%s\"\n", _I_AM, o->bamfn );
         exit( 1 );
      }

      hp = bam_header_read( bam_in );
      if ( _IS_NULL( hp ) ) {
         fprintf( stderr, "[ERROR] %s: Cannot read header in BAM file \"%s\"\n", _I_AM, o->bamfn );
         exit( 1 );
      }

      if ( o->verbosity > 0 )
         printf( "# %s: Reading BAM file \"%s\"\n", _I_AM, o->bamfn );

      while ( bam_read1( bam_in, bp ) >= 0 ) {
         char       *id;
         int         tid = ( bp->core ).tid;
         int         from, to;

         if ( tid < 0 )
            continue;

         if ( bp->core.isize > 0 )
            continue;

         id = ( hp->target_name )[tid];

         if ( _IS_NULL( id ) )
            continue;

         /* Also see http://samtools.sourceforge.net/samtools/bam/PDefines/PDefines.html */
#if 0
#endif

         to = bp->core.pos + bp->core.l_qseq - 1;
         from = to + bp->core.isize + 1;
         /* is_correct is given by ( bp->core.flag & x02 ) */

         printf( "\t%d\t%d\t%d\t%d\t%d\t%s\t%d\n", bp->core.pos, bp->core.isize, bp->core.flag, bp->core.n_cigar,
          bp->core.l_qseq, id, bp->core.flag & 0x2 );

         if ( bp->core.flag & 0x2 ) {
            /* do some magic here */
            printf( "DEBUG FROM %d TO %d (is_correct %d)\n", from, to, bp->core.flag & 0x2 );
         }
      }

      bam_destroy1( bp );
      bam_header_destroy( hp );
      bam_close( bam_in );
   }

   _FREE( id );
   options_free( o );
   tokenset_free( seqids );
   _FREE( tmpstr );
   _FREE( wrk );

   for ( i = 0; i < seqs_size; i++ )
      seqinfo_free( seqs[i] );

   _FREE( seqs );

   return 0;
}

#undef _IS_NULL
#undef _FREE

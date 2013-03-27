#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "options.h"
#include "getopt.h"
#include "main.h"

#ifdef  _IS_NULL
#undef  _IS_NULL
#endif
#define _IS_NULL(p)    ((NULL == (p)) ? (1) : (0))

#ifdef  _FREE
#undef  _FREE
#endif
#define _FREE(p)       ((NULL == (p)) ? (0) : (free((p)), (p) = NULL))

/*
 *  Structure is defined in options.h since it needs to be visible.
 */

/*** options_new() ***/

struct options *
options_new( void )
{
   struct options *tp;

   tp = ( struct options * ) malloc( sizeof ( struct options ) );
   if ( _IS_NULL( tp ) )
      return NULL;

   tp->bamfn = NULL;
   tp->fname = NULL;
   tp->verbosity = 0;

   return tp;
}


/*** options_free() ***/

void
options_free( struct options *p )
{
   if ( _IS_NULL( p ) )
      return;

   _FREE( p->bamfn );
   _FREE( p->fname );
   _FREE( p );
}


/*** options_helpmsg() ***/

void
options_helpmsg( FILE *out )
{
   char        indent[] = "      ";

   /*            "------------------------------------------------------------------------------80" */
   fprintf( out, "USAGE\n" );
   fprintf( out, "%s [options] [<infile>]\n", _I_AM );
   fprintf( out, "Generate a quality report for each contig in <infile>, FASTA format; use stdin\n" );
   fprintf( out, "otherwise. \n" );
   fprintf( out, "\nOPTIONS\n" );
   fprintf( out, "%s\n", "-b, --bam <bamfile>" );
   fprintf( out, "%s%s\n", indent, "Specify <bamfile> as a bamfile to associate with the contig set. Note that" );
   fprintf( out, "%s%s\n", indent, "these alignments are not required to be based on the RNA-seq data used to" );
   fprintf( out, "%s%s\n", indent, "generate the contigs." );
   fprintf( out, "%s\n", "-h, --help" );
   fprintf( out, "%s%s\n", indent, "Print this help message and exit." );
   fprintf( out, "%s\n", "-q, --quiet" );
   fprintf( out, "%s%s\n", indent, "Run quietly." );
   fprintf( out, "%s\n", "-V, --verbosity" );
   fprintf( out, "%s%s\n", indent, "Increase the level of reporting, multiples accumulate." );
   fprintf( out, "%s\n", "-v, --version" );
   fprintf( out, "%s%s\n", indent, "Print the version information and exit." );
}


/*** options_cmdline() ***/

void
options_cmdline( struct options *p, int argc, char *argv[] )
{
   int         c;
   static struct option long_options[] = {
      {"bam", required_argument, 0, 'b'},
      {"bamfile", required_argument, 0, 'b'},
      {"help", no_argument, 0, 'h'},
      {"quiet", no_argument, 0, 'q'},
      {"verbose", no_argument, 0, 'V'},
      {"version", no_argument, 0, 'v'},
      {0, 0, 0, 0}
   };

   while ( 1 ) {

      /* getopt_long stores the option index here. */
      int         option_index = 0;

      c = getopt_long( argc, argv, "b:hqVv", long_options, &option_index );

      /* Detect the end of the options. */
      if ( c == -1 )
         break;

      switch ( c ) {

         case 'b':
            p->bamfn = realloc( p->bamfn, sizeof ( char ) * ( 1 + strlen( optarg ) ) );
            strcpy( p->bamfn, optarg );
            break;

         case 'h':
            options_helpmsg( stdout );
            exit( 0 );

         case 'q':
            p->verbosity = 0;
            break;

         case 'V':
            /* printf( " --verbose\n" ); */
            p->verbosity += 1;
            break;

         case 'v':
            printf( "%s-%s\n", _I_AM, PACKAGE_VERSION );
            exit( 0 );

         case '?':
            /* getopt_long already printed an error message. */
            break;

         default:
            abort(  );
      }
   }

   p->optind = optind;
}

#undef _IS_NULL
#undef _FREE

#ifndef _SEQUTILS_H_
#define _SEQUTILS_H_

void        sequtils_clean( char *s, int flag );
void        sequtils_count_gc( char *s, unsigned *gc, unsigned *at, unsigned *others );

void        sequtils_crush( char *s );
int         sequtils_left_align( unsigned type, char *a, char *b, double match, double indel, double mismatch, double ambig, double *wrk, unsigned *pos, double *score );
double      sequtils_nwscore( char *a, char *b, double match, double indel, double mismatch, double ambig, double *wrk );
unsigned    sequtils_orf_length( char *s, unsigned offset );
void        sequtils_revcom( char *s );
double      sequtils_swscore( char *a, char *b, double match, double indel, double mismatch, double ambig, double *wrk );
void        sequtils_to_dna( char *s );
void        sequtils_to_rna( char *s );
const char *sequtils_version( void );



#endif

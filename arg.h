#ifndef _ARG_H_
#define _ARG_H_

/* Using macros to build a new(& easy) syntax for parsing
 * command line arguments.
 *                                          by Kueppo :-)
 */

/* BEGINNING STATEMENT with 'ARG {' */
#define ARG \
    PROGNAME = *argv++, argc--; \
    for ( ; argv[0] && argv[0][0] == '-'; argc--, argv++ ) { \
       if ( argv[0][1] == '-' && argv[0][2] == '\0' ) { \
           argv++, argc--; \
           break; \
       } \
       int used_GET = 0; \
       while ( argv[0]++, argv[0][0] && !used_GET ) { \
           switch( argv[0][0] ) \
				

/*
 * {
 *    case 'X':
 *       action();
 *    case 'Y':
 *       action();
 * }
 */

/* GET THE ARG OF A COMMAND LINE OPTION: to be executed in `case' */
#define GET(x) \
    ( argv[0][1] == '\0' && argv[1] == NULL ) ? \
        ( (x), (char *)0 ) : \
        ( used_GET = 1, ( argv[0][1] != '\0' ) ? \
            ( ++argv[0] ) : \
            ( argc--, argv++, argv[0] ) \
        )


/* ENDING STATEMENT with '} GRA;' (ARG <-----) */
#define GRA \
        } \
     }

#endif

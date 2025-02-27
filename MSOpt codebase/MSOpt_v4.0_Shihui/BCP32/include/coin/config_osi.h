/* inc/config_osi.h.  Generated by configure.  */
/* inc/config_osi.h.in.  Generated from configure.ac by autoheader.  */

/* If defined, the Ampl Solver Library is available. */
/* #undef COIN_HAS_ASL */

/* Define to 1 if the Cbc package is used */
/* #undef COIN_HAS_CBC */

/* Define to 1 if the Cgl package is used */
#define COIN_HAS_CGL 1

/* Define to 1 if the Clp package is used */
#define COIN_HAS_CLP 1

/* Define to 1 if the CoinUtils package is used */
#define COIN_HAS_COINUTILS 1

/* Define to 1 if the Cplex package is available */
/* #undef COIN_HAS_CPX */

/* Define to 1 if the DyLP package is used */
/* #undef COIN_HAS_DYLP */

/* Define to 1 if the FortMP package is available */
/* #undef COIN_HAS_FMP */

/* Define to 1 if Glpk package is available */
/* #undef COIN_HAS_GLPK */

/* Define to 1 if the Mosek package is available */
/* #undef COIN_HAS_MSK */

/* Define to 1 if the Osi package is used */
#define COIN_HAS_OSI 1

/* Define to 1 if the Osl package is available */
/* #undef COIN_HAS_OSL */

/* Define to 1 if the Soplex package is available */
/* #undef COIN_HAS_SPX */

/* Define to 1 if the SYMPHONY package is used */
/* #undef COIN_HAS_SYMPHONY */

/* Define to 1 if the Vol package is used */
#define COIN_HAS_VOL 1

/* Define to 1 if the Xpress package is available */
/* #undef COIN_HAS_XPR */

/* Define to the debug sanity check level (0 is no test) */
#define COIN_OSI_CHECKLEVEL 0

/* Define to the debug verbosity level (0 is no output) */
#define COIN_OSI_VERBOSITY 0

/* Define to 1 if GLPK has the advanced B&B solver lpx_intopt */
/* #undef GLPK_HAS_INTOPT */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define this variable to enable OsiDylp's informational printing features.
   */
#define ODSI_INFOMSGS 1

/* Control OsiDylp's paranoid checks. Legal values: 0 - off; 1 - normal; 2 -
   consistency (expensive) */
#define ODSI_PARANOIA 1

/* Define this variable to enable support for dylp's statistics collection
   features. */
/* #undef ODSI_STATISTICS */

/* define to the name of the default solver interface class, e.g.,
   OsiClpSolverInterface */
#define OSICBC_DFLT_SOLVER OsiClpSolverInterface

/* define this symbol if clp is the default solver */
#define OSICBC_DFLT_SOLVER_CLP 1

/* define to the name of the .hpp file for the default solver interface class,
   e.g., "OsiClpSolverInterface.hpp" (include quotes) */
#define OSICBC_DFLT_SOLVER_HPP "OsiClpSolverInterface.hpp"

/* Name of package */
#define PACKAGE "osi"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "coin-osi@list.coin-or.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Osi"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Osi 0.98.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "osi"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.98.2"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.98.2"

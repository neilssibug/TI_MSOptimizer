/*
 * Include file for the configuration of Alps.
 *
 * On systems where the code is configured with the configure script
 * (i.e., compilation is always done with HAVE_CONFIG_H defined), this
 * header file includes the automatically generated header file, and
 * undefines macros that might configure with other Config.h files.
 *
 * On systems that are compiled in other ways (e.g., with the
 * Developer Studio), a header files is included to define those
 * macros that depend on the operating system and the compiler.  The
 * macros that define the configuration of the particular user setting
 * (e.g., presence of other COIN packages or third party code) are set
 * here.  The project maintainer needs to remember to update this file
 * and choose reasonable defines.  A user can modify the default
 * setting by editing this file here.
 *
 */

#ifndef __BCPCONFIG_H__

#ifdef HAVE_CONFIG_H
#include "config_bcp.h"

/* undefine macros that could conflict with those in other config.h
   files */
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#undef VERSION

#else /* HAVE_CONFIG_H */

/* include the COIN-wide system specific configure header */
#include "configall_system.h"

/***************************************************************************/
/*             HERE DEFINE THE CONFIGURATION SPECIFIC MACROS               */
/***************************************************************************/

/* If defined, debug sanity checks are performed during runtime */
/* #define COIN_DEBUG 1 */

/* Define to 1 if the CoinUtils package is used */
#define COIN_HAS_COINUTILS 1

/* Define to 1 if the Mpi package is used */
/* #define COIN_HAS_MPI 1 */

/* a few things that exists under unix, but not under windows */
#include <Windows.h>
#define gethostname(c, l) { DWORD cnlen = l; GetComputerName(c, &cnlen); }
#include <process.h>

#endif /* HAVE_CONFIG_H */

#endif /*__HAVE_BCP_CONFIG_H__*/

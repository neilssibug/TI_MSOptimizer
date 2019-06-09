// __START__
/**
 * @file main.cpp
 * @ingroup Main
 * 
 * @brief main() entry point for the application.
 *
 * Detailed description of file...
 * 
 */

#include "main.hpp"

/**
 * @brief main() entry point for the application
 *
 * @param argc count of the command line arguments
 * @param argv array of command line argments
 *
 * @return return code for the application
 * @retval 0 successful execution
 * @retval !0 issue encountered during execution
 */
int
main( int argc, char *argv[] )
{
    return bcp_main( argc, argv, BCP_user_init( ) );
}

/**
 * @brief user initialization point for BCP library
 *
 * @return user data object to control BCP session
 */
USER_initialize * BCP_user_init( )
{
    return new AT_init;
}


// __END__

/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  01.09.2012 12:58:02
 *         Author:  Miroslav Bendík
 *        Company:  LinuxOS.sk
 * =====================================================================
 */

#if MSDOS || __OS2__ || __NT__ || _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#define BLOCK_SIZE 16

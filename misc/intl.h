/* Dummy header for libintl.h */

#include "config.h"

#if HAVE_LIBINTL_H
#include <libintl.h>
#define _(String) gettext((String))
#else
void bindtextdomain(const char * package, const char * dir);
void textdomain(const char * package);
#define _(String) (String)
#endif


#ifndef H_RPMURL
#define H_RPMURL

/** \ingroup rpmio
 * \file rpmio/rpmurl.h
 */

#include <assert.h>

/**
 * Supported URL types.
 */
typedef enum {
    URL_IS_UNKNOWN	= 0,	/*!< unknown (aka a file) */
    URL_IS_DASH		= 1,	/*!< stdin/stdout */
    URL_IS_PATH		= 2,	/*!< file://... */
    URL_IS_FTP		= 3,	/*!< ftp://... */
    URL_IS_HTTP		= 4	/*!< http://... */
} urltype;

#define	URLMAGIC	0xd00b1ed0
#define	URLSANE(u)	assert(u && u->magic == URLMAGIC)

/**
 * URL control structure.
 */
typedef /*@abstract@*/ /*@refcounted@*/ struct urlinfo {
/*@refs@*/ int nrefs;		/*!< no. of references */
    const char * url;		/*!< copy of original url */
    const char * service;
    const char * user;
    const char * password;
    const char * host;
    const char * portstr;
    const char * proxyu;	/*!< FTP: proxy user */
    const char * proxyh;	/*!< FTP/HTTP: proxy host */
    int proxyp;			/*!< FTP/HTTP: proxy port */
    int	port;
    int urltype;
    FD_t ctrl;			/*!< control channel */
    FD_t data;			/*!< per-xfer data channel */
    int bufAlloced;		/*!< sizeof I/O buffer */
    char *buf;			/*!< I/O buffer */
    int openError;		/*!< Type of open failure */
    int httpVersion;
    int httpHasRange;
    int magic;
} *urlinfo;

#ifdef __cplusplus
extern "C" {
#endif

extern int url_iobuf_size;

/**
 * Create a URL control structure instance.
 * @param msg		debugging identifier (unused)
 * @return		new instance
 */
urlinfo	urlNew(const char * msg);
urlinfo	XurlNew(const char * msg, const char * file, unsigned line);
#define	urlNew(_msg) XurlNew(_msg, __FILE__, __LINE__)

/**
 * Reference a URL control structure instance.
 * @param u		URL control structure
 * @param msg		debugging identifier (unused)
 * @return		referenced instance
 */
urlinfo	urlLink(urlinfo u, const char * msg);
urlinfo	XurlLink(urlinfo u, const char * msg, const char * file, unsigned line);
#define	urlLink(_u, _msg) XurlLink(_u, _msg, __FILE__, __LINE__)

/**
 * Dereference a URL control structure instance.
 * @param u		URL control structure
 * @param msg		debugging identifier (unused)
 * @return		dereferenced instance (NULL if freed)
 */
urlinfo	urlFree( /*@killref@*/ urlinfo u, const char * msg);
urlinfo	XurlFree( /*@killref@*/ urlinfo u, const char * msg, const char * file, unsigned line);
#define	urlFree(_u, _msg) XurlFree(_u, _msg, __FILE__, __LINE__)

/**
 * Free cached URL control structures.
 */
void	urlFreeCache(void);

/**
 * Return type of URL.
 * @param url		url string
 * @return		type of url
 */
urltype	urlIsURL(const char * url)	/*@*/;

/**
 * Return path component of URL.
 * @param url		url string
 * @retval pathp	pointer to path component of url
 * @return		type of url
 */
urltype	urlPath(const char * url, /*@out@*/ const char ** pathp)
	/*@modifies *pathp@*/;

/**
 * Parse URL string into a control structure.
 * @param url		url string
 * @retval u		address of new control instance pointer
 * @return		0 on success, -1 on error
 */
int 	urlSplit(const char * url, /*@out@*/ urlinfo * u)
		/*@modifies *u @*/;

/**
 * Copy data from URL to local file.
 * @param url		url string of source
 * @param dest		file name of destination
 * @return		0 on success, otherwise FTPERR_* code
 */
int	urlGetFile(const char * url, const char * dest);

#ifdef __cplusplus
}
#endif

#endif	/* H_RPMURL */

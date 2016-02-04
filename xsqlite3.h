#ifndef __include_xsqlite3_h__
#define __include_xsqlite3_h__

#include <stdarg.h>
#include <sqlite3.h>

typedef enum {
	XSQLITE3_END,
	XSQLITE3_INT,
	XSQLITE3_INT64,
	XSQLITE3_TEXT,
} XSQLITE3;

typedef void (*XSQLITE3_CBFUNC)(void *arg, sqlite3 *db, sqlite3_stmt *stmt);

void xsqlite3_err(const char *func, int err);

void xsqlite3_exec(sqlite3 *db, const char *fmt, ...);
void xsqlite3_fexec(sqlite3 *db, void *arg, XSQLITE3_CBFUNC cb, const char *fmt, ...);
void xsqlite3_vexec(sqlite3 *db, const char *fmt, va_list ap);
void xsqlite3_vfexec(sqlite3 *db, void *arg, XSQLITE3_CBFUNC cb, const char *fmt, va_list ap);

sqlite3_stmt *xsqlite3_prepare(sqlite3 *db, const char *fmt, ...);
sqlite3_stmt *xsqlite3_vprepare(sqlite3 *db, const char *fmt, va_list ap);

#endif /* __include_xsqlite3_h__ */

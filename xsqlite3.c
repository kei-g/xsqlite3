#include <stdio.h>
#include <stdlib.h>

#include "xsqlite3.h"

static void xsqlite3_do_nothing(void *arg, sqlite3 *db, sqlite3_stmt *stmt)
{
}

void xsqlite3_err(const char *func, int res)
{
	(void)fprintf(stderr, "%s => %d, %s\n", func, res, sqlite3_errstr(res));
	exit(EXIT_FAILURE);
}

void xsqlite3_exec(sqlite3 *db, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	xsqlite3_vexec(db, fmt, ap);
	va_end(ap);
}

void xsqlite3_fexec(sqlite3 *db, void *arg, XSQLITE3_CBFUNC cb, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	xsqlite3_vfexec(db, arg, cb, fmt, ap);
	va_end(ap);
}

void xsqlite3_vexec(sqlite3 *db, const char *fmt, va_list ap)
{
	xsqlite3_vfexec(db, NULL, xsqlite3_do_nothing, fmt, ap);
}

void xsqlite3_vfexec(sqlite3 *db, void *arg, XSQLITE3_CBFUNC cb, const char *fmt, va_list ap)
{
	sqlite3_stmt *stmt = xsqlite3_vprepare(db, fmt, ap);
	for (;;) {
		int res = sqlite3_step(stmt);
		if (res == SQLITE_DONE)
			break;
		if (res != SQLITE_ROW)
			xsqlite3_err("sqlite3_step", res);
		(*cb)(arg, db, stmt);
	}
	int res = sqlite3_finalize(stmt);
	if (res != SQLITE_OK)
		xsqlite3_err("sqlite3_finalize", res);
}

sqlite3_stmt *xsqlite3_prepare(sqlite3 *db, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	sqlite3_stmt *stmt = xsqlite3_vprepare(db, fmt, ap);
	va_end(ap);
	return (stmt);
}

sqlite3_stmt *xsqlite3_vprepare(sqlite3 *db, const char *fmt, va_list ap)
{
	sqlite3_stmt *stmt;
	int res = sqlite3_prepare(db, fmt, -1, &stmt, NULL);
	if (res != SQLITE_OK)
		xsqlite3_err("sqlite3_prepare", res);
	for (;;) {
		XSQLITE3 type = va_arg(ap, XSQLITE3);
		if (type == XSQLITE3_END)
			return (stmt);
		const char *name = va_arg(ap, const char *);
		int index = sqlite3_bind_parameter_index(stmt, name);
		if (!index)
			xsqlite3_err("sqlite3_parameter_index", SQLITE_NOTFOUND);
		switch (type) {
		case XSQLITE3_END:
			break;
		case XSQLITE3_INT:
			res = sqlite3_bind_int(stmt, index, va_arg(ap, int));
			if (res != SQLITE_OK)
				xsqlite3_err("sqlite3_bind_int", res);
			break;
		case XSQLITE3_INT64:
			res = sqlite3_bind_int64(stmt, index, va_arg(ap, sqlite3_int64));
			if (res != SQLITE_OK)
				xsqlite3_err("sqlite3_bind_int64", res);
			break;
		case XSQLITE3_TEXT:
			res = sqlite3_bind_text(stmt, index, va_arg(ap, const char *), -1,
				SQLITE_TRANSIENT);
			if (res != SQLITE_OK)
				xsqlite3_err("sqlite3_bind_text", res);
			break;
		}
	}
}

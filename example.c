#include <stdio.h>

#include "xsqlite3.h"

static void show_foo(void *arg, sqlite3 *db, sqlite3_stmt *stmt)
{
	int id = sqlite3_column_int(stmt, 0);
	const char *name = (char *)sqlite3_column_text(stmt, 1);
	printf("%d, %s\n", id, name);
}

int main(argc, argv)
	int argc;
	char *argv[];
{
	sqlite3 *db;
	int res = sqlite3_open(argv[1], &db);
	if (res != SQLITE_OK)
		xsqlite3_err("sqlite3_open", res);
	xsqlite3_exec(db, "drop table if exists foo", XSQLITE3_END);
	xsqlite3_exec(db, "create table foo(id, name)", XSQLITE3_END);
	xsqlite3_exec(db, "insert into foo values (:id, :name)",
		XSQLITE3_INT64, ":id", 838861LL,
		XSQLITE3_TEXT, ":name", "foo",
		XSQLITE3_END);
	xsqlite3_exec(db, "insert into foo values (:id, :name)",
		XSQLITE3_INT, ":id", 551,
		XSQLITE3_TEXT, ":name", "butaman",
		XSQLITE3_END);
	xsqlite3_fexec(db, NULL, show_foo, "select id, name from foo", XSQLITE3_END);
	(void)sqlite3_close(db);
	return (0);
}

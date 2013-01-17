/*
 * Copyright 2007 Jon Loeliger, Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *                                                                   USA
 */

#ifndef _SRCPOS_H_
#define _SRCPOS_H_

#include <stdio.h>

struct srcfile_state {
	FILE *f;
	char *name;
	char *dir;
	int lineno, colno;
	struct srcfile_state *prev;
};

extern struct srcfile_state *current_srcfile; /* = NULL */

FILE *srcfile_relative_open(const char *fname, char **fullnamep);
void srcfile_push(const char *fname);
int srcfile_pop(void);

struct srcpos {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    struct srcfile_state *file;
};

#define YYLTYPE struct srcpos

#define YYLLOC_DEFAULT(Current, Rhs, N)						\
	do {									\
		if (N) {							\
			(Current).first_line = YYRHSLOC(Rhs, 1).first_line;	\
			(Current).first_column = YYRHSLOC(Rhs, 1).first_column;	\
			(Current).last_line = YYRHSLOC(Rhs, N).last_line;	\
			(Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
			(Current).file = YYRHSLOC(Rhs, N).file;			\
		} else {							\
			(Current).first_line = (Current).last_line =		\
				YYRHSLOC(Rhs, 0).last_line;			\
			(Current).first_column = (Current).last_column =	\
				YYRHSLOC(Rhs, 0).last_column;			\
			(Current).file = YYRHSLOC (Rhs, 0).file;		\
		}								\
	} while (0)


/*
 * Fictional source position used for IR nodes that are
 * created without otherwise knowing a true source position.
 * For example,constant definitions from the command line.
 */
extern struct srcpos srcpos_empty;

extern void srcpos_update(struct srcpos *pos, const char *text, int len);
extern struct srcpos *srcpos_copy(struct srcpos *pos);
extern char *srcpos_string(struct srcpos *pos);
extern void srcpos_dump(struct srcpos *pos);

extern void srcpos_verror(struct srcpos *pos, char const *, va_list va)
     __attribute__((format(printf, 2, 0)));
extern void srcpos_error(struct srcpos *pos, char const *, ...)
     __attribute__((format(printf, 2, 3)));
extern void srcpos_warn(struct srcpos *pos, char const *, ...)
     __attribute__((format(printf, 2, 3)));

#endif /* _SRCPOS_H_ */

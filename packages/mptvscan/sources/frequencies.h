#ifndef _FREQUENCIES_H_
#define _FREQUENCIES_H_

struct CHANLIST {
    char *name;
    int   freq;
};

struct CHANLISTS {
    char             *name;
    struct CHANLIST  *list;
    int               count;
};

#define CHAN_COUNT(x) (sizeof(x)/sizeof(struct CHANLIST))

/* --------------------------------------------------------------------- */

extern struct CHANLISTS   chanlists[];

extern int                chantab;
extern struct CHANLIST   *chanlist;
extern int                chancount;

#endif /* _FREQUENCIES_H_ */

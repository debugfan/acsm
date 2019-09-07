/* $Id: acsmx.h,v 1.21 2015/04/23 18:28:11 jocornet Exp $ */
/*
** Copyright (C) 2014-2015 Cisco and/or its affiliates. All rights reserved.
** Copyright (C) 2002-2013 Sourcefire, Inc.
** Copyright (C) 2002 Martin Roesch <roesch@sourcefire.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


/*
**   ACSMX.H
**
**
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//#include "sf_types.h"
#include "ac_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ACSMX_H
#define ACSMX_H

/*
*   Prototypes
*/

#define ALPHABET_SIZE    256

#define ACSM_FAIL_STATE   -1

typedef struct _acsm_userdata {
    uint32_t ref_count;
    void *id;

} ACSM_USERDATA;

typedef struct _acsm_pattern {

    struct  _acsm_pattern *next;
    unsigned char         *patrn;
    unsigned char         *casepatrn;
    int      n;
    int      nocase;
    int      offset;
    int      depth;
    int      negative;
    ACSM_USERDATA *udata;
    int      iid;
    void   * rule_option_tree;
    void   * neg_list;
    int main_id;
    int sub_id;
    int is_last;
	int patrn_off;
	int patrn_cut;
} ACSM_PATTERN;


typedef struct  {

    /* Next state - based on input character */
    int      NextState[ ALPHABET_SIZE ];

    /* Failure state - used while building NFA & DFA  */
    int      FailState;

    /* List of patterns that end here, if any */
    ACSM_PATTERN *MatchList;

} ACSM_STATETABLE;


/*
* State machine Struct
*/
typedef struct {

    int acsmMaxStates;
    int acsmNumStates;

    ACSM_PATTERN    * acsmPatterns;
    ACSM_STATETABLE * acsmStateTable;

    int   bcSize;
    short bcShift[256];

    int numPatterns;
    void (*userfree)(void *p);
    void (*optiontreefree)(void **p);
    void (*neg_list_free)(void **p);

    int nMainPatterns;

    int nMaxPatternLength;
} ACSM_STRUCT;

typedef struct {
    int *match_table;
    int *match_cache;
} ACSM_MATCH_CONTEXT;

typedef int acsm_match_state_t;

/*
*   Prototypes
*/
ACSM_STRUCT * acsmNew (void (*userfree)(void *p),
                       void (*optiontreefree)(void **p),
                       void (*neg_list_free)(void **p));

int acsmAddPattern( ACSM_STRUCT * p, const unsigned char *pat, int n,
          int nocase, int offset, int depth, int negative, void * id, int iid, 
          int sub_oid, int is_last);

int acsmAddPatternExtended(ACSM_STRUCT * acsm, const unsigned char *pat, int n, int nocase,
    int offset, int depth, int negative, void * id, int iid);

int acsmGetMatchTableNumbers(ACSM_STRUCT *acsm);
int acsmResetMatchTable(ACSM_STRUCT *acsm, int *match_table);

int acsmCompile ( ACSM_STRUCT * acsm,
             int (*build_tree)(void * id, void **existing_tree),
             int (*neg_list_func)(void *id, void **list));
struct _SnortConfig;
int acsmCompileWithSnortConf ( struct _SnortConfig *, ACSM_STRUCT * acsm,
                               int (*build_tree)(struct _SnortConfig *, void * id, void **existing_tree),
                               int (*neg_list_func)(void *id, void **list));

//int acsmSearch ( ACSM_STRUCT * acsm,unsigned char * T, int n,
//                 int (*Match)(void * id, void *tree, int index, void *data, void *neg_list),
//                 void * data, int* current_state );
int acsmSearch(ACSM_STRUCT * acsm, const unsigned char * T, int n,
    int(*match_func)(const unsigned char *pat, int n, void * id, int index, void *data),
    void * data, 
    int *current_state, int *match_table);

void acsmFree ( ACSM_STRUCT * acsm );
int acsmPatternCount ( ACSM_STRUCT * acsm );

int acsmPrintDetailInfo(ACSM_STRUCT *);

int acsmPrintSummaryInfo(void);

#endif

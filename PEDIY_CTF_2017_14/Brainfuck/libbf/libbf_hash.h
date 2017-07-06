/***************************************************************************
        libbf - Free BrainFuck interpreter, optimizer, compiler library
                            -------------------
    copyright            : (C) 2005-2006 by Even Rouault
    email                : even 'dot' rouault 'at' mines-paris 'dot' org
***************************************************************************/

/* This is a modified and simplified version of original ghash.h for libbf */


/* GLIB - Library of useful routines for C programming
 * Copyright (C) 1995-1997  Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/. 
 */

#ifndef __LIBBF_HASH_H__
#define __LIBBF_HASH_H__

typedef struct _LibbfHashTable  LibbfHashTable;

typedef void (*LibbfDestroyNotify)(void*);
typedef void (*LibbfHFunc)(int key, void* value, void* user_data);
typedef void* (*LibbfCloneValue)(void* value);

/* Hash tables
 */
LibbfHashTable* libbf_hash_table_new   (LibbfDestroyNotify value_destroy_func);
LibbfHashTable* libbf_hash_table_clone(LibbfHashTable *hash_table,
                                       LibbfCloneValue clone_value_func);
void	    libbf_hash_table_destroy   (LibbfHashTable	   *hash_table);
void	    libbf_hash_table_insert   (LibbfHashTable	   *hash_table,
                                     int	    key,
                                     void*	    value);
int    libbf_hash_table_remove   (LibbfHashTable	   *hash_table,
                                  int   key);
void*    libbf_hash_table_lookup   (LibbfHashTable	   *hash_table,
                                    int key);
void** libbf_hash_table_lookup_pointer (LibbfHashTable	  *hash_table, int key);
void	    libbf_hash_table_foreach (LibbfHashTable	   *hash_table,
                                    LibbfHFunc	    func,
                                    void*	    user_data);
unsigned int libbf_hash_table_size (LibbfHashTable	   *hash_table);

#endif /* __G_HASH_H__ */


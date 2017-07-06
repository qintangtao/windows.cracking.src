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


#include <stdlib.h>
#include "libbf_internal.h"

#include "libbf_hash.h"

#define HASH_TABLE_MIN_SIZE 11
#define HASH_TABLE_MAX_SIZE 13845163

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


typedef struct _LibbfHashNode      LibbfHashNode;

struct _LibbfHashNode
{
  int        key;
  void*      value;
  LibbfHashNode *next;
};

struct _LibbfHashTable
{
  int             size;
  int             nnodes;
  LibbfHashNode      **nodes;
  LibbfDestroyNotify   value_destroy_func;
};

static const unsigned int libbf_primes[] =
{
  11,
  19,
  37,
  73,
  109,
  163,
  251,
  367,
  557,
  823,
  1237,
  1861,
  2777,
  4177,
  6247,
  9371,
  14057,
  21089,
  31627,
  47431,
  71143,
  106721,
  160073,
  240101,
  360163,
  540217,
  810343,
  1215497,
  1823231,
  2734867,
  4102283,
  6153409,
  9230113,
  13845163,
};

static const unsigned int libbf_nprimes = sizeof (libbf_primes) / sizeof (libbf_primes[0]);

unsigned int libbf_spaced_primes_closest (unsigned int num)
{
  int i;

  for (i = 0; i < libbf_nprimes; i++)
    if (libbf_primes[i] > num)
      return libbf_primes[i];

  return libbf_primes[libbf_nprimes - 1];
}

#define LIBBF_HASH_TABLE_RESIZE(hash_table)				\
   do {						\
     if ((hash_table->size >= 3 * hash_table->nnodes &&	        \
	  hash_table->size > HASH_TABLE_MIN_SIZE) ||		\
	 (3 * hash_table->size <= hash_table->nnodes &&	        \
	  hash_table->size < HASH_TABLE_MAX_SIZE))		\
	   libbf_hash_table_resize (hash_table);			\
    } while(0)

static void		libbf_hash_table_resize	  (LibbfHashTable	  *hash_table);
static LibbfHashNode**	libbf_hash_table_lookup_node  (LibbfHashTable     *hash_table,
                                                   int   key);
static LibbfHashNode*	libbf_hash_node_new		  (int	   key,
                                           void*        value);
static void		libbf_hash_nodes_destroy	  (LibbfHashNode	  *hash_node,
                                           LibbfDestroyNotify   value_destroy_func);

LibbfHashTable*
libbf_hash_table_new (LibbfDestroyNotify  value_destroy_func)
{
  LibbfHashTable *hash_table;
  
  hash_table = alloc0 (LibbfHashTable, 1);
  hash_table->size               = HASH_TABLE_MIN_SIZE;
  hash_table->nnodes             = 0;
  hash_table->value_destroy_func = value_destroy_func;
  hash_table->nodes              = alloc0 (LibbfHashNode*, hash_table->size);
  
  return hash_table;
}

LibbfHashTable* libbf_hash_table_clone(LibbfHashTable *hash_table,
                                       LibbfCloneValue clone_value_func)
{
  LibbfHashTable *hash_table_new;
  LibbfHashNode *new_node;
  LibbfHashNode *node;
  int i;

  hash_table_new                     = alloc0 (LibbfHashTable, 1);
  hash_table_new->size               = hash_table->size;
  hash_table_new->nnodes             = hash_table->nnodes;
  hash_table_new->value_destroy_func = hash_table->value_destroy_func;
  hash_table_new->nodes              = alloc0 (LibbfHashNode*, hash_table_new->size);
  for (i = 0; i < hash_table->size; i++)
  {
    node = hash_table->nodes[i];
    while(node)
    {
      LibbfHashNode *next = hash_table_new->nodes[i];
      new_node = libbf_hash_node_new(node->key,
                                     (clone_value_func)? clone_value_func(node->value) : node->value);
      new_node->next = next;
      hash_table_new->nodes[i] = new_node;
      node = node->next;
    }
  }
  return hash_table_new;
}

void
libbf_hash_table_destroy (LibbfHashTable *hash_table)
{
  int i;
  
  for (i = 0; i < hash_table->size; i++)
  {
    libbf_hash_nodes_destroy (hash_table->nodes[i], 
                              hash_table->value_destroy_func);
    hash_table->nodes[i] = NULL;
  }
  free (hash_table->nodes);
  free (hash_table);
}

static inline LibbfHashNode**
libbf_hash_table_lookup_node (LibbfHashTable	*hash_table,
                              int key)
{
  LibbfHashNode **node;

  node = &hash_table->nodes[(unsigned int)key % hash_table->size];
  while (*node && (*node)->key != key)
    node = &(*node)->next;

  return node;
}

void*
libbf_hash_table_lookup (LibbfHashTable	  *hash_table, int key)
{
  LibbfHashNode *node;

  node = *libbf_hash_table_lookup_node (hash_table, key);

  return node ? node->value : NULL;
}

void**
libbf_hash_table_lookup_pointer (LibbfHashTable	  *hash_table, int key)
{
  LibbfHashNode *node;

  node = *libbf_hash_table_lookup_node (hash_table, key);

  return node ? &node->value : NULL;
}


void
libbf_hash_table_insert (LibbfHashTable *hash_table,
                         int	 key,
                         void*	 value)
{
  LibbfHashNode **node;
  
  node = libbf_hash_table_lookup_node (hash_table, key);
  
  if (*node)
    {
      /* do not reset node->key in this place, keeping
       * the old key is the intended behaviour. 
       * libbf_hash_table_replace() can be used instead.
       */
      if (hash_table->value_destroy_func)
        hash_table->value_destroy_func ((*node)->value);

      (*node)->value = value;
    }
  else
    {
      *node = libbf_hash_node_new (key, value);
      hash_table->nnodes++;
      LIBBF_HASH_TABLE_RESIZE (hash_table);
    }
}
int
libbf_hash_table_remove (LibbfHashTable	   *hash_table,
                         int  key)
{
  LibbfHashNode **node, *dest;
  
  node = libbf_hash_table_lookup_node (hash_table, key);
  if (*node)
  {
    dest = *node;
    (*node) = dest->next;
    if (hash_table->value_destroy_func)
      hash_table->value_destroy_func (dest->value);
    free (dest);
    hash_table->nnodes--;

    LIBBF_HASH_TABLE_RESIZE (hash_table);

    return TRUE;
  }

  return FALSE;
}


void
libbf_hash_table_foreach (LibbfHashTable *hash_table,
                          LibbfHFunc	  func,
                          void*	  user_data)
{
  LibbfHashNode *node;
  int i;
  
  for (i = 0; i < hash_table->size; i++)
    for (node = hash_table->nodes[i]; node; node = node->next)
      (* func) (node->key, node->value, user_data);
}

unsigned int
libbf_hash_table_size (LibbfHashTable *hash_table)
{
  return hash_table->nnodes;
}

static void
libbf_hash_table_resize (LibbfHashTable *hash_table)
{
  LibbfHashNode **new_nodes;
  LibbfHashNode *node;
  LibbfHashNode *next;
  unsigned int hash_val;
  int new_size;
  int i;

  new_size = libbf_spaced_primes_closest (hash_table->nnodes);
  new_size = CLAMP (new_size, HASH_TABLE_MIN_SIZE, HASH_TABLE_MAX_SIZE);
 
  new_nodes = alloc0 (LibbfHashNode*, new_size);
  
  for (i = 0; i < hash_table->size; i++)
    for (node = hash_table->nodes[i]; node; node = next)
    {
      next = node->next;
    
      hash_val = (unsigned int)(node->key) % new_size;
    
      node->next = new_nodes[hash_val];
      new_nodes[hash_val] = node;
    }
  
  free (hash_table->nodes);
  hash_table->nodes = new_nodes;
  hash_table->size = new_size;
}

static LibbfHashNode*
libbf_hash_node_new (int key,
                     void* value)
{
  LibbfHashNode *hash_node = alloc0 (LibbfHashNode, 1);
  
  hash_node->key = key;
  hash_node->value = value;
  hash_node->next = NULL;
  
  return hash_node;
}

static void
libbf_hash_nodes_destroy (LibbfHashNode *hash_node,
                          LibbfDestroyNotify  value_destroy_func)
{
  while (hash_node)
  {
    LibbfHashNode *next = hash_node->next;
    if (value_destroy_func)
      value_destroy_func (hash_node->value);
    free (hash_node);
    hash_node = next;
  }
}

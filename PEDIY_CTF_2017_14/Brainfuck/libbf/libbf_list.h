/***************************************************************************
        libbf - Free BrainFuck interpreter, optimizer, compiler library
                            -------------------
    copyright            : (C) 2005-2006 by Even Rouault
    email                : even 'dot' rouault 'at' mines-paris 'dot' org
***************************************************************************/

/*
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */
 
#ifndef LIBBF_LIST_H
#define LIBBF_LIST_H

/* -------------------------------------------------------- */
/* List management */
/* -------------------------------------------------------- */

typedef struct _List List;

struct _List
{
  void* data;
  List* prev;
  List* next;
};

int list_get_size(List* list);

List* list_begin(List* list);

List* list_end(List* list);

List* list_append(List* list, void* data);

List* list_prepend(List* list, void* data);

void list_insert_before(List* list, void* data);

void list_insert_after(List* list, void* data);

List* list_remove_head_link(List* list);

typedef void (*list_free_data_fct)(void*);

void list_free_elt(List* elt, list_free_data_fct free_fct);

List* list_free_elt_and_update_head(List* list, List* cur, list_free_data_fct fct);

void list_free(List* list, list_free_data_fct free_fct);

List* list_reverse(List* list);

int list_find(List* list, void* data);

int list_intersects(List* list1, List* list2);

/* list_to_be_appended is destroyed after the merge */
List* list_merge(List* list, List* list_to_be_appended);

#endif

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
 
#include <stdlib.h>

#include "libbf_list.h"

#include "libbf_internal.h"

/* -------------------------------------------------------- */
/* List management */
/* -------------------------------------------------------- */


List* list_prepend(List* list, void* data)
{
  List* first = alloc0(List, 1);
  if (list) list->prev = first;
  first->prev = NULL;
  first->next = list;
  first->data = data;
  return first;
}

int list_get_size(List* list)
{
  int n = 0;
  while(list)
  {
    n++;
    list = list->next;
  }
  return n;
}

List* list_begin(List* list)
{
  if (list)
    while(list->prev)
      list = list->prev;
  return list;
}

List* list_end(List* list)
{
  if (list)
  {
    while(list->next)
      list = list->next;
  }
  return list;
}

List* list_append(List* list, void* data)
{
  if (list == NULL)
    return list_prepend(NULL, data);
  else
  {
    List* first = list;
    list = list_end(list);
    list->next = alloc0(List, 1);
    list->next->data = data;
    list->next->next = NULL;
    list->next->prev = list;
    return first;
  }
}

List* list_remove_head_link(List* list)
{
  if (list == NULL)
  {
    return NULL;
  }
  else
  {
    List* next = list->next; 
    if (next) next->prev = NULL;
    free(list);
    return next;
  }
}

void list_insert_before(List* list, void* data)
{
  List* new_elt;
  ASSERT(list != NULL);
  
  new_elt = alloc0(List, 1);
  new_elt->data = data;
  new_elt->next = list;
  new_elt->prev = list->prev;
  if (list->prev) list->prev->next = new_elt;
  list->prev = new_elt;
}  

void list_insert_after(List* list, void* data)
{
  List* new_elt;
  ASSERT(list != NULL);
  
  new_elt = alloc0(List, 1);
  new_elt->data = data;
  new_elt->next = list->next;
  new_elt->prev = list;
  if (list->next) list->next->prev = new_elt;
  list->next = new_elt;
}

void list_free_elt(List* elt, list_free_data_fct free_fct)
{
  if (elt->next) elt->next->prev = elt->prev;
  if (elt->prev) elt->prev->next = elt->next;

  if (free_fct)
    free_fct(elt->data);
  free(elt);
}

List* list_free_elt_and_update_head(List* list, List* cur, list_free_data_fct fct)
{
  if (fct) fct(cur->data);
  if (cur == list)
    list = list_remove_head_link(list);
  else
    list_free_elt(cur, NULL);
  return list;
}

void list_free(List* list, list_free_data_fct free_fct)
{
  while(list)
  {
    List* next = list->next;
    if (free_fct)
      free_fct(list->data);
    free(list);
    list = next;
  }
}

  
List* list_reverse(List* list)
{
  while(list)
{
    List* next = list->next;

    list->next = list->prev;
    list->prev = next;

    if (next == NULL) return list;

    list = next;
}

  return list;
}

int list_find(List* list, void* data)
{
  List* ptr = list;
  while(ptr)
  {
    if (ptr->data == data)
    {
      return TRUE;
    }
    ptr = ptr->next;
  }
  return FALSE;
}

int list_intersects(List* list1, List* list2)
{
  List* ptr1 = list1;
  while(ptr1)
  {
    List* ptr2 = list2;
    while(ptr2)
    {
      if (ptr1->data == ptr2->data)
      {
        return TRUE;
      }
      ptr2 = ptr2->next;
    }    
    
    ptr1 = ptr1->next;
  }
  return FALSE;
}


/* list_to_be_appended is destroyed after the merge */
List* list_merge(List* list, List* list_to_be_appended)
{
  List* end = list_end(list);
  ASSERT(list_to_be_appended == NULL || list_to_be_appended->prev == NULL);
  if (end == NULL)
  {
    return list_to_be_appended;
  }
  else
  {
    end->next = list_to_be_appended;
    if (list_to_be_appended)
      list_to_be_appended->prev = end;
    return list;
  }
}

/*
 * list.c
 * 
 * Copyright The OBDH 2.0 Contributors.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief List implementation.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.21
 * 
 * \date 2024/09/01
 * 
 * \addtogroup list
 * \{
 */

#include <stddef.h>
#include <stdint.h>

#include "list.h"

void list_init( struct list * const list )
{
    list->index = NULL;
    list->end = NULL;
    list->items = ( uint32_t ) 0U;
}

void list_insert_end( struct list * const list, struct list_item * const item )
{
    if( list->index == NULL )
    {
        list->index = item;
        list->end = item;
        list->index->prev = NULL;
        list->index->next = NULL;
    }
    else
    {
        item->prev = list->end;
        item->next = NULL;
        list->end->next = item;
        list->end = item;
    }

    item->container = list;
    list->items++;
}

struct list_item * list_remove( struct list_item * const item )
{
    struct list_item * retval = NULL;
    struct list * const pList = item->container;

    if( pList != NULL )
    {
        item->next->prev = item->prev;

        if( pList->index == item )
        {
            pList->index = pList->index->next;
        }
        else
        {
            item->prev->next = item->next;
        }

        item->container = NULL;
        pList->items--;

        retval = item;
    }

    return retval;
}
/**< \} End of list group */

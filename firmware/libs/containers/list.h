/*
 * list.h
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
 * \brief List definition.
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.21
 * 
 * \date 2024/09/01
 * 
 * \defgroup list List
 * \ingroup containers
 * \{
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * \brief Forward declaring the list struct.
 */
struct list;

/*
 * \brief List item struct.
 */
struct list_item
{
    struct list * container;
    struct list_item * next;
    struct list_item * prev;
    void * data;
};

/*
 * \brief List struct.
 */
struct list
{
    uint32_t items;           /**< Number of items in the list. */
    struct list_item * index; /**< First item on the list. */
    struct list_item * end;   /**< Used to mark the last item on the list. */
};

/*
 * \brief Check if the list is empty
 *
 * \param[in] pList is a pointer to a list struct.
 */
#define list_is_empty( pList ) ( ( ( pList )->items == 0U ) ? true : false )

/*
 * \brief Iterates over all the items of the list.
 *
 * \param[in] pItem is a iterator that iterates over every entry of the pList.
 *
 * \param[in] pList is a pointer to a list struct.
 */
#define list_for_each( pList, pItem )                     \
    for( ( pItem ) = ( pList )->index; ( pItem ) != NULL; \
         ( pItem ) = ( pItem )->next ) // cppcheck-suppress misra-c2012-15.6

/*
 * \brief Iterates over all the items of the list in reverse order.
 *
 * \param[in] pItem is a iterator that iterates over every entry of the pList.
 *
 * \param[in] pList is a pointer to a list struct.
 */
#define list_for_each_reverse( pList, pItem )           \
    for( ( pItem ) = ( pList )->end; ( pItem ) != NULL; \
         ( pItem ) = ( pItem )->prev ) // cppcheck-suppress misra-c2012-15.6

/*
 * \brief Initialise the list.
 *
 * \param[in] list is a pointer to a list structure.
 *
 * \return None.
 */
void list_init( struct list * const list );

/*
 * \brief Inserts an item to a list.
 *
 * \param[in] list is a pointer to a list structure.
 *
 * \param[in] item is a pointer to item to be inserted.
 *
 * \return None.
 */
void list_insert_end( struct list * const list, struct list_item * const item );

/*
 * \brief Removes an item from a list.
 *
 * IMPORTANT!! This functions does not free the memory of
 * the item, memory management is entirely up to the caller.
 *
 * \param[in] list is a pointer to a list structure.
 *
 * \param[in] item is a pointer to item to be inserted.
 *
 * \return A pointer to the removed item or NULL if the item was not
 * in a list to begin with.
 */
struct list_item * list_remove( struct list_item * const item );

#ifdef __cplusplus
}
#endif

#endif

/**< \} End of list group */

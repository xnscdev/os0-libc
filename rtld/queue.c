/* queue.c -- This file is part of OS/0 libc.
   Copyright (C) 2021 XNSC

   OS/0 libc is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   OS/0 libc is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with OS/0 libc. If not, see <https://www.gnu.org/licenses/>. */

#include <branch.h>
#include <rtld.h>
#include <stdio.h>
#include <stdlib.h>

static struct queue_node *
rtld_queue_node_create (void *data, unsigned long priority)
{
  struct queue_node *node = malloc (sizeof (struct queue_node));
  if (unlikely (node == NULL))
    {
      fprintf (stderr, "ld.so: couldn't allocate memory\n");
      abort ();
    }
  node->q_data = data;
  node->q_priority = priority;
  node->q_next = NULL;
  return node;
}

void
rtld_queue_add (struct queue_node **head, void *data, int priority)
{
  struct queue_node *start = *head;
  struct queue_node *new = rtld_queue_node_create (data, priority);
  if (*head == NULL)
    *head = new;
  else if ((*head)->q_priority > priority)
    {
      new->q_next = *head;
      *head = new;
    }
  else
    {
      while (start->q_next != NULL && start->q_next->q_priority < priority)
	start = start->q_next;
      new->q_next = start->q_next;
      start->q_next = new;
    }
}

void *
rtld_queue_poll (struct queue_node **head)
{
  void *data = (*head)->q_data;
  *head = (*head)->q_next;
  return data;
}

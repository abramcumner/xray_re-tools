//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// FILE:        linklist.h
// DESCRIPTION: Linked-list template classes
// AUTHOR:      Tom Hudson
// HISTORY:     created 10 December 1995
//**************************************************************************/

#ifndef __LINKLIST_H__

#define __LINKLIST_H__

#include "maxheap.h"

template <class T> class LinkedEntryT: public MaxHeapOperators {
public:
	T data;
	void *next;
	LinkedEntryT(T& d) { data = d; next = NULL; }
};

template <class T, class TE> class LinkedListT: public MaxHeapOperators {
private:
    TE* head;
	TE* tail;
	int count;
public:
					LinkedListT() { head = tail = NULL; count = 0; }
					~LinkedListT() { New(); }
		void		New() {
						while(head) {
							TE* next = (TE*)head->next;
							delete head;
							head = next;
							}
						head = tail = NULL;
						count = 0;
						}
		int			Count() { return count; }
		void		Append(T& item) {
						TE *entry = new TE(item);
						if(tail)
							tail->next = entry;
						tail = entry;
						if(!head)
							head = entry;
						count++;	
						}
		T			&operator[](int index) {
						TE *e = head;
						while(index && e) {
							e = (TE*)e->next;
							index--;
							}
						// This should never happen, so we'll punt and return...
						// the head's data
						if(!e) {
							assert(0);
							return head->data;
							}
						return e->data;
						}
		LinkedListT	&operator=(LinkedListT &from) {
						New();
						for(int i = 0; i < from.Count(); ++i)
							Append(from[i]);
						return *this;
						}
	};

// Handy macro for defining linked-lists

#define MakeLinkedList(TYPE) typedef LinkedEntryT<TYPE> TYPE##Entry; typedef LinkedListT<TYPE,TYPE##Entry> TYPE##List;

#endif // __LINKLIST_H__

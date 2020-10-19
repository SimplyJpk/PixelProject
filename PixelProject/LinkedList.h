#pragma once

template <class T>
struct LinkedListNode {
		LinkedListNode<T>* Next = NULL;
		T* Data = NULL;
		LinkedListNode<T>* Prev = NULL;

		LinkedListNode(T* data) {
				Data = data;
		}
};

template <class T>
class LinkedList
{
public:
		LinkedListNode<T>* head;
		LinkedListNode<T>* tail;

		LinkedList() {
				head = NULL;
				tail = NULL;
		};
		~LinkedList() {
				//TODO Should we delete the data? Probably not.
		}

		void AddFrontNode(T* data) {
				LinkedListNode<T>* node = new LinkedListNode<T>(data);
				LinkedListNode<T>* tmp = head;
				if (head == NULL) {
						head = node;
						tail = node;
				}
				else {
						node->Next = head;
						head = node;
						head->Prev = node;
				}
		}
		void AddBackNode(T* data) {
				LinkedListNode<T>* node = new LinkedListNode<T>(data);
				if (tail == NULL) {
						tail = node;
						head = node;
				}
				else if(tail->Next == NULL) {
						tail->Next = node;
						tail = node;
				}
		}
		// Removes the node from the system, returning a pointer to the data, if deleteData is true, this will be NULL.
		T* RemoveNode(LinkedListNode<T>* node, bool deleteData = false) {
				// Removing Head
				if (node->Prev == NULL) {
						head = node->Next;
						head->Prev = NULL;
				}
				// Removing Tail
				else if (node->Next == NULL) {
						tail = node->Prev;
						tail->Next = NULL;
				}
				else {
						node->Prev->Next = node->Next;
						node->Next->Prev = node->Prev;
				}
				// We check if we want to delete data since some data types we don't really want to delete.
				if (deleteData) {
						delete &node->Data;
						delete node;
						return NULL;
				}
				// We delete the node, but return the pointer to the data
				T* data = node->Data;
				delete node;
				return data;
		}
};
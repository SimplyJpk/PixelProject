#pragma once
#include <cstddef>

template <class T>
struct LinkedListNode {
		LinkedListNode<T>* next = nullptr;
		T* data = nullptr;
		LinkedListNode<T>* prev = nullptr;

		LinkedListNode(T* data) {
				data = data;
		}
};

template <class T>
class LinkedList
{
public:
		LinkedListNode<T>* head;
		LinkedListNode<T>* tail;

		LinkedList() {
				head = nullptr;
				tail = nullptr;
		};
		~LinkedList() {
				//TODO Should we delete the data? Probably not.
		}

		void AddFrontNode(T* data) {
				LinkedListNode<T>* node = new LinkedListNode<T>(data);
				LinkedListNode<T>* tmp = head;
				if (head == nullptr) {
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
				if (tail == nullptr) {
						tail = node;
						head = node;
				}
				else if(tail->Next == NULL) {
						tail->Next = node;
						tail = node;
				}
		}
		// Removes the node from the system, returning a pointer to the data, if deleteData is true, this will be NULL.
		T* RemoveNode(LinkedListNode<T>* node, const bool delete_data = false) {
				// Removing Head
				if (node->Prev == nullptr) {
						head = node->Next;
						head->Prev = nullptr;
				}
				// Removing Tail
				else if (node->Next == nullptr) {
						tail = node->Prev;
						tail->Next = nullptr;
				}
				else {
						node->Prev->Next = node->Next;
						node->Next->Prev = node->Prev;
				}
				// We check if we want to delete data since some data types we don't really want to delete.
				if (delete_data) {
						delete &node->Data;
						delete node;
						return nullptr;
				}
				// We delete the node, but return the pointer to the data
				T* data = node->Data;
				delete node;
				return data;
		}
};
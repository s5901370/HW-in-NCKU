/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->next; return *(this); }
      iterator operator ++ (int) { iterator temp = *this; _node= _node->_next; return temp; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator temp = *this; _node=_node->_prev; return temp; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const {  return _node!=i._node; }
      bool operator == (const iterator& i) const {  return _node==i._node; }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return (iterator)_head; }
   iterator end() const { return (iterator)_head->_prev; }
   bool empty() const { return _head == _head->_prev; }
   size_t size() const {
      DListNode<T>* temp = _head;
      size_t t = 0;
      for(iterator it = begin();it != end();++it)
         t++;
      return t; }

   void push_back(const T& x) { 
      if(size()==0){
         _head = new DListNode<T>(x);
         _head->_next = _head;
         _head->_prev = _head;
      }
      else{
         DListNode<T> *temp = new DListNode<T>(x,_head->_prev->_prev,_head->_prev); 
         temp->_prev->next = temp;
         temp->_next->_prev = temp;
      }
   }
   void pop_front() { 
      if(size()==0)
         return;
      else{
         DListNode<T> *del = _head;
         _head->_next->_prev = _head->_prev;
         _head->_prev->_next = _head->_next;
         _head = _head->_next;
         delete del;
      }
   }
   void pop_back() { 
      if(size()==0)
         return;
      else{
         DListNode<T> *del = _head->_prev->_prev;
         _head->_prev->_prev = del->_prev;
         del->_prev->_next = _head->_prev;
         delete del;
      }
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if(empty())
         return false;
      else{
         DListNode<T> *del = pos._node;
         del->_prev->_next = del->_next;
         del->_next->_prev = del->_prev;
         delete del;
         return true;
      }

          }
   bool erase(const T& x) { 
      if(empty())
         return false; 
      else{
         for(iterator it = begin();it!=end();++it){
            if(*it==x){
               DListNode<T> *del = pos._node;
               del->_prev->_next = del->_next;
               del->_next->_prev = del->_prev;
               delete del;
               return true;
            }
         }
         return false;
            
      }
   }

   void clear() { 
      
   }  // delete all nodes except for the dummy node

   void sort() const { }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H

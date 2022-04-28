#pragma once
#include <iostream>
using namespace std;

template <class T>
class TStack {
private:
    T* arr;
    int size;
    int pos;
public:
    TStack(int _size = 20);
    ~TStack();
    TStack(const TStack& st);

    TStack& operator=(const TStack& st);

    bool operator==(const TStack& st) const;
    bool operator!=(const TStack& st) const;

    bool empty() const;
    bool full() const;
    void clear();

    void push(T elem);
    T pop();
    T top() const;

    friend ostream& operator<<(ostream& out, const TStack& st) {
        for (int i = 0; i <= st.pos; i++) {
            out << st.arr[i] << ' ';
        }
        return out;
    }
};

template <class T>
TStack<T>::TStack(int _size) {
    if (_size <= 0) {
        throw _size;
    }

    size = _size;
    arr = new T[size];
    pos = -1;
}

template <class T>
TStack<T>::~TStack() {
    delete[] arr;
}

template <class T>
TStack<T>::TStack(const TStack& st) {
    size = st.size;
    pos = st.pos;

    arr = new T[size];
    for (int i = 0; i <= pos; i++) {
        arr[i] = st.arr[i];
    }
}

template <class T>
TStack<T>& TStack<T>::operator=(const TStack<T>& st) {
    if (size != st.size) {
        delete[] arr;
        size = st.size;
        arr = new T[size];
    }
    pos = st.pos;

    for (int i = 0; i <= pos; i++) {
        arr[i] = st.arr[i];
    }
    return *this;
}

template <class T>
bool TStack<T>::operator==(const TStack& st) const {
    if (size != st.size) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        if (arr[i] != st.arr[i]) {
            return false;
        }
    }

    return true;
}

template<class T>
bool TStack<T>::operator!=(const TStack& st) const {
    return !operator==(st);
}

template <class T>
bool TStack<T>::empty() const {
    return pos == -1;
}

template <class T>
bool TStack<T>::full() const {
    return pos == (size - 1);
}

template <class T>
void TStack<T>::clear() {
    pos = -1;
}

template <class T>
void TStack<T>::push(T elem) {
    if (full()) {
        throw "Can't push: stack is already full";
    }

    pos++;
    arr[pos] = elem;
}

template <class T>
T TStack<T>::pop() {
    if (empty()) {
        throw "Can't pop: stack is empty";
    }

    T elem = arr[pos];
    pos--;

    return elem;
}

template <class T>
T TStack<T>::top() const {
    if (empty()) {
        throw "Can't access top element: stack is empty";
    }

    return arr[pos];
}
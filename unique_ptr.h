#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

class unique_ptr {
private:
    char* ptr;
public:
    unique_ptr() :ptr(nullptr) {};
    unique_ptr(char* _ptr) :ptr(_ptr) {};
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr(unique_ptr&& _ptr) : ptr(_ptr.ptr) {
        _ptr.ptr = nullptr;
    }
    unique_ptr(int n) : ptr(new char[n]) {}
    unique_ptr& operator=(unique_ptr&& _ptr) {
        this->ptr = _ptr.ptr;
        _ptr.ptr = nullptr;
        return *this;
    }
    char& operator*() {
        return *this->ptr;
    }
    char* operator->() {
        return this->ptr;
    }
    char* get() const {
        return this->ptr;
    }
    void reset() {
        if (ptr != nullptr) {
            delete[] ptr;
            ptr = nullptr;
        }
    }
    ~unique_ptr() {
        delete[] ptr;
    }
};

#endif // UNIQUE_PTR_H

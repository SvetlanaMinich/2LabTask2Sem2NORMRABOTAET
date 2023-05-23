#ifndef STRING_H
#define STRING_H
#include "unique_ptr.h"
#include "cfunc.h"
#include <memory>
class StringIterator
{
private:
    char* it;
public:
    StringIterator() :it(nullptr) {};
    StringIterator(char* _it) :it(_it) {};
    ~StringIterator() {};
    StringIterator& operator++() {
        ++it;
        return *this;
    }
    StringIterator operator++(int) {
        StringIterator _it = *this;
        ++it;
        return _it;
    }
    StringIterator& operator--() {
        --it;
        return *this;
    }
    StringIterator operator--(int) {
        StringIterator _it = *this;
        --it;
        return _it;
    }
    char& operator*() const {
        return *it;
    }
    char* operator->() const {
        return it;
    }
    bool operator==(const StringIterator& _it) {
        return it == _it.it;
    }
    bool operator!=(const StringIterator& _it) {
        return it != _it.it;
    }
    StringIterator& operator=(const StringIterator& _it) {
        it = _it.it;
        return *this;
    }
    StringIterator& begin();
    StringIterator& end();
};

class String : public StringIterator
{
private:
    unique_ptr string;
    int size;
    int capacity;
public:
    String() {
        size = 0;
        capacity = 1;
        string = unique_ptr(new char[1]);
        *string.get() = '\0';
    }
    String(const char* str) {
        size = strlen(str);
        capacity = size + 1;
        string = unique_ptr(new char[capacity]);
        vstrncpy(string.get(), str, capacity);
    }
    String(const String& str) {
        size = str.size;
        capacity = str.capacity;
        string = unique_ptr(new char[capacity]);
        vstrncpy(string.get(), str.string.get(), capacity);
    }
    String(String&& str) {
        string = std::move(str.string);
        size = str.size;
        capacity = str.capacity;
        str.size = 0;
        str.capacity = 0;
    }
    String& operator=(String&& str) {
        string = std::move(str.string);
        size = str.size;
        capacity = str.capacity;
        str.size = 0;
        str.capacity = 0;
        return *this;
    }
    bool operator==(const String& str) {
        return vstrcmp(string.get(), str.string.get()) == 0;
    }
    bool operator!=(const String& str) {
        return !(*this == str);
    }
    bool operator<(const String& str) {
        return vstrcmp(string.get(), str.string.get()) < 0;
    }
    bool operator>(const String& str) {
        return vstrcmp(string.get(), str.string.get()) > 0;
    }
    bool operator<=(const String& str) {
        return vstrcmp(string.get(), str.string.get()) <= 0;
    }
    bool operator>=(const String& str) {
        return vstrcmp(string.get(), str.string.get()) >= 0;
    }
    String operator+(const String& str) {
        String result;
        result.size = size + str.size;
        result.capacity = capacity + str.capacity;
        result.string = unique_ptr(new char[result.capacity]);
        vstrncpy(result.string.get(), string.get(), size);
        vstrncpy(result.string.get() + size, str.string.get(), str.size + 1);
        return result;
    }
    String operator+=(const String& str) {
        *this = *this + str;
        return *this;
    }
    char& operator[](int index) {
        return string.get()[index];
    }
    size_t get_size() {
        return vstrlen(string.get());
    }
    char* begin() {
        return string.get();
    }
    char* end() {
        return string.get() + size;
    }
    void reserve(int n) {
        if (n > capacity) {
            unique_ptr new_str(capacity);
            vmemcpy(new_str.get(), string.get(), size + 1);
            string = std::move(new_str);
            capacity = n;
        }
    }
    void resize(int n) {
        if (n > size) {
            reserve(n);
            vmemset(string.get() + size, '\0', n - size);
        }
        size = n;
    }
    void resize(int n, char c) {
        if (n > size) {
            reserve(n);
            vmemset(string.get() + size, c, n - size);
        }
        size = n;
    }
    void clear() {
        string.reset();
        size = 0;
        capacity = 0;
    }
    void push_back(char c) {
        if (size + 1 >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        string.get()[size] = c;
        string.get()[++size] = '\0';
    }
    String append(int n, char c) {
        String add;
        add.size = n;
        add.capacity = n + 1;
        add.string = unique_ptr(new char[add.capacity]);
        for (int i = 0; i < n; i++) add[i] = c;
        *this += add;
        return *this;
    }
    String append(const String& str) {
        *this += str;
        return *this;
    }
    String append(const String& str, int pos, int count) {
        String add;
        add.size = count;
        add.capacity = count + 1;
        add.string = unique_ptr(new char[capacity]);
        vstrncpy(add.string.get(), str.string.get() + pos, count);
        *this += add;
        return *this;
    }
    void erase(int pos) {
        String buf;
        buf.size = size - pos;
        buf.capacity = buf.size + 1;
        buf.string = unique_ptr(new char[buf.capacity]);
        vstrncpy(buf.string.get(), string.get(), buf.size);
        string.reset();
        size = buf.size;
        capacity = buf.capacity;
        string = unique_ptr(new char[capacity]);
        vstrcpy(string.get(), buf.string.get());
    }
    void erase(int pos, int count) {
        String buf1;
        String buf2;
        buf1.size = size - pos;
        buf1.capacity = buf1.size + 1;
        buf1.string = unique_ptr(new char[buf1.capacity]);
        buf2.size = size - (pos + count);
        buf2.capacity = buf2.size + 1;
        buf2.string = unique_ptr(new char[buf2.capacity]);
        vstrncpy(buf1.string.get(), string.get(), buf1.size);
        vstrncpy(buf2.string.get(), string.get() + pos + count, buf2.size);
        string.reset();
        size = buf1.size;
        capacity = size + 1;
        string = unique_ptr(new char[capacity]);
        vstrcpy(string.get(), buf1.string.get());
        *this += buf2;
    }
    void insert(size_t pos, const String& new_str)
    {
        size_t len = new_str.size;
        if (pos > size)
        {
            pos = size;
        }
        if (size + len > capacity)
        {
            reserve(size + len);
        }
        vmemmove(string.get() + pos + len, string.get() + pos, size - pos + 1);
        vmemcpy(string.get() + pos, new_str.string.get(), len);
        size += len;
    }
    String& substr(size_t pos, size_t len, const String& new_str)
    {
        if (pos > size) {
            throw std::out_of_range("Invalid position");
        }
        len = std::min(len, size - pos);
        erase(pos, len);
        insert(pos, new_str);
        return *this;
    }
    String& replace(const String& old_str, const String& new_str)
    {
        size_t pos = 0;
        size_t old_len = old_str.size;
        size_t new_len = new_str.size;
        while ((pos = find(old_str, pos)) != std::string::npos)
        {
            erase(pos, old_len);
            insert(pos, new_str);
            pos += new_len;
        }
        return *this;
    }
    size_t find(const String& str, size_t pos = 0) const
    {
        const char* c_str = string.get() + pos;
        const char* sub_str = str.string.get();
        const char* result = strstr(c_str, sub_str);
        return result ? result - string.get() : std::string::npos;
    }
};

#endif // STRING_H

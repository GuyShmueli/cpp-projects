#ifndef FINAL_PROJECT_C___VL_STRING_HPP
#define FINAL_PROJECT_C___VL_STRING_HPP
#include "vl_vector.hpp"
#include <cstring> // for std::strlen

template<size_t static_capacity = 16>
class vl_string {
private:
    vl_vector<char, static_capacity> chars;

public:
    /**
     * Default constructor that initializes the string to be empty.
     */
    vl_string() : chars() {} // Start with an empty vl_vector

    /**
     * Implicit constructor that constructs a string
     * object from a C-style string.
     * @param cstr A pointer to the null-terminated byte string
     * to be used to initialize the string.
     */
    vl_string(const char* cstr)
    {
        if (cstr)
        {
            size_t len = std::strlen(cstr);
            // Initialize len characters to null
            chars = vl_vector<char, static_capacity>(len, 0);
            std::copy(cstr, cstr + len, chars.begin());
        }
    }

    /**
     * Copy constructor that creates a copy of another string.
     * @param other Another string object to be used as a source
     * to initialize the object with.
     */
    vl_string(const vl_string& other) : chars(other.chars) {}

    /**
     * Destructor - uses default synthesized destructor
     */
    ~vl_string() = default;


    /**
     * Copy assignment operator that replaces the contents with a
     * copy of the contents of another string.
     * @param other Another string object to be used as a source to
     * initialize the object with.
     * @return Reference to the current string.
     */
    vl_string& operator=(const vl_string& other)
    {
        chars = other.chars;
        return *this;
    }

    /**
     * Assignment operator that replaces the contents with those
     * of a null-terminated C character string.
     * @param cstr A pointer to the C string to assign.
     * @return Reference to the current string.
     */
    vl_string& operator=(const char* cstr)
    {
        chars.clear();
        if (cstr)
        {
            size_t len = std::strlen(cstr);
            chars = vl_vector<char, static_capacity>(cstr, cstr + len);
        }
        return *this;
    }

    /**
     * Appends the contents of a null-terminated C
     * character string to the current string.
     * @param suffix The C string to append.
     * @return Reference to the current string.
     */
    vl_string& append(const char* suffix)
    {
        if (suffix)
        {
            size_t len = std::strlen(suffix);
            chars.insert(chars.end(), suffix, suffix + len);
        }
        return *this;
    }

    /**
     * Appends the contents of another string to the current string.
     *
     * @param suffix Another string to append.
     * @return Reference to the current string.
     */
    vl_string& append(const vl_string& suffix)
    {
        chars.insert(chars.end(), suffix.chars.begin(), suffix.chars.end());
        return *this;
    }

    /**
     * Appends the contents of a null-terminated C
     * character string to the current string.
     * @param suffix The C string to append.
     * @return Reference to the current string.
     */
    vl_string& operator+=(const char* suffix) {
        return append(suffix);
    }

    /**
     * Appends the contents of another string to the current string.
     * @param other Another string to append.
     * @return Reference to the current string.
     */
    vl_string& operator+=(const vl_string& other)
    {
        return append(other);
    }

    /**
     * Appends the contents of another string to the current string.
     * @param other Another string to append.
     * @return Reference to the current string.
     */
    vl_string& operator+=(char ch) {
        chars.push_back(ch);
        return *this;
    }

    /**
     * Appends the contents of a character vector to the current string.
     *
     * @param v A character vector to append.
     * @return Reference to the current string.
     */
    vl_string& operator+=(const vl_vector<char, static_capacity>& v)
    {
        chars.insert(chars.end(), v.begin(), v.end());
        return *this;
    }


    class Proxy
    {
    private:
        vl_string& str;
        size_t index;

    public:
        Proxy(vl_string& s, size_t i) : str(s), index(i) {}

        // Assignment operator to allow modification of characters via proxy
        Proxy& operator=(char c)
        {
            if (index >= str.size())
            {
                throw std::out_of_range("Index out of range");
            }
            str.chars[index] = c;
            return *this;
        }

        // Conversion operator to convert Proxy to char
        operator char() const
        {
            if (index >= str.size())
            {
                throw std::out_of_range("Index out of range");
            }
            return str.chars[index];
        }
    };

    /**
     * Returns a Proxy object to provide safe access to string characters
     * by index with bounds checking.
     * This allows for modification of the character at the
     * given index if needed.
     * @param index The index of the character in the string.
     * @return A Proxy object representing the character at the specified index.
     */
    Proxy operator[](size_t index)
    {
        if (index >= size())
        {
            throw std::out_of_range("Index out of range");
        }
        return Proxy(*this, index);
    }

    /**
     * Returns a reference to the character at the specified index
     * in the string, with bounds checking.
     * This const version ensures the character cannot be modified.
     * @param index The index of the character to return.
     * @return The character at the specified index.
     */
    char operator[](size_t index) const
    {
        if (index >= size())
        {
            throw std::out_of_range("Index out of range");
        }
        return chars[index];
    }

    /**
     * Returns the size of the string, excluding the null terminator.
     * @return The size of the string.
     */
    size_t size() const
    {
        return chars.size();
    }

    /**
     * Returns the capacity of the string,
     * which is the size of the allocated storage.
     * @return The capacity of the string.
     */
    size_t capacity() const
    {
        return chars.capacity();
    }

    /**
     * Checks if the string is empty (i.e., its size is 0).
     * @return True if the string is empty, false otherwise.
     */
    bool empty() const
    {
        return chars.empty();
    }

    /**
     * Converts the string to a standard string object.
     * @return A standard string object with the same content.
     */
    operator std::string() const
    {
        return std::string(chars.begin(), chars.end());
    }

    /**
     * Compares the string to another string for equality.
     * @param rhs The string to compare with.
     * @return True if the strings are equal, false otherwise.
     */
    bool operator==(const vl_string& rhs) const
    {
        // First, check if sizes match
        if (size() != rhs.size())
        {
            return false;
        }

        // Compare characters one by one
        for (size_t i = 0; i < size(); ++i)
        {
            if (chars[i] != rhs.chars[i])
            {
                return false;
            }
        }

        // If all characters match, strings are equal
        return true;
    }

    /**
     * Compares the string to another string for inequality.
     * @param rhs The string to compare with.
     * @return True if the strings are NOT equal, false otherwise.
     */
    bool operator!=(const vl_string& rhs) const
    {
        return !(operator==(rhs));
    }

    /**
     * Compares the string to a C-style string for equality.
     * @param rhs The C-style string to compare with.
     * @return True if the strings are equal, false otherwise.
     */
    bool operator==(const char* rhs) const
    {
        // Check if rhs is null
        if (rhs == nullptr)
        {
            return empty();
        }

        // Compare sizes first
        size_t rhs_len = std::strlen(rhs);
        if (size() != rhs_len)
        {
            return false;
        }

        // Compare characters one by one
        for (size_t i = 0; i < rhs_len; ++i)
        {
            if (chars[i] != rhs[i])
            {
                return false;
            }
        }

        // If all characters match, strings are equal
        return true;
    }

    /**
     * Compares the string to a C-style string for inequality.
     * @param rhs The C-style string to compare with.
     * @return True if the strings are NOT equal, false otherwise.
     */
    bool operator!=(const char* rhs) const
    {
        return !(operator==(rhs));
    }


    /**
     * Checks if rhs is equal to lhs.
     * @param lhs A pointer to the first character of the C-style string.
     * @param rhs A reference to the instance of `vl_string` to compare with.
     * @return True if the C-style string and the `vl_string`
     * contain the same characters, false otherwise.
     */
    friend bool operator==(const char* lhs, const vl_string& rhs)
    {
        return rhs == lhs;
    }

    /**
     * Checks if rhs is NOT equal to lhs.
     * @param lhs A pointer to the first character of the C-style string.
     * @param rhs A reference to the instance of `vl_string` to compare with.
     * @return True if the C-style string and the `vl_string`
     * DON'T contain the same characters, false otherwise.
     */
    friend bool operator!=(const char* lhs, const vl_string& rhs)
    {
        return !(rhs == lhs);
    }


    // Iterator Definitions
    class iterator
    {
        char* ptr;
        size_t idx;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = char;
        using difference_type = std::ptrdiff_t;
        using pointer = char*;
        using reference = char&;

        iterator(char* p = nullptr, size_t i = 0) : ptr(p), idx(i) {}

        reference operator*() const
        {
            return *(ptr + idx);
        }

        pointer operator->() const
        {
            return (ptr + idx);
        }

        reference operator[](difference_type n) const
        {
            return *(ptr + idx + n);
        }

        iterator& operator++()
        {
            ++idx; return *this;
        }

        iterator operator++(int)
        {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        iterator& operator--()
        {
            --idx;
            return *this;
        }

        iterator operator--(int)
        {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        iterator& operator+=(difference_type n)
        {
            idx += n;
            return *this;
        }

        iterator& operator-=(difference_type n)
        {
            idx -= n;
            return *this;
        }

        iterator operator+(difference_type n) const
        {
            return iterator(ptr, idx + n);
        }

        iterator operator-(difference_type n) const
        {
            return iterator(ptr, idx - n);
        }

        difference_type operator-(const iterator& other) const
        {
            return idx - other.idx;
        }

        bool operator==(const iterator& other) const
        {
            return ptr == other.ptr && idx == other.idx;
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

        bool operator<(const iterator& other) const
        {
            return idx < other.idx;
        }

        bool operator>(const iterator& other) const
        {
            return idx > other.idx;
        }

        bool operator<=(const iterator& other) const
        {
            return idx <= other.idx;
        }

        bool operator>=(const iterator& other) const
        {
            return idx >= other.idx;
        }
    };

    class const_iterator
    {
        const char* ptr;
        size_t idx;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const char;
        using difference_type = std::ptrdiff_t;
        using pointer = const char*;
        using reference = const char&;

        const_iterator(const char* p = nullptr, size_t i = 0) :
        ptr(p), idx(i) {}

        reference operator*() const
        {
            return *(ptr + idx);
        }

        pointer operator->() const
        {
            return (ptr + idx);
        }

        reference operator[](difference_type n) const
        {
            return *(ptr + idx + n);
        }

        const_iterator& operator++()
        {
            ++idx;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        const_iterator& operator--()
        {
            --idx;
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator temp = *this;
            --(*this);
            return temp;
        }

        const_iterator& operator+=(difference_type n)
        { idx += n;
            return *this;
        }

        const_iterator& operator-=(difference_type n)
        {
            idx -= n;
            return *this;
        }

        const_iterator operator+(difference_type n) const
        {
            return const_iterator(ptr, idx + n);
        }

        const_iterator operator-(difference_type n) const
        {
            return const_iterator(ptr, idx - n);
        }

        difference_type operator-(const const_iterator& other) const
        {
            return idx - other.idx;
        }

        bool operator==(const const_iterator& other) const
        {
            return ptr == other.ptr && idx == other.idx;
        }

        bool operator!=(const const_iterator& other) const
        {
            return !(*this == other);
        }

        bool operator<(const const_iterator& other) const
        {
            return idx < other.idx;
        }

        bool operator>(const const_iterator& other) const
        {
            return idx > other.idx;
        }

        bool operator<=(const const_iterator& other) const
        {
            return idx <= other.idx;
        }

        bool operator>=(const const_iterator& other) const
        {
            return idx >= other.idx;
        }
    };

    using vl_iterator =
            typename vl_vector<char, static_capacity>::iterator;
    using vl_const_iterator =
            typename vl_vector<char, static_capacity>::const_iterator;
    using reverse_vl_iterator = std::reverse_iterator<vl_iterator>;
    using const_reverse_iterator = std::reverse_iterator<vl_const_iterator>;

    vl_iterator begin()
    {
        return chars.begin();
    }

    vl_iterator end()
    {
        return chars.end();
    }

    vl_const_iterator begin() const
    {
        return chars.begin();
    }

    vl_const_iterator end() const
    {
        return chars.end();
    }

    vl_const_iterator cbegin() const
    {
        return chars.cbegin();
    }

    vl_const_iterator cend() const
    {
        return chars.size() > 0 ? chars.cend() - 1 : chars.cend();
    }

    reverse_vl_iterator rbegin()
    {
        return chars.rbegin();
    }

    reverse_vl_iterator rend()
    {
        return chars.size() > 0 ? chars.rend() - 1 : chars.rend();
    }

    const_reverse_iterator crbegin() const
    {
        return chars.crbegin();
    }

    const_reverse_iterator crend() const
    {
        return chars.size() > 0 ? chars.crend() - 1 : chars.crend();
    }

    const_reverse_iterator rbegin() const
    {
        return chars.rbegin();
    }

    const_reverse_iterator rend() const
    {
        return chars.size() > 0 ? chars.rend() - 1 : chars.rend();
    }

    void print_vec() const {
        for (auto value : *this) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }

    /**
     * Returns a const reference to the character at the specified
     * position in the string.
     * Does not allow modification of the character.
     * @param idx The index of the character.
     * @return Reference to the character at the index.
     */
    const char& at(int idx) const
    {
        return chars.at(idx);
    }

    /**
     * Returns a reference to the character at the specified
     * position in the string.
     * Allows modification of the character.
     * @param idx The index of the character.
     * @return Reference to the character at the index.
     */
    char& at(int idx)
    {
        return chars.at(idx);
    }

    /**
     * Adds a character to the end of the string.
     * @param ch The character to append.
     */
    void push_back(const char& ch)
    {
        chars.push_back(ch);
    }

    /**
     * Inserts a single character at the specified position.
     * @param position Iterator to the position where the
     * character should be inserted.
     * @param ch The character to insert.
     * @return An iterator that points to the inserted character.
     */
    vl_iterator insert(vl_iterator position, const char& ch)
    {
        return chars.insert(position, ch);
    }

    /**
     * Inserts characters from another range before the specified position.
     * @param position Iterator to the position where the
     * characters should be inserted.
     * @param first Iterator to the beginning of the range to insert from.
     * @param last Iterator to the end of the range to insert from.
     * @return Iterator that points to the first of the inserted characters.
     */
    template<class ForwardIterator>
    vl_iterator insert(vl_iterator position, ForwardIterator first,
                    ForwardIterator last)
    {
        return chars.insert(position, first, last);
    }

/**
 * Removes the last character from the string.
 */
    void pop_back()
    {
        chars.pop_back();
    }

    /**
     * Erases the character at the specified position.
     * @param position Iterator to the character to be erased.
     * @return An iterator following the last removed character.
     */
    vl_iterator erase(vl_iterator position)
    {
        return chars.erase(position);
    }

    /**
     * Erases the characters in the range [first, last).
     * @param first Iterator to the first character to erase.
     * @param last Iterator following the last character to erase.
     * @return An iterator following the last removed character.
     */
    vl_iterator erase(vl_iterator first, vl_iterator last)
    {
        return chars.erase(first, last);
    }

    /**
     * Clears the contents of the string, which becomes an empty string.
     */
    void clear()
    {
        chars.clear();
    }

    /**
     * Provides direct access to the underlying array.
     * @return A pointer to the character array.
    */
    char* data() const
    {
        return chars.data();
    }



};

#endif //FINAL_PROJECT_C___VL_STRING_HPP

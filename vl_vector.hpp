#ifndef FINAL_PROJECT_C___VL_VECTOR_HPP
#define FINAL_PROJECT_C___VL_VECTOR_HPP

#include <iostream>
#include <iterator>

template <typename T,  size_t static_capacity = 16UL>
class vl_vector
{
private:
    T data_static[static_capacity]; // Static array
    T* data_dynamic = nullptr; // Pointer for dynamic allocation
    size_t curr_size = 0; // Keeps track of the number of elements
    size_t dynamic_capacity = 0; // Capacity of the dynamic memory

public:

    /**
     * Default constructor for vl_vector.
     * Initializes an empty vector with no elements, preparing
     * either static or dynamic storage.
     */
    vl_vector() : data_dynamic(nullptr), curr_size(0),
    dynamic_capacity(static_capacity) {}


/**
 * This copy constructor initializes a new vector with the same elements
 * and capacity as the provided vector (other).
 * It decides whether to use static or dynamic memory
 * based on the original vector's storage usage.
 * @param other A reference to the vector from which to
 * copy elements and capacity.
 */
vl_vector(const vl_vector<T, static_capacity>& other)
            : curr_size(other.curr_size),
            dynamic_capacity(other.dynamic_capacity)
    {
        if (other.data_dynamic)
        {
            data_dynamic = new T[other.dynamic_capacity]();
            std::copy(other.data_dynamic,
                      other.data_dynamic + other.curr_size, data_dynamic);
        }

        else
        {
            data_dynamic = nullptr;
            std::copy(other.data_static,
                      other.data_static + other.curr_size, data_static);
        }
    }

    /**
     * Sequence-based constructor.
     * Constructs a vector from a range defined by two iterators.
     * @param first Iterator pointing to the beginning of the range.
     * @param last Iterator pointing to the end of the range.
     * The range used is [first, last).
     */
    template<class ForwardIterator>
    explicit vl_vector(const ForwardIterator& first,
                       const ForwardIterator& last)
    {
        // Calculate the number of elements between first and last iterators
        auto dist = std::distance(first, last);

        // Use static memory
        if (dist <= static_capacity)
        {
            std::copy(first, last, this->data_static);
            this->data_dynamic = nullptr;
            this->curr_size = dist;
            this->dynamic_capacity = static_capacity;
        }

        // Use dynamic memory
        else
        {
            this->data_dynamic = new T[(size_t)(1.5*dist)];
            std::copy(first, last, this->data_dynamic);
            this->curr_size = dist;
            this->dynamic_capacity = (size_t)(1.5*dist);
        }
    }

    /**
     * Single-Value Initialized Constructor.
     * Constructs a vector initialized with a given number of an element.
     * @param count The number of times the value should be
     * inserted into the vector.
     * @param v The value to insert multiple times.
     */
    explicit vl_vector(size_t count, const T& v)
    {
        // Use static memory
        if (count <= static_capacity)
        {
            for (size_t i = 0; i < count; i++)
            {
                this->data_static[i] = v;
            }
            this->data_dynamic = nullptr;
            this->curr_size = count;
            this->dynamic_capacity = static_capacity;
        }

        // Use dynamic memory
        else
        {
            this->data_dynamic = new T[(size_t)(1.5*count)];
            for (size_t i = 0; i < count; i++)
            {
                this->data_dynamic[i] = v;
            }

            this->curr_size = count;
            this->dynamic_capacity = (size_t)(1.5*count);
        }
    }

    /**
     * Initializer List Constructor.
     * Constructs a vector using an initializer list.
     * @param init_list Initializer list containing elements of type T.
     */
    vl_vector(std::initializer_list<T> init_list)
    {
        auto list_size = init_list.size();

        // Use static memory
        if (list_size <= static_capacity)
        {
            std::copy(init_list.begin(), init_list.end(), this->data_static);
            this->data_dynamic = nullptr;
            this->curr_size = list_size;
            this->dynamic_capacity = static_capacity;
        }

        // Use dynamic memory
        else
        {
            this->data_dynamic = new T[(size_t)(1.5*list_size)];
            std::copy(init_list.begin(), init_list.end(), this->data_dynamic);
            this->curr_size = list_size;
            this->dynamic_capacity = (size_t)(1.5*list_size);
        }
    }

    /**
     * Destructor for vl_vector.
     * Cleans up dynamic memory if it has been allocated.
     */
    ~vl_vector()
    {
        delete[] this->data_dynamic;
        data_dynamic = nullptr;
    }

    /**
     * Returns the number of elements in the vector.
     * @return The current size of the vector.
     */
    size_t size() const
    {
        return this->curr_size;
    }

    /**
     * Returns the total capacity of the vector.
     * @return The total capacity of the vector.
     */
    size_t capacity() const
    {
        // Static memory
        if (this->data_dynamic == nullptr)
        {
            return static_capacity;
        }

        return this->dynamic_capacity;
    }


    /**
     * Checks whether the vector is empty.
     * @return True if the vector is empty, false otherwise.
     */
    bool empty() const
    {
        return this->curr_size == 0;
    }


    /**
     * Returns a reference to the element at specified location idx,
     * with bounds checking.
     * @param idx The index of the element to return.
     * @return A reference to the requested element.
     */
    T& at(size_t idx) noexcept(false)
    {
        // Checking boundaries
        if(idx >= curr_size)
        {
            throw std::out_of_range("vl_vector::at: index out of range");
        }

        // The data_static is stored on the stack
        if (data_dynamic == nullptr)
        {
            return data_static[idx];
        }

        // The data_static is stored on the heap
        return data_dynamic[idx];
    }

    /**
     * Returns a const reference to the element at specified location idx,
     * with bounds checking.
     * @param idx The index of the element to return.
     * @return A const reference to the requested element.
     */
    const T& at(size_t idx) const noexcept(false)
    {
        // Checking boundaries
        if(idx >= curr_size)
        {
            throw std::out_of_range("vl_vector::at: index out of range");
        }

        // The data_static is stored on the stack
        if (data_dynamic == nullptr)
        {
            return data_static[idx];
        }

        // The data_static is stored on the heap
        return data_dynamic[idx];
    }


    /**
     * Adds an element to the end of the vector.
     * @param elem The element to be added to the end of the vector.
     */
    void push_back(const T& elem)
    {
        // Scenario 1: Static that remains static
        if (data_dynamic == nullptr &&
            this->curr_size < static_capacity)
        {
            this->data_static[curr_size] = elem;
            this->curr_size++;
        }

        // Scenario 2: Transition from static to dynamic
        else if (data_dynamic == nullptr &&
                 this->curr_size >= static_capacity)
        {
            this->dynamic_capacity = static_cast<size_t>
                                    (1.5 * (this->curr_size + 1));
            this->data_dynamic = new T[this->dynamic_capacity]();
            std::copy(this->data_static, this->data_static + this->curr_size,
                      this->data_dynamic);
            this->data_dynamic[this->curr_size] = elem;
            this->curr_size++;
        }

        // Scenario 3: Dynamic that doesn't need to change the dynamic cap
        else if (data_dynamic != nullptr &&
                this->curr_size < this->dynamic_capacity)
        {
            this->data_dynamic[this->curr_size] = elem;
            this->curr_size++;
        }

        // Scenario 4: Dynamic that does need to change the dynamic cap
        else
        {
            // Calculate new capacity
            auto new_dyn_cap = static_cast<size_t>
                                 (1.5 * (this->curr_size + 1));

            // Allocate new memory block with the updated capacity
            // If this fails, a std::bad_alloc exception will be thrown
            T* new_data = new T[new_dyn_cap]();

            // Didn't fail, hence we can safely copy the existing data_static to the
            // new_data
            this->dynamic_capacity = new_dyn_cap;
            std::copy(this->data_dynamic,
                      this->data_dynamic + this->curr_size, new_data);
            new_data[this->curr_size] = elem;

            // Delete what was stored in data_dynamic
            delete[] this->data_dynamic;

            // Assign data_dynamic pointer to the address of new_data
            this->data_dynamic = new_data;

            // Increment the curr_size
            this->curr_size++;
        }
    }

    // Iterator Definitions
    class iterator
    {
        T* ptr;
        size_t idx;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(T* p = nullptr, size_t i = 0) : ptr(p), idx(i) {}

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
            ++idx;
            return *this;
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
        const T* ptr;
        size_t idx;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const T* p = nullptr, size_t i = 0) : ptr(p), idx(i) {}

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
        {
            idx += n;
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


    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;



    /**
     * Inserts an element at the specified position within the vector.
     * Handles transition from static to dynamic storage if necessary.
     * @param position Iterator pointing to the position where the new element
     * should be inserted.
     * @param elem The element to be inserted.
     * @return An iterator pointing to the newly inserted element.
     */
    iterator insert (iterator position, const T& elem)
    {
        size_t index = position - begin();  // Compute index

        // Scenario 1: Static that remains static
        if (data_dynamic == nullptr &&
            curr_size < static_capacity)
        {
            std::copy_backward(begin() + index, end(), end() + 1);
            data_static[index] = elem;
            curr_size++;
            return begin() + index;
        }

        // Scenario 2: Transition from static to dynamic
        if (data_dynamic == nullptr &&
            curr_size >= static_capacity)
        {
            auto new_capacity =
                    static_cast<size_t>(1.5 * (curr_size + 1));
            T* new_data = new T[new_capacity];

            std::copy(begin(), position, new_data);
            new_data[index] = elem;  // Insert element
            std::copy(position, begin() + curr_size, new_data + index + 1);

            data_dynamic = new_data;
            dynamic_capacity = new_capacity;
            curr_size++;
            return iterator(data_dynamic + index);
        }

        // Scenario 3: Dynamic that doesn't need to change the dynamic cap
        if (data_dynamic != nullptr &&
            curr_size < dynamic_capacity)
        {
            std::copy_backward(begin() + index, end(), end() + 1);
            data_dynamic[index] = elem;
            curr_size++;
            return begin() + index;
        }

        // Scenario 4: Dynamic that does need to change the dynamic cap
        auto new_capacity = static_cast<size_t>(1.5 * (curr_size + 1));
        T* new_data = new T[new_capacity];

        try
        {
            std::copy(data_dynamic, data_dynamic + index, new_data);
            new_data[index] = elem;  // Insert element
            std::copy(data_dynamic + index, data_dynamic + curr_size, new_data + index + 1);
        } catch (...)
        {
            delete[] new_data;  // Cleanup in case of exception
            throw;
        }

        delete[] data_dynamic;  // Delete old data
        data_dynamic = new_data;
        dynamic_capacity = new_capacity;
        curr_size++;
        return iterator(data_dynamic + index);
    }

    /**
     * Inserts a range of elements at the specified position
     * within the vector.
     * Handles all necessary storage transitions.
     * @param position Iterator pointing to the position where new
     * elements should be inserted.
     * @param first Iterator pointing to the beginning of
     * the elements to insert.
     * @param last Iterator pointing to the end of the elements to insert.
     * @return An iterator pointing to the first newly inserted element.
     */
    template<class ForwardIterator>
    iterator insert(iterator position, ForwardIterator first,
                                        ForwardIterator last)
    {
        // Distance between first and last ForwardIterators
        // (# of elements)
        size_t insert_count = std::distance(first, last);

        // Distance between the first element in the vl_vector and position
        // (# of elements)
        size_t insert_position = std::distance(this->begin(), position);

        // Scenario 1: Static storage remains static
        if (this->data_dynamic == nullptr
            && this->curr_size + insert_count <= static_capacity)
        {
            // Make room in the vl_vector for the inserted vector
            std::copy_backward(this->begin() + insert_position,
                               this->end(), this->end() + insert_count);

            // Copy the elements of the inserted vector into the right spots
            std::copy(first, last, this->begin() + insert_position);

            this->curr_size += insert_count;
            return iterator(this->data_static + insert_position);
        }

        // Scenario 2: Transition from static to dynamic storage
        else if (this->data_dynamic == nullptr
        && this->curr_size + insert_count > static_capacity)
        {
            auto new_capacity = static_cast<size_t>
                                    (1.5 * (this->curr_size + insert_count));
            T* new_data_dynamic = new T[new_capacity]();

            // Copy elements before the insertion point
            std::copy(this->begin(), position, new_data_dynamic);

            // Insert new elements
            std::copy(first, last, new_data_dynamic + insert_position);

            // Copy elements after the insertion point
            std::copy(position, this->end(),
                      new_data_dynamic + insert_position + insert_count);

            this->data_dynamic = new_data_dynamic;
            this->dynamic_capacity = new_capacity;
            this->curr_size += insert_count;
            return iterator(data_dynamic + insert_position);
        }

        // Scenario 3: Sufficient dynamic storage
        else if (data_dynamic != nullptr &&
                 this->curr_size + insert_count <= dynamic_capacity)
        {
            // Make room in the vl_vector for the inserted vector
            std::copy_backward(begin() + insert_position, end(),
                                          end() + insert_count);

            // Copy the new elements to the room-spaced vector
            std::copy(first, last, begin() + insert_position);

            curr_size += insert_count;
            return iterator(data_dynamic + insert_position);
        }

        // Scenario 4: Insufficient dynamic storage, reallocation needed
        else
        {
            auto new_capacity = static_cast<size_t>
                                    (1.5 * (this->curr_size + insert_count));

            T *new_data_dynamic = new T[new_capacity]();

            // Copy elements before the insertion point
            std::copy(this->begin(), position, new_data_dynamic);

            // Insert new elements
            std::copy(first, last, new_data_dynamic + insert_position);

            // Copy elements after the insertion point
            std::copy(this->begin() + insert_position, end(),
                      new_data_dynamic + insert_position + insert_count);

            delete[] this->data_dynamic;
            this->data_dynamic = new_data_dynamic;
            this->dynamic_capacity = new_capacity;
            this->curr_size += insert_count;
            return iterator(this->data_dynamic + insert_position);
        }
    }

    /**
     * Removes the last element from the vl_vector.
     */
    void pop_back()
    {
        // Static remains static
        if (this->data_dynamic == nullptr)
        {
            this->curr_size--;
        }

        // Dynamic turns into static
        else if (this->data_dynamic != nullptr &&
                 this->curr_size - 1 == static_capacity)
        {
            this->curr_size--;
            std::copy(this->begin(), this->end(), this->data_static);
            delete[] this->data_dynamic;
            this->data_dynamic = nullptr;
        }

        // Dynamic that remains dynamic
        else
        {
            this->curr_size--;
        }
    }

    /**
     * Erases a single element from the vector at the specified position.
     * Adjusts size and manages storage transition if necessary.
     * @param position Iterator to the element to erase.
     * @return Iterator to the next element.
     */
    iterator erase(iterator position)
    {
        // Static remains static
        if (this->data_dynamic == nullptr)
        {
            std::move(position + 1, this->end(), position);
            this->curr_size--;
        }

        // Dynamic turns into static
        else if (this->data_dynamic != nullptr &&
                 this->curr_size - 1 == static_capacity)
        {
            std::move(position + 1, this->end(), position);
            this->curr_size--;
            std::copy(this->begin(), this->end(), this->data_static);
            delete[] this->data_dynamic;
            this->data_dynamic = nullptr;
        }

        // Dynamic that remains dynamic
        else
        {
            std::move(position + 1, this->end(), position);
            this->curr_size--;
        }

        return position;
    }


    /**
     * Erases a range of elements from the vector, specified by [first, last).
     * Adjusts size and manages storage transition if needed.
     * @param first Iterator to the first element in the range.
     * @param last Iterator to the element following the last in the range.
     * @return Iterator to the next element after the last erased.
     */
    iterator erase(iterator first, iterator last)
    {
        // Calculate the number of elements to remove
        size_t num_elements_to_remove = std::distance(first, last);

        // Static remains static
        if (this->data_dynamic == nullptr)
        {
            // Move the elements after 'last' to 'first'
            std::move(last, this->end(), first);

            this->curr_size -= num_elements_to_remove;
        }

        // Dynamic turns into static
        else if (this->data_dynamic != nullptr &&
                 this->curr_size - 1 == static_capacity)
        {
            std::move(last, this->end(), first);
            this->curr_size -= num_elements_to_remove;
            std::copy(this->begin(), this->end(), this->data_static);
            delete[] this->data_dynamic;
            this->data_dynamic = nullptr;
        }

        // Dynamic that remains dynamic
        else
        {
            // Move the elements after 'last' to 'first'
            std::move(last, this->end(), first);

            this->curr_size -= num_elements_to_remove;
        }

        return first;
    }

    /**
     * Clears all contents from the vector, resetting its size to zero.
     * If using dynamic memory, it deallocates the dynamic array and resets
     * the capacity to the initial static capacity.
     */
    void clear()
    {
        // Static
        if (this->data_dynamic == nullptr)
        {
            this->curr_size = 0;
        }

        // Dynamic
        else
        {
            delete[] this->data_dynamic;
            this->data_dynamic = nullptr;
            this->curr_size = 0;

            // Reset dynamic capacity to static capacity
            this->dynamic_capacity = static_capacity;

        }

    }

    /**
     * Provides access to an element at a given position.
     * @param idx Index of the element to access.
     * @return A reference to the element at the specified index.
     */
    T& operator[] (size_t idx)
    {
        if (this->data_dynamic == nullptr)
        {
            return this->data_static[idx];
        }
        return *(this->data_dynamic + idx);
    }

    /**
     * Provides const access to an element at a given position.
     * @param idx Index of the element to access.
     * @return A const reference to the element at the specified index.
     */
    T operator[] (size_t idx) const
    {
        if (this->data_dynamic == nullptr)
        {
            return this->data_static[idx];
        }
        return *(this->data_dynamic + idx);
    }

    /**
     * Provides direct access to the underlying array.
     * @return A pointer to the beginning of the array.
     */
    T* data()
    {
        return data_dynamic ? data_dynamic : data_static;
    }

    /**
     * Provides direct const access to the underlying array.
     * @return A const pointer to the beginning of the array.
     */
    const T* data() const
    {
        return data_dynamic ? data_dynamic : data_static;
    }

    /**
     * Swaps the contents of two vectors.
     * @param first The first vector to swap.
     * @param second The second vector to swap with the first.
     */
    void swap(vl_vector<T, static_capacity>& first,
              vl_vector<T, static_capacity>& second)
    {
        using std::swap;
        swap(first.data_static, second.data_static);
        swap(first.data_dynamic, second.data_dynamic);
        swap(first.curr_size, second.curr_size);
        swap(first.dynamic_capacity, second.dynamic_capacity);
    }

    /**
     * Copy assignment operator using the copy-and-swap idiom.
     * @param other Another vector to assign to this one.
     * @return Reference to this instance after assignment.
     */
    vl_vector<T, static_capacity>&
            operator=(vl_vector<T, static_capacity> other)
    {
        swap(*this, other);
        return *this;
    }

    /**
     * Checks if the current vector is equal to another vector.
     * Equality is defined as having the same elements in the same order.
     * @param rhs The right-hand side vector to compare.
     * @return True if the vectors are equal, false otherwise.
     */
    bool operator==(const vl_vector<T, static_capacity>& rhs) const
    {
        if (this->curr_size != rhs.curr_size)
        {
            return false;
        }

        // Compare static data_static if no dynamic allocation is present
        if (this->data_dynamic == nullptr && rhs.data_dynamic == nullptr)
        {
            for (size_t i = 0; i < this->curr_size; i++)
            {
                if (this->data_static[i] != rhs.data_static[i])
                {
                    return false;
                }
            }
        }
        // If dynamic data_static exists, compare dynamic data_static
        else if (this->data_dynamic && rhs.data_dynamic)
        {
            for (size_t i = 0; i < this->curr_size; i++)
            {
                if (this->data_dynamic[i] != rhs.data_dynamic[i])
                {
                    return false;
                }
            }
        }
        // If one is dynamic and the other isn't, they are not equal
        else if ((this->data_dynamic && rhs.data_dynamic == nullptr) ||
        (this->data_dynamic == nullptr && rhs.data_dynamic))
        {
            return false;
        }

        return true; // Both are equal
    }


    /**
     * Checks if the current vector is not equal to another vector.
     * @param rhs The right-hand side vector to compare.
     * @return True if the vectors are not equal, false if they are equal.
     */
    bool operator!=(const vl_vector<T, static_capacity>& rhs) const
    {
        return !operator==(rhs);
    }

    void print_vec() const {
        for (auto value : *this) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }

    // Iterator methods
    iterator begin()
    {
        return iterator(data_dynamic ? data_dynamic : data_static, 0);
    }

    iterator end()
    {
        return iterator(data_dynamic ? data_dynamic :
                                            data_static, curr_size);
    }

    const_iterator begin() const
    {
        return const_iterator(data_dynamic ? data_dynamic : data_static, 0);
    }

    const_iterator end() const
    {
        return const_iterator(data_dynamic ? data_dynamic :
                                                data_static, curr_size);
    }

    const_iterator cbegin() const
    {
        return const_iterator(data_dynamic ? data_dynamic :
                                                    data_static, 0);
    }

    const_iterator cend() const
    {
        return const_iterator(data_dynamic ? data_dynamic :
                                                data_static, curr_size);
    }

    reverse_iterator rbegin()
    {
        return std::reverse_iterator<iterator>(end());
    }

    reverse_iterator rend()
    {
        return std::reverse_iterator<iterator>(begin());
    }

    const_reverse_iterator crbegin() const
    {
        return std::reverse_iterator<const_iterator>(cend());
    }

    const_reverse_iterator crend() const
    {
        return std::reverse_iterator<const_iterator>(cbegin());
    }

    const_reverse_iterator rbegin() const
    {
        return std::reverse_iterator<const_iterator>(end());
    }

    const_reverse_iterator rend() const
    {
        return std::reverse_iterator<const_iterator>(begin());
    }

};


#endif //FINAL_PROJECT_C___VL_VECTOR_HPP

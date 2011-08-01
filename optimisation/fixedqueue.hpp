#ifndef fixed_size_queue_hpp
#define fixed_size_queue_hpp

#include <list>
#include <iterator>
template<typename T>
class fixed_queue {
    size_t n_, current_size_;
    std::list<T> data_;
    typedef std::list<T> container_type;
public:

    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef typename container_type::reverse_iterator reverse_iterator;
    typedef typename container_type::const_reverse_iterator const_reverse_iterator;

    fixed_queue(size_t n):current_size_(0), n_(n) {}

  size_type size() const{
    return current_size_;
  }

  size_type max_size() const{
    return n_;
  }

    void push_back(const_reference d) {
        if(current_size_<n_) {
            ++current_size_;
        } else {
            data_.pop_front();
        }
	data_.push_back(d);

    }

    reference operator[](size_t i) {
        iterator it = begin();
        std::advance(it, i);
        return *it;
    }
    const_reference operator[](size_t i) const {
        iterator it = begin();
        std::advance(it, i);
        return *it;
    }

    reference at(size_t i) {
        iterator it = begin();
        std::advance(it, i);
        return *it;
    }

    const_reference at(size_t i)const {
        iterator it = begin();
        std::advance(it, i);
        return *it;
    }

    reference reverse_at(size_t i) {
        reverse_iterator it = rbegin();
        std::advance(it, i);
        return *it;
    }

    const_reference reverse_at(size_t i)const {
        reverse_iterator it = rbegin();
        std::advance(it, i);
        return *it;
    }

    void pop_front() {
        data_.pop_front();
        --current_size_;
    }

    iterator begin() {
        return data_.begin();
    }
    iterator end() {
        return data_.end();
    }

    const_iterator begin() const {
        return data_.begin();
    }
    const_iterator end() const {
        return data_.end();
    }

    reverse_iterator rbegin() {
        return data_.rbegin();
    }
    reverse_iterator rend() {
        return data_.rend();
    }

    const_reverse_iterator rbegin() const {
        return data_.rbegin();
    }
    const_reverse_iterator rend() const {
        return data_.rend();
    }


};

#endif

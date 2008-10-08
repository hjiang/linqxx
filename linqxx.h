#ifndef LINQXX_H_
#define LINQXX_H_

#include <vector>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace linqxx {

using std::vector;
using boost::shared_ptr;
using boost::function;

// T is usually an STL container.
template<typename T>
class DataSet {
  public:
    DataSet(shared_ptr<T>);
    unsigned int count();
    DataSet<T> where(function<bool (typename T::value_type)> pred);

    // TODO: There is probably some way to get rid of the hard-coded
    // vector and instead use the same container type as T.
    template<typename S>
    DataSet<vector<S> > select(function<S (typename T::value_type)> sel);

    // Map is an alias of select.
    template<typename S>
    DataSet<vector<S> > map(function<S (typename T::value_type)> sel) {
        select<S>(sel);
    }

  private:
    shared_ptr<T> container_;
};

template<typename T>
DataSet<T>::DataSet(shared_ptr<T> container) : container_(container) {}

template<typename T>
unsigned int DataSet<T>::count() {
    return container_->size();
}

template<typename T>
DataSet<T> DataSet<T>::where(function<bool (typename T::value_type)> pred) {
    shared_ptr<T> new_container(new T());
    for (typename T::const_iterator i = container_->begin();
         i != container_->end(); ++i) {
        if (pred(*i)) {
            new_container->push_back(*i);
        }
    }
    return DataSet<T>(new_container);
}

template<typename T>
template<typename S>
DataSet<vector<S> > DataSet<T>::select(function<S (typename T::value_type)> sel) {
    shared_ptr<vector<S> > new_container(new vector<S>());
    for (typename T::const_iterator i = container_->begin();
         i != container_->end(); ++i) {
        new_container->push_back(sel(*i));
    }
    return DataSet<vector<S> >(new_container);
}

template<typename T>
DataSet<T> from(shared_ptr<T> source) {
    return DataSet<T>(source);
}

}  // namespace linqxx

#define LX_ATTR(A) &_1 ->* &(A)

#endif  // LINQXX_H_

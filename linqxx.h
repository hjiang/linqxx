#ifndef LINQXX_H_
#define LINQXX_H_

#include <vector>

#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace linqxx {

using std::vector;
using boost::shared_ptr;
using boost::function;

enum MutationMode {
    IMMUTABLE,  // leave the old container alone and create new ones
    IN_PLACE    // modify the container in place
};

// T is usually an STL container.
template<typename T, typename TPtr = shared_ptr<T> >
class DataSet {
  public:
    typedef T underlying_container_type;
    DataSet(TPtr);

    TPtr get();

    unsigned int count();
    DataSet<T> where(function<bool (typename T::value_type)> pred);

    // TODO: There is probably some way to get rid of the hard-coded
    // vector and instead use the same container type as T.
    template<typename S>
    DataSet<vector<S> > select(function<S (typename T::value_type)> sel);

    // Map is an alias of select.
    template<typename S>
    DataSet<vector<S> > map(function<S (typename T::value_type)> sel) {
        return select<S>(sel);
    }

  private:
    TPtr container_;
};

template<typename T, typename TPtr>
DataSet<T, TPtr>::DataSet(TPtr container) : container_(container) {}

template<typename T, typename TPtr>
TPtr DataSet<T, TPtr>::get() {
    return container_;
}

template<typename T, typename TPtr>
unsigned int DataSet<T, TPtr>::count() {
    return container_->size();
}

template<typename T, typename TPtr>
DataSet<T> DataSet<T, TPtr>::where(function<bool (typename T::value_type)> pred) {
    shared_ptr<T> new_container(new T());
    for (typename T::const_iterator i = container_->begin();
         i != container_->end(); ++i) {
        if (pred(*i)) {
            new_container->push_back(*i);
        }
    }
    return DataSet<T>(new_container);
}

template<typename T, typename TPtr>
template<typename S>
DataSet<vector<S> > DataSet<T, TPtr>::select(function<S (typename T::value_type)> sel) {
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

template<typename T>
DataSet<T, T*> from(T* source) {
    return DataSet<T, T*>(source);
}

template<typename T>
class Inserter {
  public:
    Inserter(const T& data_set) : data_set_(data_set) {}

    template<typename S>
    S into(S target, MutationMode mode = IMMUTABLE);
  private:
    T data_set_;
};

template<typename T>
template<typename S>
S Inserter<T>::into(S target, MutationMode mode) {
    shared_ptr<typename S::underlying_container_type> target_container;
    shared_ptr<typename T::underlying_container_type> source_container = data_set_.get();
    if (mode == IMMUTABLE) {
        target_container.reset(new typename S::underlying_container_type);
        *target_container = *(target.get());
    } else {
        target_container = target.get();
    }
    typename T::underlying_container_type::const_iterator it;
    for (it = source_container->begin(); it != source_container->end(); ++it) {
        target_container->push_back(*it);
    }
    return from(target_container);
}

//template<typename T>
//Inserter<T> insert(T data_set);

template<typename C>
Inserter<DataSet<C> > insert(DataSet<C> data_set) {
    return Inserter<DataSet<C> >(data_set);
}

}  // namespace linqxx

#define LX_ATTR(A) &_1 ->* &(A)

#endif  // LINQXX_H_

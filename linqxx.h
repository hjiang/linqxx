#ifndef LINQXX_H_
#define LINQXX_H_

namespace linqxx {

// T is usually an STL container.
template<typename T>
class DataSet {
  public:
    DataSet(const T&);
    unsigned int count();
  private:
    T container_;
};

template<typename T>
DataSet<T>::DataSet(const T& container) : container_(container) {}

template<typename T>
unsigned int DataSet<T>::count() {
    return container_.size();
}

template<typename T>
DataSet<T> from(const T& source) {
    return DataSet<T>(source);
}

}  // namespace linqxx

#endif  // LINQXX_H_

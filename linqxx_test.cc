#include <iostream>
#include <string>
#include <vector>

#include "linqxx.h"

using namespace std;
using namespace linqxx;

struct Person {
    string name;
    int age;
    Person(const string& aName, int anAge) : name(aName), age(anAge) {}
};

int main() {
    vector<Person> guests;
    guests.push_back(Person("John", 32));
    guests.push_back(Person("Mike", 28));
    guests.push_back(Person("Eliz", 27));
    cout << from(guests).count() << endl;
}

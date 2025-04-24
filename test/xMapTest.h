#ifndef XMAPTEST_CPP
#define XMAPTEST_CPP
#include "../include/hash/xMap.h"
#include <iostream>
#include <string>

using namespace std;

string stringToString(string &value) {
    return value;
}

// Custom Person class to test with custom objects
class Person {
private:
    string name;
    int age;

public:
    Person(string name = "", int age = 0) : name(name), age(age) {}

    bool operator==(const Person &other) const {
        return name == other.name && age == other.age;
    }

    string getName() const { return name; }
    int getAge() const { return age; }
    friend ostream &operator<<(ostream &os, const Person &p) {
        os << p.name << " (" << p.age << ")";
        return os;
    }
};

// Hash function for Person class
int personHash(Person &p, int capacity) {
    string name = p.getName();
    int age = p.getAge();
    long long hash = 0;
    for (char c : name) {
        hash = hash * 31 + c;
    }
    hash = (hash + age) % capacity;
    return hash;
}

// Person to string conversion
string personToString(Person &p) {
    return p.getName() + "(" + to_string(p.getAge()) + ")";
}

// Test basic operations with integer keys and values
void testIntegerMap() {
    cout << "\n===== Testing Integer Map =====\n";

    // Create map
    xMap<int, int> map(xMap<int, int>::intKeyHash);

    // Test put() and printTable()
    map.put(132, 664);
    map.put(312, 305);
    map.put(232, 743);
    map.put(322, 101);
    map.put(452, 651);
    cout << map.toString(intToString, intToString) << endl;
    map.put(542, 836);
    cout << map.toString(intToString, intToString) << endl;
    map.put(672, 129);
    cout << map.toString(intToString, intToString) << endl;
    map.put(762, 432);
    cout << map.toString(intToString, intToString) << endl;
    map.put(892, 923);
    map.put(982, 489);
    cout << map.toString(intToString, intToString) << endl;
}

// Test with string keys and int values
void testStringIntMap() {
    cout << "\n===== Testing String-Int Map =====\n";

    // Create map
    xMap<string, int> map(xMap<string, int>::stringKeyHash);

    // Test put() and get()
    cout << "Adding elements...\n";
    map.put("apple", 5);
    map.put("banana", 7);
    map.put("cherry", 10);
    map.put("date", 3);
    map.put("elderberry", 8);

    cout << "Size: " << map.size() << endl;

    cout << "\nRetrieving elements...\n";
    cout << "apple: " << map.get("apple") << endl;
    cout << "banana: " << map.get("banana") << endl;
    cout << "cherry: " << map.get("cherry") << endl;

    // Test updating values
    cout << "\nUpdating values...\n";
    map.put("apple", 15);
    cout << "Updated apple: " << map.get("apple") << endl;

    // Test toString()
    cout << "\nMap contents:\n"
         << map.toString(stringToString, intToString);

    // Test remove with specific value
    cout << "\nRemoving banana=7...\n";
    bool removed = map.remove("banana", 7);
    cout << "Removed? " << (removed ? "Yes" : "No") << endl;
    cout << "Size: " << map.size() << endl;
}

// Test with custom objects
void testCustomObjects() {
    cout << "\n===== Testing Custom Object Map =====\n";

    // Create map with Person objects
    xMap<Person, string> map(personHash);

    // Test put() and get()
    cout << "Adding elements...\n";
    map.put(Person("Alice", 25), "Developer");
    map.put(Person("Bob", 30), "Manager");
    map.put(Person("Charlie", 22), "Designer");

    cout << "Size: " << map.size() << endl;

    // Test lookup
    cout << "\nLooking up Alice...\n";
    Person alice("Alice", 25);
    if (map.containsKey(alice)) {
        cout << "Alice is a " << map.get(alice) << endl;
    }

    // Test toString with custom converter
    cout << "\nMap contents:\n"
         << map.toString(personToString, stringToString);
}

// Test copy constructor and assignment operator
void testCopyOperations() {
    cout << "\n===== Testing Copy Operations =====\n";

    // Create original map
    xMap<int, string> original(xMap<int, int>::intKeyHash);
    original.put(1, "One");
    original.put(2, "Two");
    original.put(3, "Three");

    cout << "Original map:\n"
         << original.toString(intToString, stringToString);

    // Test copy constructor
    cout << "\nCopy constructor test...\n";
    xMap<int, string> copy1(original);
    cout << "Copy map:\n"
         << copy1.toString(intToString, stringToString);

    // Test assignment operator
    cout << "\nAssignment operator test...\n";
    xMap<int, string> copy2(xMap<int, int>::intKeyHash);
    copy2.put(100, "Hundred");
    cout << "Before assignment:\n"
         << copy2.toString(intToString, stringToString);

    copy2 = original;
    cout << "After assignment:\n"
         << copy2.toString(intToString, stringToString);

    // Modify copy and check original
    cout << "\nModifying copy...\n";
    copy2.put(4, "Four");
    cout << "Modified copy:\n"
         << copy2.toString(intToString, stringToString);
    cout << "Original map:\n"
         << original.toString(intToString, stringToString);
}

// Test edge cases
void testEdgeCases() {
    cout << "\n===== Testing Edge Cases =====\n";

    // Empty map operations
    xMap<int, int> map(xMap<int, int>::intKeyHash);

    cout << "Empty map size: " << map.size() << endl;
    cout << "Empty map toString:\n"
         << map.toString(intToString, intToString);

    // Test exception handling
    cout << "\nTrying to get non-existent key...\n";
    try {
        int value = map.get(5);
        cout << "Value: " << value << endl; // Should not reach here
    } catch (const exception &e) {
        cout << "Exception caught: " << e.what() << endl;
    }

    // Test with numerous elements (rehashing)
    cout << "\nTesting with many elements (rehashing)...\n";
    for (int i = 0; i < 100; i++) {
        map.put(i, i * i);
    }
    cout << "Map size after adding 100 elements: " << map.size() << endl;
    cout << "Map capacity: " << map.getCapacity() << endl;

    // Test clashes with designed key collisions
    cout << "\nTesting hash collisions...\n";
    xMap<int, string> collisionMap([](int &key, int capacity) -> int { return key % 5; }, 0.75f);

    for (int i = 0; i < 20; i++) {
        collisionMap.put(i, "Value" + to_string(i));
    }

    cout << "Collision map contents:\n"
         << collisionMap.toString(intToString, stringToString);
    cout << "Clashes in collision map:\n";
    DLinkedList<int> clashes = collisionMap.clashes();
    for (int i = 0; i < clashes.size(); i++) {
        cout << "Bucket " << i << ": " << clashes.get(i) << " entries\n";
    }
}

// Main function
// int main() {
//     cout << "===== xMap Test Program =====\n";

//     testIntegerMap();
//     testStringIntMap();
//     testCustomObjects();
//     testCopyOperations();
//     testEdgeCases();

//     cout << "\nAll tests completed.\n";
//     return 0;
// }
#endif // XMAPTEST_CPP
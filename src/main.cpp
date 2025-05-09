/*
Author: NDTH - 5/9/2025
All in one
*/


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "heap/Heap.h"
#include "hash/xMap.h"
#include "app/inventory_compressor.h"

using namespace std;

string output = "";
string expect = "";
streambuf* original_cout = std::cout.rdbuf();

class Object {
    int key;
    string val;
public:
    static int count;
    Object() : key(-1), val("") { ++count; }
    Object(int key, string val) : key(key), val(val) { ++count; }
    Object(string val) : key(-1), val(val) { ++count; }
    Object(const Object& obj) : key(obj.key), val(obj.val) { ++count; }
    ~Object() { --count; }
    static string ObjectToString(Object obj) { return obj.val; }
    static string ObjectToString(Object *obj) { return obj->val; }
    static bool ObjectEqual(Object& obj1, Object& obj2) { return obj1.val == obj2.val; }
    static bool ObjectEqual(Object *&obj1, Object*& obj2) { return obj1->val == obj2->val; }
    friend ostream& operator<< (ostream& ss, Object& obj);
    friend ostream& operator<< (ostream& ss, Object* obj);
    friend int ObjectHash(Object& obj, int capacity);
    friend int ObjectHash(Object*& obj, int capacity);
    bool operator== (Object& obj) { return ObjectEqual(*this, obj); }
    Object& operator= (const Object& obj) { key = obj.key, val = obj.val; return *this; }
};

int Object::count = 0;

ostream& operator<< (ostream& ss, Object& obj) {
    ss << Object::ObjectToString(obj);
    return ss;
}

ostream& operator<< (ostream& ss, Object* obj) {
    ss << Object::ObjectToString(obj);
    return ss;
}

int ObjectHash(Object& obj, int capacity) {
    if (obj.key == -1) return xMap<int, int>::stringKeyHash(obj.val, capacity);
    else return xMap<int, int>::simpleHash(obj.key, capacity);
}

int ObjectHash(Object*& obj, int capacity) {
    if (obj->key == -1) return xMap<int, int>::stringKeyHash(obj->val, capacity);
    else return xMap<int, int>::simpleHash(obj->key, capacity);
}

void hash001() {
    expect = "==================================================\ncapacity:   10\nsize:       0\n0   : \n1   : \n2   : \n3   : \n4   : \n5   : \n6   : \n7   : \n8   : \n9   : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.println();
}

void hash002() {
    expect = "1";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    try {
        cout << map.get(1);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash003() {
    expect = "11";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 11);
    try {
        cout << map.get(1);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash004() {
    expect = "111";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(11, 111);
    try {
        cout << map.get(11);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash005() {
    expect = "111";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(11, 111);
    try {
        cout << map.get(11);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash006() {
    // add it to the back
    expect = "==================================================\ncapacity:   10\nsize:       3\n0   : \n1   :  (1,1); (11,1); (21,1)\n2   : \n3   : \n4   : \n5   : \n6   : \n7   : \n8   : \n9   : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(11, 1);
    map.put(21, 1);
    map.println();
}

void hash007() {
    expect = "key (11) is not found\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 11);
    try {
        cout << map.get(11);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash008() {
    expect = "12";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 11);
    map.put(1, 12);
    try {
        cout << map.remove(1);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash009() {
    expect = "key (12) is not found\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    try {
        cout << map.remove(12);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash010() {
    expect = "2";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    try {
        map.remove(1);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
    cout << map.size();
}

void hash011() {
    expect = "1";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    cout << map.containsKey(2);
}

void hash012() {
    expect = "0";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    cout << map.containsKey(12);
}

void hash013() {
    expect = "1";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    cout << map.containsValue(12);
}

void hash014() {
    expect = "0";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    cout << map.containsValue(0);
}

void hash015() {
    expect = "0";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    try {
        map.remove(1);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
    cout << map.empty();
}

void hash016() {
    expect = "1";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    try {
        map.remove(1);
        map.remove(2);
        map.remove(3);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
    cout << map.empty();
}

void hash017() {
    // capacity is 10 after clearing
    expect = "==================================================\ncapacity:   10\nsize:       0\n0   : \n1   : \n2   : \n3   : \n4   : \n5   : \n6   : \n7   : \n8   : \n9   : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(0, 1);
    map.put(30, 1);
    map.put(60, 1);
    map.put(90, 1);
    map.put(120, 1);
    map.put(150, 1);
    map.put(180, 1);
    map.put(210, 1);
    map.put(240, 1);
    map.clear();
    map.println();
}

void hash018() {
    expect = "==================================================\ncapacity:   10\nsize:       3\n0   : \n1   :  (1,1); (11,1); (21,1)\n2   : \n3   : \n4   : \n5   : \n6   : \n7   : \n8   : \n9   : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    map.clear();
    map.put(1, 1);
    map.put(11, 1);
    map.put(21, 1);
    map.println();
}

void hash019() {
    expect = "[1, 2, 3]\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    auto keys = map.keys();
    keys.println();
}

void hash020() {
    expect = "[12, 12, 120]\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(3, 120);
    auto values = map.values();
    values.println();
}

void hash021() {
    expect = "[0, 1, 1, 3, 0, 0, 0, 0, 0, 0]\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(1, 12);
    map.put(2, 1);
    map.put(2, 12);
    map.put(3, 10);
    map.put(13, 10);
    map.put(23, 10);
    map.put(3, 120);
    auto clashes = map.clashes();
    clashes.println();
}

void hash022() {
    // rehash's work
    expect = "==================================================\ncapacity:   15\nsize:       10\n0   :  (0,1); (30,1); (60,1); (90,1); (120,1); (150,1); (180,1); (210,1); (240,1)\n1   : \n2   : \n3   :  (3,1)\n4   : \n5   : \n6   : \n7   : \n8   : \n9   : \n10  : \n11  : \n12  : \n13  : \n14  : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(0, 1);
    map.put(30, 1);
    map.put(60, 1);
    map.put(90, 1);
    map.put(120, 1);
    map.put(150, 1);
    map.put(180, 1);
    map.put(210, 1);
    map.put(240, 1);
    map.put(3, 1);
    map.println();
}

void hash023() {
    // ensure that the load factor is assigned
    expect = "==================================================\ncapacity:   15\nsize:       6\n0   :  (0,1); (30,1); (60,1); (90,1); (120,1)\n1   : \n2   : \n3   :  (3,1)\n4   : \n5   : \n6   : \n7   : \n8   : \n9   : \n10  : \n11  : \n12  : \n13  : \n14  : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash, 0.55f);
    map.put(0, 1);
    map.put(30, 1);
    map.put(60, 1);
    map.put(90, 1);
    map.put(120, 1);
    map.put(3, 1);
    map.println();
}

void hash024() {
    expect = "==================================================\ncapacity:   10\nsize:       3\n0   : \n1   :  (1,1); (11,1)\n2   : \n3   : \n4   : \n5   :  (5,1)\n6   : \n7   : \n8   : \n9   : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(11, 1);
    map.put(5, 1);
    auto map2 = map;
    map2.println();
}

void hash025() {
    expect = "==================================================\ncapacity:   10\nsize:       3\n0   : \n1   :  (1,1); (11,1)\n2   : \n3   : \n4   : \n5   :  (5,1)\n6   : \n7   : \n8   : \n9   : \n==================================================\n\n";
    xMap<int, int> map(xMap<int, int>::simpleHash);
    map.put(1, 1);
    map.put(11, 1);
    map.put(5, 1);
    xMap<int, int> map2(xMap<int, int>::simpleHash);
    map2 = map;
    map2.println();
}

void hash026() {
    expect = "345";
    xMap<Object*, int> map(ObjectHash, 0.75f, 0, 0, Object::ObjectEqual, xMap<Object*, int>::freeKey);
    map.put(new Object("ABC"), 123);
    map.put(new Object("abc"), 345);
    try {
        Object get("abc");
        cout << map.get(&get);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash027() {
    expect = "3";
    xMap<Object*, int> map(ObjectHash, 0.75f, 0, 0, Object::ObjectEqual, xMap<Object*, int>::freeKey);
    map.put(new Object("ABC"), 123);
    map.put(new Object("abc"), 345);
    Object same("abc");
    map.put(&same, 654);
    map.put(new Object("654"), 786);
    cout << map.size();
}

void hash028() {
    expect = "345\n345";
    xMap<Object*, int> map(ObjectHash, 0.75f, 0, 0, Object::ObjectEqual, xMap<Object*, int>::freeKey);
    map.put(new Object("ABC"), 123);
    cout << map.put(new Object("abc"), 345) << endl;
    Object same("abc");
    cout << map.put(&same, 654);
}

void hash029() {
    expect = "1";
    xMap<Object*, int> map(ObjectHash, 0.75f, 0, 0, Object::ObjectEqual, xMap<Object*, int>::freeKey);
    map.put(new Object("ABC"), 123);
    map.put(new Object("abc"), 345);
    try {
        Object remove("abc");
        map.remove(&remove);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
    cout << map.size();
}

void hash030() {
    expect = "1";
    xMap<Object*, int> map(ObjectHash, 0.75f, 0, 0, Object::ObjectEqual, xMap<Object*, int>::freeKey);
    map.put(new Object("ABC"), 123);
    map.put(new Object("abc"), 345);
    try {
        Object key("abc");
        cout << map.containsKey(&key);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash031() {
    expect = "1";
    xMap<int, Object*> map(xMap<int, Object*>::simpleHash, 0.75f, Object::ObjectEqual, xMap<int, Object*>::freeValue, 0, 0);
    map.put(123, new Object("ABC"));
    map.put(345, new Object("abc"));
    try {
        Object value("abc");
        cout << map.containsValue(&value);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash032() {
    // Wow Dlinkedlist<>(0, keyEqual)
    expect = "1";
    xMap<Object*, int> map(ObjectHash, 0.75f, 0, 0, Object::ObjectEqual, 0);
    Object obj[] = {
        Object(0, "n,"),
        Object(1, "i"),
        Object(2, "miss"),
        Object(3, "you"),
        Object(4, "..."),
        Object(5, "..."),
    };
    map.put(&obj[0], 1);
    map.put(&obj[1], 1);
    map.put(&obj[2], 1);
    map.put(&obj[3], 1);
    map.put(&obj[4], 1);
    map.put(&obj[5], 1);
    try {
        Object key("n,");
        auto keys = map.keys();
        cout << keys.contains(&key);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash033() {
    // Dlinkedlist<>(0, valueEqual)
    expect = "1";
    xMap<int, Object*> map(xMap<int, Object*>::simpleHash, 0.75f, Object::ObjectEqual, 0, 0, 0);
    Object obj[] = {
        Object(0, "n,"),
        Object(1, "give"),
        Object(2, "me"),
        Object(3, "signal"), // he means that n can send a signal to his fb :v
        Object(4, "..."),
        Object(5, "..."),
    };
    map.put(1, &obj[0]);
    map.put(2, &obj[1]);
    map.put(3, &obj[2]);
    map.put(4, &obj[3]);
    map.put(5, &obj[4]);
    map.put(6, &obj[5]);
    try {
        Object value("n,");
        auto values = map.values();
        cout << values.contains(&value);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash034() {
    // valueEqual = map.valueEqual
    // keyEqual = map.keyEqual
    expect = "11";
    xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
    map.put(new Object("a"), new Object("f"));
    map.put(new Object("b"), new Object("e"));
    map.put(new Object("c"), new Object("d"));
    map.put(new Object("d"), new Object("c"));
    map.put(new Object("e"), new Object("b"));
    auto map1 = map;
    map1.put(new Object("f"), new Object("a"));
    try {
        Object contains("f");
        cout << map1.containsKey(&contains);
        cout << map1.containsValue(&contains);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash035() {
    // valueEqual = map.valueEqual
    // keyEqual = map.keyEqual
    expect = "11";
    xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
    map.put(new Object("a"), new Object("f"));
    map.put(new Object("b"), new Object("e"));
    map.put(new Object("c"), new Object("d"));
    map.put(new Object("d"), new Object("c"));
    map.put(new Object("e"), new Object("b"));
    xMap<Object*, Object*> map2(ObjectHash);
    map2 = map;
    map2.put(new Object("f"), new Object("a"));
    try {
        Object contains("f");
        cout << map2.containsKey(&contains);
        cout << map2.containsValue(&contains);
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash036() {
    expect = "[0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0]";
    xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
    map.put(new Object("a"), new Object("f"));
    map.put(new Object("b"), new Object("e"));
    map.put(new Object("c"), new Object("d"));
    map.put(new Object("d"), new Object("c"));
    map.put(new Object("e"), new Object("b"));
    auto map1 = map;
    map.put(new Object("g"), new Object("a"));
    map1.put(new Object("f"), new Object("a"));
    cout << map1.clashes().toString();
}

void hash037() {
    expect = "[0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0]";
    xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
    map.put(new Object("a"), new Object("f"));
    map.put(new Object("b"), new Object("e"));
    map.put(new Object("c"), new Object("d"));
    map.put(new Object("d"), new Object("c"));
    map.put(new Object("e"), new Object("b"));
    xMap<Object*, Object*> map2(ObjectHash);
    map2 = map;
    map.put(new Object("g"), new Object("a"));
    map2.put(new Object("f"), new Object("a"));
    cout << map2.clashes().toString();
}

void hash038() {
    expect = "0";
    {
        xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, 0, Object::ObjectEqual, 0);
        map.put(new Object("a"), new Object("f"));
        map.put(new Object("b"), new Object("e"));
        map.put(new Object("c"), new Object("d"));
        map.put(new Object("d"), new Object("c"));
        map.put(new Object("e"), new Object("b"));
        xMap<Object*, Object*> map2(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
        map2.put(new Object("a"), new Object("f"));
        map2.put(new Object("b"), new Object("e"));
        map2.put(new Object("c"), new Object("d"));
        map2.put(new Object("d"), new Object("c"));
        map2.put(new Object("e"), new Object("b"));
        map2 = map;
    }
    cout << Object::count;
}

void hash039() {
    expect = "0";
    {
        xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
        map.put(new Object("a"), new Object("f"));
        map.put(new Object("b"), new Object("e"));
        map.put(new Object("c"), new Object("d"));
        map.put(new Object("d"), new Object("c"));
        map.put(new Object("e"), new Object("b"));
        Object remove("c");
        try {
            delete map.remove(&remove, [](Object* obj) { delete obj; });
        }
        catch (KeyNotFound& e) {
            cout << e.what() << endl;
        }
    }
    cout << Object::count;
}

void hash040() {
    // remember to delete entry
    expect = "0";
    {
        xMap<Object, Object> map(ObjectHash, 0.55f, Object::ObjectEqual, 0, Object::ObjectEqual, 0);
        map.put(Object("a"), Object("f"));
        map.put(Object("b"), Object("e"));
        map.put(Object("c"), Object("d"));
        map.put(Object("d"), Object("c"));
        map.put(Object("e"), Object("b"));
        Object remove("c");
        try {
            map.remove(remove);
        }
        catch (KeyNotFound& e) {
            cout << e.what() << endl;
        }
    }
    cout << Object::count;
}

void hash041() {
    expect = "1\n4";
    xMap<Object, Object> map(ObjectHash, 0.55f, Object::ObjectEqual, 0, Object::ObjectEqual, 0);
    map.put(Object("a"), Object("f"));
    map.put(Object("b"), Object("e"));
    map.put(Object("c"), Object("d"));
    map.put(Object("d"), Object("c"));
    map.put(Object("e"), Object("b"));
    Object removeKey("c");
    Object removeValue("d");
    try {
        cout << map.remove(removeKey, removeValue) << endl;
        cout << map.size();
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash042() {
    expect = "0\n5";
    xMap<Object, Object> map(ObjectHash, 0.55f, Object::ObjectEqual, 0, Object::ObjectEqual, 0);
    map.put(Object("a"), Object("f"));
    map.put(Object("b"), Object("e"));
    map.put(Object("c"), Object("d"));
    map.put(Object("d"), Object("c"));
    map.put(Object("e"), Object("b"));
    Object removeKey("c");
    Object removeValue("c");
    try {
        cout << map.remove(removeKey, removeValue) << endl;
        cout << map.size();
    }
    catch (KeyNotFound& e) {
        cout << e.what() << endl;
    }
}

void hash043() {
    // delete entry
    expect = "0";
    {
        xMap<Object, Object> map(ObjectHash, 0.55f, Object::ObjectEqual, 0, Object::ObjectEqual, 0);
        map.put(Object("a"), Object("f"));
        map.put(Object("b"), Object("e"));
        map.put(Object("c"), Object("d"));
        map.put(Object("d"), Object("c"));
        map.put(Object("e"), Object("b"));
        Object removeKey("c");
        Object removeValue("d");
        try {
            map.remove(removeKey, removeValue);
        }
        catch (KeyNotFound& e) {
            cout << e.what() << endl;
        }
    }
    cout << Object::count;
}

void hash044() {
    // delete entry
    expect = "0";
    {
        xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
        map.put(new Object("a"), new Object("f"));
        map.put(new Object("b"), new Object("e"));
        map.put(new Object("c"), new Object("d"));
        map.put(new Object("d"), new Object("c"));
        map.put(new Object("e"), new Object("b"));
        Object removeKey("c");
        Object removeValue("d");
        try {
            map.remove(&removeKey, &removeValue, [](Object* obj) { delete obj; }, [](Object* obj) { delete obj; });
        }
        catch (KeyNotFound& e) {
            cout << e.what() << endl;
        }
    }
    cout << Object::count;
}

void hash045() {
    // delete entry
    expect = "0";
    {
        xMap<Object*, Object*> map(ObjectHash, 0.55f, Object::ObjectEqual, xMap<Object*, Object*>::freeValue, Object::ObjectEqual, xMap<Object*, Object*>::freeKey);
        map.put(new Object("a"), new Object("f"));
        map.put(new Object("b"), new Object("e"));
        map.put(new Object("c"), new Object("d"));
        map.put(new Object("d"), new Object("c"));
        map.put(new Object("e"), new Object("b"));
        map.clear();
        map.put(new Object("a"), new Object("f"));
        map.put(new Object("b"), new Object("e"));
    }
    cout << Object::count;
}


void (*func_ptr[])() = {
    hash001,
    hash002,
    hash003,
    hash004,
    hash005,
    hash006,
    hash007,
    hash008,
    hash009,
    hash010,
    hash011,
    hash012,
    hash013,
    hash014,
    hash015,
    hash016,
    hash017,
    hash018,
    hash019,
    hash020,
    hash021,
    hash022,
    hash023,
    hash024,
    hash025,
    hash026,
    hash027,
    hash028,
    hash029,
    hash030,
    hash031,
    hash032,
    hash033,
    hash034,
    hash035,
    hash036,
    hash037,
    hash038,
    hash039,
    hash040,
    hash041,
    hash042,
    hash043,
    hash044,
    hash045,
};

bool run(int func_idx)
{
    try {
        Object::count = 0;
        cout << "Testcase " << func_idx + 1 << ": ";
        ostringstream oss;
        cout.rdbuf(oss.rdbuf());
        func_ptr[func_idx]();
        cout.rdbuf(original_cout);
        output = oss.str();
        bool ans = expect == output;
        cout << (ans ? "Pass\n" : "Fail\n");
        cout << "Expect:\n" << expect << endl;
        cout << "Output:\n" << output << endl;
        cout << "====================================================================================================================\n";
        return ans;
        // Always show output & expect cuz the author wants someone see them
    }
    catch (...){
        cout << "Error\n";
        return 0;
    }
}

int main(int argc, char **argv)
{
    cout << "Assignment-2" << endl;
    if (argc == 1) {
        int result = 0;
        int n = sizeof(func_ptr) / sizeof(func_ptr[0]);
        for (int i = 0; i != n; ++i) result += run(i);
        cout << "Result: " << result << " / " << n << endl;
    }
    else {
        run(stoi(argv[1]));
    }
    // hash032();
    return 0;
}
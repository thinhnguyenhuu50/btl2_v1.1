#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/DLinkedList.h"
#include "list/XArrayList.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

#define Abs(x) ((x) < 0 ? -(x) : (x))

// -------------------- List1D --------------------
template <typename T>
class List1D {
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();

    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);
    string toString() const;

    void removeAt(int index);

    template <typename U>
    friend ostream &operator<<(ostream &os, const List1D<U> &list);

    // custom method
    void clear();
    List1D<T> &operator=(const List1D<T> &other);
    void add(int index, const T &value) { pList->add(index, value); }
};

// -------------------- List2D --------------------
template <typename T>
class List2D {
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();

    int rows() const;
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    string toString() const;

    void add(IList<T> *e);

    void removeAt(int rowIndex);

    template <typename U>
    friend ostream &operator<<(ostream &os, const List2D<U> &matrix);

    // custom method
    void clear();
    List2D<T> &operator=(const List2D<T> &other);
};

struct InventoryAttribute {
    string name;
    double value;
    InventoryAttribute(const string &name = "", double value = 0) : name(name), value(value) {}
    InventoryAttribute(const InventoryAttribute &other) : name(other.name), value(other.value) {}
    string toString() const { return name + ": " + to_string(value); }
    bool operator==(const InventoryAttribute &other) const {
        return (name == other.name) && (Abs(value - other.value) < 1e-9);
    }
    friend ostream &operator<<(ostream &os, const InventoryAttribute &attr) {
        os << attr.toString();
        return os;
    }
};

// -------------------- InventoryManager --------------------
class InventoryManager {
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
    InventoryManager(const InventoryManager &other);

    int size() const;
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(string attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;

    // custom method
    void clear();
    InventoryManager &operator=(const InventoryManager &other);
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D() {
    // TODO
    pList = new XArrayList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements) {
    // TODO
    pList = new XArrayList<T>(0, 0, num_elements);
    for (int i = 0; i < num_elements; ++i) {
        pList->add(T());
    }
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements) {
    // TODO
    pList = new XArrayList<T>(0, 0, num_elements);
    for (int i = 0; i < num_elements; ++i) {
        pList->add(array[i]);
    }
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other) {
    // TODO
    pList = new XArrayList<T>(0, 0, other.size());
    for (int i = 0; i < other.size(); ++i) {
        pList->add(other.get(i));
    }
}

template <typename T>
List1D<T>::~List1D() {
    // TODO
    delete pList;
}

template <typename T>
int List1D<T>::size() const {
    // TODO
    return pList->size();
}

template <typename T>
T List1D<T>::get(int index) const {
    // TODO
    return pList->get(index);
}

template <typename T>
void List1D<T>::set(int index, T value) {
    // TODO
    pList->get(index) = value;
}

template <typename T>
void List1D<T>::add(const T &value) {
    // TODO
    pList->add(value);
}

template <typename T>
string List1D<T>::toString() const {
    // TODO
    stringstream ss;

    ss << pList->toString();
    return ss.str();
}

template <typename T>
ostream &operator<<(ostream &os, const List1D<T> &list) {
    // TODO
    os << list.toString();
    return os;
}

template <typename T>
void List1D<T>::removeAt(int index) {
    pList->removeAt(index);
}

template <typename T>
inline void List1D<T>::clear() {
    pList->clear();
}

template <typename T>
List1D<T> &List1D<T>::operator=(const List1D<T> &other) {
    if (this == &other) {
        return *this; // Self-assignment check
    }

    // Delete old list
    delete pList;

    // Create new list and copy data
    pList = new XArrayList<T>(0, 0, other.size());
    for (int i = 0; i < other.size(); i++) {
        pList->add(other.get(i));
    }

    return *this;
}
// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D() {
    // TODO
    pMatrix = new XArrayList<IList<T> *>();
    // pMatrix->add(new XArrayList<T>());
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows) {
    // TODO
    pMatrix = new XArrayList<IList<T> *>(0, 0, num_rows);
    for (int i = 0; i < num_rows; ++i) {
        XArrayList<T> *row = new XArrayList<T>();
        for (int j = 0; j < array[i].size(); ++j) {
            row->add(array[i].get(j));
        }
        pMatrix->add(row);
    }
}

template <typename T>
List2D<T>::List2D(const List2D<T> &other) {
    // TODO
    pMatrix = new XArrayList<IList<T> *>();
    for (int i = 0; i < other.rows(); ++i) {
        XArrayList<T> *row = new XArrayList<T>();
        for (int j = 0; j < other.pMatrix->get(i)->size(); ++j) {
            row->add(other.pMatrix->get(i)->get(j));
        }
        pMatrix->add(row);
    }
}

template <typename T>
List2D<T>::~List2D() {
    // TODO
    for (int i = 0; i < pMatrix->size(); ++i) {
        delete pMatrix->get(i);
    }
    delete pMatrix;
}

template <typename T>
int List2D<T>::rows() const {
    // TODO
    return pMatrix->size();
}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row) {
    // TODO
    pMatrix->get(rowIndex)->clear();
    for (int i = 0; i < row.size(); ++i) {
        pMatrix->get(rowIndex)->add(row.get(i));
    }
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const {
    // TODO
    return pMatrix->get(rowIndex)->get(colIndex);
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const {
    // TODO
    List1D<T> result;

    for (int i = 0; i < pMatrix->get(rowIndex)->size(); ++i) {
        result.add(this->get(rowIndex, i));
    }

    return result;
}

template <typename T>
string List2D<T>::toString() const {
    // TODO
    stringstream ss;
    ss << "[";
    for (int i = 0; i < pMatrix->size(); ++i) {
        ss << pMatrix->get(i)->toString();
        if (i != pMatrix->size() - 1)
            ss << ", ";
    }

    ss << "]";
    return ss.str();
}

template <typename T>
ostream &operator<<(ostream &os, const List2D<T> &matrix) {
    // TODO
    os << matrix.toString();
    return os;
}

template <typename T>
void List2D<T>::removeAt(int rowIndex) {
    pMatrix->removeAt(rowIndex);
}

template <typename T>
void List2D<T>::add(IList<T> *e) {
    pMatrix->add(e);
}

template <typename T>
inline void List2D<T>::clear() {
    pMatrix->clear();
}

template <typename T>
List2D<T> &List2D<T>::operator=(const List2D<T> &other) {
    if (this == &other) {
        return *this; // Self-assignment check
    }

    // Delete current matrix contents
    for (int i = 0; i < pMatrix->size(); ++i) {
        delete pMatrix->get(i);
    }
    pMatrix->clear();

    // Copy data from other matrix
    for (int i = 0; i < other.rows(); ++i) {
        XArrayList<T> *row = new XArrayList<T>();
        for (int j = 0; j < other.pMatrix->get(i)->size(); ++j) {
            row->add(other.pMatrix->get(i)->get(j));
        }
        pMatrix->add(row);
    }

    return *this;
}
// -------------------- InventoryManager Method Definitions --------------------
InventoryManager::InventoryManager()
    : attributesMatrix(List2D<InventoryAttribute>()), productNames(List1D<string>()), quantities(List1D<int>()) {
    // TODO
}

InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
    : attributesMatrix(matrix), productNames(names), quantities(quantities) {
    // TODO
}

InventoryManager::InventoryManager(const InventoryManager &other)
    : attributesMatrix(other.attributesMatrix), productNames(other.productNames), quantities(other.quantities) {
    // TODO
}

int InventoryManager::size() const {
    // TODO
    return attributesMatrix.rows();
}

List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const {
    // TODO
    if (index < 0 || index >= attributesMatrix.rows()) {
        throw out_of_range("Index is invalid!");
    }
    return attributesMatrix.getRow(index);
}

string InventoryManager::getProductName(int index) const {
    // TODO
    if (index < 0 || index >= productNames.size()) {
        throw out_of_range("Index is invalid!");
    }
    return productNames.get(index);
}

int InventoryManager::getProductQuantity(int index) const {
    // TODO
    if (index < 0 || index >= quantities.size()) {
        throw out_of_range("Index is invalid!");
    }
    return quantities.get(index);
}

void InventoryManager::updateQuantity(int index, int newQuantity) {
    // TODO
    if (index < 0 || index >= quantities.size()) {
        throw out_of_range("Index is invalid!");
    }
    quantities.set(index, newQuantity);
}

void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity) {
    // TODO
    // Create a new row in the attributesMatrix
    XArrayList<InventoryAttribute> *rowToAdd = new XArrayList<InventoryAttribute>();
    for (int i = 0; i < attributes.size(); i++) {
        rowToAdd->add(attributes.get(i));
    }

    attributesMatrix.add(rowToAdd);
    productNames.add(name);
    quantities.add(quantity);
}

void InventoryManager::removeProduct(int index) {
    // TODO
    if (index < 0 || index >= size()) {
        throw out_of_range("Index is invalid!");
    }
    // TODO: CHECK WHETHER THE LIST IS EMPTY
    attributesMatrix.removeAt(index);
    productNames.removeAt(index);
    quantities.removeAt(index);
}

List1D<string> InventoryManager::query(string attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const {
    // TODO
    List1D<string> result;
    XArrayList<double> values;

    for (int i = 0; i < size(); ++i) {
        if (quantities.get(i) < minQuantity) {
            continue;
        }
        List1D<InventoryAttribute> attributes = getProductAttributes(i);
        for (int j = 0; j < attributes.size(); ++j) {
            InventoryAttribute attr = attributes.get(j);
            if (attr.name == attributeName) {
                if (attr.value >= minValue && attr.value <= maxValue) {
                    int insertPos = result.size() - 1;
                    if (ascending) {
                        while (insertPos >= 0 && values.get(insertPos) > attr.value) {
                            --insertPos;
                        }
                    } else {
                        while (insertPos >= 0 && values.get(insertPos) < attr.value) {
                            --insertPos;
                        }
                    }
                    result.add(insertPos + 1, productNames.get(i));
                    values.add(insertPos + 1, attr.value);
                    break;
                }
            }
        }
    }
    return result;
}

void InventoryManager::removeDuplicates() {
    // TODO
    for (int i = 0; i < this->size(); i++) {
        for (int j = i + 1; j < this->size(); j++) {
            bool isDuplicate = true;

            // Check if product attributes are the same
            List1D<InventoryAttribute> attrsI = getProductAttributes(i);
            List1D<InventoryAttribute> attrsJ = getProductAttributes(j);

            // Check if names are the same
            if (productNames.get(i) != productNames.get(j)) {
                isDuplicate = false;
            } else if (attrsI.size() != attrsJ.size()) {
                isDuplicate = false;
            } else {
                for (int k = 0; k < attrsI.size(); k++) {
                    if (!(attrsI.get(k) == attrsJ.get(k))) {
                        isDuplicate = false;
                        break;
                    }
                }
            }

            // If duplicate found, combine quantities and remove the duplicate
            if (isDuplicate) {
                // Combine quantities
                int newQuantity = quantities.get(i) + quantities.get(j);
                quantities.set(i, newQuantity);

                // Remove duplicate product
                attributesMatrix.removeAt(j);
                productNames.removeAt(j);
                quantities.removeAt(j);

                // Adjust index to account for removed item
                j--;
            }
        }
    }
}

InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2) {
    // TODO
    InventoryManager newInventory;

    // First, add all products from inv1
    for (int i = 0; i < inv1.size(); i++) {
        newInventory.addProduct(
            inv1.getProductAttributes(i),
            inv1.getProductName(i),
            inv1.getProductQuantity(i));
    }

    // Then add all products from inv2
    for (int i = 0; i < inv2.size(); i++) {
        newInventory.addProduct(
            inv2.getProductAttributes(i),
            inv2.getProductName(i),
            inv2.getProductQuantity(i));
    }

    return newInventory;
}

void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const {
    // TODO
    // Clear section1 and section2 before adding new products
    section1.clear();
    section2.clear();

    int totalProducts = this->size();
    if (totalProducts == 0)
        return; // Nothing to split

    // Calculate how many products go to section1
    int sec2Count = static_cast<int>(totalProducts * (1 - ratio));
    if (sec2Count > totalProducts)
        sec2Count = totalProducts;
    int sec1Count = totalProducts - sec2Count;

    // Populate section1
    for (int i = 0; i < sec1Count; i++) {
        section1.addProduct(
            this->getProductAttributes(i),
            this->getProductName(i),
            this->getProductQuantity(i));
    }

    // Populate section2
    for (int i = sec1Count; i < totalProducts; i++) {
        section2.addProduct(
            this->getProductAttributes(i),
            this->getProductName(i),
            this->getProductQuantity(i));
    }
}

List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const {
    // TODO
    return attributesMatrix;
}

List1D<string> InventoryManager::getProductNames() const {
    // TODO
    return productNames;
}

List1D<int> InventoryManager::getQuantities() const {
    // TODO
    return quantities;
}

string InventoryManager::toString() const {
    // TODO
    stringstream ss;

    ss << "InventoryManager[" << endl
       << "  AttributesMatrix: " << attributesMatrix << ",\n"
       << "  ProductNames: " << productNames << ",\n"
       << "  Quantities: " << quantities << "\n"
       << "]";
    return ss.str();
}

void InventoryManager::clear() {
    attributesMatrix.clear();
    productNames.clear();
    quantities.clear();
}

InventoryManager &InventoryManager::operator=(const InventoryManager &other) {
    if (this == &other) {
        return *this; // Self-assignment check
    }

    // Copy data from other manager
    attributesMatrix = other.attributesMatrix;
    productNames = other.productNames;
    quantities = other.quantities;

    return *this;
}
#endif /* INVENTORY_MANAGER_H */

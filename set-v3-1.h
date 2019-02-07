// FILE: set.h (part of the namespace main_savitch_11)
// TEMPLATE CLASS PROVIDED: set<Item>
//   (a container template class for a set of items)
//
// TYPEDEFS for the set<Item> class:
//   set<Item>::value_type
//     set<Item>::value_type is the data type of the items in the set. It may be
//     any of the C++ built-in types (int, char, etc.), or a class with a
//     default constructor, a copy constructor, an assignment operator, and a
//     less-than operator forming a strict weak ordering.
//
// CONSTRUCTOR for the set<Item> class:
//   set( )
//     Postcondition: The set is empty.
//
// MODIFICATION MEMBER FUNCTIONS for the set<Item> class:
//   void clear( )
//     Postcondition: The set is empty.
//
//   bool insert(const Item& entry)
//     Postcondition: If an equal entry was already in the set, the set is
//     unchanged and the return value is false. Otherwise, entry was added
//     to the set and the return value is true. This is slightly different than
//     the C++ Standard Library set (see Appendix H).
//
//   size_t erase(const Item& target)
//     Postcondition: If target was in the set, then it has been removed from
//     the set and the return value is 1. Otherwise the set is unchanged and the
//     return value is zero.
//
// CONSTANT MEMBER FUNCTIONS for the Set<Item> class:
//   size_t count(const Item& target) const
//     Postcondition: Returns the number of items equal to the target
//     (either 0 or 1 for a set).
//
//  bool empty( ) const
//     Postcondition: Returns true if the set is empty; otherwise returns false.
//
// VALUE SEMANTICS for the set<Item> class:
//   Assignments and the copy constructor may be used with set<Item> objects.
//
// DYNAMIC MEMORY USAGE by the set<Item> class:
//   If there is insufficient dynamic memory, then the following functions throw
//   bad_alloc:
//   The constructors, insert, and the assignment operator.

#ifndef SET_V2_H
#define SET_V2_H
#include <cstdlib>   // Provides size_t

using namespace std;

    template<class Item>
    class set {
    public:
        // TYPEDEFS
        typedef Item value_type;

        // CONSTRUCTORS and DESTRUCTOR
        set();

        set(const set &source);

        ~set() { clear(); }

        // MODIFICATION MEMBER FUNCTIONS
        void operator=(const set &source);

        void clear();

        bool insert(const Item &entry);

        std::size_t erase(const Item &target);

        // CONSTANT MEMBER FUNCTIONS
        std::size_t count(const Item &target) const;

        bool empty() const { return (data_count == 0); }

        // SUGGESTED FUNCTION FOR DEBUGGING
        void print(int indent) const;

    private:
        // MEMBER CONSTANTS
        static const std::size_t MINIMUM = 1;   //originally 200
        static const std::size_t MAXIMUM = 2 * MINIMUM;     //2
        // MEMBER VARIABLES
        std::size_t data_count;
        Item data[MAXIMUM + 1];     //3
        std::size_t child_count;
        set *subset[MAXIMUM + 2];   //4

        // HELPER MEMBER FUNCTIONS
        bool is_leaf() const { return (child_count == 0); }

        bool loose_insert(const Item &entry);

        bool loose_erase(const Item &target);

        void remove_biggest(Item &removed_entry);

        void fix_excess(std::size_t i);

        void fix_shortage(std::size_t i);
        // NOTE: The implementor may want to have additional helper functions
    };


    template<class Item>
    set<Item>::set() {
//    Postcondition: The set is empty
        data_count = 0;
        data[0] = Item();
        data[1] = Item();
        data[2] = Item();
        child_count = 0;
        subset[0] = NULL;
        subset[1] = NULL;
        subset[2] = NULL;
        subset[3] = NULL;

    }

    template<class Item>
    set<Item>::set(const set &source) {
        data_count = source.data_count;
        data[0] = source.data[0];
        data[1] = source.data[1];
        data[2] = source.data[2];
        child_count = source.child_count;
        subset[0] = source.subset[0];
        subset[1] = source.subset[1];
        subset[2] = source.subset[2];
        subset[3] = source.subset[3];
    }

    template<class Item>
    void set<Item>::operator=(const set &source) {
        data_count = source.data_count;
        data[0] = source.data[0];
        data[1] = source.data[1];
        data[2] = source.data[2];
        child_count = source.child_count;
        subset[0] = source.subset[0];
        subset[1] = source.subset[1];
        subset[2] = source.subset[2];
        subset[3] = source.subset[3];
    }

    template<class Item>
    void set<Item>::clear() {
//     Postcondition: The set is empty.
        for(size_t i = 0; i < MAXIMUM + 1; i++){
            data[i] = Item();
        }
        for(size_t i = 0; i < child_count; i++){
            subset[i]->clear();
        }
        for(size_t i = 0; i <= MAXIMUM + 1; i++){
            subset[i] = NULL;
        }
        data_count = 0;
        child_count = 0;
    }

    template<class Item>
    bool set<Item>::insert(const Item &entry) {
//     loose insert then correct
//     Postcondition: If an equal entry was already in the set, the set is
//     unchanged and the return value is false. Otherwise, entry was added
//     to the set and the return value is true. This is slightly different than
//     the C++ Standard Library set (see Appendix H).

        if(!loose_insert(entry)){
            return 0;
        }
        if(data_count>MAXIMUM){
            //fix the root of the entire tree so there are no longer too many entries
        }
        return 1;
    }

    template<class Item>
    size_t set<Item>::erase(const Item &target) {
//    Postcondition: If target was in the set, then it has been removed from
//     the set and the return value is 1. Otherwise the set is unchanged and the
//     return value is zero.

        if(!loose_erase(target)){
            return 0;
        }
        if(data_count == 0 && (child_count == 1)){
            set* temp = subset[0];
            data_count = temp->data_count;
            child_count = temp->child_count;

            for(size_t i = 0; i <= MAXIMUM; i++){
                data[i] = temp->data[i];
            }

            for(size_t i = 0; i <= MAXIMUM + 1; i++){
                subset[i] = temp->subset[i];
            }
            temp->clear();
        }
        return 1;
    }

    template<class Item>
    size_t set<Item>::count(const Item &target) const {
//     Postcondition: Returns the number of items equal to the target
//     (either 0 or 1 for a set).
        size_t found = 0;
        if (data[0]==target){
            found = 1;
        }
        if (data[1]==target){
            found = 1;
        }
        if (data[2]==target){
            found = 1;
        }
        return found;
    }

    template<class Item>
    void set<Item>::print(int indent) const {
        if(subset[0] != NULL){
            subset[0]->print(indent);
        }
        cout << data[0] << " ";
        if(subset[0] != NULL){
            subset[1]->print(indent);
        }
        cout << data[1] << " ";
        if(subset[0] != NULL){
            subset[2]->print(indent);
        }
        cout << data[2] << " ";
        if(subset[0] != NULL){
            subset[3]->print(indent);
        }
    }

    template<class Item>
    bool set<Item>::loose_insert(const Item &entry) {
        size_t i = 0;
        while(data[i]<entry&&i<2){
            i++;
        }
        if(data[i]<entry){
            i = data_count;
        }
        if(data[i]==entry){
            return 0;
        }
        else if(child_count==0){
            data[2] = data[1];
            data[1] = data[0];
            data[0] = entry;
            data_count++;
            return 1;
        }
        else{
            bool temp = subset[i]->loose_insert(entry);
            if(subset[i]->data_count>2){
                fix_excess(i);
            }
            return temp;
        }
    }

    template<class Item>
    bool set<Item>::loose_erase(const Item &target) {
        //precondition: entire B-tree is valid
        //postcondition: if target was not in set, set is unchanged and false is returned
        //otherwise target was removed, true is returned and B-tree is still valid
        //Except number of entries in root of this set might be one less than minimum
        size_t i = 0;
        while(data[i]<target&&i<2){
            i++;
        }
        if(data[i]<target){
            i = data_count;
        }
        if(child_count == 0 && count(target) == 0){
            return 0;
        }
        if(child_count == 0 && count(target) == 1){
            data[i] = Item();       //essentially changes data[i] to NULL
            --data_count;
            return true;
        }
        if(child_count != 0 && count(target) == 0){
            bool temp = subset[i]->loose_erase(target);
            if(subset[i]->child_count < MINIMUM){
                fix_shortage(i);
            }
            return temp;
        }
        if(child_count != 0 && count(target) == 1){
            subset[i]->remove_biggest(data[i]);
            if(subset[i]->data_count < MINIMUM){
                fix_shortage(i);
            }
            return true;
        }
    }

    template<class Item>
    void set<Item>::remove_biggest(Item &removed_entry) {
//removes biggest value from b-tree
        if(child_count == 0){
            removed_entry = data[data_count - 1];
            data[data_count-1] = Item();
            --data_count;
        }
        else{
            subset[child_count - 1]->remove_biggest(removed_entry);
            if(subset[child_count - 1]->data_count < MINIMUM){
                fix_shortage(child_count - 1);
            }
        }
    }

    template<class Item>
    void set<Item>::fix_excess(size_t i) {
//when maximum is exceeded, divides data values into new nodes
        for(size_t m = data_count; m>i; m--){
            data[m] = data[m-1];
        }
        data[i] = subset[i]->data[MINIMUM];
        for(size_t m = child_count; m>i; m--){
            subset[m] = subset[m-1];
        }
        set<Item> *new1;
        set<Item> *new2;
        new1->data[0] = subset[i]->data[0];
        new2->data[0] = subset[i]->data[2];
        subset[i] = new1;
        subset[i+1] = new2;
        child_count++;

        //data[i+1] = subset[i] = data[MINIMUM];
    }

    template<class Item>
    void set<Item>::fix_shortage(size_t i) {
//when minimum is violated, combines data values from different nodes into one
        if(i > 0 && subset[i -1]->data_count > MINIMUM){
            subset[i]->data[2] = subset[i]->data[1];
            subset[i]->data[1] = subset[i]->data[0];
            subset[i]->data[2] = data[i-1];
            subset[i]->data_count++;
            data[i-1] = subset[i-1]->data[subset[i-1]->data_count-1];
            subset[i-1]->data[subset[i-1]->data_count-1] = Item();
            subset[i-1]->data_count--;
            if(subset[i-1]->child_count != 0){
                subset[i]->subset[3] = subset[i]->subset[2];
                subset[i]->subset[2] = subset[i]->subset[1];
                subset[i]->subset[1] = subset[i]->subset[0];
                subset[i]->subset[0] = subset[i-1]->subset[subset[i-1]->child_count-1];
                subset[i]->child_count++;
                subset[i-1]->child_count--;
            }
        }
        else if(i < child_count && subset[i + 1]->data_count > MINIMUM){
            subset[i]->data[2] = subset[i]->data[1];
            subset[i]->data[1] = subset[i]->data[0];
            subset[i]->data[2] = data[i-1];
            subset[i]->data_count++;
            data[i-1] = subset[i+1]->data[subset[i+1]->data_count-1];
            subset[i+1]->data[subset[i+1]->data_count-1] = Item();
            subset[i+1]->data_count--;
            if(subset[i+1]->child_count != 0){
                subset[i]->subset[3] = subset[i]->subset[2];
                subset[i]->subset[2] = subset[i]->subset[1];
                subset[i]->subset[1] = subset[i]->subset[0];
                subset[i]->subset[0] = subset[i+1]->subset[subset[i+1]->child_count-1];
                subset[i]->child_count++;
                subset[i+1]->child_count--;
            }
        }
        else if(i > 0 && subset[i-1]->child_count == MINIMUM){
            subset[i-1]->data[subset[i-1]->data_count-1] = data[i-1];
            for (size_t j = i; j < data_count; j++){
                data[j-1] = data[j];
            }
            subset[i-1]->data_count++;
            data_count--;
            int m = 0;
            for (size_t k = subset[i-1]->child_count; k < MAXIMUM + 2; k++){
                subset[i-1]->subset[k] = subset[i]->subset[m];
                m++;
            }
            int x = 0;
            for (size_t k = subset[i-1]->child_count; k < MAXIMUM + 2; k++){
                subset[i-1]->subset[k] = subset[i]->subset[x];
                x++;
            }
            subset[i-1]->child_count += m;
            subset[i-1]->data_count += x;
            subset[i]->child_count = 0;
            subset[i]->data_count = 0;

            for(size_t k = i; k < MAXIMUM + 1; k++){
                subset[i] = subset[i+1];
            }
            subset[MAXIMUM+1]->clear();
            child_count--;
        }
        else{
            subset[i+1]->data[subset[i+1]->data_count-1] = data[i-1];
            for (size_t j = i; j < data_count; j++){
                data[j-1] = data[j];
            }
            subset[i+1]->data_count++;
            data_count--;
            int m = 0;
            for (size_t k = subset[i+1]->child_count; k < MAXIMUM + 2; k++){
                subset[i+1]->subset[k] = subset[i]->subset[m];
                m++;
            }
            int x = 0;
            for (size_t k = subset[i+1]->child_count; k < MAXIMUM + 2; k++){
                subset[i+1]->subset[k] = subset[i]->subset[x];
                x++;
            }
            subset[i+1]->child_count += m;
            subset[i+1]->data_count += x;
            subset[i]->child_count = 0;
            subset[i]->data_count = 0;

            for(size_t k = i; k < MAXIMUM + 1; k++){
                subset[i] = subset[i+1];
            }
            subset[MAXIMUM+1]->clear();
            child_count--;
        }

    }



#endif
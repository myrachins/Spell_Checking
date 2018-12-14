#include  "hashset.h"

using namespace std;

template<class key_type, class hash_func, class key_equal>
const int HashSet<key_type,hash_func,key_equal>::num_primes = 25;


template<class key_type, class hash_func, class key_equal>
const unsigned long HashSet<key_type,hash_func,key_equal>::prime_list[] =
        {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
         196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
         50331653, 100663319, 201326611, 402653189, 805306457};


template<class key_type, class hash_func, class key_equal>
bool HashSet<key_type,hash_func,key_equal>::search(const key_type& k) {

    int p = hf(k) % table_size();

    while (ht->at(p).state == Entry::Full || ht->at(p).state == Entry::Removed)
    {
        if (eq(ht->at(p).key, k))
            return true;

        p++;
        if (p == table_size())
            p = 0;
    }
    return false;
}

template<class key_type, class hash_func, class key_equal>
void HashSet<key_type,hash_func,key_equal>::remove(const key_type& k)
{
    int p = hf(k) % table_size();

    while (ht->at(p).state == Entry::Full || ht->at(p).state == Entry::Removed)
    {
        if (eq(ht->at(p).key, k))
        {
            ht->at(p).state = Entry::Removed;
            entries--;
            return;
        }
        p++;
        if (p == table_size())
            p = 0;
    }
    throw std::invalid_argument("This element can't be deleted");
}


template<class key_type, class hash_func, class key_equal>
void HashSet<key_type,hash_func,key_equal>::insert(const key_type& k)
{
    if (load_factor() > .7) {
        resize();
    }

    int p = hf(k) % table_size();

    while (ht->at(p).state == Entry::Full)
    {
        p++;
        if(p == table_size())
            p = 0;
    }
    ht->at(p).state = Entry::Full;
    (*ht)[p].key = k;
    entries++;
}

template<class key_type, class hash_func, class key_equal>
int HashSet<key_type,hash_func,key_equal>::resize() {

    if (prime == num_primes - 1) {
        std::cerr << "maximal table size reached, aborting ... " << std::endl;
        exit(2);
    }

    int prevSize = prime_list[prime];
    int newSize = prime_list[prime + 1]; // counting new size of storage
    std::vector<Entry> newStorage(newSize);

    for(int i = 0; i < prevSize; i++)
    {
        if(ht->at(i).state == Entry::Full) {
            int p = hf((*ht)[i].key) % newSize;

            while (newStorage[p].state == Entry::Full) {
                p++;
                if(p == newSize)
                    p = 0;
            }
            (newStorage)[p].state = Entry::Full;
            (newStorage)[p].key = (*ht)[i].key;
        }
    }

    ht->swap(newStorage);
    prime++;

    return newSize;
}

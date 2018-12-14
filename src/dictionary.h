#ifndef  _DICTIONARY_H_ 
#define  _DICTIONARY_H_

#include  <iostream>
#include  <vector>
#include  <list>
#include  <algorithm>
#include  <string>
#include  <stdlib.h>

#include  "hashset.h"
#include  "hashset.cpp"

using namespace std;

class hash_function
{
public:

    unsigned int operator()( const string& s )  const {
        unsigned int hash = 0;

        for(int i = 0; i < s.length(); i++)
        {
            if(i & 1)
                hash ^= (~((hash << 11) + s[i] ^ (hash >> 5)));
            else
                hash ^= ((hash << 7) ^ s[i] * (hash >> 3));
        }

        return hash;
    }
};

class equality
{
public:
    equality() {}
    bool  operator()( const string& A, const string& B )  const 
    {
        return  (A == B);
    }
};

class Dictionary: public HashSet<string, hash_function, equality> {

    // Complete definition
public:
	explicit Dictionary(string filename);
};
#endif // _DICTIONARY_H_

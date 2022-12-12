#include "key_value_pair.h"
/**
  almacena key y value    Constructors*/
KeyValuePair::KeyValuePair(){}
KeyValuePair::KeyValuePair(int k, string v){
    key = k;
    value = v;
}

/**    Returns */
int KeyValuePair::get_key(){return key;}

/**Returns */
string KeyValuePair::get_value(){return value;}

KeyValuePair::~KeyValuePair(){}
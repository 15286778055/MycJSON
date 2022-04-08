#ifndef MY_CJSON__h
#define MY_CJSON__h

#include <string>
#include <cstring>

using namespace std;

/* begin of namespace __MyCJSON__ */
namespace __MyCJSON__ {

/* MycJSON value types */
#define MycJSON_Object 0
#define MycJSON_Array 1
#define MycJSON_String 2;
#define MycJSON_Number 3;
#define MycJSON_True 4;
#define MycJSON_False 5;
#define MycCJON_Null 6;

/* a flag pointer to error */
static const char* ep;

/* memory manage */
static void* (*MycJSON_malloc)(size_t sz) = malloc;
static void (*MycJSON_free)(void* ptr) = free;


/* MycJSON Object 对象 */
class MycJSONObject {

public:
    /* value typename */
    typedef MycJSONObject object_type;
    typedef MycJSONObject array_type;
    typedef string string_type;
    typedef int numberint_type;
    typedef double numberdouble_type;
    typedef bool true_type;
    typedef bool false_type;
    typedef char null_type;

    /* 键值的类型别名 */
    typedef string key_type;
    typedef union value_type {
        object_type* value_object;
        array_type* value_array;
        string_type value_string;
        int value_int;
        double value_double;
        bool value_true;
        bool value_false;
        char value_null;
    } value_type;

    /* parse to json object*/
    char* parse(MycJSONObject* item, const char* value);
    char* parse_object(MycJSONObject* item, const char* value);
    char* parse_array(MycJSONObject* item, const char* value);
    static const char* parse_string(MycJSONObject* item, const char* str);
    char* parse_number(MycJSONObject* item, const char* value);

    /* to json string */
    char* toJSONString(MycJSONObject* item);

    /* construcotr destructor */
    // MycJSONObject() {}
    // ~MycJSONObject() {
    //     delete next;
    //     delete prev;
    //     if (type == MycJSON_Object) delete value.value_object;
    //     if (type == MycJSON_Array) delete value.value_array;
    // }


private:
    
    /* member data */
    int type; /* The type of the item */
    key_type key;
    
    value_type value;
    
    MycJSONObject* next;
    MycJSONObject* prev;
    // MycJSONObject* child;
    
};

/* declare some functions */
static const char* erase_space(const char* value);



/* define funcitons belong to MycJSONObject */
char* MycJSONObject::parse(MycJSONObject* item, const char* value) {
    
    if (!value) return nullptr; /* 空串 */
    if ('{' == *value) { return parse_object(item, value); }
    if ('[' == *value) { return parse_array(item, value); }
    if ('-' == *value || ('0' <= *value && '9' >= *value)) { return parse_number(item, value); }
    if ('\"' == *value) {}
    if (!strncmp(value, "true", 4)) {}
    if (!strncmp(value, "false", 5)) {}
    if (!strncmp(value, "null", 4)) {}
    
    //

}

char* MycJSONObject::parse_object(MycJSONObject* item, const char* value) {
    item->type = MycJSON_Object;
    value = erase_space(value);
    if ('\"' != *value) return nullptr;
    parse_string(item, value);      /* parse key */

    parse(item, value);     /* parse value */


    while (',' == *value) {
        parse_string(item, value);      /* parse key */
        parse(item, value);     /* parse value */
    }

}

const char* MycJSONObject::parse_string(MycJSONObject* item, const char* str) {
    
    if ('\"' != *str) { ep = str; return 0; };      /* not a string */
    
    const char* ptr = str+1;
    char* ptr2;
    char* out;
    int len = 0;
    unsigned uc, uc2;
    while('\"' != *ptr  && *ptr && ++len) if ('\\' == *ptr++) ++ptr; /* skip escaped quotes */

    out = (char*)MycJSON_malloc(len + 1);

}

char* MycJSONObject::parse_number(MycJSONObject* item, const char* value) {

}

char* MycJSONObject::parse_array(MycJSONObject* item, const char* value) {

}


char* MycJSONObject::toJSONString(MycJSONObject* item) {
    
}


/* Utility to jump whitespace and cl/lr */
static const char* erase_space(const char* value) {
    /* 0 ~ 31 为控制字符（非打印），32 为 空格 */
    while (value && *value && 32 >= (unsigned char)*value) ++value;
    return value;
}



} /* end of namespace __MyCJSON__ */






#endif
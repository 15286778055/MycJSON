#ifndef MY_CJSON__h
#define MY_CJSON__h

#include <string>

using namespace std;

namespace __MyCJSON__ {

/* MycJSON value types */
#define MycJSON_Object 0
#define MycJSON_Array 1
#define MycJSON_String 2;
#define MycJSON_Number 3;
#define MycJSON_True 4;
#define MycJSON_False 5;
#define MycCJON_Null 6;


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
    typedef char* key_type;
    typedef union value_type {
        object_type value_object;
        string_type value_string;
    } value_type;
    // typedef T value_type;
    
    


    /* parse */
    MycJSONObject *parse(const char *value);
    /* to json-string*/
    char* toJSONString(MycJSONObject *item);


    /* constructor */
    MycJSONObject() {}
    MycJSONObject(int type, key_type key, value_type value) : type(type), key(key), value(value), next(nullptr), prev(nullptr) {}
    // MycJSONObject(const MycJSONObject& object) : type(object.type), key(object.key), value(object.value), next(nullptr), prev(nullptr) {
    //     if (nullptr != object.next)  {

    //     }
    //     if (nullptr != object.prev) {

    //     }
    // }
    MycJSONObject(MycJSONObject&& object) : type(object.type), key(object.key), value(object.value), next(object.next), prev(object.prev) {
        object.next = nullptr;
        object.prev = nullptr;
    }
    // MycJSONObject& operator=(const MycJSONObject& object) {
    //     /* 自我复制检测 */
    //     type = object.type;
    //     key = object.key;
    //     value = object.value;
    //     if (nullptr != object.next)  {

    //     } else next = nullptr;
    //     if (nullptr != object.prev) {

    //     } else prev = nullptr;
    // }
    MycJSONObject& operator=(MycJSONObject&& object) {
        /* 自我复制检测 */
        type = object.type;
        key = object.key;
        value = object.value;
        next = object.next;
        prev = object.prev;
        object.next = nullptr;
        object.prev = nullptr;
    }
    ~MycJSONObject() {
        delete next;
        delete prev;
    };


private:
    
    /* member data */
    key_type key;
    value_type value;
    
    MycJSONObject* next, * prev;
    int type; /* The type of the item */


};

MycJSONObject* MycJSONObject::parse(const char* value) {

}





class MyCJSON {
    
};





}




#endif
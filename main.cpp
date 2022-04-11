#include <iostream>
#include "my_cjson.hpp"


using namespace std;

int main() {

    typedef typename __MyCJSON__::MycJSONObject Object;

    const char *str = "{\"name\":\"xiaopeng\",\"passwd\":\"123456\",\"num\":\"456\"}";

    Object *po;
    cout << "to object" << endl;
    Object::parse_value(po, str);
    
    Object *child;
    child  = po->value.value_object;
    while (child) {
        cout << child->key << endl;
        if (child->type == MycJSON_String) cout << child->value.value_string << endl;
        child = child->next;
    }

    
    cout << "to jsonstring" << endl;
    char *out = Object::print_value(po, 0, 0, 0);



    cout << "The outcome is:\n" 
        << out 
        << endl;


    return 0;
}

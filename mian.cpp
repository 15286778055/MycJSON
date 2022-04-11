#include <iostream>
#include "my_cjson.hpp"


using namespace std;

int main() {

    typedef typename __MyCJSON__::MycJSONObject Object;

    char *str = "";

    Object *po;
    Object::parse_value(po, str);

    char *out = Object::print_value(po, 0, 0, 0);

    cout << "The outcome is:\n" 
        << out 
        << endl;


    return 0;
}

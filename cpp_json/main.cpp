
#include "JsonAPI/Jsonparser.h"
#include "JsonAPI/Jsonstringifier.h"



int main() {
    JSON::json j, j2;
    Jsonparser::parser p(j);
    p.parse_file("testexample1.json"); //you should use absolute path better.
    //p.parse("1.4");
    //cout << j.get_number() << endl;
    Jsonparser::parser p2(j2);
    cout << j.get_type_format() << ' ' << endl;
    Jsonstringifier::stringifier g;
    cout << g.stringify(j);
    return 0;
}

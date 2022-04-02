
#include "JsonAPI/Jsonparser.h"
#include "JsonAPI/Jsonstringifier.h"






int main() {
    JSON::json j, j2;
    Jsonparser::parser p(j);
    vector<int> data{4, 3, 4, 1, 8, 0, 8};
    p.parse_file("testexample1.json");
    Jsonparser::parser p2(j2);
    cout << j.get_type_format() << ' ' << endl;
    Jsonstringifier::stringifier g;
    //cout << g.stringify(j);
    return 0;
}

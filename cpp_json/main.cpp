
#include "JsonAPI/Jsonparser.h"
#include "JsonAPI/Jsonstringifier.h"



int main() {
    JSON::json j, j2;
    Jsonparser::parser p(j);
    p.parse("{\"key\":[true, false, {\"1\":true}, {\"2\": false}, \"sfjdls\", [true, {\"a\": false}]]}");
    Jsonparser::parser p2(j2);
    p2.parse("{\"key\":[true, false, {\"1\":true}, {\"2\": false}, \"sfjdls\", [true, {\"a\": false}]]}");
    cout << j.get_type_format() << ' ' << endl;
    Jsonstringifier::stringifier g;
    cout << g.stringify(j);
    return 0;
}

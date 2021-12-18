
#include "JsonAPI/Jsonparser.h"
#include "JsonAPI/Jsonstringifier.h"



int main() {
    JSON::json j, j2;
    Jsonparser::parser p(j);
    p.parse("{\n"
            "  \"name\": \"vue_test\",\n"
            "  \"version\": \"0.1.0\",\n"
            "  \"private\": true,\n"
            "  \"scripts\": {\n"
            "    \"serve\": \"vue-cli-service serve\",\n"
            "    \"build\": \"vue-cli-service build\",\n"
            "    \"lint\": \"vue-cli-service lint\"\n"
            "  },\n"
            "  \"dependencies\": {\n"
            "    \"core-js\": \"^3.6.5\",\n"
            "    \"vue\": \"^2.6.11\"\n"
            "  },\n"
            "  \"devDependencies\": {\n"
            "    \"@vue/cli-plugin-babel\": \"~4.5.0\",\n"
            "    \"@vue/cli-plugin-eslint\": \"~4.5.0\",\n"
            "    \"@vue/cli-service\": \"~4.5.0\",\n"
            "    \"babel-eslint\": \"^10.1.0\",\n"
            "    \"eslint\": \"^6.7.2\",\n"
            "    \"eslint-plugin-vue\": \"^6.2.2\",\n"
            "    \"vue-template-compiler\": \"^2.6.11\"\n"
            "  },\n"
            "  \"eslintConfig\": {\n"
            "    \"root\": true,\n"
            "    \"env\": {\n"
            "      \"node\": true\n"
            "    },\n"
            "    \"extends\": [\n"
            "      \"plugin:vue/essential\",\n"
            "      \"eslint:recommended\"\n"
            "    ],\n"
            "    \"parserOptions\": {\n"
            "      \"parser\": \"babel-eslint\"\n"
            "    },\n"
            "    \"rules\": {}\n"
            "  },\n"
            "  \"browserslist\": [\n"
            "    \"> 1%\",\n"
            "    \"last 2 versions\",\n"
            "    \"not dead\"\n"
            "  ]\n"
            "}");
    Jsonparser::parser p2(j2);
    //p2.parse("{\"key\":[true, false, {\"1\":true}, {\"2\": false}, \"sfjdls\", [true, {\"a\": false}]]}");
    cout << j.get_type_format() << ' ' << endl;
    Jsonstringifier::stringifier g;
    cout << g.stringify(j);
    return 0;
}

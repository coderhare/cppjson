s## 三个模块


#### JSON数值类型：
1. `json_object`定义为`mem`,使用`unordered_map<string, json_value>`来实现，`unordered_map`底层是哈希表，可以`O(1)`时间访问元素。
2. `json_array`定义为`arr`，使用`vector<json>`来实现，
3. `json`类为单个`json_value`类的指针
4. `json_value`是一个聚合类，其包含的数值有`double, string, arr, mem`,使用该类型是为了节约空间




`json_value`类和`json`类应当提供数据访问的接口

#### JSON解析类 (Jsonparser)
include数字类型，使用堆栈完成数值解析以及抛出异常

#### JSON序列化类（Jsonstringifier)
解析类的反操作。



12.13 完成`json_value`和`json`类的设计以及`get`和`set`系列函数。保证接口隔离，实现了多层套用。


12.14 参考RapidJSON的实现来构思C++的混合堆栈.
`reinterpret_cast`是一种无精度损失的强制类型转换，是转换级别最高的，可以避免掉编译器的警告，比如说, 整数强转成指针，然后从指针转换成整数，内容是不变的，
它的自由性很高，正因如此，使用者必须清楚自己使用它来干什么。 最后因为内存管理比较麻烦，决定暂时放弃这种方案，采用拷贝对象的方式，等学习更深入之后再进行深一步的改进


`jsonparser`以及`jsonstringifier`都是共用一个堆栈结构，决定将解析堆栈封装到一个头文件中，然后两者都可以创建实例。

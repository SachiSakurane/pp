# pp
Proposal to use pipe operators to operate like RX.

## Example

```C++
pp::behavior<int> x{2};

// If return is other than void, generate a return-valued observable.
auto observable = x | [](auto i) { return  static_cast<double>(i) * 1.9; } | [](auto d) { return std::to_string(d) };
// If return is void, subscribe to observable
auto subscription = observable | [](auto s) { std::cout << "string: " << s.c_str() << std::endl; }

x = 42;
```

Output
```
string: 3.8
string: 79.8
```

## Test

```
bazel test //... --test_output=all
```

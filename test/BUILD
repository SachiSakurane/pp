cc_test(
    name = "pp_test",
    size = "small",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//:pp_library",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    visibility = ["//visibility:public"],
)

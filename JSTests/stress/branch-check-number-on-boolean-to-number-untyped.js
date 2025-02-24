function foo(o) {
    if (o.f)
        return "yes";
    else
        return "no";
}

noInline(foo);

function test(value, expected) {
    var result = foo({f:value});
    if (result != expected)
        throw "Error: bad result for " + value + ": " + result;
}

for (var i = 0; i < testLoopCount; ++i) {
    test(1.5, "yes");
    test(0.0, "no");
    test(true, "yes");
    test(false, "no");
}

test("yes", "yes");

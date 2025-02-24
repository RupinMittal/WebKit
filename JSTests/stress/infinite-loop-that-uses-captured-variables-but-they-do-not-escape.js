var count = 0;

function bar(f) {
    if (++count < 10) {
        var result = f();
        if (result != i + 1)
            throw "Error: bad result insie closure for i = " + i + ": " + result;
        return;
    }
    count = 0;
    throw "done";
}

noInline(bar);

function foo(a) {
    var x = a + 1;
    for (;;) {
        bar(function() { return x; });
    }
}

noInline(foo);

for (var i = 0; i < testLoopCount; ++i) {
    try {
        foo(i);
    } catch (done) {
        if (done != "done")
            throw "Error: bad exception: " + done;
    }
}


function shouldBe(actual, expected) {
    if (actual !== expected)
        throw new Error('bad value: ' + actual);
}

var object = { a: 10 };
Object.defineProperties(object, {
    "0": {
        get: function() { return this.a; },
        set: function(x) { this.a = x; },
    },
});

var array1 = [0.1, "OK", 0.3, 0.4, 0.5];
var array2 = [1, "HELLO", 3, 4, 5];
ensureArrayStorage(array2);
array1.ok = 42;
array2.ok = 42;
array2.__proto__ = object;

// Arrayify(SlowPutArrayStorage) works with ftl-eager
function testArrayStorage(array)
{
    return array.length;
}
noInline(testArrayStorage);

for (var i = 0; i < testLoopCount; ++i) {
    shouldBe(testArrayStorage(array1), 5);
    shouldBe(testArrayStorage(array2), 5);
}

var array3 = [1, 2, 3];
ensureArrayStorage(array3);
shouldBe(testArrayStorage(array3), 3);
var array4 = [1, 2, 3];
shouldBe(testArrayStorage(array4), 3);
var array5 = {0:1, 1:2};
ensureArrayStorage(array5);
shouldBe(testArrayStorage(array5), undefined);

//@ requireOptions("--useTemporal=1")

function shouldBe(actual, expected) {
    if (actual !== expected)
        throw new Error(`expected ${expected} but got ${actual}`);
}

function shouldThrow(func, errorType, message) {
    let error;
    try {
        func();
    } catch (e) {
        error = e;
    }

    if (!(error instanceof errorType))
        throw new Error(`Expected ${errorType.name}!`);
    if (message !== undefined)
        shouldBe(String(error), message);
}

shouldBe(Temporal.PlainMonthDay instanceof Function, true);
shouldBe(Temporal.PlainMonthDay.length, 2);
shouldBe(Object.getOwnPropertyDescriptor(Temporal.PlainMonthDay, 'prototype').writable, false);
shouldBe(Object.getOwnPropertyDescriptor(Temporal.PlainMonthDay, 'prototype').enumerable, false);
shouldBe(Object.getOwnPropertyDescriptor(Temporal.PlainMonthDay, 'prototype').configurable, false);
shouldBe(Temporal.PlainMonthDay.prototype.constructor, Temporal.PlainMonthDay);

const monthDay = new Temporal.PlainMonthDay(4, 29);

{
    shouldBe(monthDay.monthCode, "M04");
    shouldBe(monthDay.day, 29);
    shouldBe(monthDay.year, undefined);
    shouldBe(monthDay.calendarId, "iso8601");

    shouldThrow(() => new Temporal.PlainMonthDay(20, 1), RangeError);
    shouldThrow(() => new Temporal.PlainMonthDay(1, 40), RangeError);
}

{
    shouldBe(monthDay.toString(), '04-29');
    shouldBe(monthDay.toJSON(), monthDay.toString());
    shouldBe(monthDay.toLocaleString(), monthDay.toString());
}


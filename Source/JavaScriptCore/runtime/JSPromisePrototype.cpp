/*
 * Copyright (C) 2013-2021 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "JSPromisePrototype.h"

#include "BuiltinNames.h"
#include "JSCInlines.h"
#include "JSInternalPromise.h"
#include "JSPromise.h"

namespace JSC {

STATIC_ASSERT_IS_TRIVIALLY_DESTRUCTIBLE(JSPromisePrototype);

static JSC_DECLARE_HOST_FUNCTION(promiseProtoFuncCatch);

}

#include "JSPromisePrototype.lut.h"

namespace JSC {

const ClassInfo JSPromisePrototype::s_info = { "Promise"_s, &Base::s_info, &promisePrototypeTable, nullptr, CREATE_METHOD_TABLE(JSPromisePrototype) };

/* Source for JSPromisePrototype.lut.h
@begin promisePrototypeTable
  finally      JSBuiltin            DontEnum|Function 1
@end
*/

JSPromisePrototype* JSPromisePrototype::create(VM& vm, JSGlobalObject* globalObject, Structure* structure)
{
    JSPromisePrototype* object = new (NotNull, allocateCell<JSPromisePrototype>(vm)) JSPromisePrototype(vm, structure);
    object->finishCreation(vm, globalObject);
    object->addOwnInternalSlots(vm, globalObject);
    return object;
}

Structure* JSPromisePrototype::createStructure(VM& vm, JSGlobalObject* globalObject, JSValue prototype)
{
    return Structure::create(vm, globalObject, prototype, TypeInfo(ObjectType, StructureFlags), info());
}

JSPromisePrototype::JSPromisePrototype(VM& vm, Structure* structure)
    : JSNonFinalObject(vm, structure)
{
}

void JSPromisePrototype::finishCreation(VM& vm, JSGlobalObject* globalObject)
{
    Base::finishCreation(vm);
    putDirectWithoutTransition(vm, vm.propertyNames->builtinNames().thenPublicName(), globalObject->promiseProtoThenFunction(), static_cast<unsigned>(PropertyAttribute::DontEnum));
    JSC_NATIVE_FUNCTION_WITHOUT_TRANSITION(vm.propertyNames->catchKeyword, promiseProtoFuncCatch, static_cast<unsigned>(PropertyAttribute::DontEnum), 1, ImplementationVisibility::Public);
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

void JSPromisePrototype::addOwnInternalSlots(VM& vm, JSGlobalObject* globalObject)
{
    putDirectWithoutTransition(vm, vm.propertyNames->builtinNames().thenPrivateName(), globalObject->promiseProtoThenFunction(), PropertyAttribute::DontEnum | PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);
}

bool promiseSpeciesWatchpointIsValid(VM& vm, JSPromise* thisObject)
{
    auto* structure = thisObject->structure();
    JSGlobalObject* globalObject = structure->globalObject();
    if (globalObject->promiseSpeciesWatchpointSet().state() != IsWatched) [[unlikely]] {
        if (structure->classInfoForCells() == JSInternalPromise::info())
            return true;
        return false;
    }

    if (structure == globalObject->promiseStructure())
        return true;

    if (structure->classInfoForCells() == JSInternalPromise::info())
        return true;

    ASSERT(globalObject->promiseSpeciesWatchpointSet().state() != ClearWatchpoint);
    auto* promisePrototype = globalObject->promisePrototype();
    if (promisePrototype != structure->storedPrototype(thisObject))
        return false;

    if (!thisObject->hasCustomProperties())
        return true;

    return thisObject->getDirectOffset(vm, vm.propertyNames->constructor) == invalidOffset;
}

JSC_DEFINE_HOST_FUNCTION(promiseProtoFuncThen, (JSGlobalObject* globalObject, CallFrame* callFrame))
{
    VM& vm = globalObject->vm();
    auto scope = DECLARE_THROW_SCOPE(vm);

    JSValue thisValue = callFrame->thisValue().toThis(globalObject, ECMAMode::strict());

    JSValue onFulfilled = callFrame->argument(0);
    JSValue onRejected = callFrame->argument(1);

    auto* promise = jsDynamicCast<JSPromise*>(thisValue);
    if (!promise) [[unlikely]]
        return throwVMTypeError(globalObject, scope, "|this| is not a Promise");

    RELEASE_AND_RETURN(scope, JSValue::encode(promise->then(globalObject, onFulfilled, onRejected)));
}

JSC_DEFINE_HOST_FUNCTION(promiseProtoFuncCatch, (JSGlobalObject* globalObject, CallFrame* callFrame))
{
    VM& vm = globalObject->vm();
    auto scope = DECLARE_THROW_SCOPE(vm);

    JSValue thisValue = callFrame->thisValue().toThis(globalObject, ECMAMode::strict());
    JSValue onRejected = callFrame->argument(0);

    if (auto* promise = jsDynamicCast<JSPromise*>(thisValue); promise && promise->isThenFastAndNonObservable()) [[likely]]
        RELEASE_AND_RETURN(scope, JSValue::encode(promise->then(globalObject, jsUndefined(), onRejected)));

    JSValue then = thisValue.get(globalObject, vm.propertyNames->then);
    RETURN_IF_EXCEPTION(scope, { });

    auto thenCallData = getCallDataInline(then);
    if (thenCallData.type == CallData::Type::None) [[unlikely]]
        return throwVMTypeError(globalObject, scope, "|this|.then is not a function"_s);
    MarkedArgumentBuffer thenArguments;
    thenArguments.append(jsUndefined());
    thenArguments.append(onRejected);
    ASSERT(!thenArguments.hasOverflowed());
    RELEASE_AND_RETURN(scope, JSValue::encode(call(globalObject, then, thenCallData, thisValue, thenArguments)));
}

} // namespace JSC

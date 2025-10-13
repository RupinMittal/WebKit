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
#include "JSPromiseConstructor.h"

#include "BuiltinNames.h"
#include "GetterSetter.h"
#include "JSCBuiltins.h"
#include "JSCInlines.h"
#include "JSPromise.h"
#include "JSPromisePrototype.h"

namespace JSC {

STATIC_ASSERT_IS_TRIVIALLY_DESTRUCTIBLE(JSPromiseConstructor);
static JSC_DECLARE_HOST_FUNCTION(promiseConstructorFuncResolve);
static JSC_DECLARE_HOST_FUNCTION(promiseConstructorFuncReject);
static JSC_DECLARE_HOST_FUNCTION(promiseConstructorFuncWithResolvers);

}

#include "JSPromiseConstructor.lut.h"

namespace JSC {

const ClassInfo JSPromiseConstructor::s_info = { "Function"_s, &Base::s_info, &promiseConstructorTable, nullptr, CREATE_METHOD_TABLE(JSPromiseConstructor) };

/* Source for JSPromiseConstructor.lut.h
@begin promiseConstructorTable
  resolve         promiseConstructorFuncResolve        DontEnum|Function 1 PromiseConstructorResolveIntrinsic
  reject          promiseConstructorFuncReject         DontEnum|Function 1 PromiseConstructorRejectIntrinsic
  race            JSBuiltin                            DontEnum|Function 1
  all             JSBuiltin                            DontEnum|Function 1
  allSettled      JSBuiltin                            DontEnum|Function 1
  any             JSBuiltin                            DontEnum|Function 1
  withResolvers   promiseConstructorFuncWithResolvers  DontEnum|Function 0
@end
*/

JSPromiseConstructor* JSPromiseConstructor::create(VM& vm, Structure* structure, JSPromisePrototype* promisePrototype)
{
    JSGlobalObject* globalObject = structure->globalObject();
    FunctionExecutable* executable = promiseConstructorPromiseConstructorCodeGenerator(vm);
    JSPromiseConstructor* constructor = new (NotNull, allocateCell<JSPromiseConstructor>(vm)) JSPromiseConstructor(vm, executable, globalObject, structure);
    constructor->finishCreation(vm, promisePrototype);
    return constructor;
}

Structure* JSPromiseConstructor::createStructure(VM& vm, JSGlobalObject* globalObject, JSValue prototype)
{
    return Structure::create(vm, globalObject, prototype, TypeInfo(JSFunctionType, StructureFlags), info());
}

JSPromiseConstructor::JSPromiseConstructor(VM& vm, FunctionExecutable* executable, JSGlobalObject* globalObject, Structure* structure)
    : Base(vm, executable, globalObject, structure)
{
}

void JSPromiseConstructor::finishCreation(VM& vm, JSPromisePrototype* promisePrototype)
{
    Base::finishCreation(vm);
    putDirectWithoutTransition(vm, vm.propertyNames->prototype, promisePrototype, PropertyAttribute::DontEnum | PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);
    putDirectWithoutTransition(vm, vm.propertyNames->length, jsNumber(1), PropertyAttribute::DontEnum | PropertyAttribute::ReadOnly);

    JSGlobalObject* globalObject = this->globalObject();

    putDirectNonIndexAccessorWithoutTransition(vm, vm.propertyNames->speciesSymbol, globalObject->promiseSpeciesGetterSetter(), PropertyAttribute::Accessor | PropertyAttribute::ReadOnly | PropertyAttribute::DontEnum);
    JSC_BUILTIN_FUNCTION_WITHOUT_TRANSITION(vm.propertyNames->tryKeyword, promiseConstructorTryCodeGenerator, static_cast<unsigned>(PropertyAttribute::DontEnum));
}

JSC_DEFINE_HOST_FUNCTION(promiseConstructorFuncResolve, (JSGlobalObject* globalObject, CallFrame* callFrame))
{
    VM& vm = globalObject->vm();
    auto scope = DECLARE_THROW_SCOPE(vm);

    JSValue thisValue = callFrame->thisValue().toThis(globalObject, ECMAMode::strict());
    JSValue argument = callFrame->argument(0);

    if (!thisValue.isObject()) [[unlikely]]
        return throwVMTypeError(globalObject, scope, "|this| is not an object"_s);

    RELEASE_AND_RETURN(scope, JSValue::encode(JSPromise::promiseResolve(globalObject, asObject(thisValue), argument)));
}

JSC_DEFINE_HOST_FUNCTION(promiseConstructorFuncReject, (JSGlobalObject* globalObject, CallFrame* callFrame))
{
    VM& vm = globalObject->vm();
    auto scope = DECLARE_THROW_SCOPE(vm);

    JSValue thisValue = callFrame->thisValue().toThis(globalObject, ECMAMode::strict());
    JSValue argument = callFrame->argument(0);

    if (!thisValue.isObject()) [[unlikely]]
        return throwVMTypeError(globalObject, scope, "|this| is not an object"_s);

    RELEASE_AND_RETURN(scope, JSValue::encode(JSPromise::promiseReject(globalObject, asObject(thisValue), argument)));
}

JSC_DEFINE_HOST_FUNCTION(promiseConstructorFuncWithResolvers, (JSGlobalObject* globalObject, CallFrame* callFrame))
{
    JSValue thisValue = callFrame->thisValue().toThis(globalObject, ECMAMode::strict());
    return JSValue::encode(JSPromise::createNewPromiseCapability(globalObject, thisValue));
}

} // namespace JSC

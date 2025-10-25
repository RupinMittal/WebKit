/*
 * Copyright (C) 2025 Apple Inc. All rights reserved.
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
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(REMOTE_INSPECTOR) && ENABLE(WEBASSEMBLY)

#include "MessageReceiver.h"
#include <wtf/CheckedRef.h>
#include <wtf/Ref.h>
#include <wtf/text/WTFString.h>

namespace WTF {
class WorkQueue;
}

namespace WebKit {

class WebProcess;

class WasmDebuggerDispatcher final : private IPC::MessageReceiver {
public:
    explicit WasmDebuggerDispatcher(WebProcess&);
    ~WasmDebuggerDispatcher();

    void initializeConnection(IPC::Connection&);

    void ref() const final;
    void deref() const final;

private:
    // IPC::MessageReceiver overrides.
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    // Message handlers - called on WorkQueue thread, safe to call while mutator is blocked.
    void dispatchMessage(const String& message);

    const CheckedRef<WebProcess> m_process;
    const Ref<WTF::WorkQueue> m_queue;
};

} // namespace WebKit

#endif // ENABLE(REMOTE_INSPECTOR) && ENABLE(WEBASSEMBLY)

/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#pragma once

#include <wtf/Forward.h>
#include <wtf/TZoneMallocInlines.h>

namespace WebCore {
enum DiagnosticLoggingResultType : uint8_t;
enum class DiagnosticLoggingDomain : uint8_t;
}

namespace WebKit {
class WebPageProxy;
}

namespace API {

class Dictionary;

class DiagnosticLoggingClient {
    WTF_MAKE_TZONE_ALLOCATED_INLINE(DiagnosticLoggingClient);
public:
    virtual ~DiagnosticLoggingClient() { }

    virtual void logDiagnosticMessage(WebKit::WebPageProxy*, const WTF::String& message, const WTF::String& description) = 0;
    virtual void logDiagnosticMessageWithResult(WebKit::WebPageProxy*, const WTF::String& message, const WTF::String& description, WebCore::DiagnosticLoggingResultType) = 0;
    virtual void logDiagnosticMessageWithValue(WebKit::WebPageProxy*, const WTF::String& message, const WTF::String& description, const WTF::String& value) = 0;
    virtual void logDiagnosticMessageWithEnhancedPrivacy(WebKit::WebPageProxy*, const WTF::String& message, const WTF::String& description) = 0;

    virtual void logDiagnosticMessageWithValueDictionary(WebKit::WebPageProxy*, const WTF::String& message, const WTF::String& description, Ref<API::Dictionary>&&) = 0;

    virtual void logDiagnosticMessageWithDomain(WebKit::WebPageProxy*, const WTF::String&, WebCore::DiagnosticLoggingDomain) { }

    virtual bool isWebKitDiagnosticLoggingClient() const { return false; }
};

} // namespace API

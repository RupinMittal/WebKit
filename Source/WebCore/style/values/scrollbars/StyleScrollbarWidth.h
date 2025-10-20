/*
 * Copyright (C) 2025 Samuel Weinig <sam@webkit.org>
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

#include <WebCore/ScrollTypes.h>
#include <WebCore/StyleValueTypes.h>

namespace WebCore {
namespace Style {

// <'scrollbar-width'> = auto | thin | none
// https://drafts.csswg.org/css-scrollbars/#propdef-scrollbar-width
struct ScrollbarWidth {
    constexpr ScrollbarWidth(CSS::Keyword::Auto) : value { WebCore::ScrollbarWidth::Auto } { }
    constexpr ScrollbarWidth(CSS::Keyword::Thin) : value { WebCore::ScrollbarWidth::Thin } { }
    constexpr ScrollbarWidth(CSS::Keyword::None) : value { WebCore::ScrollbarWidth::None } { }

    constexpr ScrollbarWidth(WebCore::ScrollbarWidth platform) : value { platform } { }
    constexpr WebCore::ScrollbarWidth platform() const { return value; }

    constexpr bool isAuto() const { return value == WebCore::ScrollbarWidth::Auto; }
    constexpr bool isThin() const { return value == WebCore::ScrollbarWidth::Thin; }
    constexpr bool isNone() const { return value == WebCore::ScrollbarWidth::None; }

    constexpr bool operator==(const ScrollbarWidth&) const = default;

    WebCore::ScrollbarWidth value;
};
DEFINE_TYPE_WRAPPER_GET(ScrollbarWidth, value);

// MARK: - Conversion

// `ScrollbarWidth` is special-cased to apply `needsScrollbarWidthThinDisabledQuirk` quirk.
template<> struct CSSValueConversion<ScrollbarWidth> { auto operator()(BuilderState&, const CSSValue&) -> ScrollbarWidth; };

} // namespace Style
} // namespace WebCore

DEFINE_TUPLE_LIKE_CONFORMANCE_FOR_TYPE_WRAPPER(WebCore::Style::ScrollbarWidth);

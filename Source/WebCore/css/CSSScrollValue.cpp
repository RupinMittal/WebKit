/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CSSScrollValue.h"

#include "CSSPrimitiveValueMappings.h"
#include <wtf/text/MakeString.h>

namespace WebCore {

String CSSScrollValue::customCSSText(const CSS::SerializationContext& context) const
{
    auto hasScroller = m_scroller && m_scroller->valueID() != CSSValueNearest;
    auto hasAxis = m_axis && m_axis->valueID() != CSSValueBlock;

    return makeString(
        "scroll("_s,
        hasScroller ? m_scroller->cssText(context) : ""_s,
        hasScroller && hasAxis ? " "_s : ""_s,
        hasAxis ? m_axis->cssText(context) : ""_s,
        ")"_s
    );
}

bool CSSScrollValue::equals(const CSSScrollValue& other) const
{
    return compareCSSValuePtr(m_scroller, other.m_scroller)
        && compareCSSValuePtr(m_axis, other.m_axis);
}

}

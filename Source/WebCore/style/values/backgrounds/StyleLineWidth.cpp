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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "StyleLineWidth.h"

#include "CSSPrimitiveValue.h"
#include "RenderStyleInlines.h"
#include "StyleBuilderChecking.h"
#include "StylePrimitiveNumericTypes+CSSValueConversion.h"
#include "StylePrimitiveNumericTypes+Evaluation.h"

namespace WebCore {
namespace Style {

// MARK: - Conversion

auto CSSValueConversion<LineWidth>::operator()(BuilderState& state, const CSSValue& value) -> LineWidth
{
    RefPtr primitiveValue = requiredDowncast<CSSPrimitiveValue>(state, value);
    if (!primitiveValue)
        return CSS::Keyword::Medium { };

    if (primitiveValue->isValueID()) {
        switch (primitiveValue->valueID()) {
        case CSSValueThin:
            return CSS::Keyword::Thin { };
        case CSSValueMedium:
            return CSS::Keyword::Medium { };
        case CSSValueThick:
            return CSS::Keyword::Thick { };
        default:
            state.setCurrentPropertyInvalidAtComputedValueTime();
            return CSS::Keyword::Medium { };
        }
    }

    if (evaluationTimeZoomEnabled(state))
        return toStyleFromCSSValue<LineWidth::Length>(state, value);

    // Any original result that was >= 1 should not be allowed to fall below 1. This keeps border lines from vanishing.

    auto result = primitiveValue->resolveAsLength<float>(state.cssToLengthConversionData());
    if (state.style().usedZoom() < 1.0f && result < 1.0f) {
        auto originalLength = primitiveValue->resolveAsLength<float>(state.cssToLengthConversionData().copyWithAdjustedZoom(1.0));
        if (originalLength >= 1.0f)
            return CSS::Keyword::Thin { };
    }

    if (auto minimumLineWidth = 1.0f / state.document().deviceScaleFactor(); result > 0.0f && result < minimumLineWidth)
        return LineWidth::Length { minimumLineWidth };

    return LineWidth::Length { floorToDevicePixel(result, state.document().deviceScaleFactor()) };
}

// MARK: - Serialize

void Serialize<LineWidth>::operator()(StringBuilder& builder, const CSS::SerializationContext& context, const RenderStyle& style, const LineWidth& value)
{
    float result = value.value.unresolvedValue();
    auto deviceScaleFactor = style.deviceScaleFactor();

    if (auto minimumLineWidth = 1.0f / deviceScaleFactor; result > 0.0f && result < minimumLineWidth)
        result = minimumLineWidth;

    result = floorToDevicePixel(result, deviceScaleFactor);

    CSS::serializationForCSS(builder, context, CSS::LengthRaw<> { CSS::LengthUnit::Px, result });
}

// MARK: - Evaluate

auto Evaluation<LineWidthBox, FloatBoxExtent>::operator()(const LineWidthBox& value, ZoomFactor zoom) -> FloatBoxExtent
{
    return {
        evaluate<float>(value.top(), zoom),
        evaluate<float>(value.right(), zoom),
        evaluate<float>(value.bottom(), zoom),
        evaluate<float>(value.left(), zoom),
    };
}

auto Evaluation<LineWidthBox, LayoutBoxExtent>::operator()(const LineWidthBox& value, ZoomFactor zoom) -> LayoutBoxExtent
{
    return {
        evaluate<LayoutUnit>(value.top(), zoom),
        evaluate<LayoutUnit>(value.right(), zoom),
        evaluate<LayoutUnit>(value.bottom(), zoom),
        evaluate<LayoutUnit>(value.left(), zoom),
    };
}

} // namespace Style
} // namespace WebCore

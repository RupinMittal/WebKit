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
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(THREADED_ANIMATION_RESOLUTION)

#include "RemoteAnimationTimeline.h"
#include <WebCore/AcceleratedEffect.h>
#include <WebCore/WebAnimationTime.h>
#include <wtf/RefCounted.h>
#include <wtf/TZoneMalloc.h>

namespace WebKit {

class RemoteAnimation : public RefCounted<RemoteAnimation> {
    WTF_MAKE_TZONE_ALLOCATED(RemoteAnimation);
public:
    static Ref<RemoteAnimation> create(const WebCore::AcceleratedEffect&, const RemoteAnimationTimeline&);

    const RemoteAnimationTimeline& timeline() const { return m_timeline.get(); }

    const OptionSet<WebCore::AcceleratedEffectProperty>& animatedProperties() const { return m_effect->animatedProperties(); }
    const Vector<WebCore::AcceleratedEffect::Keyframe>& keyframes() const { return m_effect->keyframes(); }

    void apply(WebCore::AcceleratedEffectValues&);

private:
    RemoteAnimation(const WebCore::AcceleratedEffect&, const RemoteAnimationTimeline&);

    Ref<const WebCore::AcceleratedEffect> m_effect;
    Ref<const RemoteAnimationTimeline> m_timeline;
};

} // namespace WebKit

#endif // ENABLE(THREADED_ANIMATION_RESOLUTION)

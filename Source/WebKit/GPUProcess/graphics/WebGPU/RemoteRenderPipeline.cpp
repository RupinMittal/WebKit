/*
 * Copyright (C) 2021-2023 Apple Inc. All rights reserved.
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
#include "RemoteRenderPipeline.h"

#if ENABLE(GPU_PROCESS)

#include "RemoteBindGroupLayout.h"
#include "RemoteRenderPipelineMessages.h"
#include "StreamServerConnection.h"
#include "WebGPUObjectHeap.h"
#include <WebCore/WebGPUBindGroupLayout.h>
#include <WebCore/WebGPURenderPipeline.h>
#include <wtf/TZoneMallocInlines.h>

namespace WebKit {

WTF_MAKE_TZONE_ALLOCATED_IMPL(RemoteRenderPipeline);

RemoteRenderPipeline::RemoteRenderPipeline(WebCore::WebGPU::RenderPipeline& renderPipeline, WebGPU::ObjectHeap& objectHeap, Ref<IPC::StreamServerConnection>&& streamConnection, RemoteGPU& gpu, WebGPUIdentifier identifier)
    : m_backing(renderPipeline)
    , m_objectHeap(objectHeap)
    , m_streamConnection(WTFMove(streamConnection))
    , m_gpu(gpu)
    , m_identifier(identifier)
{
    protectedStreamConnection()->startReceivingMessages(*this, Messages::RemoteRenderPipeline::messageReceiverName(), m_identifier.toUInt64());
}

RemoteRenderPipeline::~RemoteRenderPipeline() = default;

void RemoteRenderPipeline::destruct()
{
    Ref { m_objectHeap.get() }->removeObject(m_identifier);
}

void RemoteRenderPipeline::stopListeningForIPC()
{
    protectedStreamConnection()->stopReceivingMessages(Messages::RemoteRenderPipeline::messageReceiverName(), m_identifier.toUInt64());
}

void RemoteRenderPipeline::getBindGroupLayout(uint32_t index, WebGPUIdentifier identifier)
{
    // "A new GPUBindGroupLayout wrapper is returned each time"
    auto bindGroupLayout = protectedBacking()->getBindGroupLayout(index);
    Ref objectHeap = m_objectHeap.get();
    auto remoteBindGroupLayout = RemoteBindGroupLayout::create(bindGroupLayout, objectHeap, m_streamConnection.copyRef(), protectedGPU(), identifier);
    objectHeap->addObject(identifier, remoteBindGroupLayout);
}

void RemoteRenderPipeline::setLabel(String&& label)
{
    protectedBacking()->setLabel(WTFMove(label));
}

Ref<WebCore::WebGPU::RenderPipeline> RemoteRenderPipeline::protectedBacking()
{
    return m_backing;
}

Ref<IPC::StreamServerConnection> RemoteRenderPipeline::protectedStreamConnection() const
{
    return m_streamConnection;
}

} // namespace WebKit

#endif // ENABLE(GPU_PROCESS)

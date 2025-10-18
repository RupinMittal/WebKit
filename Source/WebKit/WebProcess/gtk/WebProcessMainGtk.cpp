/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2010 Motorola Mobility, Inc. All rights reserved.
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
#include "WebProcessMain.h"

#include "AuxiliaryProcessMain.h"
#include "WebProcess.h"
#include <libintl.h>

#if !USE(GTK4) && USE(CAIRO)
#include <gtk/gtk.h>
#endif

#if USE(GSTREAMER)
#include <WebCore/GStreamerCommon.h>
#endif

#if USE(GCRYPT)
#include <pal/crypto/gcrypt/Initialization.h>
#endif

#if USE(SKIA)
WTF_IGNORE_WARNINGS_IN_THIRD_PARTY_CODE_BEGIN
#include <skia/core/SkGraphics.h>
#if USE(SKIA_OPENTYPE_SVG)
#include <skia/modules/svg/SkSVGOpenTypeSVGDecoder.h>
#endif
WTF_IGNORE_WARNINGS_IN_THIRD_PARTY_CODE_END
#endif

#if USE(SYSPROF_CAPTURE)
#include <wtf/SystemTracing.h>
#endif

namespace WebKit {
using namespace WebCore;

class WebProcessMainGtk final: public AuxiliaryProcessMainBase<WebProcess> {
public:
    bool platformInitialize() override
    {
#if USE(SYSPROF_CAPTURE)
        SysprofAnnotator::createIfNeeded("WebKit (Web)"_s);
#endif

#if USE(GCRYPT)
        PAL::GCrypt::initialize();
#endif

#if USE(SKIA)
        SkGraphics::Init();
#if USE(SKIA_OPENTYPE_SVG)
        SkGraphics::SetOpenTypeSVGDecoderFactory(SkSVGOpenTypeSVGDecoder::Make);
#endif
#endif

#if ENABLE(DEVELOPER_MODE)
        if (g_getenv("WEBKIT2_PAUSE_WEB_PROCESS_ON_LAUNCH"))
            g_usleep(30 * G_USEC_PER_SEC);
#endif

#if !USE(GTK4) && USE(CAIRO)
        gtk_init(nullptr, nullptr);
#endif

        bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
        bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");

        return true;
    }

    void platformFinalize() override
    {
#if USE(GSTREAMER)
        deinitializeGStreamer();
#endif
    }
};

int WebProcessMain(int argc, char** argv)
{
#if !USE(GTK4) && USE(CAIRO)
#if USE(ATSPI)
    // Disable ATK accessibility support in the WebProcess.
    g_setenv("NO_AT_BRIDGE", "1", TRUE);
#endif

    // Ignore the GTK_THEME environment variable, the theme is always set by the UI process now.
    // This call needs to happen before any threads begin execution
    unsetenv("GTK_THEME");
#endif

    return AuxiliaryProcessMain<WebProcessMainGtk>(argc, argv);
}

} // namespace WebKit

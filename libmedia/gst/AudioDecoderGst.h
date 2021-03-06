// AudioDecoderGst.h: Audio decoding using Gstreamer.
// 
//   Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012
//   Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef GNASH_AUDIODECODERGST_H
#define GNASH_AUDIODECODERGST_H

#include "log.h"
#include "AudioDecoder.h"
#include "HostInterface.h"

#include <gst/gst.h>
#include "GnashImage.h"

#include "swfdec_codec_gst.h"

// Forward declarations
namespace gnash {
    namespace media {
        class AudioInfo;
        class SoundInfo;
    }
}

namespace gnash {
namespace media {
namespace gst {

/// GST based AudioDecoder
class DSOEXPORT AudioDecoderGst : public AudioDecoder {
	
public:
    AudioDecoderGst(const AudioInfo& info, std::shared_ptr<HostInterface> handler);
    AudioDecoderGst(SoundInfo& info, std::shared_ptr<HostInterface> handler);

    ~AudioDecoderGst();

    std::uint8_t* decode(const std::uint8_t* input, std::uint32_t inputSize,
                           std::uint32_t& outputSize, std::uint32_t& decodedData);
    std::uint8_t* decode(const EncodedAudioFrame& ef, std::uint32_t& outputSize);

private:

    std::shared_ptr<HostInterface> _eventHandler;

    std::uint8_t* pullBuffers(std::uint32_t&  outputSize);

    void setup(GstCaps* caps);

    SwfdecGstDecoder _decoder;

};

} // gnash.media.gst namespace
} // media namespace
} // gnash namespace

#endif // __AUDIODECODERGST_H__


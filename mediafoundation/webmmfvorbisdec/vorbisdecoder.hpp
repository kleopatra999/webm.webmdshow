#ifndef _MEDIAFOUNDATION_WEBMMFVORBISDEC_VORBISDECODER_HPP_
#define _MEDIAFOUNDATION_WEBMMFVORBISDEC_VORBISDECODER_HPP_

#include "vorbis/codec.h"

namespace WebmMfVorbisDecLib
{

const UINT32 VORBIS_SETUP_HEADER_COUNT = 3;

class VorbisDecoder
{
public:
    VorbisDecoder();
    ~VorbisDecoder();
    int CreateDecoder(const BYTE** const ptr_headers,
                      const DWORD* const header_lengths,
                      unsigned int num_headers /* must be == 3 */);
    void DestroyDecoder();

    int GetWaveFormat(WAVEFORMATEX* ptr_out_wave_format);

    int Decode(BYTE* ptr_samples, UINT32 length);

    int ConsumeOutputSamples(BYTE* ptr_output_buffer,
                             UINT32 buffer_limit_in_bytes,
                             UINT32* ptr_output_bytes_written,
                             UINT32* ptr_output_sample_count);
    void Flush();

    int GetVorbisRate() {
        return m_vorbis_info.rate;
    };
    int GetVorbisChannels() {
        return m_vorbis_info.channels;
    };

    int GetOutputWaveFormat(int* ptr_format_tag, int* ptr_bits_per_sample);
    int SetOutputWaveFormat(int format_tag, int bits_per_sample);

private:
    int NextOggPacket_(const BYTE* ptr_packet, DWORD packet_size);
    void SetDecodedWaveFormat_();

    int ConvertOutputSamplesS16_(BYTE* ptr_output_buffer,
                                 UINT32 buffer_limit_in_bytes,
                                 UINT32* ptr_output_bytes_written,
                                 UINT32* ptr_output_sample_count);

    ogg_packet m_ogg_packet;
    DWORD m_ogg_packet_count;

    vorbis_info m_vorbis_info; // contains static bitstream settings
    vorbis_comment m_vorbis_comment; // contains user comments
    vorbis_dsp_state m_vorbis_state; // decoder state
    vorbis_block m_vorbis_block; // working space for packet->PCM decode

    int m_output_format_tag;
    int m_output_bits_per_sample;

    WAVEFORMATEX m_wave_format;


    typedef std::vector<float> pcm_samples_t;
    pcm_samples_t m_output_samples;

    // disallow copy and assign
    VorbisDecoder(const VorbisDecoder&);
    void operator=(const VorbisDecoder&);
};

} // namespace WebmMfVorbisDecLib

#endif // _MEDIAFOUNDATION_WEBMMFVORBISDEC_VORBISDECODER_HPP_
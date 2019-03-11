#pragma once

#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

namespace pte
{
    struct video_profile
    {
        uint16_t width = 0;
        uint16_t height = 0;
        uint32_t bitrate_offset = 0;
    };

    class engine
    {
    private:
        std::vector<video_profile> m_profiles;

    public:
        engine();

        std::vector<video_profile> get_video_profiles(const char* path);
        video_profile get_video_profile(const char* path);
    };
}

std::ostream& operator<<(std::ostream& os, const pte::video_profile& profile);

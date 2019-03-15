#pragma once

#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <QProcess>

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
        std::string name = "main";
        uint32_t threshold = 0;
    };

    class engine
    {
    public:
        std::vector<video_profile> m_profiles;
        QProcess* m_encoding;
        QProcess* m_scaling;
        QProcess* m_measuring;

        engine();

        std::vector<video_profile> get_video_profiles(const char* path);
        video_profile get_video_profile(const char* path);
        void get_psnr_ssim(const char* diff, const char* ref, double& psnr, double& ssim);
        std::string encode_video(const char* ref,
                                 const video_profile& profile_dif,
                                 const uint32_t bitrate);
    };
}

std::ostream& operator<<(std::ostream& os, const pte::video_profile& profile);

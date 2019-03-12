#include "pte_engine.h"

namespace pte
{
    engine::engine()
    {
        m_profiles =
        {
            {3840,2160,7000},
            {2560,1440,3000},
            {1920,1080,1000},
            {1280,720,500},
            {960,540,300},
            {640,360,100},
            {480,270,100}
        };
    }

    std::vector<video_profile> engine::get_video_profiles(const char* path)
    {   
        std::vector<video_profile> profiles;
        video_profile ref_profile = get_video_profile(path);

        size_t profile_count = m_profiles.size();

        size_t ind = 0;
        for(size_t i=0; i<profile_count; i++)
        {
            if(ref_profile.height == m_profiles[i].height)
            {
                ind = i;
            }
        }

        for(size_t i = ind; i<profile_count; i++)
        {
            profiles.push_back(m_profiles[i]);
        }

        return profiles;
    }

    video_profile engine::get_video_profile(const char* path)
    {
        video_profile profile;
        int err = 0;
        av_register_all();

        AVFormatContext* format_context = avformat_alloc_context();

        if(!format_context)
        {
            std::cerr << "Format context is null" << std::endl;
            return video_profile();
        }

        if(err = avformat_open_input(&format_context,path,nullptr,nullptr))
        {
            char buff[256];
            av_make_error_string(buff, sizeof(buff), err);
            std::cerr << buff << std::endl;
            return video_profile();
        }

        AVCodec* cdc = nullptr;
        int stream = 0;
        stream = av_find_best_stream(format_context,AVMEDIA_TYPE_VIDEO,-1,-1,&cdc,0);
        AVStream* video_stream = format_context->streams[err];
        profile.width = video_stream->codecpar->width;
        profile.height = video_stream->codecpar->height;

        avformat_close_input(&format_context);
        avformat_free_context(format_context);

        return profile;
    }
}

std::ostream& operator<<(std::ostream& os, const pte::video_profile& profile)
{
    std::cout << "width=" << profile.width << '\n' << "height=" << profile.height << '\n';
    return os;
}

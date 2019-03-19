#include "pte_engine.h"

namespace pte
{
    engine::engine()
    {
        m_profiles =
        {
            {3840,2160,10000,"main10",2000},
            {2560,1440,3000,"main10",1000},
            {1920,1080,1000,"main10",1000},
            {1280,720,500,"main",500},
            {960,540,300,"main",300},
            {640,360,100,"main",200},
            {480,270,100,"main",100}
        };

        m_measuring = new QProcess();
        m_scaling = new QProcess();
        m_encoding = new QProcess();
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
        profile.width = static_cast<uint16_t>(video_stream->codecpar->width);
        profile.height = static_cast<uint16_t>(video_stream->codecpar->height);

        avformat_close_input(&format_context);
        avformat_free_context(format_context);

        return profile;
    }

    void engine::get_psnr_ssim(const char *diff, const char *ref, double& psnr, double& ssim)
    {
        std::stringstream cmd;
        cmd << "ffmpeg -i " << diff << " -i " << ref << " -lavfi \"ssim;[0:v][1:v]psnr\" -f null –";
        std::cout << "MESURE:\n" << cmd.str() << "\n\n\n" << std::endl;

        m_measuring->start(cmd.str().c_str());
        m_measuring->waitForFinished(-1);
        QString output(m_measuring->readAllStandardError());
        m_measuring->close();
        std::string str(output.toStdString());

        ssim = QString(str.substr(str.find("All:")+4,8).c_str()).toDouble();
        psnr = QString(str.substr(str.find("average:")+8,8).c_str()).toDouble();

        std::cout << str << std::endl;
        remove(diff);
    }

    std::string engine::encode_video(const char *ref,
                                     const video_profile& profile_dif,
                                     const uint32_t bitrate)
    {
        std::string str_ref = ref;
        size_t ind=str_ref.size()-1;
        std::string ref_name;
        while(str_ref[ind] != '/' )
        {
            ref_name.push_back(str_ref[ind]);
            ind--;
        }
        std::reverse(ref_name.begin(), ref_name.end());
        std::stringstream ss;
        ss << profile_dif.width << 'x' << profile_dif.height << "_br" << bitrate << "_" << ref_name;
        std::string encoded_name = ss.str().c_str();
        std::string path_to_encoded_video = str_ref.substr(0,ind+1).append(encoded_name);
        std::string path_to_encoded_video_not_scaled = str_ref.substr(0,ind+1).append("S").append(encoded_name);
        path_to_encoded_video_not_scaled = path_to_encoded_video_not_scaled.substr(0, path_to_encoded_video_not_scaled.size()-3).append("mp4");

        std::stringstream cmd;
        cmd << "ffmpeg -y -i " << ref << " -an -c:v libx265 -b:v "
            << bitrate << " -minrate " << bitrate << " -maxrate "
            << bitrate << " -bufsize " << bitrate << " -profile:v -x265-params --strict-cbr ";
        if(profile_dif.name == "main")
            cmd << "main -g " << 100 << " -r 25 "<< " -pix_fmt yuv420p ";
        else
            cmd << "main10 -g " << 100 << " -r 25 "<<  " -pix_fmt yuv420p10le ";
        cmd << "-s " << profile_dif.width <<'x' << profile_dif.height << ' ' << path_to_encoded_video_not_scaled;
        std::cout <<"ENCODAGE:\n" <<cmd.str() << "\n\n\n" << std::endl;

        m_encoding->start(cmd.str().c_str());
        m_encoding->waitForFinished(-1);
        m_encoding->close();

        cmd.clear();
        cmd.str("");

        ///////////////////////////////////////////////////////////////////////////////////////////////

        path_to_encoded_video = path_to_encoded_video.substr(0, path_to_encoded_video.size()-3).append("mp4");

        video_profile profile_ref = get_video_profile(ref);
        if(profile_dif.height != profile_ref.height)
        {
            cmd << "ffmpeg -y -i " << path_to_encoded_video_not_scaled << " -s "
            << profile_ref.width <<'x' << profile_ref.height << ' ' << path_to_encoded_video;
            std::cout <<"RESCALLAGE:\n" <<cmd.str() << "\n\n\n" << std::endl;
            m_scaling->start(cmd.str().c_str());
            m_scaling->waitForFinished(-1);
            m_scaling->close();
            remove(path_to_encoded_video_not_scaled.c_str());
        }
        else
        {
            path_to_encoded_video = path_to_encoded_video_not_scaled;
        }

        return path_to_encoded_video;
    }

    void engine::remove_all_encoded_file_from_folder(const char* path_to_folder,
                                                     const char* path_to_ref)
    {
        std::string name_of_ref = get_file_name(path_to_ref);
        //Delete the files which contains the reference name
        DIR* dir;
        struct dirent* dirp;
        if((dir = opendir(path_to_folder)) != nullptr)
        {
            while((dirp = readdir(dir)) != nullptr)
            {
                std::string file_name = dirp->d_name;
                std::string file_name_path = std::string(path_to_folder).append("/").append(file_name);

                if(file_name.find(std::string("_").append(name_of_ref).c_str()) != std::string::npos)
                {
                    remove(file_name_path.c_str());
                    std::clog << "Deleting:" << file_name << std::endl;
                }
            }
        }

        else
        {
            std::cerr << "Cannot open dir" << std::endl;
            return;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const pte::video_profile& profile)
{
    std::cout << "width=" << profile.width << '\n' << "height=" << profile.height << '\n';
    return os;
}

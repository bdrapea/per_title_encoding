#include "pte_video_profile.h"

namespace pte
{
    video_profile get_video_profile(const char* path_to_video)
    {
        video_profile vp;
        std::stringstream ss;
        std::stringbuf sbuf(std::ios::out);
        auto* olbuf = std::cout.rdbuf(&sbuf);

        //Starting ffprobe
        ss << "ffprobe " << path_to_video;
        system(ss.str().c_str());

        //Read values in buffer


        return vp;
    }
}

std::ostream& operator<<(std::ostream& os, const pte::video_profile& vp)
{
    os << "Resolution: " << vp.width << 'x' << vp.height;
    return os;
}

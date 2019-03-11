#pragma once

#include <iostream>
#include <sstream>
#include <cctype>

namespace pte
{
    struct video_profile
    {
        int width;
        int height;
    };

    video_profile get_video_profile(const char* path_to_video);
}

std::ostream& operator<<(std::ostream& os, const pte::video_profile& vp);

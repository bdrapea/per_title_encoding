#include "pte_video_profile.h"

int main()
{
    pte::video_profile vp = pte::get_video_profile("/home/bdrapeaud/Bureau/SampleVideo_1280x720_1mb.mp4");

    std::cout << vp << std::endl;
    return 0;
}

#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <lvgl.h>

class VideoPlayer {
public:
    static bool init();
    static void play(const char *path, lv_obj_t *parent);
};

#endif // VIDEO_PLAYER_H

#include "video_player.h"
#include <SD_MMC.h>
#include <Arduino.h>

static lv_obj_t *gif_obj = nullptr;
static uint8_t *gif_buf = nullptr;
static lv_img_dsc_t gif_dsc;

bool VideoPlayer::init() {
    if(!SD_MMC.begin()) {
        Serial.println("[Video] SD card mount failed");
        return false;
    }
    Serial.println("[Video] SD card mounted");
    return true;
}

void VideoPlayer::play(const char *path, lv_obj_t *parent) {
    if(!gif_obj) {
        gif_obj = lv_gif_create(parent);
        lv_obj_center(gif_obj);
    }

    File f = SD_MMC.open(path);
    if(!f) {
        Serial.println("[Video] Failed to open file");
        return;
    }
    size_t sz = f.size();
    if(gif_buf) {
        lv_mem_free(gif_buf);
        gif_buf = nullptr;
    }
    gif_buf = (uint8_t*)lv_mem_alloc(sz);
    if(!gif_buf) {
        Serial.println("[Video] malloc failed");
        f.close();
        return;
    }
    f.read(gif_buf, sz);
    f.close();

    gif_dsc.header.always_zero = 0;
    gif_dsc.header.w = 0;
    gif_dsc.header.h = 0;
    gif_dsc.header.cf = LV_IMG_CF_RAW;
    gif_dsc.data_size = sz;
    gif_dsc.data = gif_buf;

    lv_gif_set_src(gif_obj, &gif_dsc);
    lv_obj_center(gif_obj);
}

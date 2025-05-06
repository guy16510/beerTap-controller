SET(SOURCES ui_Loading.c
    ui_Home.c
    ui_WifiConfig.c
    ui_QRCode.c
    ui_BeerPouring.c
    ui_KegConfig.c
    ui_Configuration.c
    ui_WiFiStatus.c
    ui_videoAnimation.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_beertaplogo_png.c
    ui_img_beercan_png.c)

add_library(ui ${SOURCES})

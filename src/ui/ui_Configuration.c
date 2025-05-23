// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"

void ui_Configuration_screen_init(void)
{
ui_Configuration = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Configuration, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Panel7 = lv_obj_create(ui_Configuration);
lv_obj_set_height( ui_Panel7, 75);
lv_obj_set_width( ui_Panel7, lv_pct(100));
lv_obj_set_x( ui_Panel7, lv_pct(0) );
lv_obj_set_y( ui_Panel7, lv_pct(-45) );
lv_obj_set_align( ui_Panel7, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_Panel7, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Panel7, lv_color_hex(0x5E5F5F), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Panel7, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label25 = lv_label_create(ui_Panel7);
lv_obj_set_width( ui_Label25, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label25, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label25, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_Label25,"");
lv_obj_add_flag( ui_Label25, LV_OBJ_FLAG_CLICKABLE );   /// Flags
lv_obj_set_style_text_font(ui_Label25, &lv_font_montserrat_40, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_wifiLabelisConnected4 = lv_label_create(ui_Panel7);
lv_obj_set_width( ui_wifiLabelisConnected4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_wifiLabelisConnected4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_wifiLabelisConnected4, LV_ALIGN_TOP_RIGHT );
lv_obj_set_flex_flow(ui_wifiLabelisConnected4,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_wifiLabelisConnected4, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_label_set_text(ui_wifiLabelisConnected4,"");
lv_obj_set_style_text_font(ui_wifiLabelisConnected4, &lv_font_montserrat_40, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Button4 = lv_btn_create(ui_Configuration);
lv_obj_set_width( ui_Button4, 258);
lv_obj_set_height( ui_Button4, 84);
lv_obj_set_x( ui_Button4, -4 );
lv_obj_set_y( ui_Button4, -71 );
lv_obj_set_align( ui_Button4, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button4, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button4, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Label23 = lv_label_create(ui_Button4);
lv_obj_set_width( ui_Label23, lv_pct(100));
lv_obj_set_height( ui_Label23, lv_pct(100));
lv_obj_set_x( ui_Label23, lv_pct(10) );
lv_obj_set_y( ui_Label23, lv_pct(12) );
lv_obj_set_align( ui_Label23, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label23," Setup");
lv_obj_set_style_text_font(ui_Label23, &lv_font_montserrat_40, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Button6 = lv_btn_create(ui_Configuration);
lv_obj_set_width( ui_Button6, 270);
lv_obj_set_height( ui_Button6, 93);
lv_obj_set_x( ui_Button6, -3 );
lv_obj_set_y( ui_Button6, 52 );
lv_obj_set_align( ui_Button6, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button6, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button6, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Label26 = lv_label_create(ui_Button6);
lv_obj_set_width( ui_Label26, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label26, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label26, -2 );
lv_obj_set_y( ui_Label26, -1 );
lv_obj_set_align( ui_Label26, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label26,"Keg Setup");
lv_obj_set_style_text_font(ui_Label26, &lv_font_montserrat_40, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_screenBrightnessSlider = lv_slider_create(ui_Configuration);
lv_slider_set_range(ui_screenBrightnessSlider, 2,100);
lv_slider_set_value( ui_screenBrightnessSlider, 100, LV_ANIM_OFF);
if (lv_slider_get_mode(ui_screenBrightnessSlider)==LV_SLIDER_MODE_RANGE ) lv_slider_set_left_value( ui_screenBrightnessSlider, 0, LV_ANIM_OFF);
lv_obj_set_width( ui_screenBrightnessSlider, 172);
lv_obj_set_height( ui_screenBrightnessSlider, 18);
lv_obj_set_x( ui_screenBrightnessSlider, 113 );
lv_obj_set_y( ui_screenBrightnessSlider, -269 );
lv_obj_set_align( ui_screenBrightnessSlider, LV_ALIGN_CENTER );

ui_Label33 = lv_label_create(ui_Configuration);
lv_obj_set_width( ui_Label33, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label33, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label33, -152 );
lv_obj_set_y( ui_Label33, -269 );
lv_obj_set_align( ui_Label33, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label33,"Brightness:");
lv_obj_set_style_text_font(ui_Label33, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_screenBrightnessLabelValue = lv_label_create(ui_Configuration);
lv_obj_set_width( ui_screenBrightnessLabelValue, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_screenBrightnessLabelValue, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_screenBrightnessLabelValue, -34 );
lv_obj_set_y( ui_screenBrightnessLabelValue, -266 );
lv_obj_set_align( ui_screenBrightnessLabelValue, LV_ALIGN_CENTER );
lv_label_set_text(ui_screenBrightnessLabelValue,"100");

ui_WiFiStatusButton = lv_btn_create(ui_Configuration);
lv_obj_set_width( ui_WiFiStatusButton, 332);
lv_obj_set_height( ui_WiFiStatusButton, 50);
lv_obj_set_x( ui_WiFiStatusButton, -9 );
lv_obj_set_y( ui_WiFiStatusButton, 168 );
lv_obj_set_align( ui_WiFiStatusButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_WiFiStatusButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_WiFiStatusButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_WiFiStatusButtonLabel = lv_label_create(ui_WiFiStatusButton);
lv_obj_set_width( ui_WiFiStatusButtonLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_WiFiStatusButtonLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_WiFiStatusButtonLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_WiFiStatusButtonLabel,"WiFi Status");
lv_obj_set_style_text_font(ui_WiFiStatusButtonLabel, &lv_font_montserrat_40, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_Label25, ui_event_Label25, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_wifiLabelisConnected4, ui_event_wifiLabelisConnected4, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Button4, ui_event_Button4, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Button6, ui_event_Button6, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_screenBrightnessSlider, ui_event_screenBrightnessSlider, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_WiFiStatusButton, ui_event_WiFiStatusButton, LV_EVENT_ALL, NULL);

}

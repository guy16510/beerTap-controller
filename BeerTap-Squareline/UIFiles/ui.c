// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
void PouringAnimation_Animation( lv_obj_t *TargetObject, int delay);

// SCREEN: ui_Loading
void ui_Loading_screen_init(void);
lv_obj_t *ui_Loading;
void ui_event_Image1( lv_event_t * e);
lv_obj_t *ui_Image1;
// CUSTOM VARIABLES

// SCREEN: ui_Home
void ui_Home_screen_init(void);
lv_obj_t *ui_Home;
void ui_event_Button8( lv_event_t * e);
lv_obj_t *ui_Button8;
lv_obj_t *ui_Label17;
void ui_event_Button10( lv_event_t * e);
lv_obj_t *ui_Button10;
lv_obj_t *ui_Label19;
void ui_event_Button11( lv_event_t * e);
lv_obj_t *ui_Button11;
lv_obj_t *ui_Label20;
lv_obj_t *ui_Panel6;
void ui_event_Label14( lv_event_t * e);
lv_obj_t *ui_Label14;
void ui_event_Label18( lv_event_t * e);
lv_obj_t *ui_Label18;
lv_obj_t *ui_Label28;
lv_obj_t *ui_beerPouringValue;
lv_obj_t *ui_Label30;
lv_obj_t *ui_totalBeerPoured;
lv_obj_t *ui_Label29;
lv_obj_t *ui_zAxisValue;
lv_obj_t *ui_Label31;
lv_obj_t *ui_temperatureValue;
// CUSTOM VARIABLES

// SCREEN: ui_WifiConfig
void ui_WifiConfig_screen_init(void);
lv_obj_t *ui_WifiConfig;
lv_obj_t *ui_Label2;
lv_obj_t *ui_Panel1;
lv_obj_t *ui_Label4;
void ui_event_wifiNetworksAvailable( lv_event_t * e);
lv_obj_t *ui_wifiNetworksAvailable;
void ui_event_wifiName( lv_event_t * e);
lv_obj_t *ui_wifiName;
lv_obj_t *ui_Label5;
lv_obj_t *ui_Label6;
void ui_event_wifiPassword( lv_event_t * e);
lv_obj_t *ui_wifiPassword;
void ui_event_Button1( lv_event_t * e);
lv_obj_t *ui_Button1;
lv_obj_t *ui_Label9;
void ui_event_Button2( lv_event_t * e);
lv_obj_t *ui_Button2;
lv_obj_t *ui_Label10;
lv_obj_t *ui_wifiLabelisConnected;
lv_obj_t *ui_wifiKeyboard;
void ui_event_Button9( lv_event_t * e);
lv_obj_t *ui_Button9;
lv_obj_t *ui_Label32;
// CUSTOM VARIABLES

// SCREEN: ui_QRCode
void ui_QRCode_screen_init(void);
lv_obj_t *ui_QRCode;
lv_obj_t *ui_Label11;
lv_obj_t *ui_Label12;
lv_obj_t *ui_Image7;
void ui_event_Button5( lv_event_t * e);
lv_obj_t *ui_Button5;
lv_obj_t *ui_Label15;
// CUSTOM VARIABLES

// SCREEN: ui_BeerPouring
void ui_BeerPouring_screen_init(void);
lv_obj_t *ui_BeerPouring;
lv_obj_t *ui_pourImage1;
void ui_event_Button3( lv_event_t * e);
lv_obj_t *ui_Button3;
lv_obj_t *ui_Label13;
lv_obj_t *ui_Panel3;
lv_obj_t *ui_Panel4;
void ui_event_Label24( lv_event_t * e);
lv_obj_t *ui_Label24;
// CUSTOM VARIABLES

// SCREEN: ui_KegConfig
void ui_KegConfig_screen_init(void);
lv_obj_t *ui_KegConfig;
lv_obj_t *ui_Label3;
void ui_event_Dropdown1( lv_event_t * e);
lv_obj_t *ui_Dropdown1;
lv_obj_t *ui_Label7;
lv_obj_t *ui_Label8;
void ui_event_psiSlider( lv_event_t * e);
lv_obj_t *ui_psiSlider;
lv_obj_t *ui_psiValue;
lv_obj_t *ui_Panel2;
void ui_event_Label22( lv_event_t * e);
lv_obj_t *ui_Label22;
void ui_event_Button7( lv_event_t * e);
lv_obj_t *ui_Button7;
lv_obj_t *ui_Label27;
// CUSTOM VARIABLES

// SCREEN: ui_Configuration
void ui_Configuration_screen_init(void);
lv_obj_t *ui_Configuration;
lv_obj_t *ui_Panel7;
void ui_event_Label25( lv_event_t * e);
lv_obj_t *ui_Label25;
void ui_event_Label16( lv_event_t * e);
lv_obj_t *ui_Label16;
void ui_event_Button4( lv_event_t * e);
lv_obj_t *ui_Button4;
lv_obj_t *ui_Label23;
void ui_event_Button6( lv_event_t * e);
lv_obj_t *ui_Button6;
lv_obj_t *ui_Label26;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t *ui____initial_actions0;

// IMAGES AND IMAGE SETS
const lv_img_dsc_t *ui_imgset_image[3] = {&ui_img_image1_png, &ui_img_image2_png, &ui_img_image3_png};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
void PouringAnimation_Animation( lv_obj_t *TargetObject, int delay)
{
ui_anim_user_data_t *PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
PropertyAnimation_0_user_data->target = TargetObject;
PropertyAnimation_0_user_data->imgset = (lv_img_dsc_t **)ui_imgset_image;
PropertyAnimation_0_user_data->imgset_size = sizeof(ui_imgset_image)/(sizeof(lv_img_dsc_t*));
PropertyAnimation_0_user_data->val = -1;
lv_anim_t PropertyAnimation_0;
lv_anim_init(&PropertyAnimation_0);
lv_anim_set_time(&PropertyAnimation_0, 1000);
lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_frame );
lv_anim_set_values(&PropertyAnimation_0, 0, 3 );
lv_anim_set_path_cb( &PropertyAnimation_0, lv_anim_path_linear);
lv_anim_set_delay( &PropertyAnimation_0, delay + 0 );
lv_anim_set_deleted_cb( &PropertyAnimation_0, _ui_anim_callback_free_user_data );
lv_anim_set_playback_time(&PropertyAnimation_0, 0);
lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
 lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE );
lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
lv_anim_set_early_apply( &PropertyAnimation_0, true );
 lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_frame );
lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////
void ui_event_Image1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_Home, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Home_screen_init);
      _ui_screen_delete( &ui_Loading);
}
}

void ui_event_Button8( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_Loading, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Loading_screen_init);
      _ui_screen_delete( &ui_Home);
}
}

void ui_event_Button10( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_QRCode, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_QRCode_screen_init);
      _ui_screen_delete( &ui_Home);
}
}

void ui_event_Button11( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_BeerPouring, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_BeerPouring_screen_init);
      _ui_screen_delete( &ui_Home);
}
}

void ui_event_Label14( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_Configuration, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Configuration_screen_init);
      _ui_screen_delete( &ui_Home);
}
}

void ui_event_Label18( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_SCREEN_LOADED) {
      checkWifiStatusEvent( e );
}
}

void ui_event_wifiNetworksAvailable( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      selectedWifiSSID( e );
}
}

void ui_event_wifiName( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_CLICKED) {
      wifiName( e );
}
}

void ui_event_wifiPassword( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_flag_modify( ui_wifiKeyboard, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
}
}

void ui_event_Button1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      saveWifiConfiguration( e );
      _ui_screen_delete( &ui_WifiConfig);
}
}

void ui_event_Button2( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_Home, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Home_screen_init);
      _ui_screen_delete( &ui_WifiConfig);
}
}

void ui_event_Button9( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      deleteWiFiConfig( e );
}
}

void ui_event_Button5( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_Home, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Home_screen_init);
      _ui_screen_delete( &ui_QRCode);
}
}

void ui_event_Button3( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      PouringAnimation_Animation(ui_pourImage1, 0);
}
}

void ui_event_Label24( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_Home, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Home_screen_init);
      _ui_screen_delete( &ui_BeerPouring);
}
}

void ui_event_Dropdown1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      barrelSelected( e );
}
}

void ui_event_psiSlider( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);

if ( event_code == LV_EVENT_CLICKED) {
      _ui_slider_set_text_value( ui_psiValue, target, "", "");
}
}

void ui_event_Label22( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_Home, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Home_screen_init);
      _ui_screen_delete( &ui_KegConfig);
}
}

void ui_event_Button7( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      kegConfigSave( e );
}
}

void ui_event_Label25( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_Home, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Home_screen_init);
      _ui_screen_delete( &ui_KegConfig);
}
}

void ui_event_Label16( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_SCREEN_LOADED) {
      WifiStatus( e );
}
}

void ui_event_Button4( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_WifiConfig, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_WifiConfig_screen_init);
      _ui_screen_delete( &ui_Configuration);
}
}

void ui_event_Button6( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);

if ( event_code == LV_EVENT_RELEASED) {
      _ui_screen_change( &ui_KegConfig, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_KegConfig_screen_init);
      _ui_screen_delete( &ui_Configuration);
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_Loading_screen_init();
ui_Home_screen_init();
ui_WifiConfig_screen_init();
ui_QRCode_screen_init();
ui_BeerPouring_screen_init();
ui_KegConfig_screen_init();
ui_Configuration_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_Loading);
}

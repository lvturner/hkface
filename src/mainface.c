#include "mainface.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_bold_subset_49;
static GFont s_res_gothic_14;
static GFont s_res_gothic_18;
static GFont s_res_bitham_30_black;
static GBitmap *s_res_aqi_moderate;
static TextLayer *time_layer;
static TextLayer *date_layer;
static TextLayer *warning_layer;
static TextLayer *hdiv;
static TextLayer *temperature_layer;
static BitmapLayer *s_bitmaplayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, true);
  
  s_res_roboto_bold_subset_49 = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
  s_res_aqi_moderate = gbitmap_create_with_resource(RESOURCE_ID_AQI_MODERATE);
  // time_layer
  time_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_text(time_layer, "14:09");
  text_layer_set_text_alignment(time_layer, GTextAlignmentRight);
  text_layer_set_font(time_layer, s_res_roboto_bold_subset_49);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
  
  // date_layer
  date_layer = text_layer_create(GRect(0, 0, 54, 14));
  text_layer_set_text(date_layer, "Fri 03 Oct");
  text_layer_set_font(date_layer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)date_layer);
  
  // warning_layer
  warning_layer = text_layer_create(GRect(6, 62, 135, 53));
  text_layer_set_background_color(warning_layer, GColorClear);
  text_layer_set_text_color(warning_layer, GColorWhite);
  text_layer_set_text(warning_layer, "Thunderstorm Warning was issued at 23:30 HKT (3 Oct 2014) ");
  text_layer_set_font(warning_layer, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)warning_layer);
  
  // hdiv
  hdiv = text_layer_create(GRect(0, 131, 144, 1));
  text_layer_set_text(hdiv, "Text layer");
  layer_add_child(window_get_root_layer(s_window), (Layer *)hdiv);
  
  // temperature_layer
  temperature_layer = text_layer_create(GRect(-1, 129, 59, 39));
  text_layer_set_text(temperature_layer, "28°");
  text_layer_set_text_alignment(temperature_layer, GTextAlignmentCenter);
  text_layer_set_font(temperature_layer, s_res_bitham_30_black);
  layer_add_child(window_get_root_layer(s_window), (Layer *)temperature_layer);
  
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(56, 129, 93, 41));
  bitmap_layer_set_bitmap(s_bitmaplayer_1, s_res_aqi_moderate);
  bitmap_layer_set_background_color(s_bitmaplayer_1, GColorWhite);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(warning_layer);
  text_layer_destroy(hdiv);
  text_layer_destroy(temperature_layer);
  bitmap_layer_destroy(s_bitmaplayer_1);
  gbitmap_destroy(s_res_aqi_moderate);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_mainface(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_mainface(void) {
  window_stack_remove(s_window, true);
}

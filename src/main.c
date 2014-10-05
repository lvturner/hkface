#include <pebble.h>
#include "mainface.h"

static AppSync sync;
static uint8_t sync_buffer[1024];

enum {
    AQI = 0x1,
    TEMPERATURE = 0x2,
    WARNING = 0x3
};

void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
    switch (key) {
        case AQI:
            set_aqi(new_tuple->value->cstring); 
            break;
        case TEMPERATURE:
            set_temperature(new_tuple->value->cstring); 
            break;
        case WARNING:
            set_warning(new_tuple->value->cstring); 
            break;
    }
}

char *translate_error(AppMessageResult result) {
    switch (result) {
        case APP_MSG_OK: return "APP_MSG_OK";
        case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
        case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
        case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
        case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
        case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
        case APP_MSG_BUSY: return "APP_MSG_BUSY";
        case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
        case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
        case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
        case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
        case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
        case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
        case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
        default: return "UNKNOWN ERROR";
    }
}

void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "... Sync Error: %s", translate_error(app_message_error));
}

static void ask_for_update() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Asking pebble for update");
    Tuplet value = TupletInteger(1, 1);

    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    if (iter == NULL) {
        return;
    }

    dict_write_tuplet(iter, &value);
    dict_write_end(iter);

    app_message_outbox_send();
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
    set_aqi("");
    set_temperature("");
    set_warning("\n\tUPDATING...");

    ask_for_update();
}

static void handle_tick(struct tm* tick_time, TimeUnits units_changed) {
    static char time_text[] = "00:00";
    strftime(time_text, sizeof(time_text), "%I:%M", tick_time);
    set_time(time_text);// time_text);

    if(units_changed & DAY_UNIT) {
        static char date_text[] = "XXX xx XXX";
        strftime(date_text, sizeof(date_text), "%a %d %b", tick_time);
        set_date(date_text);
        ask_for_update();
    }

    if(units_changed & HOUR_UNIT) {
        ask_for_update();
    }
}

static void deinit(void) {
    app_sync_deinit(&sync);
}

int main(void) {
    show_mainface();  

    Tuplet initial_values[] = {
        TupletCString(AQI, "aqi"),
        TupletCString(TEMPERATURE, "0°"),
        TupletCString(WARNING, "\n\tUPDATING...") 
    };

    app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
            sync_tuple_changed_callback, sync_error_callback, NULL);

    app_message_open(256, 256);

    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    handle_tick(current_time, DAY_UNIT);
    tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
    accel_tap_service_subscribe(tap_handler);



    app_event_loop();
}

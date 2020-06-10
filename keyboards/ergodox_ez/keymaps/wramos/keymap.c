#include QMK_KEYBOARD_H
#include "version.h"
#include <print.h>
#include <inttypes.h>

/* Layers */

#define BASE 0 // default layer
#define NUMB 1 // numbers
#define SYMB 2 // symbols
#define ARRW 3 // arrow key layer
#define SHFT 4 // shift lock layer

uint32_t CTL_C_WHEN_TAPPED_CTL_ALT_WHEN_HELD_TIMER = 0;

enum custom_keycodes {
    CAPS_A = SAFE_RANGE,
    CAPS_E,
    CAPS_S,
    CAPS_O,
    CAPS_U,
    CAPS_SLSH,
    CAPS_BSLS,
    CAPS_NUBS,
    CAPS_GRV,
    CAPS_M,
    CAPS_SCLN,
    CAPS_DOT,
    ctl_c_when_tapped_ctl_alt_when_held,
};

/* Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[BASE] = LAYOUT_ergodox(
// left hand
KC_NUBS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,
KC_NO,  KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,  KC_NO,
KC_EQL, KC_A,  LT(NUMB,KC_S),  MT(MOD_LSFT,KC_D),  KC_F,  KC_G,
KC_GRV,  KC_Y,  KC_X,  KC_C,  KC_V,  KC_B,  KC_LGUI,
KC_NO,   KC_NO, KC_NO, KC_NO, MO(NUMB),
                     KC_INS,              KC_NO,
                                          TG(ARRW),
MT(MOD_LCTL,KC_ENT), MT(MOD_LALT,KC_TAB), ctl_c_when_tapped_ctl_alt_when_held,
// right hand
KC_F7,      KC_F8, KC_F9,    KC_F10,  KC_F11,  KC_F12,  EEPROM_RESET,
KC_ESC,     KC_Z,  KC_U,     KC_I,    KC_O,    KC_P,    KC_BSLS,
KC_H,  KC_J,     MT(MOD_RSFT,KC_K),    LT(SYMB,KC_L),    KC_SCLN, KC_QUOT,
TG(SHFT),   KC_N,  KC_M,     KC_COMM, KC_DOT,  KC_SLSH, KC_MINS,
                   MO(ARRW), KC_NO,   KC_NO,   KC_NO,   KC_NO,
KC_NO,                        KC_PSCR,
TG(NUMB),
MT(MOD_LCTL|MOD_LALT,KC_DEL), MT(MOD_LALT,KC_BSPC), MT(MOD_RCTL,KC_SPC)
),

[NUMB] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_1   , KC_2,    KC_3,    KC_4,    KC_5,    KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                             KC_TRNS, KC_TRNS,
                                                      KC_TRNS,
                                    KC_TRNS, KC_TRNS, KC_TRNS,
// right hand
KC_TRNS, KC_6   , KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS,
KC_TRNS, CAPS_E, KC_7,    KC_8,    KC_9,    CAPS_S,  KC_TRNS,
         CAPS_A,    KC_4,    KC_5,    KC_6,    KC_0,  KC_TRNS,
KC_TRNS, CAPS_U, KC_1,    KC_2,    KC_3,    CAPS_O,  KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),

[SYMB] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_1   , KC_2,    KC_3,    KC_4,    KC_5,    KC_TRNS,
KC_TRNS,  KC_AT,   KC_PIPE, KC_LCBR, KC_RCBR, KC_DLR,  KC_TRNS,
KC_TRNS,   KC_EXLM, KC_AMPR, KC_LPRN, KC_RPRN, KC_HASH,
KC_TRNS,   KC_ASTR, KC_CIRC, KC_LBRC, KC_RBRC, KC_PERC, KC_TRNS,
KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS,   KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS,
// right hand
KC_TRNS, KC_6   , KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),

[ARRW] = LAYOUT_ergodox(
// left hand
CAPS_NUBS, KC_TRNS,   KC_TRNS,  KC_TRNS, KC_TRNS,       KC_TRNS,   KC_TRNS,
KC_TRNS, KC_TRNS,   KC_HOME,  KC_UP,   KC_END,        KC_PGUP, KC_TRNS,
KC_TRNS, KC_APP,    KC_LEFT,  KC_DOWN, KC_RGHT,       KC_PGDN,
KC_TRNS, CAPS_SLSH, CAPS_DOT, CAPS_M,  CAPS_BSLS, CAPS_SCLN, KC_TRNS,
CAPS_GRV, KC_TRNS,   KC_TRNS,  KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS,
// right hand
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),

[SHFT] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
KC_TRNS, LSFT(KC_Q), LSFT(KC_W), LSFT(KC_E), LSFT(KC_R), LSFT(KC_T), KC_TRNS,
KC_TRNS, LSFT(KC_A), LSFT(KC_S), LSFT(KC_D), LSFT(KC_F), LSFT(KC_G),
KC_TRNS, LSFT(KC_Y), LSFT(KC_X), LSFT(KC_C), LSFT(KC_V), LSFT(KC_B), KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS,
// right hand
KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
KC_TRNS, LSFT(KC_Z), LSFT(KC_U), LSFT(KC_I), LSFT(KC_O), LSFT(KC_P), KC_TRNS,
         LSFT(KC_H), LSFT(KC_J), LSFT(KC_K), LSFT(KC_L), KC_TRNS,    KC_TRNS,
KC_TRNS, LSFT(KC_N), LSFT(KC_M), KC_TRNS,    KC_TRNS,    KC_UNDS,    KC_TRNS,
                     KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),
};

void caps(uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed) {
        tap_code(KC_CAPS);
        tap_code(keycode);
        tap_code(KC_CAPS);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case ctl_c_when_tapped_ctl_alt_when_held:
        if(record->event.pressed) {
            CTL_C_WHEN_TAPPED_CTL_ALT_WHEN_HELD_TIMER = timer_read32();
            register_code(KC_LCTL);
            register_code(KC_LALT);
        } else {
            uint32_t elapsed = timer_elapsed32(CTL_C_WHEN_TAPPED_CTL_ALT_WHEN_HELD_TIMER);
            if(elapsed <= TAPPING_TERM)
            {
                unregister_code(KC_LALT);
                tap_code(KC_C);
                unregister_code(KC_LCTL);
            } else {
                unregister_code(KC_LALT);
                unregister_code(KC_LCTL);
            }
        }
        return false;
    case CAPS_A:
        caps(KC_A, record);
        return false;
    case CAPS_E:
        caps(KC_E, record);
        return false;
    case CAPS_S:
        caps(KC_S, record);
        return false;
    case CAPS_O:
        caps(KC_O, record);
        return false;
    case CAPS_U:
        caps(KC_U, record);
        return false;
    case CAPS_SLSH:
        caps(KC_SLSH, record);
        return false;
    case CAPS_BSLS:
        caps(KC_BSLS, record);
        return false;
    case CAPS_NUBS:
        caps(KC_NUBS, record);
        return false;
    case CAPS_GRV:
        caps(KC_GRV, record);
        return false;
    case CAPS_M:
        caps(KC_M, record);
        return false;
    case CAPS_SCLN:
        caps(KC_SCLN, record);
        return false;
    case CAPS_DOT:
        caps(KC_DOT, record);
        return false;
    default:
        return true;
    }
}


// Runs just one time when the keyboard initializes.
/* void matrix_init_user(void) { */
/* #ifdef RGBLIGHT_COLOR_LAYER_0 */
/*   rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0); */
/* #endif */
/* }; */


// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    bool number_layer_is_on = layer_state_cmp(state, NUMB);
    bool arrow_layer_is_on = layer_state_cmp(state, ARRW);
    bool shift_lock_layer_is_on = layer_state_cmp(state, SHFT);
    if(number_layer_is_on) {
        ergodox_right_led_1_on();
    }
    else {
        ergodox_right_led_1_off();
    }
    if(arrow_layer_is_on) {
        ergodox_right_led_2_on();
    }
    else {
        ergodox_right_led_2_off();
    }
    if(shift_lock_layer_is_on) {
        ergodox_right_led_3_on();
    }
    else {
        ergodox_right_led_3_off();
    }
  return state;
};

#include QMK_KEYBOARD_H
#include "version.h"
#include <print.h>
#include <inttypes.h>

/* Layers */

#define BASE 0 // default layer
#define SHFT 1 // shift lock layer
#define NUMB 2 // numbers
#define SYMB 3 // symbols
#define ARRW 4 // arrow key layer

// dummy macro for aligning keyboard layout in source code
#define _NOKEY


/* tap hold implementation */

// supports up to 32 tap hold key definitions
#define TAP_HOLD_MIN_KEYCODE 0x5600
#define TAP_HOLD_MAX_KEYCODE 0x561F
#define TAP_HOLD_INDEX_MASK 0x1F

#define IS_TAP_HOLD_KEY(keycode) (keycode >= TAP_HOLD_MIN_KEYCODE && keycode <= TAP_HOLD_MAX_KEYCODE)

#define TAP_HOLD_LCTL 0x01
#define TAP_HOLD_LSFT 0x02
#define TAP_HOLD_LALT 0x04
#define TAP_HOLD_LGUI 0x08
#define TAP_HOLD_RCTL 0x10
#define TAP_HOLD_RSFT 0x20
#define TAP_HOLD_RALT 0x40
#define TAP_HOLD_RGUI 0x80

typedef void (*tap_hold_key_down_function_t)(keyrecord_t *record);
typedef void (*tap_hold_key_up_function_t)(keyrecord_t *record, uint16_t elapsed);

typedef struct {
    tap_hold_key_down_function_t on_key_down;
    tap_hold_key_up_function_t on_key_up;
    uint16_t timer;
} tap_hold_t;



/* to be used in array of tap holds */
#define TAP_HOLD(on_key_down, on_key_up) { on_key_down, on_key_up }

/* to be used in keymap */
#define TH(index) (TAP_HOLD_MIN_KEYCODE | ((index)&( TAP_HOLD_INDEX_MASK )))

extern tap_hold_t tap_holds[];

bool process_tap_hold(uint16_t keycode, keyrecord_t *record) {
    if(IS_TAP_HOLD_KEY(keycode)) {
        uint16_t idx = keycode & TAP_HOLD_INDEX_MASK;
        tap_hold_t *tap_hold;
        tap_hold = &tap_holds[idx];

        if(record->event.pressed) {
            tap_hold->timer = timer_read32();
            if(tap_hold->on_key_down)
            {
                tap_hold->on_key_down(record);
            }
        } else {
            uint16_t elapsed = timer_elapsed(tap_hold->timer);
            if(tap_hold->on_key_up)
            {
                tap_hold->on_key_up(record, elapsed);
            }
        }
        return false;
    }
    return true;
}


/* tap hold configuration */
void shift_d_lshift_key_down(keyrecord_t *record)
{
    register_mods(TAP_HOLD_LSFT);
}

void shift_d_lshift_key_up(keyrecord_t *record, uint16_t elapsed)
{
    if(elapsed <= TAPPING_TERM) {
        tap_code(KC_D);
    }
    unregister_mods(TAP_HOLD_LSFT);
}

void shift_k_lshift_key_down(keyrecord_t *record)
{
    register_mods(TAP_HOLD_LSFT);
}

void shift_k_lshift_key_up(keyrecord_t *record, uint16_t elapsed)
{
    if(elapsed <= TAPPING_TERM) {
        tap_code(KC_K);
    }
    unregister_mods(TAP_HOLD_LSFT);
}

void shift_s_number_layer_key_down(keyrecord_t *record)
{
    layer_on(NUMB);
}

void shift_s_number_layer_key_up(keyrecord_t *record, uint16_t elapsed)
{
    layer_off(NUMB);
    if(elapsed <= TAPPING_TERM) {
        register_mods(TAP_HOLD_LSFT);
        tap_code(KC_S);
        unregister_mods(TAP_HOLD_LSFT);
    }
}

void shift_l_symbol_layer_key_down(keyrecord_t *record)
{
    layer_on(SYMB);
}

void shift_l_symbol_layer_key_up(keyrecord_t *record, uint16_t elapsed)
{
    layer_off(SYMB);
    if(elapsed <= TAPPING_TERM) {
        register_mods(TAP_HOLD_LSFT);
        tap_code(KC_L);
        unregister_mods(TAP_HOLD_LSFT);
    }
}

void shift_j_arrow_layer_key_down(keyrecord_t *record)
{
    layer_on(ARRW);
}

void shift_j_arrow_layer_key_up(keyrecord_t *record, uint16_t elapsed)
{
    layer_off(ARRW);
    if(elapsed <= TAPPING_TERM) {
        register_mods(TAP_HOLD_LSFT);
        tap_code(KC_J);
        unregister_mods(TAP_HOLD_LSFT);
    }
}

enum tap_hold_enum {
    SHIFT_S_NUMBER_LAYER = 0,
    SHIFT_L_SYMBOL_LAYER,
    SHIFT_J_ARROW_LAYER,
    SHIFT_K_LSHIFT,
    SHIFT_D_LSHIFT,
};

tap_hold_t tap_holds[] = {
    [SHIFT_D_LSHIFT] = TAP_HOLD(shift_d_lshift_key_down, shift_d_lshift_key_up),
    [SHIFT_K_LSHIFT] = TAP_HOLD(shift_k_lshift_key_down, shift_k_lshift_key_up),
    [SHIFT_S_NUMBER_LAYER] = TAP_HOLD(shift_s_number_layer_key_down, shift_s_number_layer_key_up),
    [SHIFT_L_SYMBOL_LAYER] = TAP_HOLD(shift_l_symbol_layer_key_down, shift_l_symbol_layer_key_up),
    [SHIFT_J_ARROW_LAYER] = TAP_HOLD(shift_j_arrow_layer_key_down, shift_j_arrow_layer_key_up),
};


enum custom_keycodes {
    CAPS_A = SAFE_RANGE, // german umlaut ae
    CAPS_E, // e with trema
    CAPS_S, // german umlaut sz
    CAPS_O, // german umlaut oe
    CAPS_U, // german umlaut ue
    CAPS_SLSH, // euro sign
    CAPS_BSLS, // paragraph sign
    CAPS_NUBS, // unshifted: dead key accent acute; shifted: accent circonflex
    CAPS_GRV, // unshifted: backtick; shifted: dead key accent grave
    CAPS_M, // greek letter mu
    CAPS_T, // tilde
    CAPS_D, // degree sign
    INV_QUOTE, // unshifted: double quote; shifted: single quote
    INV_MINUS, // unshifted: underscore; shifted: minus
};

/* Inverting shift on keys*/
void press_key_with_level_mods(uint16_t key) {
    const uint8_t interesting_mods = MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT) | MOD_BIT(KC_RALT);

    // Save the state
    const uint8_t real_mods = get_mods();
    const uint8_t weak_mods = get_weak_mods();
    const uint8_t macro_mods = get_macro_mods();

    uint8_t target_mods = (key >> 8) & (QK_MODS_MAX >> 8);
    // The 5th bit indicates that it's a right hand mod,
    // which needs some fixup
    if (target_mods & 0x10) {
        target_mods &= 0xF;
        target_mods <<= 4;
    }

    // Clear the mods that we are potentially going to modify,
    del_mods(interesting_mods);
    del_weak_mods(interesting_mods);
    del_macro_mods(interesting_mods);

    // Enable the mods that we need
    add_mods(target_mods & interesting_mods);

    // Press and release the key
    register_code(key & 0xFF);
    unregister_code(key & 0xFF);

    // Restore the previous state
    set_mods(real_mods);
    set_weak_mods(weak_mods);
    set_macro_mods(macro_mods);
    send_keyboard_report();
}

void override_key(keyrecord_t* record, uint16_t normal, uint16_t shifted) {
    const uint8_t shift = MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT);
    if (record->event.pressed) {
        bool shift_pressed = keyboard_report->mods & shift;
        const uint16_t target = shift_pressed ? shifted : normal;
        uint8_t keycode = target & 0xFF;
        if (keycode == KC_NO) {
            return;
        }
        press_key_with_level_mods(target);
    }
}

/* Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[BASE] = LAYOUT_ergodox(
// left hand
KC_NO,   KC_F1,   KC_F2,         KC_F3,             KC_F4,   KC_F5, KC_F6,
KC_NUBS, KC_Q,    KC_W,          KC_E,              KC_R,    KC_T,  LSFT(LALT(KC_S)),
KC_MINS, KC_A,    LT(NUMB,KC_S), KC_LSFT,           KC_F,    KC_G,
KC_GRV,  KC_Y,    KC_X,          KC_C,              KC_V,    KC_B,  KC_INS,
KC_NO,   KC_LGUI, KC_NO,         KC_NO,             MT(MOD_LCTL|MOD_LALT,KC_EQL),
_NOKEY               KC_PAUSE,            KC_NO,
_NOKEY               _NOKEY               TG(ARRW),
MT(MOD_LCTL,KC_ENT), MT(MOD_LALT,KC_TAB), LSFT(KC_TAB),

// right hand
KC_F7,    KC_F8,  KC_F9,                            KC_F10,  KC_F11,        KC_F12,  EEPROM_RESET,
KC_ESC,   KC_Z,   KC_U,                             KC_I,    KC_O,          KC_P,    KC_BSLS,
_NOKEY    KC_H,   LT(ARRW,KC_J),                    KC_LSFT, LT(SYMB,KC_L), KC_K,    INV_QUOTE,
TG(SHFT), KC_N,   KC_M,                             KC_COMM, KC_DOT,        KC_D,    KC_SLSH,
_NOKEY    _NOKEY  MT(MOD_LCTL|MOD_LALT,KC_SCLN),    KC_NO,   KC_NO,         KC_LGUI, KC_NO,
KC_NO,                        KC_PSCR,
TG(NUMB),
KC_DEL, MT(MOD_LALT,KC_BSPC), MT(MOD_LCTL,KC_SPC)
),

[SHFT] = LAYOUT_ergodox(
// left hand
KC_TRNS,   KC_TRNS,    KC_TRNS,                  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
KC_TRNS,   RSFT(KC_Q), RSFT(KC_W),               RSFT(KC_E), RSFT(KC_R), RSFT(KC_T), KC_TRNS,
INV_MINUS, RSFT(KC_A), TH(SHIFT_S_NUMBER_LAYER), KC_LSFT,    RSFT(KC_F), RSFT(KC_G),
KC_TRNS,   RSFT(KC_Y), RSFT(KC_X),               RSFT(KC_C), RSFT(KC_V), RSFT(KC_B), KC_TRNS,
KC_TRNS,   KC_TRNS,    KC_TRNS,                  KC_TRNS,    KC_TRNS,
_NOKEY   KC_TRNS,    KC_TRNS,
_NOKEY   _NOKEY      KC_TRNS,
KC_TRNS, KC_TRNS,    KC_TRNS,
// right hand
KC_TRNS, KC_TRNS,    KC_TRNS,                 KC_TRNS,    KC_TRNS,                  KC_TRNS,    KC_TRNS,
KC_TRNS, RSFT(KC_Z), RSFT(KC_U),              RSFT(KC_I), RSFT(KC_O),               RSFT(KC_P), KC_TRNS,
_NOKEY   RSFT(KC_H), TH(SHIFT_J_ARROW_LAYER), KC_LSFT,    TH(SHIFT_L_SYMBOL_LAYER), RSFT(KC_K), KC_TRNS,
KC_TRNS, RSFT(KC_N), RSFT(KC_M),              KC_TRNS,    KC_TRNS,                  RSFT(KC_D), KC_TRNS,
_NOKEY   _NOKEY      KC_TRNS,                 KC_TRNS,    KC_TRNS,                  KC_TRNS,    KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS,    KC_TRNS
),

[NUMB] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
_NOKEY   KC_TRNS, KC_TRNS,
_NOKEY   _NOKEY   KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS,

// right hand
KC_TRNS, KC_6,   KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS,
KC_TRNS, CAPS_E, KC_7,    KC_8,    KC_9,    CAPS_S,  KC_TRNS,
_NOKEY   CAPS_A, KC_4,    KC_5,    KC_6,    KC_0,    KC_TRNS,
KC_TRNS, CAPS_U, KC_1,    KC_2,    KC_3,    CAPS_O,  KC_TRNS,
_NOKEY   _NOKEY  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),

[SYMB] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_TRNS,
KC_TRNS, KC_CIRC, KC_ASTR, KC_LCBR, KC_RCBR, KC_HASH,  KC_TRNS,
KC_TRNS, KC_EXLM, KC_AMPR, KC_LPRN, KC_RPRN, KC_DLR,
KC_TRNS, CAPS_T,  KC_AT,   KC_LBRC, KC_RBRC, KC_PERC, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
_NOKEY   KC_TRNS, KC_TRNS,
_NOKEY   _NOKEY   KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS,
// right hand
KC_TRNS, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
_NOKEY   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
_NOKEY    _NOKEY  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),

[ARRW] = LAYOUT_ergodox(
// left hand
CAPS_NUBS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,
KC_TRNS,   CAPS_M,    KC_HOME, KC_UP,   KC_END,    KC_PGUP, LSFT(LALT(KC_S)),
KC_TRNS,   KC_APP,    KC_LEFT, KC_DOWN, KC_RGHT,   KC_PGDN,
KC_TRNS,   CAPS_SLSH, CAPS_D,  KC_LT,   KC_GT,     CAPS_BSLS, KC_TRNS,
CAPS_GRV,  KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,
_NOKEY   KC_TRNS, KC_TRNS,
_NOKEY   _NOKEY   KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS,
// right hand
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
_NOKEY   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
_NOKEY   _NOKEY   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
)
};

void caps(uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed) {
        tap_code(KC_CAPS);
        tap_code(keycode);
        tap_code(KC_CAPS);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if(!process_tap_hold(keycode, record)){
        return false;
    }

    switch (keycode) {
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
    case CAPS_T:
        caps(KC_T, record);
        return false;
    case CAPS_D:
        caps(KC_D, record);
        return false;
    case INV_QUOTE:
        override_key(record, LSFT(KC_QUOT), KC_QUOT);
        return false;
    case INV_MINUS:
        override_key(record, LSFT(KC_MINS), KC_MINS);
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
    if(shift_lock_layer_is_on) {
        ergodox_right_led_1_on();
    }
    else {
        ergodox_right_led_1_off();
    }
    if(number_layer_is_on) {
        ergodox_right_led_2_on();
    }
    else {
        ergodox_right_led_2_off();
    }
    if(arrow_layer_is_on) {
        ergodox_right_led_3_on();
    }
    else {
        ergodox_right_led_3_off();
    }
  return state;
};

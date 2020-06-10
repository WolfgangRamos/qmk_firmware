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



/* common elements used in tap layer and tap mod implementation */

bool is_printable(uint16_t keycode) {
    switch(keycode & 0xFF) {
    case 0x04 ... 0x28:
    case 0x2B ... 0x38:
        return true;
    default:
        return false;
    }
}



/* tap layer implementation */

// supports up to 16 tap layer defintions using layer number from 0 to 31
#define TAP_LAYER_MIN_KEYCODE 0x5620
#define TAP_LAYER_MAX_KEYCODE 0x562F
#define TAP_LAYER_INDEX_MASK 0xF

#define IS_TAP_LAYER_KEY(keycode) (keycode >= TAP_LAYER_MIN_KEYCODE && keycode <= TAP_LAYER_MAX_KEYCODE)

uint16_t tap_layer_pending = 0x0000;
uint16_t tap_layer_active = 0x0000;

typedef struct {
    uint8_t keycode;
    uint8_t layer;
} tap_layer_t;

/* to be used in array of tap layers */
#define TAP_LAYER(kc, layer) { ((kc)&0xFF), ((layer)&0xFF)}

/* to be used in keymap */
#define TL(index) (TAP_LAYER_MIN_KEYCODE | ((index)&( TAP_LAYER_INDEX_MASK )))

extern tap_layer_t tap_layers[];

void activate_pending_layer_and_tap_key(uint16_t keycode, keyrecord_t *record) {
    uint16_t idx = tap_layer_pending & TAP_LAYER_INDEX_MASK;
    tap_layer_t *tap_layer;
    tap_layer = &tap_layers[idx];
    uint8_t layer = tap_layer->layer;

    if(is_printable(tap_layer->keycode)) {
        tap_code(KC_BSPC);
    }

    layer_on(layer);
    uint16_t key_to_tap = get_record_keycode(record, true);
    if(IS_TAP_LAYER_KEY(key_to_tap)) {
        uint16_t idx2 = key_to_tap & TAP_LAYER_INDEX_MASK;
        tap_layer_t *tap_layer2;
        tap_layer2 = &tap_layers[idx2];
        tap_code(tap_layer2->keycode);
    } else {
        // somehow a modifieres present in key_to_tap can go missing here. We add it manually to ensure it is there before the actual key is submitted.
        // todo fix the other modifiers as well
        if(key_to_tap & QK_LSFT) {
            register_mods(0x02);
        }
        tap_code(key_to_tap);
        if(key_to_tap & QK_LSFT) {
            unregister_mods(0x02);
        }
    }

    tap_layer_active = tap_layer_pending;
    tap_layer_pending = 0x0000;
}

bool process_tap_layer(uint16_t keycode, keyrecord_t *record) {
    if(tap_layer_pending && record->event.pressed) {
        activate_pending_layer_and_tap_key(keycode, record);
        return false;
    }
    if(IS_TAP_LAYER_KEY(keycode)) { // tap_layer_pending is zero here
        uint16_t idx = keycode & TAP_LAYER_INDEX_MASK;
        tap_layer_t *tap_layer;
        tap_layer = &tap_layers[idx];

        if(record->event.pressed) {
            if(tap_layer_active == 0) {
                tap_code(tap_layer->keycode);
                tap_layer_pending = keycode;
            } else {
                tap_code(tap_layer->keycode);
            }
        } else {
            if(keycode == tap_layer_active) {
                layer_off(tap_layer->layer);
                tap_layer_active = 0x0000;
            }
            tap_layer_pending = 0x0000;
        }
        return false;
    }
    return true;
}



/* tap mod implementation */

// supports up to 32 tap mod key definitions
#define TAP_MOD_MIN_KEYCODE 0x5600
#define TAP_MOD_MAX_KEYCODE 0x561F
#define TAP_MOD_INDEX_MASK 0x1F

#define IS_TAP_MOD_KEY(keycode) (keycode >= TAP_MOD_MIN_KEYCODE && keycode <= TAP_MOD_MAX_KEYCODE)

#define TAP_MOD_LCTL 0x01
#define TAP_MOD_LSFT 0x02
#define TAP_MOD_LALT 0x04
#define TAP_MOD_LGUI 0x08
#define TAP_MOD_RCTL 0x10
#define TAP_MOD_RSFT 0x20
#define TAP_MOD_RALT 0x40
#define TAP_MOD_RGUI 0x80

uint16_t tap_mod_pending = 0x0000;
uint16_t tap_mod_active = 0x0000;

typedef struct {
    uint8_t keycode;
    uint8_t modifier;
} tap_mod_t;

/* to be used in array of tap mods */
#define TAP_MOD(kc, mod) { ((kc)&0xFF), ((mod)&0xFF)}

/* to be used in keymap */
#define TM(index) (TAP_MOD_MIN_KEYCODE | ((index)&( TAP_MOD_INDEX_MASK )))

extern tap_mod_t tap_mods[];

void activate_pending_modifiers_and_tap_key(uint16_t keycode, keyrecord_t *record) {
    uint16_t idx = tap_mod_pending & TAP_MOD_INDEX_MASK;
    tap_mod_t *tap_mod;
    tap_mod = &tap_mods[idx];

    if(is_printable(tap_mod->keycode)) {
        tap_code(KC_BSPC);
    }
    register_mods(tap_mod->modifier);

    if(IS_TAP_MOD_KEY(keycode)) {
        uint16_t idx2 = keycode & TAP_MOD_INDEX_MASK;
        tap_mod_t *tap_mod2;
        tap_mod2 = &tap_mods[idx2];
        tap_code(tap_mod2->keycode);
    } else {
        tap_code(keycode);
    }
    tap_mod_active = tap_mod_pending;
    tap_mod_pending = 0x0000;
}

bool process_tap_mod(uint16_t keycode, keyrecord_t *record) {
    if(tap_mod_pending && record->event.pressed) {
        activate_pending_modifiers_and_tap_key(keycode, record);
        return false;
    }
    if(IS_TAP_MOD_KEY(keycode)) { // tap_mod_pending is zero here
        uint16_t idx = keycode & TAP_MOD_INDEX_MASK;
        tap_mod_t *tap_mod;
        tap_mod = &tap_mods[idx];

        if(record->event.pressed) {
            if(tap_mod_active == 0) {
                tap_code(tap_mod->keycode);
                tap_mod_pending = keycode;
            } else {
                tap_code(tap_mod->keycode);
            }
        } else {
            if(keycode == tap_mod_active) {
                unregister_mods(tap_mod->modifier);
                tap_mod_active = 0x0000;
            }
            tap_mod_pending = 0x0000;
        }
        return false;
    }
    return true;
}


/* tap mod configuration */

enum tap_mod_enum {
    K_RSFT = 0,
    J_RCTL_RSFT,
    H_RALT_RSFT,
    G_RALT_RSFT,
    F_RCTL_RSFT,
    D_RSFT,
};

tap_mod_t tap_mods[] = {
    [K_RSFT] = TAP_MOD(KC_K, TAP_MOD_RSFT),
    [J_RCTL_RSFT] = TAP_MOD(KC_J, TAP_MOD_RCTL | TAP_MOD_RSFT),
    [H_RALT_RSFT] = TAP_MOD(KC_H, TAP_MOD_RALT | TAP_MOD_RSFT),
    [G_RALT_RSFT] = TAP_MOD(KC_G, TAP_MOD_RALT | TAP_MOD_RSFT),
    [F_RCTL_RSFT] = TAP_MOD(KC_F, TAP_MOD_RCTL | TAP_MOD_RSFT),
    [D_RSFT] = TAP_MOD(KC_D, TAP_MOD_RSFT),
};


/* tap layer configuration */

enum tap_layer_enum {
    S_NUMB = 0,
    L_SYMB,
};

tap_layer_t tap_layers[] = {
    [S_NUMB] = TAP_LAYER(KC_S, NUMB),
    [L_SYMB] = TAP_LAYER(KC_L, SYMB),
};

/* custom key codes for use in process_record_user */

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
};

/* Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[BASE] = LAYOUT_ergodox(
// left hand
KC_NUBS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,
KC_GRV,  KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,  KC_PGUP,
KC_LSFT, KC_A,  TL(S_NUMB),  TM(D_RSFT),  TM(F_RCTL_RSFT),  TM(G_RALT_RSFT),
KC_EQL,  KC_Y,  KC_X,  KC_C,  KC_V,  KC_B,  KC_LGUI,
KC_NO,   KC_NO, KC_NO, KC_NO, MO(NUMB),
                     KC_INS,              KC_NO,
                                          TG(ARRW),
MT(MOD_LCTL,KC_ENT), MT(MOD_LALT,KC_TAB), MT(MOD_LCTL|MOD_LALT,KC_PGDN),
// right hand
KC_F7,      KC_F8, KC_F9,    KC_F10,  KC_F11,  KC_F12,  EEPROM_RESET,
KC_ESC,     KC_Z,  KC_U,     KC_I,    KC_O,    KC_P,    KC_QUOT,
            TM(H_RALT_RSFT),  TM(J_RCTL_RSFT),     TM(K_RSFT),    TL(L_SYMB),    KC_SCLN, KC_LSFT,
TG(SHFT),   KC_N,  KC_M,     KC_COMM, KC_DOT,  KC_MINS, MO(SYMB),
                   MO(ARRW), KC_NO,   KC_NO,   KC_NO,   KC_NO,
KC_NO,                        KC_PSCR,
TG(NUMB),
MT(MOD_LCTL|MOD_LALT,KC_DEL), MT(MOD_LALT,KC_BSPC), MT(MOD_LCTL,KC_SPC)
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
KC_TRNS, KC_BSLS, KC_7,    KC_8,    KC_9,    CAPS_S,  KC_TRNS,
         KC_0,    KC_4,    KC_5,    KC_6,    CAPS_O,  KC_TRNS,
KC_TRNS, KC_SLSH, KC_1,    KC_2,    KC_3,    CAPS_U,  KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),

[SYMB] = LAYOUT_ergodox(
// left hand
CAPS_NUBS, KC_1   , KC_2,    KC_3,    KC_4,    KC_5,    KC_TRNS,
CAPS_GRV,  KC_AT,   KC_PIPE, KC_LCBR, KC_RCBR, KC_DLR,  KC_TRNS,
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
KC_TRNS, KC_TRNS,   KC_TRNS,  KC_TRNS, KC_TRNS,       KC_TRNS,   KC_TRNS,
KC_TRNS, CAPS_SLSH, KC_HOME,  KC_UP,   KC_END,        CAPS_SCLN, KC_TRNS,
KC_TRNS, CAPS_A,    KC_LEFT,  KC_DOWN, KC_RGHT,       CAPS_E,
KC_TRNS, KC_APP,    CAPS_DOT, CAPS_M,  LSFT(KC_SLSH), CAPS_BSLS, KC_TRNS,
KC_TRNS, KC_TRNS,   KC_TRNS,  KC_TRNS, KC_TRNS,
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

    if(process_tap_layer(keycode, record) == false) {
        return false;
    }
    if(process_tap_mod(keycode, record) == false) {
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

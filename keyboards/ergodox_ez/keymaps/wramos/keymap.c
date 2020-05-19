#include QMK_KEYBOARD_H
#include "version.h"
#include <print.h>
#include <inttypes.h>

/* Layers */

#define BASE 0 // default layer
#define NUMB 1 // numbers
#define SYMB 2 // symbols
#define SPECIAL_LEFT 3 // special keys left hand
#define SPECIAL_RIGHT 4 // special keys right hand

/* Global Variables */

bool IS_SHIFT_LOCK_ON = false;
uint32_t CTL_C_WHEN_TAPPED_CTL_ALT_WHEN_HELD_TIMER = 0;
uint32_t CTL_DOT_WHEN_TAPPED_CTL_ALT_WHEN_HELD_TIMER = 0;
uint32_t GRAVE_WHEN_TAPPED_CAPS_WHEN_HELD_TIMER = 0;
uint32_t MINUS_WHEN_TAPPED_CAPS_WHEN_HELD_TIMER = 0;

enum custom_keycodes {
    NON_LOCKING_CAPS = SAFE_RANGE,
    SHIFT_LOCK,
    ctl_c_when_tapped_ctl_alt_when_held,
    ctl_dot_when_tapped_ctl_alt_when_held,
    grave_when_tapped_caps_when_held,
    minus_when_tapped_caps_when_held,
};

/* Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Basic layer
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Print  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | Reset  |
 * |--------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |  NUBS  |   Q  |   W  |   E  |   R  |   T  | Page |           | ESC  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|  up  |           |      |------+------+------+------+------+--------|
 * |Shift/= | L4/A | L1/S |S-C/D |S-A/F |MEH/G |------|           |------|MEH/H |S-A/J |S-C/K | L2/L | L3/; |Shift/' |
 * |--------+------+------+------+------+------| Page |           |Delete|------+------+------+------+------+--------|
 * | CAPS/` |   Z  |   X  |   C  |   V  |   B  | down |           |      |   N  |   M  |   ,  |   .  |   /  | CAPS/- |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | HOME | LGUI |      |      | END  |                                       | SLock|      |      | RGUI |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 * L1 = number layer (NUMB)               |Insert|      |       |      |      |
 * L2 = symbol layer (SYMB)        ,------|------|------|       |------+------+------.
 * L3 = arrow layer (SPECIAL_LEFT) |CTL/  |ALT/  |ARROWL|       | NUML |ALT/  |CTL/  |
 * L4 = mouse layer (SPECIAL_RIGHT)|ENTER |TAB   |------|       |------|BCKSPC|SPACE |
 *                                 |      |      |CA/C-.|       |CA/C-c|      |      |
 *                                 `--------------------'       `--------------------'
 * CA = CTRL + ALT
 * SHIFT + NUBS = ^ (dead keys)
 * CTRL + ALT + NUBS = ´ (dead key)
 * CTRL + ALT + ` = ` (dead key)
 * RALT = ALTGR
 */
[BASE] = LAYOUT_ergodox(
  // left hand
  KC_PSCR,                          KC_F1,                  KC_F2,         KC_F3,             KC_F4,                      KC_F5,                      KC_F6,
  KC_NUBS,                          KC_Q,                   KC_W,          KC_E,              KC_R,                       KC_T,                       KC_PGUP,
  MT(MOD_MEH,KC_EQL),               LT(SPECIAL_RIGHT,KC_A), LT(NUMB,KC_S), MT(MOD_LSFT,KC_D), MT(MOD_LSFT|MOD_LCTL,KC_F), MT(MOD_LSFT|MOD_LALT,KC_G),
  grave_when_tapped_caps_when_held, KC_Z,                   KC_X,          KC_C,              KC_V,                       KC_B,                       KC_PGDN,
  KC_HOME,                          KC_LGUI,                KC_NO,         KC_NO,             KC_END,
                                                           KC_INS,              KC_NO,
                                                                                TG(SPECIAL_LEFT),
                                      MT(MOD_LCTL,KC_ENT), MT(MOD_LALT,KC_TAB), ctl_c_when_tapped_ctl_alt_when_held,
  // right hand
  KC_F7,  KC_F8,                      KC_F9,                      KC_F10,            KC_F11,        KC_F12,                   EEPROM_RESET,
  KC_ESC, KC_Y,                       KC_U,                       KC_I,              KC_O,          KC_P,                     KC_BSLS,
          MT(MOD_RSFT|MOD_LALT,KC_H), MT(MOD_RSFT|MOD_RCTL,KC_J), MT(MOD_RSFT,KC_K), LT(SYMB,KC_L), LT(SPECIAL_LEFT,KC_SCLN), MT(MOD_MEH,KC_QUOT),
  KC_DEL, KC_N,                       KC_M,                       KC_COMM,           KC_DOT,        KC_SLSH,                  minus_when_tapped_caps_when_held,
                                      SHIFT_LOCK,                 KC_NO,             KC_NO,         KC_RGUI,                  KC_NO,
  KC_NO,                                 KC_NO,
  TG(NUMB),
  ctl_dot_when_tapped_ctl_alt_when_held, MT(MOD_LALT,KC_BSPC), MT(MOD_RCTL,KC_SPC)
),
/* Keymap 1: Number Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           |      |   +  |   7  |   8  |   9  |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |------|           |------|   *  |   4  |   5  |   6  |   0  |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           |      |   -  |   1  |   2  |   3  |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
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
  KC_TRNS, KC_PLUS, KC_7,    KC_8,    KC_9,    KC_TRNS, KC_TRNS,
           KC_ASTR, KC_4,    KC_5,    KC_6,    KC_0,    KC_TRNS,
  KC_TRNS, KC_MINS, KC_1,    KC_2,    KC_3,    KC_TRNS, KC_TRNS,
                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS,
  KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 2: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |   @  |   |  |   {  |   }  |   $  |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   ^  |   &  |   (  |   )  |   #  |------|           |------|      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |   *  |   !  |   [  |   ]  |   %  |      |           |      |      |      |      |      |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[SYMB] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_1   , KC_2,    KC_3,    KC_4,    KC_5,    KC_TRNS,
KC_TRNS, KC_AT,   KC_PIPE, KC_LCBR, KC_RCBR, KC_DLR,  KC_TRNS,
KC_TRNS, KC_CIRC, KC_AMPR, KC_LPRN, KC_RPRN, KC_HASH,
KC_TRNS, KC_ASTR, KC_EXLM, KC_LBRC, KC_RBRC, KC_PERC, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
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
/* Keymap 3: Special (left hand side)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |  UP  |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | APP  | LEFT | DOWN | RIGHT|      |------|           |------|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[SPECIAL_LEFT] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_UP,   KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_APP,  KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
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
/* Keymap 4: Special (right hand side)
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           |      |      | MWup | MsUp |MWdown|      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |------|           |------| LClk |MsLeft|MsDown|MsRght| RClk |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |      |      |      |      |      |      |           |      |      |MWleft|      | MWrgt|      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |Mouse |Mouse |      |       |      |      |      |
 *                                 |Speed |Speed |------|       |------|      |      |
 *                                 |  1   |  2   |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[SPECIAL_RIGHT] = LAYOUT_ergodox(
// left hand
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
         KC_TRNS, KC_TRNS,
                  KC_TRNS,
KC_ACL1, KC_ACL2, KC_TRNS,
// right hand
KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS, KC_WH_U, KC_MS_U, KC_WH_D, KC_TRNS, KC_TRNS,
         KC_BTN1, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN2, KC_TRNS,
KC_TRNS, KC_TRNS, KC_WH_L, KC_TRNS, KC_WH_R, KC_TRNS, KC_TRNS,
                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
KC_TRNS, KC_TRNS,
KC_TRNS,
KC_TRNS, KC_TRNS, KC_TRNS
),
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case NON_LOCKING_CAPS:
        tap_code(KC_CAPS);
        return false;
    case SHIFT_LOCK:
        if (record->event.pressed) {
            if(IS_SHIFT_LOCK_ON == false) {
                register_code(KC_LSHIFT);
                IS_SHIFT_LOCK_ON = true;
                ergodox_right_led_2_on();
                ergodox_right_led_3_on();
            } else {
                unregister_code(KC_LSHIFT);
                IS_SHIFT_LOCK_ON = false;
                ergodox_right_led_2_off();
                ergodox_right_led_3_off();
            }
        }
        return false;
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
    case ctl_dot_when_tapped_ctl_alt_when_held:
        if(record->event.pressed) {
            CTL_DOT_WHEN_TAPPED_CTL_ALT_WHEN_HELD_TIMER = timer_read32();
            register_code(KC_LCTL);
            register_code(KC_LALT);
        } else {
            uint32_t elapsed = timer_elapsed32(CTL_DOT_WHEN_TAPPED_CTL_ALT_WHEN_HELD_TIMER);
            if(elapsed <= TAPPING_TERM)
            {
                unregister_code(KC_LALT);
                tap_code(KC_DOT);
                unregister_code(KC_LCTL);
            } else {
                unregister_code(KC_LALT);
                unregister_code(KC_LCTL);
            }
        }
        return false;
    case grave_when_tapped_caps_when_held:
        if(record->event.pressed) {
            GRAVE_WHEN_TAPPED_CAPS_WHEN_HELD_TIMER = timer_read32();
            tap_code(KC_CAPS);
        } else {
            uint32_t elapsed = timer_elapsed32(GRAVE_WHEN_TAPPED_CAPS_WHEN_HELD_TIMER);
            if(elapsed <= TAPPING_TERM)
            {
                tap_code(KC_CAPS);
                tap_code(KC_GRV);
            } else {
                tap_code(KC_CAPS);
            }
        }
        return false;
    case minus_when_tapped_caps_when_held:
        if(record->event.pressed) {
            MINUS_WHEN_TAPPED_CAPS_WHEN_HELD_TIMER = timer_read32();
            tap_code(KC_CAPS);
        } else {
            uint32_t elapsed = timer_elapsed32(MINUS_WHEN_TAPPED_CAPS_WHEN_HELD_TIMER);
            if(elapsed <= TAPPING_TERM)
            {
                tap_code(KC_CAPS);
                tap_code(KC_MINS);
            } else {
                tap_code(KC_CAPS);
            }
        }
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
uint32_t layer_state_set_user(uint32_t state) {
  ergodox_board_led_off();
  ergodox_right_led_1_off(); // red
  ergodox_right_led_2_off(); // green
  ergodox_right_led_3_off(); // blue

  uint8_t layer = biton32(state);
  switch (layer) {
      case 0:
        break;
      case 1:
        ergodox_right_led_1_on();
        break;
      case 2:
        ergodox_right_led_2_on();
        break;
      case 3:
        ergodox_right_led_3_on();
        break;
      case 4:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        break;
      case 5:
        ergodox_right_led_1_on();
        ergodox_right_led_3_on();
        break;
      case 6:
          /* reserved for shift lock */
          /* ergodox_right_led_2_on(); */
          /* ergodox_right_led_3_on(); */
        break;
      case 7:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        break;
      default:
        break;
    }

  return state;
};

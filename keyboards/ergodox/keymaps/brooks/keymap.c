#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define EPRM M(1) // Macro 1: Reset EEPROM

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  | LAY1 |           | LAY2 |   6  |   7  |   8  |   9  |   0  |   =    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | Tab    |   Q  |   W  |   E  |   R  |   T  |  [   |           |   ]  |   Y  |   U  |   I  |   O  |   P  |   -    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |Ctl/esc |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |   '    |
 * |--------+------+------+------+------+------|  \   |           |   /  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  Up  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      | LAlt | LAlt | LGui | LGui |                                       | RGui | RAlt | Rght | Down | Left |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | ESC  | Mute |       | Pl/Ps|  Esc |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Prev |       | Next |      |      |
 *                                 | Space|Backsp|------|       |------| Enter| Space|
 *                                 |      |ace   | Vol- |       | Vol+ |      |      |
 *                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default

        // Left Hand
        KC_GRV,         KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_FN0,
        KC_TAB,         KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_LBRACKET,
        CTL_T(KC_ESC),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,
        KC_LSFT,        KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_BSLASH,
        KC_TRNS,        KC_LALT,  KC_LALT,  KC_LGUI,  KC_LGUI,

                                                                KC_ESC,   KC_MUTE,
                                                                          KC_MPRV,
                                                      KC_SPC,   KC_BSPC,  KC_VOLD,

        // Right Hand
        KC_FN1,         KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_PEQL,
        KC_RBRC,        KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_MINS,
                        KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
        KC_SLSH,        KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_UP,    KC_RSFT,
                                  KC_RGUI,  KC_RALT,  KC_LEFT,  KC_DOWN,  KC_RGHT,

        KC_MPLY,  KC_ESC,
        KC_MNXT,
        KC_VOLU,  KC_ENT, KC_SPC
    ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |Version |  F1  |  F2  |  F3  |  F4  |  F5  | LAY1 |           | LAY2 |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |   F12  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      | MsUp |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |MsLeft|MsDown|MsRght|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      | EPRM |       | EPRM |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 | Lclk | Lclk |------|       |------| Rclk | Rclk |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = KEYMAP(
       // Left Hand
       M(0),            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_FN0,
       KC_TRNS,         KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
       KC_TRNS,         KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
       KC_TRNS,         KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
       KC_TRNS,         KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,

                                                                KC_TRNS,  EPRM,
                                                                          KC_TRNS,
                                                      KC_BTN1,  KC_BTN1,  KC_TRNS,
       // Right hand
       KC_FN1,          KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
       KC_TRNS,         KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_F12,
                        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
       KC_TRNS,         KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_MS_U,  KC_TRNS,
                                  KC_TRNS,  KC_TRNS,  KC_MS_L,  KC_MS_D,  KC_MS_R,
       EPRM,    KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_BTN2, KC_BTN2
),
};

const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_LAYER_SET(BASE, true),
    [1] = ACTION_LAYER_SET(SYMB, true)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
        case 1:
        if (record->event.pressed) { // For resetting EEPROM
          eeconfig_init();
        }
        break;
      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        default:
            // none
            break;
    }

};

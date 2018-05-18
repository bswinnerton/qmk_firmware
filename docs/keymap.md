# Keymap Overview

QMK keymaps are defined inside a C source file. The data structure is an array of arrays. The outer array is a list of layer arrays while the inner layer array is a list of keys. Most keyboards define a `KEYMAP()` macro to help you create this array of arrays.


## Keymap and Layers
In QMK,  **`const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS]`** holds multiple **layers** of keymap information in **16 bit** data holding the **action code**. You can define **32 layers** at most.

For trivial key definitions, the higher 8 bits of the **action code** are all 0 and the lower 8 bits holds the USB HID usage code generated by the key as **keycode**.

Respective layers can be validated simultaneously. Layers are indexed with 0 to 31 and higher layer has precedence.

    Keymap: 32 Layers                   Layer: action code matrix
    -----------------                   ---------------------
    stack of layers                     array_of_action_code[row][column]
           ____________ precedence               _______________________
          /           / | high                  / ESC / F1  / F2  / F3   ....
      31 /___________// |                      /-----/-----/-----/-----
      30 /___________// |                     / TAB /  Q  /  W  /  E   ....
      29 /___________/  |                    /-----/-----/-----/-----
       :   _:_:_:_:_:__ |               :   /LCtrl/  A  /  S  /  D   ....
       :  / : : : : : / |               :  /  :     :     :     :
       2 /___________// |               2 `--------------------------
       1 /___________// |               1 `--------------------------
       0 /___________/  V low           0 `--------------------------


Sometimes, the action code stored in keymap may be referred as keycode in some documents due to the TMK history.

### Keymap Layer Status
The state of the Keymap layer is determined by two 32 bit parameters:

* **`default_layer_state`** indicates a base keymap layer (0-31) which is always valid and to be referred (the default layer).
* **`layer_state`** has current on/off status of each layer in its bits.

Keymap layer '0' is usually the `default_layer`, with other layers initially off after booting up the firmware, although this can configured differently in `config.h`. It is useful to change `default_layer` when you completely switch a key layout, for example, if you want to switch to Colemak instead of Qwerty.

    Initial state of Keymap          Change base layout
    -----------------------          ------------------

      31                               31
      30                               30
      29                               29
       :                                :
       :                                :   ____________
       2   ____________                 2  /           /
       1  /           /              ,->1 /___________/
    ,->0 /___________/               |  0
    |                                |
    `--- default_layer = 0           `--- default_layer = 1
         layer_state   = 0x00000001       layer_state   = 0x00000002

On the other hand, you can change `layer_state` to overlay the base layer with other layers for features such as navigation keys, function keys (F1-F12), media keys, and/or special actions.

    Overlay feature layer
    ---------------------      bit|status
           ____________        ---+------
      31  /           /        31 |   0
      30 /___________// -----> 30 |   1
      29 /___________/  -----> 29 |   1
       :                        : |   :
       :   ____________         : |   :
       2  /           /         2 |   0
    ,->1 /___________/  ----->  1 |   1
    |  0                        0 |   0
    |                                 +
    `--- default_layer = 1            |
         layer_state   = 0x60000002 <-'



### Layer Precedence and Transparency
Note that ***higher layer has higher priority on stack of layers***, namely firmware falls down from top layer to bottom to look up keycode. Once it spots keycode other than **`KC_TRNS`**(transparent) on a layer it stops searching and lower layers aren't referred.

You can place `KC_TRANS` on overlay layer changes just part of layout to fall back on lower or base layer.
Key with `KC_TRANS` (`KC_TRNS` and `_______` are the alias) doesn't has its own keycode and refers to lower valid layers for keycode, instead.

## Anatomy of a `keymap.c`

For this example we will walk through an [older version of the default Clueboard 66% keymap](https://github.com/qmk/qmk_firmware/blob/ca01d94005f67ec4fa9528353481faa622d949ae/keyboards/clueboard/keymaps/default/keymap.c). You'll find it helpful to open that file in another browser window so you can look at everything in context.

There are 3 main sections of a `keymap.c` file you'll want to concern yourself with:

* [The Definitions](#definitions)
* [The Layer/Keymap Datastructure](#layers-and-keymaps)
* [Custom Functions](#custom-functions), if any

### Definitions

At the top of the file you'll find this:

    #include "clueboard.h"

    // Helpful defines
    #define GRAVE_MODS  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT)|MOD_BIT(KC_LGUI)|MOD_BIT(KC_RGUI)|MOD_BIT(KC_LALT)|MOD_BIT(KC_RALT))
    #define _______ KC_TRNS

    // Each layer gets a name for readability.
    // The underscores don't mean anything - you can
    // have a layer called STUFF or any other name.
    // Layer names don't all need to be of the same
    // length, and you can also skip them entirely
    // and just use numbers.
    #define _BL 0
    #define _FL 1
    #define _CL 2

These are some handy definitions we can use when building our keymap and our custom function. The `GRAVE_MODS` definition will be used later in our custom function. The `_______` define makes it easier to see what keys a layer is overriding, while the `_BL`, `_FL`, and `_CL` defines make it easier to refer to each of our layers.

### Layers and Keymaps

The main part of this file is the `keymaps[]` definition. This is where you list your layers and the contents of those layers. This part of the file begins with this definition:

    const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

After this you'll find a list of KEYMAP() macros. A KEYMAP() is simply a list of keys to define a single layer. Typically you'll have one or more "base layers" (such as QWERTY, Dvorak, or Colemak) and then you'll layer on top of that one or more "function" layers. Due to the way layers are processed you can't overlay a "lower" layer on top of a "higher" layer.

`keymaps[][MATRIX_ROWS][MATRIX_COLS]` in QMK holds the 16 bit action code (sometimes referred as the quantum keycode) in it.  For the keycode representing typical keys, its high byte is 0 and its low byte is the USB HID usage ID for keyboard.

> TMK from which QMK was forked uses `const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS]` instead and holds the 8 bit keycode.  Some keycode values are reserved to induce execution of certain action codes via the `fn_actions[]` array.

#### Base Layer

Here is an example of the Clueboard's base layer:

      /* Keymap _BL: Base Layer (Default Layer)
       */
    [_BL] = KEYMAP(
      F(0),    KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,     KC_MINS,  KC_EQL,   KC_GRV,  KC_BSPC,          KC_PGUP, \
      KC_TAB,  KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,                   KC_PGDN, \
      KC_CAPS, KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,                             \
      KC_LSFT, KC_NUBS, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,   KC_SLSH,  KC_RO,    KC_RSFT,          KC_UP,            \
      KC_LCTL, KC_LGUI, KC_LALT, KC_MHEN,          KC_SPC,KC_SPC,                        KC_HENK,  KC_RALT,  KC_RCTL,  MO(_FL), KC_LEFT, KC_DOWN, KC_RGHT),

Some interesting things to note about this:

* From a C source point of view it's only a single array, but we have embedded whitespace to more easily visualize where each key is on the physical device.
* Plain keyboard scancodes are prefixed with KC_, while "special" keys are not.
* The upper left key activates custom function 0 (`F(0)`)
* The "Fn" key is defined with `MO(_FL)`, which moves to the `_FL` layer while that key is being held down.

#### Function Overlay Layer

Our function layer is, from a code point of view, no different from the base layer. Conceptually, however, you will build that layer as an overlay, not a replacement. For many people this distinction does not matter, but as you build more complicated layering setups it matters more and more.

    [_FL] = KEYMAP(
      KC_GRV,  KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______, KC_DEL,           BL_STEP, \
      _______, _______, _______,_______,_______,_______,_______,_______,KC_PSCR,KC_SLCK, KC_PAUS,  _______,  _______,  _______,                   _______, \
      _______, _______, MO(_CL),_______,_______,_______,_______,_______,_______,_______, _______,  _______,  _______,  _______,                           \
      _______, _______, _______,_______,_______,_______,_______,_______,_______,_______, _______,  _______,  _______,  _______,          KC_PGUP,         \
      _______, _______, _______, _______,        _______,_______,                        _______,  _______,  _______,  MO(_FL), KC_HOME, KC_PGDN, KC_END),

Some interesting things to note:

* We have used our `_______` definition to turn `KC_TRNS` into `_______`. This makes it easier to spot the keys that have changed on this layer.
* While in this layer if you press one of the `_______` keys it will activate the key in the next lowest active layer.

### Custom Functions

At the bottom of the file we've defined a single custom function. This function defines a key that sends `KC_ESC` when pressed without modifiers and `KC_GRAVE` when modifiers are held. There are a couple pieces that need to be in place for this to work, and we will go over both of them.

#### `fn_actions[]`

We define the `fn_actions[]` array to point to custom functions. `F(N)` in a keymap will call element N of that array. For the Clueboard's that looks like this:

    const uint16_t PROGMEM fn_actions[] = {
      [0] = ACTION_FUNCTION(0),  // Calls action_function()
    };

In this case we've instructed QMK to call the `ACTION_FUNCTION` callback, which we will define in the next section.

> This `fn_actions[]` interface is mostly for backward compatibility.  In QMK, you don't need to use `fn_actions[]`.  You can directly use `ACTION_FUNCTION(N)` or any other action code value itself normally generated by the macro in `keymaps[][MATRIX_ROWS][MATRIX_COLS]`.  N in `F(N)` can only be 0 to 31.  Use of the action code directly in `keymaps` unlocks this limitation.

You can get a full list of Action Functions in [action_code.h](https://github.com/qmk/qmk_firmware/blob/master/tmk_core/common/action_code.h). 

#### `action_function()`

To actually handle the keypress event we define an `action_function()`. This function will be called when the key is pressed, and then again when the key is released. We have to handle both situations within our code, as well as determining whether to send/release `KC_ESC` or `KC_GRAVE`.

    void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
      static uint8_t mods_pressed;

      switch (id) {
        case 0:
          /* Handle the combined Grave/Esc key
           */
          mods_pressed = get_mods()&GRAVE_MODS; // Check to see what mods are pressed

          if (record->event.pressed) {
            /* The key is being pressed.
             */
            if (mods_pressed) {
              add_key(KC_GRV);
              send_keyboard_report();
            } else {
              add_key(KC_ESC);
              send_keyboard_report();
            }
          } else {
            /* The key is being released.
             */
            if (mods_pressed) {
              del_key(KC_GRV);
              send_keyboard_report();
            } else {
              del_key(KC_ESC);
              send_keyboard_report();
            }
          }
          break;
      }
    }

# Nitty Gritty Details

This should have given you a basic overview for creating your own keymap. For more details see the following resources:

* [Keycodes](keycodes.md)
* [Keymap FAQ](faq_keymap.md)

We are actively working to improve these docs. If you have suggestions for how they could be made better please [file an issue](https://github.com/qmk/qmk_firmware/issues/new)!
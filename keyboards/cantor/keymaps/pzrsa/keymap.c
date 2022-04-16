// Copyright 2022 Diego Palacios (@diepala)
// SPDX-License-Identifier: GPL-2.0

#include QMK_KEYBOARD_H

// Tap Dance keycodes
enum tap_dance_codes {
 CMD_SPC,
};

typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

static tap dance_state = {
    .is_press_action = true,
    .step = 0
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // base layer
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LOPT,  KC_Z,   KC_X,    KC_C,      KC_V,    KC_B,                         KC_N,   KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RCTL,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                         KC_LCMD, TD(CMD_SPC), MO(1),     MO(2), KC_ENT, KC_RSFT
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB, XXXXXXX, XXXXXXX, KC_TILD, KC_MINS, KC_LBRC,                      KC_RBRC, KC_EQL, KC_BSLS,  XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LOPT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LPRN,                      KC_RPRN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RCTL,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, _______,     XXXXXXX, KC_RSFT,KC_RCMD
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                        XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX,XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX,                      KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LOPT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RCTL,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCMD, KC_LSFT, XXXXXXX,     _______,XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),
};

void on_cmd_spc(qk_tap_dance_state_t *state, void *user_data);
uint8_t cmd_spc_dance_step(qk_tap_dance_state_t *state);
void cmd_spc_finished(qk_tap_dance_state_t *state, void *user_data);
void cmd_spc_reset(qk_tap_dance_state_t *state, void *user_data);

void on_cmd_spc(qk_tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
        tap_code16(KC_SPACE);
    }
    if(state->count > 3) {
        tap_code16(KC_SPACE);
    }
}

uint8_t cmd_spc_dance_step(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void cmd_spc_finished(qk_tap_dance_state_t *state, void *user_data) {
    dance_state.step = cmd_spc_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP: register_code16(KC_SPACE); break;
        case SINGLE_HOLD: register_code16(LGUI(KC_SPACE)); break;
        case DOUBLE_HOLD: register_code16(KC_SPACE); break;
    }
}

void cmd_spc_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (dance_state.step) {
        case SINGLE_TAP: unregister_code16(KC_SPACE); break;
        case SINGLE_HOLD: unregister_code16(LGUI(KC_SPACE)); break;
        case DOUBLE_HOLD: unregister_code16(KC_SPACE); break;
    }
    dance_state.step = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [CMD_SPC] = ACTION_TAP_DANCE_FN_ADVANCED(on_cmd_spc, cmd_spc_finished, cmd_spc_reset),
};

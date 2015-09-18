/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Sung Joon Won <sungjoon.won@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CALL_MANAGER_EXTENSION_H__
#define __CALL_MANAGER_EXTENSION_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <glib.h>
#include <tizen.h>

// TODO:
// Please add doxygen comment for all header files.

typedef struct __cm_call_data cm_call_data_t;
typedef struct __cm_call_event_data cm_call_event_data_t;
typedef struct __cm_conf_call_data cm_conf_call_data_t;

/*
 * This enum is inline with cm_telephony_call_state_e, so update both enum's together.
 * Also the state of the call can be retrieved from the call_data by the UI which has the call-state enum which can be compared with 'cm_call_state_e' to
 * fetch the call state by the UI.
 */
typedef enum _call_state {
	CM_CALL_STATE_IDLE,		/**< Call is in idle state */
	CM_CALL_STATE_ACTIVE,		/**< Call is in connected and conversation state */
	CM_CALL_STATE_HELD,			/**< Call is in held state */
	CM_CALL_STATE_DIALING,		/**< Call is in dialing state */
	CM_CALL_STATE_ALERT,		/**< Call is in alerting state */
	CM_CALL_STATE_INCOMING,		/**< Call is in incoming state */
	CM_CALL_STATE_WAITING,		/**< Call is in answered state, and waiting for connected indication event */
	CM_CALL_STATE_MAX			/**< Call state unknown */
} cm_call_state_e;

/*This enum is inline with cm_telephony_call_type_e and callmgr_call_type_e, so update all enum's together*/
typedef enum _call_type {
	CM_CALL_TYPE_VOICE,		/**< Voice call type. */
	CM_CALL_TYPE_VIDEO,		/**< Video call type. */
	CM_CALL_TYPE_INVALID,	/**< Invalid call type */
} cm_call_type_e;

/*This enum is inline with cm_telephony_call_domain_e and callmgr_call_domain_e, so update all enum's together*/
typedef enum _call_domain {
	CM_CALL_DOMAIN_CS,		/**< CS call domain. */
	CM_CALL_DOMAIN_PS,		/**< PS call domain. */
	CM_CALL_DOMAIN_HFP,		/**< Wearable call domain. */
} cm_call_domain_e;

/*This enum is inline with cm_telephony_call_direction_e, so update both enum's together*/
typedef enum {
	CM_CALL_DIRECTION_MO,
	CM_CALL_DIRECTION_MT,
} cm_call_direction_e;

/*This enum is inline with callmgr_call_event_e, so update both enum's together*/
typedef enum _call_event {
	CM_CALL_EVENT_IDLE = 0,
	CM_CALL_EVENT_DIALING,
	CM_CALL_EVENT_ACTIVE,
	CM_CALL_EVENT_HELD,
	CM_CALL_EVENT_ALERT,
	CM_CALL_EVENT_INCOMING,
	CM_CALL_EVENT_WAITING,
	CM_CALL_EVENT_JOIN,
	CM_CALL_EVENT_SPLIT,
	CM_CALL_EVENT_SWAPPED,
	CM_CALL_EVENT_RETRIEVED,
	CM_CALL_EVENT_SAT_CALL_CONTROL,
} cm_call_event_e;

typedef enum _dial_status {
	CM_DIAL_SUCCESS = 0,
	CM_DIAL_CANCEL,
	CM_DIAL_FAIL,
	CM_DIAL_FAIL_SS,
	CM_DIAL_FAIL_FDN,
	CM_DIAL_FAIL_FLIGHT_MODE,
} cm_dial_status_e;

typedef enum {
	CM_VR_STATUS_STARTED = 0,
	CM_VR_STATUS_STOPPED,
} cm_vr_status_e;

typedef enum {
	CM_VR_STATUS_EXTRA_START_TYPE = 0x00,
	CM_VR_STATUS_EXTRA_START_TYPE_NORMAL,		/**< Normal recording*/
	CM_VR_STATUS_EXTRA_START_TYPE_ANSWER_MSG,		/**< Answering message*/
	CM_VR_STATUS_EXTRA_START_TYPE_MAX = 0x0f,

	CM_VR_STATUS_EXTRA_STOP_TYPE = 0x10,
	CM_VR_STATUS_EXTRA_STOP_TYPE_BY_NORMAL,						/**< BY_NORMAL*/
	CM_VR_STATUS_EXTRA_STOP_TYPE_BY_MAX_SIZE,					/**< by MAX_SIZE*/
	CM_VR_STATUS_EXTRA_STOP_TYPE_BY_NO_FREE_SPACE,		/**< BY_NO_FREE_SPACE*/
	CM_VR_STATUS_EXTRA_STOP_TYPE_BY_TIME_LIMIT,				/**< BY_TIME_LIMIT*/
	CM_VR_STATUS_EXTRA_STOP_TYPE_ERROR,								/**< ERROR*/
	CM_VR_STATUS_EXTRA_STOP_TYPE_MAX = 0x1f,
} cm_vr_status_extra_type_e;

/**
 * This enum defines call answer types for accepting the Incoming Call
 */
typedef enum _call_answer_type_t {
	CALL_ANSWER_TYPE_NORMAL = 0,				/**< Only single call exist, Accept the Incoming call*/
	CALL_ANSWER_TYPE_HOLD_ACTIVE_AND_ACCEPT,	/**< Put the active call on hold and accepts the call*/
	CALL_ANSWER_TYPE_RELEASE_ACTIVE_AND_ACCEPT,	/**< Releases the active call and accept the call*/
	CALL_ANSWER_TYPE_RELEASE_HOLD_AND_ACCEPT,	/**< Releases the held call and accept the call*/
	CALL_ANSWER_TYPE_RELEASE_ALL_AND_ACCEPT		/**< Releases all calls and accept the call*/
} cm_call_answer_type_e;

/**
 * This enum defines call release type for releasing the call.
 */
typedef enum {
	CALL_RELEASE_TYPE_BY_CALL_HANDLE = 0, /**< Release call using given call_handle*/
	CALL_RELEASE_TYPE_ALL_CALLS,			/**< Release all Calls*/
	CALL_RELEASE_TYPE_ALL_HOLD_CALLS,	/**< Releases all hold calls*/
	CALL_RELEASE_TYPE_ALL_ACTIVE_CALLS,	/**< Releases all active calls*/
} cm_call_release_type_e;

typedef enum {
	CM_DTMF_INDI_IDLE_E = 0,
	CM_DTMF_INDI_PROGRESSING_E,
	CM_DTMF_INDI_WAIT_E,
} cm_dtmf_indi_type_e;

/**
 * This enum defines audio state type.
 */
typedef enum {
	CM_AUDIO_STATE_NONE_E,		/**< none */
	CM_AUDIO_STATE_SPEAKER_E,	/**< System LoudSpeaker path */
	CM_AUDIO_STATE_RECEIVER_E,	/**< System Receiver*/
	CM_AUDIO_STATE_EARJACK_E,	/**< Earjack path*/
	CM_AUDIO_STATE_BT_E			/**< System BT Headset path */
} cm_audio_state_type_e;

typedef enum {
	CM_DTMF_RESP_CANCEL_E = 0,
	CM_DTMF_RESP_CONTINUE_E,
} cm_dtmf_resp_type_e;

typedef enum {
	CM_SIM_SLOT_1_E = 0,
	CM_SIM_SLOT_2_E,
	CM_SIM_SLOT_DEFAULT_E,		/**< Follow system configuration*/
} cm_multi_sim_slot_type_e;

typedef enum {
	CM_LCD_TIMEOUT_SET = 1,
	CM_LCD_TIMEOUT_UNSET,
	CM_LCD_TIMEOUT_LOCKSCREEN_SET, /*After lock-screen comes in Connected state LCD goes to OFF in 5 secs*/
	CM_LCD_TIMEOUT_KEYPAD_SET, /*When Keypad is ON, LCD goes to DIM in 3 secs then goes to OFF in 5 secs*/
	CM_LCD_TIMEOUT_DEFAULT,
} cm_lcd_timeout_e;

typedef enum {
	CM_LCD_OFF = 1,
	CM_LCD_ON,
	CM_LCD_ON_LOCK,
	CM_LCD_ON_UNLOCK,
	CM_LCD_OFF_SLEEP_LOCK,
	CM_LCD_OFF_SLEEP_UNLOCK,
} cm_lcd_control_state_e;

typedef enum {
	CM_CALL_ENDCAUSE_CALL_ENDED,						/**< Call ended */

	CM_CALL_ENDCAUSE_CALL_DISCONNECTED,				/**< Call disconnected */
	CM_CALL_ENDCAUSE_CALL_SERVICE_NOT_ALLOWED,		/**< Service not allowed */
	CM_CALL_ENDCAUSE_CALL_BARRED,						/**< Call barred */
	CM_CALL_ENDCAUSE_NO_SERVICE,						/**< No Service */
	CM_CALL_ENDCAUSE_NW_BUSY,							/**< Network busy */

	CM_CALL_ENDCAUSE_NW_FAILED,						/**< Network failed */
	CM_CALL_ENDCAUSE_NO_ANSWER,						/**< No anwer from other party */
	CM_CALL_ENDCAUSE_NO_CREDIT,						/**< No credit available */
	CM_CALL_ENDCAUSE_REJECTED,							/**< Call rejected */
	CM_CALL_ENDCAUSE_USER_BUSY,						/**< user busy */

	CM_CALL_ENDCAUSE_WRONG_GROUP,					/**< Wrong group */
	CM_CALL_ENDCAUSE_CALL_NOT_ALLOWED,				/**< Call not allowed */
	CM_CALL_ENDCAUSE_TAPI_ERROR,						/**< Tapi error */
	CM_CALL_ENDCAUSE_CALL_FAILED,						/**< Call Failed */
	CM_CALL_ENDCAUSE_NO_USER_RESPONDING,				/**< User not responding */

	CM_CALL_ENDCAUSE_USER_ALERTING_NO_ANSWER,		/**< User Alerting No Answer */
	CM_CALL_ENDCAUSE_SERVICE_TEMP_UNAVAILABLE,		/**< Circuit Channel Unavailable,Network is out of Order,Switching equipment congestion,Temporary Failure */
	CM_CALL_ENDCAUSE_USER_UNAVAILABLE,				/**< Called Party Rejects the Call */
	CM_CALL_ENDCAUSE_INVALID_NUMBER_FORMAT,			/**< Entered number is invalid or incomplete */
	CM_CALL_ENDCAUSE_NUMBER_CHANGED,				/**< Entered number has been changed */

	CM_CALL_ENDCAUSE_UNASSIGNED_NUMBER,				/**< Unassigned/Unallocated number*/
	CM_CALL_ENDCAUSE_USER_DOESNOT_RESPOND,			/**< Called Party does not respond*/
	CM_CALL_ENDCAUSE_IMEI_REJECTED,			/**< Called Party does not respond*/
	CM_CALL_ENDCAUSE_FIXED_DIALING_NUMBER_ONLY,			/**< FDN Number only */
	CM_CALL_ENDCAUSE_REJ_SAT_CALL_CTRL,				/**< SAT call control reject */
	CM_CALL_ENDCAUSE_MAX,
}cm_call_end_cause_type_e;

typedef enum {
	CM_CALL_NAME_MODE_NONE,			/**<  None */
	CM_CALL_NAME_MODE_UNKNOWN,			/**<  Unknown*/
	CM_CALL_NAME_MODE_PRIVATE,		/**<  Private*/
	CM_CALL_NAME_MODE_PAYPHONE,			/**<  Payphone*/
	CM_CALL_NAME_MODE_MAX,
} cm_name_mode_e;

typedef void (*cm_dial_status_cb) (cm_dial_status_e dial_status, void *user_data);
typedef void (*cm_call_event_cb) (cm_call_event_e call_event, cm_call_event_data_t* event_data, void *user_data);
typedef void (*cm_dtmf_indi_cb) (cm_dtmf_indi_type_e indi_type, char* dtmf_number, void *user_data);
typedef void (*cm_audio_state_changed_cb) (cm_audio_state_type_e audio_state, void *user_data);
typedef void (*cm_go_foreground_cb) (void *user_data);
typedef void (*cm_voice_record_status_cb) (cm_vr_status_e vr_status, cm_vr_status_extra_type_e extra_type, void *user_data);

int cm_enable_recovery (cm_client_h handle, char* appid);
int cm_dial_call(cm_client_h handle, char*number, cm_call_type_e call_type, cm_multi_sim_slot_type_e sim_slot);
int cm_end_call(cm_client_h handle, unsigned int call_id, cm_call_release_type_e release_type);
int cm_swap_call(cm_client_h handle);
int cm_hold_call(cm_client_h handle);
int cm_unhold_call(cm_client_h handle);
int cm_join_call(cm_client_h handle);
int cm_split_call(cm_client_h handle, unsigned int call_id);
int cm_transfer_call(cm_client_h handle);
int cm_answer_call(cm_client_h handle, cm_call_answer_type_e ans_type);

int cm_speaker_on (cm_client_h handle);
int cm_speaker_off (cm_client_h handle);
int cm_bluetooth_on (cm_client_h handle);
int cm_bluetooth_off (cm_client_h handle);
int cm_set_extra_vol (cm_client_h handle, gboolean is_extra_vol);
int cm_set_mute_state(cm_client_h handle, gboolean is_mute_state);
int cm_start_dtmf(cm_client_h handle, unsigned char dtmf_digit);
int cm_stop_dtmf(cm_client_h handle);
int cm_burst_dtmf(cm_client_h handle, char *dtmf_digits);
int cm_send_dtmf_resp(cm_client_h handle, cm_dtmf_resp_type_e resp_type);
int cm_get_audio_state(cm_client_h handle, cm_audio_state_type_e *audio_state);
int cm_activate_ui(cm_client_h handle);
int cm_set_lcd_timeout(cm_client_h handle, cm_lcd_timeout_e timeout);
int cm_get_all_call_list(cm_client_h handle, GSList **call_list);
int cm_get_conference_call_list(cm_client_h handle, GSList **call_list);
int cm_get_all_calldata(cm_client_h handle, cm_call_data_t **incom, cm_call_data_t **active, cm_call_data_t **held);
int cm_start_voice_record(cm_client_h handle, char *call_num);
int cm_stop_voice_record(cm_client_h handle);

int cm_set_call_event_cb(cm_client_h handle, cm_call_event_cb cb, void *user_data);
int cm_unset_call_event_cb(cm_client_h handle);
int cm_set_dial_status_cb(cm_client_h handle, cm_dial_status_cb cb, void *user_data);
int cm_unset_dial_status_cb(cm_client_h handle);
int cm_set_audio_state_changed_cb(cm_client_h handle, cm_audio_state_changed_cb cb, void *user_data);
int cm_unset_audio_state_changed_cb (cm_client_h handle);
int cm_set_dtmf_indication_cb(cm_client_h handle, cm_dtmf_indi_cb cb, void *user_data);
int cm_unset_dtmf_indication_cb (cm_client_h handle);
int cm_set_foreground_cb(cm_client_h handle, cm_go_foreground_cb cb, void *user_data);
int cm_unset_foreground_cb (cm_client_h handle);
int cm_set_voice_record_status_cb(cm_client_h handle, cm_voice_record_status_cb cb, void *user_data);
int cm_unset_voice_record_status_cb (cm_client_h handle);

/* GSList of cm_call_data_t structures */
int cm_call_data_free(cm_call_data_t *call_data);
int cm_call_data_get_call_id(cm_call_data_t *call_data, unsigned int *call_id);
int cm_call_data_get_call_direction(cm_call_data_t *call_data, cm_call_direction_e *call_direction);
int cm_call_data_get_call_number(cm_call_data_t *call_data, char **call_number);
int cm_call_data_get_calling_name(cm_call_data_t *call_data, char **calling_name);
int cm_call_data_get_call_type(cm_call_data_t *call_data, cm_call_type_e *call_type);
int cm_call_data_get_call_state(cm_call_data_t *call_data, cm_call_state_e *call_state);
int cm_call_data_get_call_domain(cm_call_data_t *call_data, cm_call_domain_e *call_domain);
int cm_call_data_get_person_id(cm_call_data_t *call_data, int *person_id);
int cm_call_data_get_call_member_count(cm_call_data_t *call_data, int *member_count);
int cm_call_data_is_emergency_call(cm_call_data_t *call_data, gboolean *is_emergency);
int cm_call_data_is_voicemail_number(cm_call_data_t *call_data, gboolean *is_voicemail_number);
int cm_call_data_get_start_time(cm_call_data_t *call_data, long *start_time);
int cm_call_data_get_name_mode(cm_call_data_t *call_data, cm_name_mode_e *name_mode);

int cm_call_event_data_get_call_id(cm_call_event_data_t *call_event_data, unsigned int *call_id);
int cm_call_event_data_get_sim_slot(cm_call_event_data_t *call_event_data, cm_multi_sim_slot_type_e *sim_slot);
int cm_call_event_data_get_incom_call(cm_call_event_data_t *call_event_data, cm_call_data_t **incom_call);
int cm_call_event_data_get_active_call(cm_call_event_data_t *call_event_data, cm_call_data_t **active_call);
int cm_call_event_data_get_held_call(cm_call_event_data_t *call_event_data, cm_call_data_t **held_call);
int cm_call_event_data_get_end_cause(cm_call_event_data_t *call_event_data, cm_call_end_cause_type_e *end_cause);

int cm_conf_call_data_get_call_id(cm_conf_call_data_t *conf_call, unsigned int *call_id);
int cm_conf_call_data_get_call_number(cm_conf_call_data_t *conf_call, char **call_number);
int cm_conf_call_data_get_person_id(cm_conf_call_data_t *conf_call, int *person_id);
int cm_conf_call_data_get_name_mode(cm_conf_call_data_t *conf_call, cm_name_mode_e *name_mode);
int cm_conf_call_data_free(cm_conf_call_data_t *conf_call);

int cm_set_lcd_state(cm_lcd_control_state_e state);
int cm_start_alert(cm_client_h handle);

#ifdef __cplusplus
 }
#endif

#endif /* __CALL_MANAGER_EXTENSION_H__ */


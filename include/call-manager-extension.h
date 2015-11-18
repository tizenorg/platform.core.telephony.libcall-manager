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

/**
 * @brief Sets the appid to watch appid is appeared or vanished.
 *
 * @details This API is used for ending all calls when call-ui got crashed.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] appid  The appid to be watched
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_enable_recovery (cm_client_h handle, char* appid);

/**
 * @brief Originates MO call setup.
 *
 * @details TBD.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] number  The destination phone number
 * @param[in] call_type  The type of the call (#cm_call_type_e)
 * @param[in] sim_slot  The sim card slot to be used for MO call setup (#cm_multi_sim_slot_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 * @see #cm_answer_call #cm_end_call
 */
int cm_dial_call(cm_client_h handle, char*number, cm_call_type_e call_type, cm_multi_sim_slot_type_e sim_slot);

/**
 * @brief Releases the call identified by the call id irrespective of whether the call is in the hold or active state.
 *
 * @details TBD.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] call_id  A unique id that refers to the call
 * @param[in] release_type  The end call type (#cm_call_release_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 * @see cm_dial_call()
 * @see cm_answer_call()
 */
int cm_end_call(cm_client_h handle, unsigned int call_id, cm_call_release_type_e release_type);

/**
 * @brief Swaps calls. A user can place all active calls and accept other held calls with this function.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_swap_call(cm_client_h handle);

/**
 * @brief Puts the given call which is in active state on hold.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 * @see cm_unhold_call()
 */
int cm_hold_call(cm_client_h handle);

/**
 * @brief Retrieves the call being held.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_unhold_call(cm_client_h handle);

/**
 * @brief Joins the given two calls (one call in the active conversation state and the other call in the held state) into conference.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 * @see cm_split_call()
 */
int cm_join_call(cm_client_h handle);

/**
 * @brief Triggers splitting a private call from a multiparty call.
 *
 * @details This API allows creating a private communication with one of the remote parties in a
 *          multiparty session.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] call_id  The id of the call to be made private \n
 *                       The call handle referring to the call that is to be split from the conference
 *                       (call to be made private)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 * @see cm_join_call()
 */
int cm_split_call(cm_client_h handle, unsigned int call_id);

/**
 * @brief Triggers making an explicit call transfer by connecting the two parties where one party is being
 *        active (active state) and another party is being held (held state).
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_transfer_call(cm_client_h handle);

/**
 * @brief Supports answering the incoming call by accepting or rejecting the call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] ans_type  The answer type (#cm_call_answer_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 * @see cm_dial_call()
 * @see cm_end_call()
 */
int cm_answer_call(cm_client_h handle, cm_call_answer_type_e ans_type);


/**
 * @brief Sets the audio session route to speaker.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 */
int cm_speaker_on (cm_client_h handle);

/**
 * @brief Sets the audio session route to earjack or built in receiver.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 */
int cm_speaker_off (cm_client_h handle);

/**
 * @brief Opens the bluetooth SCO.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 */
int cm_bluetooth_on (cm_client_h handle);

/**
 * @brief Closes the bluetooth SCO.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 */
int cm_bluetooth_off (cm_client_h handle);

/**
 * @brief Sets the extra volume.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] is_extra_vol  The extra volume status
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 */
int cm_set_extra_vol (cm_client_h handle, gboolean is_extra_vol);

/**
 * @brief Sets the audio TX mute status.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] is_mute_state  The tx mute status
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 *
 */
int cm_set_mute_state(cm_client_h handle, gboolean is_mute_state);

/**
 * @brief Starts continuous DTMF by sending a single digit during the call.
 *
 * @details DTMF is an abbreviation for Dual-tone-multi-frequency. It is used for telecommunication signaling\n
 *          over telephone lines in the voice-frequency band between UE and other communication devices.\n
 *          For example UE sends a DTMF tone to the server to choose from options which the server provides.\n
 *          If the UE is not within a call, this function will fail with an error code.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] dtmf_digit The DTMF digit to be sent
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 *      An active call should be present.
 * @post The should be released by using #cm_deinit()
 *
 */
int cm_start_dtmf(cm_client_h handle, unsigned char dtmf_digit);

/**
 * @brief Stops continuous DTMF during the call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_stop_dtmf(cm_client_h handle);

/**
 * @brief Sends one or more DTMF digits during the call. (3GPP2 specific)
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_burst_dtmf(cm_client_h handle, char *dtmf_digits);

/**
 * @brief Sends one or more DTMF digits during the call. (3GPP2 specific)
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_send_dtmf_resp(cm_client_h handle, cm_dtmf_resp_type_e resp_type);

/**
 * @brief Gets the current audio path.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[out] audio_state The current path of the call audio
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_get_audio_state(cm_client_h handle, cm_audio_state_type_e *audio_state);

/**
 * @brief Shows the view of call-ui.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_activate_ui(cm_client_h handle);

/**
 * @brief Sets LCD timeout values.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] timeout  The type of the LCD timeout (#cm_lcd_timeout_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_set_lcd_timeout(cm_client_h handle, cm_lcd_timeout_e timeout);

/**
 * @brief Gets the list of the current call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[out] call_list  List of call information for existing calls
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_get_all_call_list(cm_client_h handle, GSList **call_list);

/**
 * @brief Gets the list of the current conference call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[out] call_list  List of call information for existing conference calls
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_get_conference_call_list(cm_client_h handle, GSList **call_list);

/**
 * @brief Gets the informations of the current incoming/active/held call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[out] incom  List of call informations for existing incoming call
 * @param[out] active  List of call informations for existing active call
 * @param[out] held  List of call informations for existing held call
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_get_all_calldata(cm_client_h handle, cm_call_data_t **incom, cm_call_data_t **active, cm_call_data_t **held);

/**
 * @brief Triggers the voice recoding.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] call_num  The number of voice call to be recorded
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_start_voice_record(cm_client_h handle, char *call_num);

/**
 * @brief Stops the voice recoding.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_stop_voice_record(cm_client_h handle);

/**
 * @brief Sets a callback function to be invoked when the call state changes.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] cb The callback to be invoked when the call state changes
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_call_event_cb() will be invoked.
 *
 * @see cm_unset_call_event_cb()
 */
int cm_set_call_event_cb(cm_client_h handle, cm_call_event_cb cb, void *user_data);

/**
 * @brief Unsets the call state callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_call_event_cb()
 */
int cm_unset_call_event_cb(cm_client_h handle);

/**
 * @brief Sets a callback function to be invoked when the dial status changes.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] cb The callback to be invoked when the dial status changes
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_dial_status_cb() will be invoked.
 *
 * @see cm_unset_dial_status_cb()
 */
int cm_set_dial_status_cb(cm_client_h handle, cm_dial_status_cb cb, void *user_data);

/**
 * @brief Unsets the dial status changes callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_dial_status_cb()
 */
int cm_unset_dial_status_cb(cm_client_h handle);

/**
 * @brief Sets a callback function to be invoked when the audio path changes.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] cb The callback to be invoked when the audio path changes
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_audio_state_changed_cb() will be invoked.
 *
 * @see cm_unset_audio_state_changed_cb()
 */
int cm_set_audio_state_changed_cb(cm_client_h handle, cm_audio_state_changed_cb cb, void *user_data);

/**
 * @brief Unsets the audio path changes callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_audio_state_changed_cb()
 */
int cm_unset_audio_state_changed_cb (cm_client_h handle);

/**
 * @brief Sets a callback function to be invoked when the dtmf indication status changes.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] cb The callback to be invoked when the dtmf indication status changes
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_dtmf_indi_cb() will be invoked.
 *
 * @see cm_unset_dtmf_indication_cb()
 */
int cm_set_dtmf_indication_cb(cm_client_h handle, cm_dtmf_indi_cb cb, void *user_data);

/**
 * @brief Unsets the dtmf indication status changes callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_dtmf_indication_cb()
 */
int cm_unset_dtmf_indication_cb (cm_client_h handle);

/**
 * @brief Sets a callback function to be invoked when the call ui needs to be in foreground.
 * @details The application could call cm_activate_ui when call ui need to be in foreground.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] cb The callback to be invoked when the application calls cm_activate_ui
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_go_foreground_cb() will be invoked.
 *
 * @see cm_unset_foreground_cb()
 */
int cm_set_foreground_cb(cm_client_h handle, cm_go_foreground_cb cb, void *user_data);

/**
 * @brief Unsets the foreground callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_foreground_cb()
 */
int cm_unset_foreground_cb (cm_client_h handle);

/**
 * @brief Sets a callback function to be invoked when the voice recording status changes.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 * @param[in] cb The callback to be invoked when the voice recording status changes
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_voice_record_status_cb() will be invoked.
 *
 * @see cm_unset_voice_record_status_cb()
 */
int cm_set_voice_record_status_cb(cm_client_h handle, cm_voice_record_status_cb cb, void *user_data);

/**
 * @brief Unsets the voice recording status changes callback function.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_voice_record_status_cb()
 */
int cm_unset_voice_record_status_cb (cm_client_h handle);

/* GSList of cm_call_data_t structures */
/**
 * @brief Releases the call data from #cm_get_all_calldata().
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 */
int cm_call_data_free(cm_call_data_t *call_data);

/**
 * @brief Gets the call ID.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] call_id The id of the call handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_call_id(cm_call_data_t *call_data, unsigned int *call_id);

/**
 * @brief Gets whether the call is MO(Mobile Originated) call or MT(Mobile Terminated).
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] call_direction The direction of the call (#cm_call_direction_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_call_direction(cm_call_data_t *call_data, cm_call_direction_e *call_direction);

/**
 * @brief Gets the call number.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @remarks You must release @c call_number using free().
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] call_number The number of the call
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_call_number(cm_call_data_t *call_data, char **call_number);

/**
 * @brief Gets the caller name if exist.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @remarks You must release @c calling_name using free().
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] calling_name The name of the caller
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_calling_name(cm_call_data_t *call_data, char **calling_name);

/**
 * @brief Gets the call type.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] call_type The type of the call (#cm_call_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_call_type(cm_call_data_t *call_data, cm_call_type_e *call_type);

/**
 * @brief Gets the call status.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] call_state The status of the call (#cm_call_state_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_call_state(cm_call_data_t *call_data, cm_call_state_e *call_state);

/**
 * @brief Gets the domain of the call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] call_domain The domain of the call (#cm_call_domain_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_call_domain(cm_call_data_t *call_data, cm_call_domain_e *call_domain);

/**
 * @brief Gets the contact index of the Caller.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] person_id The contact index of the Caller
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_person_id(cm_call_data_t *call_data, int *person_id);

/**
 * @brief Gets the call member count.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] member_count The call member count
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_call_member_count(cm_call_data_t *call_data, int *member_count);

/**
 * @brief Indicates whether the call is emergency call or not.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] is_emergency The emergency call status
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_is_emergency_call(cm_call_data_t *call_data, gboolean *is_emergency);

/**
 * @brief Indicates whether the call is voicemail number or not.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] is_voicemail_number The voicemail number status
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_is_voicemail_number(cm_call_data_t *call_data, gboolean *is_voicemail_number);

/**
 * @brief Gets the start time of a call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] start_time The start time
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_start_time(cm_call_data_t *call_data, long *start_time);

/**
 * @brief Gets the display mode of a call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_data The handle from #cm_get_all_calldata()
 * @param[out] name_mode The name mode of a call (#cm_name_mode_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_all_calldata()
 * @post The call list should be released by using #cm_call_data_free()
 */
int cm_call_data_get_name_mode(cm_call_data_t *call_data, cm_name_mode_e *name_mode);

/**
 * @brief Gets the call id when call event callback received.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_event_data The event data from #cm_call_event_cb()
 * @param[out] call_id The id of the call
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call_event_data should be obtained from #cm_call_event_cb()
 */
int cm_call_event_data_get_call_id(cm_call_event_data_t *call_event_data, unsigned int *call_id);

/**
 * @brief Gets the activated sim slot when call event callback received.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_event_data The event data from #cm_call_event_cb()
 * @param[out] sim_slot The activated sim slot of the call (#cm_multi_sim_slot_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call_event_data should be obtained from #cm_call_event_cb()
 */
int cm_call_event_data_get_sim_slot(cm_call_event_data_t *call_event_data, cm_multi_sim_slot_type_e *sim_slot);

/**
 * @brief Gets the information of incoming call when call event callback received.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_event_data The event data from #cm_call_event_cb()
 * @param[out] incom_call The structure which includes information of the incoming call (#cm_call_data_t)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call_event_data should be obtained from #cm_call_event_cb()
 */
int cm_call_event_data_get_incom_call(cm_call_event_data_t *call_event_data, cm_call_data_t **incom_call);

/**
 * @brief Gets the information of active call when call event callback received.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_event_data The event data from #cm_call_event_cb()
 * @param[out] active_call The structure which includes information of the active call (#cm_call_data_t)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call_event_data should be obtained from #cm_call_event_cb()
 */
int cm_call_event_data_get_active_call(cm_call_event_data_t *call_event_data, cm_call_data_t **active_call);

/**
 * @brief Gets the information of held call when call event callback received.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_event_data The event data from #cm_call_event_cb()
 * @param[out] held_call The structure which includes information of the held call (#cm_call_data_t)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call_event_data should be obtained from #cm_call_event_cb()
 */
int cm_call_event_data_get_held_call(cm_call_event_data_t *call_event_data, cm_call_data_t **held_call);

/**
 * @brief Gets the call end cause when call event callback received.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] call_event_data The event data from #cm_call_event_cb()
 * @param[out] end_cause The end cause of the call (#cm_call_end_cause_type_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call_event_data should be obtained from #cm_call_event_cb()
 */
int cm_call_event_data_get_end_cause(cm_call_event_data_t *call_event_data, cm_call_end_cause_type_e *end_cause);

/**
 * @brief Gets the call ID of the conference call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] conf_call The handle from #cm_get_conference_call_list()
 * @param[out] call_id The id of the call handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_conference_call_list()
 * @post The call list should be released by using #cm_conf_call_data_free()
 */
int cm_conf_call_data_get_call_id(cm_conf_call_data_t *conf_call, unsigned int *call_id);

/**
 * @brief Gets the call number of the conference call.
 *
 * @remarks You must release @c call_number using free().
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] conf_call The handle from #cm_get_conference_call_list()
 * @param[out] call_number The number of the call
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_conference_call_list()
 * @post The call list should be released by using #cm_conf_call_data_free()
 */
int cm_conf_call_data_get_call_number(cm_conf_call_data_t *conf_call, char **call_number);

/**
 * @brief Gets the contact index of the Caller in conference call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] conf_call The handle from #cm_get_conference_call_list()
 * @param[out] person_id The contact index of the Caller
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_conference_call_list()
 * @post The call list should be released by using #cm_conf_call_data_free()
 */
int cm_conf_call_data_get_person_id(cm_conf_call_data_t *conf_call, int *person_id);

/**
 * @brief Gets the display mode of the conference call.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] conf_call The handle from #cm_get_conference_call_list()
 * @param[out] name_mode The name mode of a call (#cm_name_mode_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The call handle should be obtained from #cm_get_conference_call_list()
 * @post The call list should be released by using #cm_conf_call_data_free()
 */
int cm_conf_call_data_get_name_mode(cm_conf_call_data_t *conf_call, cm_name_mode_e *name_mode);

/**
 * @brief Releases the call data from #cm_get_conference_call_list().
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] conf_call The handle from #cm_get_conference_call_list()
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 */
int cm_conf_call_data_free(cm_conf_call_data_t *conf_call);

/**
 * @brief Sets the LCD display states.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] state The states of the LCD to be set (#cm_lcd_control_state_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 */
int cm_set_lcd_state(cm_lcd_control_state_e state);

/**
 * @brief Starts the incoming call alert ringtone.
 *
 * @since_tizen 2.4
 * @privlevel TBD
 * @privilege TBD
 *
 * @param[in] handle The handle from cm_init()
 *
 * @return @c 0 on success,
 * otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Out of memory
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED Not supported
 * @retval #CM_ERROR_OPERATION_FAILED Invalid parameter
 *
 */
int cm_start_alert(cm_client_h handle);

#ifdef __cplusplus
 }
#endif

#endif /* __CALL_MANAGER_EXTENSION_H__ */


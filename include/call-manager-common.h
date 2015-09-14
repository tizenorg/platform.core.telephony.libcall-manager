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

#ifndef __CALL_MANAGER_COMMON_H__
#define __CALL_MANAGER_COMMON_H__

#include <glib.h>
#include <gio/gio.h>
#include <dlog.h>
#include <system_info.h>

#ifndef CM_CLIENT_LOG_TAG
#define CM_CLIENT_LOG_TAG "CALL_MGR_CLIENT"
#endif

#define TELEPHONY_FEATURE	"http://tizen.org/feature/network.telephony"

#define info(fmt,args...)   { __dlog_print(LOG_ID_MAIN, DLOG_INFO,  CM_CLIENT_LOG_TAG, "<LIB> "fmt "\n", ##args); }
#define dbg(fmt,args...)    { __dlog_print(LOG_ID_MAIN, DLOG_DEBUG, CM_CLIENT_LOG_TAG, "<LIB:%s:%d> " fmt "\n", __func__, __LINE__, ##args); }
#define warn(fmt,args...)   { __dlog_print(LOG_ID_MAIN, DLOG_WARN,  CM_CLIENT_LOG_TAG, "<LIB:%s:%d> " fmt "\n", __func__, __LINE__, ##args); }
#define err(fmt,args...)    { __dlog_print(LOG_ID_MAIN, DLOG_ERROR, CM_CLIENT_LOG_TAG, "<LIB:%s:%d> " fmt "\n", __func__, __LINE__, ##args); }
#define fatal(fmt,args...)  { __dlog_print(LOG_ID_MAIN, DLOG_FATAL, CM_CLIENT_LOG_TAG, "<LIB:%s:%d> " fmt "\n", __func__, __LINE__, ##args); }

#define sec_err(fmt, arg...) {SECURE_LOGE(CM_CLIENT_LOG_TAG, fmt"\n", ##arg); }
#define sec_warn(fmt, arg...) {SECURE_LOGW(CM_CLIENT_LOG_TAG, fmt"\n", ##arg); }
#define sec_dbg(fmt, arg...) {SECURE_LOGD(CM_CLIENT_LOG_TAG, fmt"\n", ##arg); }

#define CHECK_SUPPORTED_FEATURE(feature_name) { \
	bool telephony_supported = FALSE; \
	if (!system_info_get_platform_bool(feature_name, &telephony_supported)) { \
		if (telephony_supported == FALSE) { \
			LOGE("telephony feature is disabled"); \
			return CM_ERROR_NOT_SUPPORTED; \
		} \
	} else { \
		LOGE("Error - Feature getting from System Info"); \
		return CM_ERROR_OPERATION_FAILED; \
	} \
}

#define CM_RETURN_IF_FAIL(scalar_exp) {\
	if (!scalar_exp) \
	{ \
		err("CM_RETURN_IF_FAIL: Failed: Returning from here."); \
		return;	\
	} \
}

#define CM_RETURN_VAL_IF_FAIL(scalar_exp, ret) { \
	if (!scalar_exp) \
	{ \
		err("CM_RETURN_VAL_IF_FAIL: Failed: Returning [%d]", ret); \
		return ret; \
	} \
}

/* Call-mgr DBus interface*/
#define DBUS_CALL_MANAGER      "org.tizen.callmgr"
#define DBUS_CALL_MANAGER_PATH "/org/tizen/callmgr"
#define DBUS_CALL_MANAGER_DEFAULT_INTERFACE DBUS_CALL_MANAGER

/* DeviceD DBus interface*/
#define DBUS_DEVICED			"org.tizen.system.deviced"
#define DBUS_DEVICED_OBJECT_PATH	"/Org/Tizen/System/DeviceD"
#define DBUS_DEVICED_INTERFACE		DBUS_DEVICED
#define DBUS_DEVICED_PATH_DISPLAY	DBUS_DEVICED_OBJECT_PATH"/Display"
#define DBUS_DEVICED_INTERFACE_DISPLAY	DBUS_DEVICED_INTERFACE".display"

/*Method*/
#define DBUS_CALL_MANAGER_METHOD_TEST	"Test"
#define DBUS_CALL_MANAGER_METHOD_DIAL_CALL	"DialCall"
#define DBUS_CALL_MANAGER_METHOD_SWAP_CALL	"SwapCall"
#define DBUS_CALL_MANAGER_METHOD_HOLD_CALL	"HoldCall"
#define DBUS_CALL_MANAGER_METHOD_UNHOLD_CALL	"UnholdCall"
#define DBUS_CALL_MANAGER_METHOD_END_CALL	"EndCall"
#define DBUS_CALL_MANAGER_METHOD_REJECT_CALL	"RejectCall"
#define DBUS_CALL_MANAGER_METHOD_JOIN_CALL	"JoinCall"
#define DBUS_CALL_MANAGER_METHOD_SPLIT_CALL	"SplitCall"
#define DBUS_CALL_MANAGER_METHOD_TRANSFER_CALL	"TransferCall"
#define DBUS_CALL_MANAGER_METHOD_ANSWER_CALL	"AnswerCall"
#define DBUS_CALL_MANAGER_METHOD_GET_CALL_LIST	"GetCallList"
#define DBUS_CALL_MANAGER_METHOD_GET_CONF_CALL_LIST	"GetConfCallList"
#define DBUS_CALL_MANAGER_METHOD_GET_ALL_CALL_DATA	"GetAllCallData"
#define DBUS_CALL_MANAGER_METHOD_SEND_DTMF_RESP	"SendDtmfResp"
#define DBUS_CALL_MANAGER_METHOD_START_DTMF		"StartDtmf"
#define DBUS_CALL_MANAGER_METHOD_STOP_DTMF		"StopDtmf"
#define DBUS_CALL_MANAGER_METHOD_BURST_DTMF		"BurstDtmf"
#define DBUS_CALL_MANAGER_METHOD_SPK_ON	"SpkOn"
#define DBUS_CALL_MANAGER_METHOD_SPK_OFF	"SpkOff"
#define DBUS_CALL_MANAGER_METHOD_BLUETOOTH_ON	"BluetoothOn"
#define DBUS_CALL_MANAGER_METHOD_BLUETOOTH_OFF	"BluetoothOff"
#define DBUS_CALL_MANAGER_METHOD_SET_EXTRA_VOL	"SetExtraVol"
#define DBUS_CALL_MANAGER_METHOD_SET_NOISE_REDUCTION	"SetNoiseReduction"
#define DBUS_CALL_MANAGER_METHOD_SET_MUTE_STATE	"SetMuteState"
#define DBUS_CALL_MANAGER_METHOD_GET_AUDIO_STATE	"GetAudioState"
#define DBUS_CALL_MANAGER_METHOD_STOP_ALERT		"StopAlert"
#define DBUS_CALL_MANAGER_METHOD_START_ALERT		"StartAlert"
#define DBUS_CALL_MANAGER_METHOD_ACTIVATE_UI		"ActivateUi"
#define DBUS_CALL_MANAGER_METHOD_GET_CALL_STATUS		"GetCallStatus"
#define DBUS_CALL_MANAGER_METHOD_SET_WATCH_NAME		"SetWatchName"
#define DBUS_DEVICED_METHOD_SET_LCDTIMEOUT 	"setlcdtimeout"
#define DBUS_DEVICED_METHOD_UPDATE_LCDTIMEOUT_BY_TOUCH		"UpdateLCDTimeoutByTouch"
#define DBUS_CALL_MANAGER_METHOD_START_VOICE_RECORD		"StartVoiceRecord"
#define DBUS_CALL_MANAGER_METHOD_STOP_VOICE_RECORD		"StopVoiceRecord"
#define DBUS_CALL_MANAGER_METHOD_GET_MUTE_STATUS		"GetMuteStatus"
#define DBUS_CALL_MANAGER_METHOD_GET_ANSWERING_MACHINE_STATUS	"GetAnsweringMachineStatus"

/*Signal*/
#define DBUS_CALL_MANAGER_SIGNAL_CALL_STATUS	"CallStatus"
#define DBUS_CALL_MANAGER_SIGNAL_CALL_EVENT		"CallEvent"
#define DBUS_CALL_MANAGER_SIGNAL_DIAL_STATUS	"DialStatus"
#define DBUS_CALL_MANAGER_SIGNAL_DTMF_INDI		"DtmfIndi"
#define DBUS_CALL_MANAGER_SIGNAL_AUDIO_STATE	"AudioState"
#define DBUS_CALL_MANAGER_SIGNAL_GO_FOREGROUND	"GoForeground"
#define DBUS_CALL_MANAGER_SIGNAL_VOICE_RECORD_STATUS	"VoiceRecordStatus"
#define DBUS_CALL_MANAGER_SIGNAL_MUTE_STATUS	"MuteStatus"


#define CM_DEFAULT_TIMEOUT    (5 * 1000)

struct __cm_client {
	GDBusConnection *dbus_conn;
	GCancellable *ca;
//	gchar *name;
	GHashTable *evt_list;
};

struct __cm_call_data {
	unsigned int call_id;							/**< Unique call id*/
	cm_call_direction_e call_direction;						/**< 0 : MO, 1 : MT*/
	char *call_number;							/**< call number */
	char *calling_name;							/**< calling name */
	cm_call_type_e call_type;			/**< Specifies type of call (voice, data, emergency) */
	cm_call_state_e call_state;		/**< Current Call state */
	int member_count;						/**< 1: Single call, more than 1: Conference call. It reflects call member count */
	gboolean is_ecc;						/**< is emergency*/
	gboolean is_voicemail_number;				/**<is voicemail number*/
	cm_call_domain_e call_domain;	/**< Current Call domain */
	int person_id;		/**< Contact Index */
	long start_time;		/**<Call start time */
	cm_name_mode_e name_mode;	/**<Name Mode */
};

struct __cm_call_event_data {
	unsigned int call_id;
	cm_multi_sim_slot_type_e sim_slot;
	cm_call_end_cause_type_e end_cause;

	cm_call_data_t *incom;	/* Incoming CallData, Important : Application should free this after use */
	cm_call_data_t *active_dial;	/* Active or Dial CallData, Important : Application should free this after use */
	cm_call_data_t *held;	/* Held CallData, Important : Application should free this after use */
};

struct __cm_conf_call_data {
	unsigned int call_id;							/**< Unique call id*/
	char *call_number;							/**< call number */
	int person_id;								/**< Contact Index */
	cm_name_mode_e name_mode;					/**<Name Mode */
};

#define MAKE_CALL_STATE_ASYNC_DATA(data,cb,ud)  \
	data = g_try_new0 (struct __cm_async_data, 1); \
	if (!data) { return -1; } \
	data->cb_fn = cb; \
	data->user_data = ud \

struct __cm_async_data {
	void *cb_fn;
	void* user_data;
	cm_client_h handle;
	guint id;
};

#endif	//__CALL_MANAGER_COMMON_H__


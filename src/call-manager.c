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

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <gio/gio.h>
#include <vconf.h>

#include <device/display.h>
#include <device/power.h>

#include "call-manager.h"
#include "call-manager-extension.h"
#include "call-manager-common.h"

static int __cm_fetch_call_data(GVariantIter *iter, cm_call_data_t **call_data);
static void __cm_on_signal_recv(GDBusConnection *conn, const gchar *name,
		const gchar *path, const gchar *interface, const gchar *sig,
		GVariant *param, gpointer user_data);
static int __cm_set_cb(cm_client_h handle, char *signal, void *cb, void *user_data);
static int __cm_unset_cb(cm_client_h handle, char *signal);
static void __cm_set_LCDtimeout_cb(GObject *source_object, GAsyncResult *res, gpointer user_data);


static void __cm_set_LCDtimeout_cb(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	int result = -1;
	GVariant *dbus_result = NULL;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
	}

	g_variant_get(dbus_result, "(i)", &result);

	dbg("result : %d", result);
}


static int __cm_fetch_call_data(GVariantIter *iter, cm_call_data_t **call_data)
{
	cm_call_data_t *tmp_call_data = NULL;
	gchar *key = 0;
	GVariant *value = 0;
	CM_RETURN_VAL_IF_FAIL(iter, -1);
	CM_RETURN_VAL_IF_FAIL(call_data, -1);

	tmp_call_data = (cm_call_data_t*)calloc(1, sizeof(cm_call_data_t));
	CM_RETURN_VAL_IF_FAIL(tmp_call_data, -1);

	while (g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		if (!g_strcmp0(key, "call_id")) {
			tmp_call_data->call_id = g_variant_get_uint32(value);
		} else if (!g_strcmp0(key, "call_direction")) {
			tmp_call_data->call_direction = (int)g_variant_get_int32(value);
		} else if (!g_strcmp0(key, "call_number")) {
			tmp_call_data->call_number = g_strdup(g_variant_get_string(value, 0));
		} else if (!g_strcmp0(key, "calling_name")) {
			tmp_call_data->calling_name = g_strdup(g_variant_get_string(value, 0));
		} else if (!g_strcmp0(key, "call_type")) {
			tmp_call_data->call_type = (int)g_variant_get_int32(value);
		} else if (!g_strcmp0(key, "call_state")) {
			tmp_call_data->call_state = (int)g_variant_get_int32(value);
		} else if (!g_strcmp0(key, "member_count")) {
			tmp_call_data->member_count = (int)g_variant_get_int32(value);
		} else if (!g_strcmp0(key, "is_ecc")) {
			tmp_call_data->is_ecc = (int)g_variant_get_boolean(value);
		} else if (!g_strcmp0(key, "is_voicemail_number")) {
			tmp_call_data->is_voicemail_number = (int)g_variant_get_boolean(value);
		} else if (!g_strcmp0(key, "person_id")) {
			tmp_call_data->person_id = g_variant_get_uint32(value);
		} else if (!g_strcmp0(key, "start_time")) {
			tmp_call_data->start_time = g_variant_get_int64(value);
		} else if (!g_strcmp0(key, "name_mode")) {
			tmp_call_data->name_mode = g_variant_get_int32(value);
		}
	}

	dbg("\n\n <<<<<<<<< CallData Info in Library START >>>>>>>>>> \n");
	dbg("call_id				: %d, ", tmp_call_data->call_id);
	dbg("call_direction 		: %d, ", tmp_call_data->call_direction);
	if (tmp_call_data->call_number) {
		dbg("call_number			: %s, ", tmp_call_data->call_number);
	}
	if (tmp_call_data->calling_name) {
		dbg("calling_name			: %s, ", tmp_call_data->calling_name);
	}
	dbg("call_type				: %d, ", tmp_call_data->call_type);
	dbg("call_state 			: %d, ", tmp_call_data->call_state);
	dbg("member count			: %d, ", tmp_call_data->member_count);
	dbg("is_ecc 				: %d, ", tmp_call_data->is_ecc);
	dbg("is_voicemail_number 		: %d, ", tmp_call_data->is_voicemail_number);
	dbg("person_id				: %d", tmp_call_data->person_id);
	dbg("start time				: %ld", tmp_call_data->start_time);
	dbg("name mode				: %d", tmp_call_data->name_mode);
	dbg("\n\n <<<<<<<<< CallData Info in Library END >>>>>>>>>> \n");

	*call_data = tmp_call_data;

	return 0;
}

static void __cm_on_signal_recv(GDBusConnection *conn, const gchar *name,
		const gchar *path, const gchar *interface, const gchar *sig,
		GVariant *param, gpointer user_data)
{
	info("[%s] received", sig);
	cm_client_h handle = user_data;
	struct __cm_async_data *ad = NULL;
	char *noti_id = NULL;
	int ret = -1;
	CM_RETURN_IF_FAIL(handle);

	noti_id = g_strdup_printf("%s:%s:%s", path, interface, sig);
	ad = g_hash_table_lookup(handle->evt_list, noti_id);
	if (!ad) {
		err("can't find noti_id(%s) callback info", noti_id);
		g_free(noti_id);
		return;
	}

	if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_CALL_STATUS)) {
		cm_call_status_e call_status = CM_CALL_STATUS_IDLE;
		cm_call_type_e call_type = CM_CALL_TYPE_INVALID;
		char *call_number = NULL;

		g_variant_get(param, "(iis)", &call_status, &call_type, &call_number);
		dbg("call status: %d, call_type: %d", call_status, call_type);
		if (call_number) {
			dbg("call number: %s", call_number);
		}

		((cm_call_status_cb)ad->cb_fn)(call_status, call_number, ad->user_data);
		g_free(call_number);
	} else if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_CALL_EVENT)) {
		const gchar *param_type;
		gint event = -1;
		cm_call_event_data_t *evt_data = NULL;
		cm_multi_sim_slot_type_e sim_slot = CM_SIM_SLOT_DEFAULT_E;
		cm_call_end_cause_type_e end_cause = CM_CALL_ENDCAUSE_MAX;
		unsigned int call_id = 0;
		cm_call_data_t *callData = NULL;
		GVariantIter *iter_incom = 0, *iter_active_dial = 0, *iter_held = 0;

		param_type = g_variant_get_type_string(param);
		dbg("param_type: '%s'", param_type);
		g_variant_get(param, "(iuiia{sv}a{sv}a{sv})", &event, &call_id, &sim_slot, &end_cause, &iter_incom, &iter_active_dial, &iter_held);
		dbg("received event: '%d'", event);

		evt_data = calloc(1, sizeof(cm_call_event_data_t));
		if (NULL == evt_data) {
			err("memory allocation failed...");
			g_variant_iter_free(iter_incom);
			g_variant_iter_free(iter_active_dial);
			g_variant_iter_free(iter_held);
			g_free(noti_id);
			return;
		}
		evt_data->call_id = call_id;
		evt_data->sim_slot = sim_slot;
		evt_data->end_cause = end_cause;

		dbg("call_event: %d, sim_slot: %d", event, sim_slot);

		/*Fetch IncomingData*/
		if (iter_incom && (g_variant_iter_n_children(iter_incom) > 0)) {
			ret = __cm_fetch_call_data(iter_incom, &callData);
			if (ret == 0) {
				evt_data->incom = callData;
			}
			g_variant_iter_free(iter_incom);
		}

		/*Fetch ActiveData*/
		if (iter_active_dial && (g_variant_iter_n_children(iter_active_dial) > 0)) {
			ret = __cm_fetch_call_data(iter_active_dial, &callData);
			if (ret == 0) {
				evt_data->active_dial = callData;
			}
			g_variant_iter_free(iter_active_dial);
		}

		/*Fetch HeldData*/
		if (iter_held && (g_variant_iter_n_children(iter_held) > 0)) {
			ret = __cm_fetch_call_data(iter_held, &callData);
			if (ret == 0) {
				evt_data->held = callData;
			}
			g_variant_iter_free(iter_held);
		}

		((cm_call_event_cb)ad->cb_fn)(event, evt_data, ad->user_data);
		if (evt_data->incom) {
			cm_call_data_free(evt_data->incom);
		}
		if (evt_data->active_dial) {
			cm_call_data_free(evt_data->active_dial);
		}
		if (evt_data->held) {
			cm_call_data_free(evt_data->held);
		}
		g_free(evt_data);
	} else if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_AUDIO_STATE)) {
		gint audio_status = -1;

		g_variant_get(param, "(i)", &audio_status);
		dbg("received audio_status: %d", audio_status);

		((cm_audio_state_changed_cb)ad->cb_fn)(audio_status, ad->user_data);
	} else if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_DTMF_INDI)) {
		cm_dtmf_indi_type_e indi_type = CM_DTMF_INDI_IDLE_E;
		char *dtmf_number = NULL;

		g_variant_get(param, "(is)", &indi_type, &dtmf_number);
		dbg("received indication: %d", indi_type);
		if (dtmf_number) {
			dbg("received dtmf numbe: %s", dtmf_number);
		}

		((cm_dtmf_indi_cb)ad->cb_fn)(indi_type, dtmf_number, ad->user_data);
		g_free(dtmf_number);
	} else if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_GO_FOREGROUND)) {
		((cm_go_foreground_cb)ad->cb_fn)(ad->user_data);
	} else if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_DIAL_STATUS)) {
		cm_dial_status_e dial_status = CM_DIAL_SUCCESS;
		g_variant_get(param, "(i)", &dial_status);
		dbg("received dial status: %d", dial_status);

		((cm_dial_status_cb)ad->cb_fn)(dial_status, ad->user_data);
	} else if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_VOICE_RECORD_STATUS)) {
		cm_vr_status_e vr_status = -1;
		cm_vr_status_extra_type_e extra_type;

		g_variant_get(param, "(ii)", &vr_status, &extra_type);
		dbg("received vr_status: '%d', extra_type: 0x%02x", vr_status, extra_type);

		((cm_voice_record_status_cb)ad->cb_fn)(vr_status, extra_type, ad->user_data);
	} else if (!g_strcmp0(sig, DBUS_CALL_MANAGER_SIGNAL_MUTE_STATUS)) {
		cm_mute_status_e mute_status = CM_MUTE_STATUS_MAX;
		g_variant_get(param, "(i)", &mute_status);
		dbg("received mute status: %d", mute_status);

		((cm_mute_status_cb)ad->cb_fn)(mute_status, ad->user_data);
	}
	g_free(noti_id);
}

static int __cm_set_cb(cm_client_h handle, char *signal, void *cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(signal, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	struct __cm_async_data *ad = NULL;
	guint id = 0;
	gchar *noti_id = NULL;

	noti_id = g_strdup_printf("%s:%s:%s", DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, signal);
	dbg("noti_id: '%s'", noti_id);

	ad = g_hash_table_lookup(handle->evt_list, noti_id);
	if (ad != NULL) {
		dbg("noti_id(%s) is already registered", noti_id);
		g_free(noti_id);
		return CM_ERROR_ALREADY_REGISTERED;
	}

	MAKE_CALL_STATE_ASYNC_DATA(ad, cb, user_data);
	CM_RETURN_VAL_IF_FAIL(ad, CM_ERROR_OPERATION_FAILED);

	id = g_dbus_connection_signal_subscribe(handle->dbus_conn,
			DBUS_CALL_MANAGER, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, signal, DBUS_CALL_MANAGER_PATH, NULL,
			G_DBUS_SIGNAL_FLAGS_NONE, __cm_on_signal_recv, handle, NULL);
	dbg("id = %d", id);

	ad->id = id;
	ad->handle = handle;
	g_hash_table_insert(handle->evt_list, noti_id, ad);

	return CM_ERROR_NONE;
}

static int __cm_unset_cb(cm_client_h handle, char *signal)
{
	struct __cm_async_data *ad = NULL;
	gchar *noti_id = NULL;
	gboolean rv = FALSE;
	CM_RETURN_VAL_IF_FAIL(signal, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	noti_id = g_strdup_printf("%s:%s:%s", DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, signal);
	dbg("noti_id: '%s'", noti_id);

	ad = g_hash_table_lookup(handle->evt_list, noti_id);
	if (ad == NULL) {
		warn("noti_id(%s) is not registered", noti_id);
		g_free(noti_id);
		return CM_ERROR_NOT_REGISTERED;
	}

	if (ad->id == 0) {
		warn("id(%d) is wrong", ad->id);
		g_free(noti_id);
		return CM_ERROR_OPERATION_FAILED;
	}

	rv = g_hash_table_remove(handle->evt_list, noti_id);
	if (!rv) {
		warn("fail to deregister noti event(%s)", noti_id);
		g_free(noti_id);
		return CM_ERROR_OPERATION_FAILED;
	}

	g_free(noti_id);
	return CM_ERROR_NONE;
}

static void __cm_evt_list_key_destroyed_cb(gpointer data)
{
	dbg("__cm_evt_list_key_destroyed_cb()");
	gchar *noti_id = (gchar *)data;
	CM_RETURN_IF_FAIL(noti_id);

	warn("Key[%s] is removed from hash table.", noti_id);
	g_free(noti_id);
	return;
}

static void __cm_evt_list_value_destroyed_cb(gpointer data)
{
	dbg("__cm_evt_list_value_destroyed_cb()");
	struct __cm_async_data *ad = (struct __cm_async_data *)data;
	CM_RETURN_IF_FAIL(ad);
	cm_client_h handle = ad->handle;
	CM_RETURN_IF_FAIL(handle);

	g_dbus_connection_signal_unsubscribe(handle->dbus_conn, ad->id);
	g_free(ad);
	return;
}

EXPORT_API int cm_init(cm_client_h *handle)
{
	struct __cm_client *client_handle;
	GError *error = NULL;

	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbg("cm_init");

#if !GLIB_CHECK_VERSION(2, 35, 0)
	g_type_init();
#endif

	client_handle = g_try_new0(struct __cm_client, 1);
	if (!client_handle)
		return CM_ERROR_OPERATION_FAILED;

	client_handle->dbus_conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
	if (!client_handle->dbus_conn) {
		err("dbus connection get failed: %s", error->message);
		g_error_free(error);
		g_free(client_handle);
		return CM_ERROR_OPERATION_FAILED;
	}

	client_handle->ca = g_cancellable_new();
	client_handle->evt_list = g_hash_table_new_full(g_str_hash, g_str_equal, __cm_evt_list_key_destroyed_cb,
			__cm_evt_list_value_destroyed_cb);

	*handle = client_handle;

	return CM_ERROR_NONE;
}

EXPORT_API int cm_enable_recovery(cm_client_h handle, char* appid)
{
	GError *error = NULL;
	GVariant *dbus_result = NULL;
	gint value = 0;
	guint own_id = 0;

	CM_RETURN_VAL_IF_FAIL(appid, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbg(" appid [%s] cm_enable_recovery", appid);

	own_id = g_bus_own_name(G_BUS_TYPE_SYSTEM, appid,
				G_BUS_NAME_OWNER_FLAGS_NONE, NULL, NULL, NULL,
				handle, NULL);
	if (0 == own_id) {
		err("g_bus_own_name failed");
		return CM_ERROR_OPERATION_FAILED;
	}

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SET_WATCH_NAME,
			g_variant_new("(s)", appid), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	g_variant_get(dbus_result, "(i)", &value);
	g_variant_unref(dbus_result);

	return CM_ERROR_NONE;
}


EXPORT_API int cm_deinit(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbg("cm_deinit");

	g_hash_table_remove_all(handle->evt_list);
	g_hash_table_unref(handle->evt_list);

	g_cancellable_cancel(handle->ca);

	g_object_unref(handle->ca);

	g_object_unref(handle->dbus_conn);

	memset(handle, 0, sizeof(struct __cm_client));
	g_free(handle);

	return CM_ERROR_NONE;
}

EXPORT_API int cm_dial_call(cm_client_h handle, char*number, cm_call_type_e call_type, cm_multi_sim_slot_type_e sim_slot)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = CM_ERROR_NONE;
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(number, CM_ERROR_INVALID_PARAMETER);

	dbg("cm_dial_call()");

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_DIAL_CALL,
			g_variant_new("(siiib)", number, call_type, sim_slot, 0, FALSE/*is_emergency_contact*/), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (dbus_result) {
		g_variant_get(dbus_result, "(i)", &value);
		/* TODO: get proper error cause then return proper error */
		if (value != 0) {
			value = CM_ERROR_OPERATION_FAILED;
		}
	} else {
		err("g_dbus_conn failed. error (%s)", error->message);
		if (strstr(error->message, "No access rights")) {
			err("PERMISSION_DENIED");
			value = CM_ERROR_PERMISSION_DENIED;
		} else {
			value = CM_ERROR_OPERATION_FAILED;
		}
		g_error_free(error);
	}

	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_end_call(cm_client_h handle, unsigned int call_id, cm_call_release_type_e release_type)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_end_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_END_CALL,
			g_variant_new("(ui)", call_id, release_type), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_reject_call(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = CM_ERROR_NONE;

	dbg("cm_reject_call()");

	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_REJECT_CALL,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (dbus_result) {
		g_variant_get(dbus_result, "(i)", &value);
		if (value != 0) {
			value = CM_ERROR_OPERATION_FAILED;
		}
	} else {
		err("g_dbus_conn failed. error (%s)", error->message);
		if (strstr(error->message, "No access rights")) {
			err("PERMISSION_DENIED");
			value = CM_ERROR_PERMISSION_DENIED;
		} else {
			value = CM_ERROR_OPERATION_FAILED;
		}
		g_error_free(error);
	}

	g_variant_unref(dbus_result);

	 return value;
}


EXPORT_API int cm_swap_call(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_swap_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SWAP_CALL,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_hold_call(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_hold_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_HOLD_CALL,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_unhold_call(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_unhold_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_UNHOLD_CALL,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_join_call(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_join_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_JOIN_CALL,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_split_call(cm_client_h handle, unsigned int call_id)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_split_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SPLIT_CALL,
			g_variant_new("(u)", call_id), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_transfer_call(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_transfer_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_TRANSFER_CALL,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_answer_call(cm_client_h handle, cm_call_answer_type_e ans_type)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_answer_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_ANSWER_CALL,
			g_variant_new("(i)", ans_type), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_speaker_on(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg(">>");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SPK_ON,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_speaker_off(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg(">>");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SPK_OFF,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_bluetooth_on(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg(">>");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_BLUETOOTH_ON,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_bluetooth_off(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg(">>");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_BLUETOOTH_OFF,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_set_extra_vol(cm_client_h handle, gboolean is_extra_vol)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg(">>");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SET_EXTRA_VOL,
			g_variant_new("(b)", is_extra_vol), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_set_mute_state(cm_client_h handle, gboolean is_mute_state)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg(">>");
	dbg("is_mute_state: %d", is_mute_state);
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SET_MUTE_STATE,
			g_variant_new("(b)", is_mute_state), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_start_dtmf(cm_client_h handle, unsigned char dtmf_digit)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_start_dtmf()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(dtmf_digit, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_START_DTMF,
			g_variant_new("(y)", dtmf_digit), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_stop_dtmf(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_stop_dtmf()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_STOP_DTMF,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_burst_dtmf(cm_client_h handle, char *dtmf_digits)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_burst_dtmf()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(dtmf_digits, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_BURST_DTMF,
			g_variant_new("(s)", dtmf_digits), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_send_dtmf_resp(cm_client_h handle, cm_dtmf_resp_type_e resp_type)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg(">>");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_SEND_DTMF_RESP,
			g_variant_new("(i)", resp_type), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_get_audio_state(cm_client_h handle, cm_audio_state_type_e *audio_state)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_get_audio_state()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_GET_AUDIO_STATE,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	g_variant_get(dbus_result, "(i)", &value);
	*audio_state = value;
	g_variant_unref(dbus_result);

	return CM_ERROR_NONE;
}

EXPORT_API int cm_stop_alert(cm_client_h handle)
{
	gint value = 0;
	GVariant *dbus_result;
	GError *error = NULL;

	dbg("cm_stop_alert()");

	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_STOP_ALERT,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (dbus_result) {
		g_variant_get(dbus_result, "(i)", &value);
		if (value != 0) {
			value = CM_ERROR_OPERATION_FAILED;
		}
	} else {
		err("g_dbus_conn failed. error (%s)", error->message);
		if (strstr(error->message, "No access rights")) {
			err("PERMISSION_DENIED");
			value = CM_ERROR_PERMISSION_DENIED;
		} else {
			value = CM_ERROR_OPERATION_FAILED;
		}
		g_error_free(error);
	}

	g_variant_unref(dbus_result);

	return value;
}

EXPORT_API int cm_start_alert(cm_client_h handle)
{
	gint value = 0;
	GVariant *dbus_result;
	GError *error = NULL;

	dbg("cm_start_alert()");

	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_START_ALERT,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (dbus_result) {
		g_variant_get(dbus_result, "(i)", &value);
		if (value != 0) {
			value = CM_ERROR_OPERATION_FAILED;
		}
	} else {
		err("g_dbus_conn failed. error (%s)", error->message);
		if (strstr(error->message, "No access rights")) {
			err("PERMISSION_DENIED");
			value = CM_ERROR_PERMISSION_DENIED;
		} else {
			value = CM_ERROR_OPERATION_FAILED;
		}
		g_error_free(error);
	}

	g_variant_unref(dbus_result);

	return value;
}

EXPORT_API int cm_activate_ui(cm_client_h handle)
{
	dbg("cm_activate_ui()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	GError *error = NULL;
	gint value = 0;
	GVariant *dbus_result;

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_ACTIVATE_UI,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	g_variant_get(dbus_result, "(i)", &value);
	g_variant_unref(dbus_result);
	return value;
}

EXPORT_API int cm_set_lcd_timeout(cm_client_h handle, cm_lcd_timeout_e timeout)
{
	dbg("cm_set_lcd_timeout()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	int bPowerkeyMode = 0;
	int on = 0;
	int dim = 0;
	int holdkey = 0;
	int ret = -1;
	int update_timer = 0;

	if (timeout == CM_LCD_TIMEOUT_LOCKSCREEN_SET) {
		update_timer = 0;
	} else {
		update_timer = 1;
	}

	ret = vconf_get_bool(VCONFKEY_CISSAPPL_POWER_KEY_ENDS_CALL_BOOL, &bPowerkeyMode);
	if (ret < 0) {
		err("vconf_get_bool [VCONFKEY_CISSAPPL_POWER_KEY_ENDS_CALL_BOOL] failed.\n");
	}

	dbg("set timeout : %d, powerkeymode : %d", timeout, bPowerkeyMode);
	if (timeout == CM_LCD_TIMEOUT_SET) {
		on = 10;
		dim = 20;
		holdkey = bPowerkeyMode;
	} else if (timeout == CM_LCD_TIMEOUT_UNSET) {
		on = 0;
		dim = 0;
		holdkey = bPowerkeyMode;
	} else if (timeout == CM_LCD_TIMEOUT_LOCKSCREEN_SET) { /*After lock-screen comes in Connected state LCD goes to OFF in 5 secs*/
		on = 5;
		dim = 0;
		holdkey = bPowerkeyMode;
	} else if (timeout == CM_LCD_TIMEOUT_KEYPAD_SET) {
		on = 3;
		dim = 5;
		holdkey = bPowerkeyMode;
	} else {
		on = 0;
		dim = 0;
		holdkey = 0;
	}

	dbg("on(%d), dim(%d), hold(%d)", on, dim, holdkey);

	g_dbus_connection_call(handle->dbus_conn, DBUS_DEVICED,
			DBUS_DEVICED_PATH_DISPLAY, DBUS_DEVICED_INTERFACE_DISPLAY, DBUS_DEVICED_METHOD_SET_LCDTIMEOUT,
			g_variant_new("(iii)", on, dim, holdkey), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, __cm_set_LCDtimeout_cb, NULL);

	g_dbus_connection_call(handle->dbus_conn, DBUS_DEVICED,
			DBUS_DEVICED_PATH_DISPLAY, DBUS_DEVICED_INTERFACE_DISPLAY, DBUS_DEVICED_METHOD_UPDATE_LCDTIMEOUT_BY_TOUCH,
			g_variant_new("(i)", update_timer), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, __cm_set_LCDtimeout_cb, NULL);

	return 0;
}


EXPORT_API int cm_get_all_call_list(cm_client_h handle, GSList **call_list)
{
	GVariant *dbus_result;
	GError *error = NULL;
	const gchar *param_type;
	cm_call_data_t *callData = NULL;
	GVariantIter *iter = 0, *iter_row = 0;
	int ret = -1;
	dbg("cm_get_all_calllist()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_GET_CALL_LIST,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	param_type = g_variant_get_type_string(dbus_result);
	dbg("param_type: '%s'", param_type);
	g_variant_get(dbus_result, "(aa{sv})", &iter);
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		if (iter_row && (g_variant_iter_n_children(iter_row) > 0)) {
			ret = __cm_fetch_call_data(iter_row, &callData);
			if (ret == 0) {
				*call_list = g_slist_append(*call_list, callData);
			}
			g_variant_iter_free(iter_row);
		}
	}
	g_variant_iter_free(iter);
	g_variant_unref(dbus_result);

	return CM_ERROR_NONE;
}

EXPORT_API int cm_get_conference_call_list(cm_client_h handle, GSList **call_list)
{
	GVariant *dbus_result;
	GError *error = NULL;
	const gchar *param_type;
	cm_conf_call_data_t *callData = NULL;
	GVariantIter *iter = 0, *iter_row = 0;
	dbg("cm_get_all_calllist()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_GET_CONF_CALL_LIST,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	param_type = g_variant_get_type_string(dbus_result);
	dbg("param_type: '%s'", param_type);
	g_variant_get(dbus_result, "(aa{sv})", &iter);
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		if (iter_row && (g_variant_iter_n_children(iter_row) > 0)) {
			gchar *key = 0;
			GVariant *value = 0;

			callData = (cm_conf_call_data_t*)calloc(1, sizeof(cm_conf_call_data_t));
			CM_RETURN_VAL_IF_FAIL(callData, CM_ERROR_OPERATION_FAILED);

			while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
				if (!g_strcmp0(key, "call_id")) {
					callData->call_id = g_variant_get_uint32(value);
				} else if (!g_strcmp0(key, "call_number")) {
					callData->call_number = g_strdup(g_variant_get_string(value, 0));
				} else if (!g_strcmp0(key, "person_id")) {
					callData->person_id = g_variant_get_uint32(value);
				} else if (!g_strcmp0(key, "name_mode")) {
					callData->name_mode = g_variant_get_uint32(value);
				}
			}
			dbg("\n\n <<<<<<<<< CallData Info in Library START >>>>>>>>>> \n");
			dbg("call_id				: %d, ", callData->call_id);
			if (callData->call_number) {
				dbg("call_number			: %s, ", callData->call_number);
			}
			dbg("person_id				: %d", callData->person_id);
			dbg("name_mode				: %d", callData->name_mode);
			dbg("\n\n <<<<<<<<< CallData Info in Library END >>>>>>>>>> \n");

			*call_list = g_slist_append(*call_list, callData);
			g_variant_iter_free(iter_row);
		}
	}
	g_variant_iter_free(iter);
	g_variant_unref(dbus_result);

	return CM_ERROR_NONE;
}

EXPORT_API int cm_get_all_calldata(cm_client_h handle, cm_call_data_t **incom, cm_call_data_t **active, cm_call_data_t **held)
{
	GVariant *dbus_result;
	GError *error = NULL;
	const gchar *param_type;
	GVariantIter *iter_incom = 0, *iter_active_dial = 0, *iter_held = 0;
	dbg("cm_get_all_calldata()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_GET_ALL_CALL_DATA,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	param_type = g_variant_get_type_string(dbus_result);
	dbg("param_type: '%s'", param_type);
	g_variant_get(dbus_result, "(a{sv}a{sv}a{sv})", &iter_incom, &iter_active_dial, &iter_held);

	/*Fetch IncomingData*/
	if (iter_incom && (g_variant_iter_n_children(iter_incom) > 0)) {
		__cm_fetch_call_data(iter_incom, incom);
		g_variant_iter_free(iter_incom);
	}

	/*Fetch ActiveData*/
	if (iter_active_dial && (g_variant_iter_n_children(iter_active_dial) > 0)) {
		__cm_fetch_call_data(iter_active_dial, active);
		g_variant_iter_free(iter_active_dial);
	}

	/*Fetch HeldData*/
	if (iter_held && (g_variant_iter_n_children(iter_held) > 0)) {
		__cm_fetch_call_data(iter_held, held);
		g_variant_iter_free(iter_held);
	}

	return CM_ERROR_NONE;
}

EXPORT_API int cm_get_call_status(cm_client_h handle, cm_call_status_e *call_status)
{
	GError *error = NULL;
	guint status = 0;
	guint type = 0;
	GVariant *dbus_result;
	int error_code = CM_ERROR_NONE;

	dbg("cm_get_call_status()");

	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_status, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_GET_CALL_STATUS,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (dbus_result) {
		g_variant_get(dbus_result, "(ii)", &status, &type);
		if (status < CM_CALL_STATUS_MAX) {
			*call_status = (cm_call_status_e)status;
		} else {
			error_code = CM_ERROR_OPERATION_FAILED;
		}
	} else {
		err("g_dbus_conn failed. error (%s)", error->message);
		if (strstr(error->message, "No access rights")) {
			err("PERMISSION_DENIED");
			error_code = CM_ERROR_PERMISSION_DENIED;
		} else {
			error_code = CM_ERROR_OPERATION_FAILED;
		}
		g_error_free(error);
	}

	g_variant_unref(dbus_result);

	return error_code;
}

EXPORT_API int cm_get_mute_status(cm_client_h handle, cm_mute_status_e *mute_status)
{
	GError *error = NULL;
	gint status = 0;
	GVariant *dbus_result;
	int error_code = CM_ERROR_NONE;

	dbg("cm_get_mute_status()");

	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(mute_status, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_GET_MUTE_STATUS,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (dbus_result) {
		g_variant_get(dbus_result, "(i)", &status);
		if (status < CM_MUTE_STATUS_MAX && status >= CM_MUTE_STATUS_OFF) {
			*mute_status = (cm_mute_status_e)status;
		} else {
			error_code = CM_ERROR_OPERATION_FAILED;
		}
	} else {
		err("g_dbus_conn failed. error (%s)", error->message);
		if (strstr(error->message, "No access rights")) {
			err("PERMISSION_DENIED");
			error_code = CM_ERROR_PERMISSION_DENIED;
		} else {
			error_code = CM_ERROR_OPERATION_FAILED;
		}
		g_error_free(error);
	}

	g_variant_unref(dbus_result);

	return error_code;
}


EXPORT_API int cm_set_mute_status_cb(cm_client_h handle, cm_mute_status_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_MUTE_STATUS, cb, user_data);
}

EXPORT_API int cm_unset_mute_status_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_MUTE_STATUS);
}

EXPORT_API int cm_set_call_status_cb(cm_client_h handle, cm_call_status_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_CALL_STATUS, cb, user_data);
}

EXPORT_API int cm_unset_call_status_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_CALL_STATUS);
}

EXPORT_API int cm_set_call_event_cb(cm_client_h handle, cm_call_event_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_CALL_EVENT, cb, user_data);
}

EXPORT_API int cm_unset_call_event_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_CALL_EVENT);
}

EXPORT_API int cm_set_dial_status_cb(cm_client_h handle, cm_dial_status_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_DIAL_STATUS, cb, user_data);
}

EXPORT_API int cm_unset_dial_status_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_DIAL_STATUS);
}

EXPORT_API int cm_set_audio_state_changed_cb(cm_client_h handle, cm_audio_state_changed_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_AUDIO_STATE, cb, user_data);
}

EXPORT_API int cm_unset_audio_state_changed_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_AUDIO_STATE);
}

EXPORT_API int cm_set_dtmf_indication_cb(cm_client_h handle, cm_dtmf_indi_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_DTMF_INDI, cb, user_data);
}

EXPORT_API int cm_unset_dtmf_indication_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_DTMF_INDI);
}

EXPORT_API int cm_set_foreground_cb(cm_client_h handle, cm_go_foreground_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_GO_FOREGROUND, cb, user_data);
}

EXPORT_API int cm_unset_foreground_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_GO_FOREGROUND);
}

EXPORT_API int cm_call_data_free(cm_call_data_t *call_data)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	g_free(call_data->call_number);
	g_free(call_data->calling_name);
	g_free(call_data);

	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_call_id(cm_call_data_t *call_data, unsigned int *call_id)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_id, CM_ERROR_INVALID_PARAMETER);

	*call_id = call_data->call_id;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_call_direction(cm_call_data_t *call_data, cm_call_direction_e *call_direction)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_direction, CM_ERROR_INVALID_PARAMETER);

	*call_direction = call_data->call_direction;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_call_number(cm_call_data_t *call_data, char **call_number)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_number, CM_ERROR_INVALID_PARAMETER);

	*call_number = call_data->call_number;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_calling_name(cm_call_data_t *call_data, char **calling_name)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(calling_name, CM_ERROR_INVALID_PARAMETER);

	*calling_name = call_data->calling_name;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_call_type(cm_call_data_t *call_data, cm_call_type_e *call_type)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_type, CM_ERROR_INVALID_PARAMETER);

	*call_type = call_data->call_type;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_call_state(cm_call_data_t *call_data, cm_call_state_e *call_state)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_state, CM_ERROR_INVALID_PARAMETER);

	*call_state = call_data->call_state;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_call_domain(cm_call_data_t *call_data, cm_call_domain_e *call_domain)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_domain, CM_ERROR_INVALID_PARAMETER);

	*call_domain = call_data->call_domain;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_person_id(cm_call_data_t *call_data, int *person_id)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(person_id, CM_ERROR_INVALID_PARAMETER);

	*person_id = call_data->person_id;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_call_member_count(cm_call_data_t *call_data, int *member_count)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(member_count, CM_ERROR_INVALID_PARAMETER);

	*member_count = call_data->member_count;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_is_emergency_call(cm_call_data_t *call_data, gboolean *is_emergency)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(is_emergency, CM_ERROR_INVALID_PARAMETER);

	*is_emergency = call_data->is_ecc;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_is_voicemail_number(cm_call_data_t *call_data, gboolean *is_voicemail_number)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(is_voicemail_number, CM_ERROR_INVALID_PARAMETER);

	*is_voicemail_number = call_data->is_voicemail_number;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_start_time(cm_call_data_t *call_data, long *start_time)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(start_time, CM_ERROR_INVALID_PARAMETER);

	*start_time = call_data->start_time;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_data_get_name_mode(cm_call_data_t *call_data, cm_name_mode_e *name_mode)
{
	CM_RETURN_VAL_IF_FAIL(call_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(name_mode, CM_ERROR_INVALID_PARAMETER);

	*name_mode = call_data->name_mode;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_event_data_get_call_id(cm_call_event_data_t *call_event_data, unsigned int *call_id)
{
	CM_RETURN_VAL_IF_FAIL(call_event_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_id, CM_ERROR_INVALID_PARAMETER);
	*call_id = call_event_data->call_id;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_event_data_get_sim_slot(cm_call_event_data_t *call_event_data, cm_multi_sim_slot_type_e *sim_slot)
{
	CM_RETURN_VAL_IF_FAIL(call_event_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(sim_slot, CM_ERROR_INVALID_PARAMETER);
	*sim_slot = call_event_data->sim_slot;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_event_data_get_end_cause(cm_call_event_data_t *call_event_data, cm_call_end_cause_type_e *end_cause)
{
	CM_RETURN_VAL_IF_FAIL(call_event_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(end_cause, CM_ERROR_INVALID_PARAMETER);
	*end_cause = call_event_data->end_cause;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_event_data_get_incom_call(cm_call_event_data_t *call_event_data, cm_call_data_t **incom_call)
{
	CM_RETURN_VAL_IF_FAIL(call_event_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(incom_call, CM_ERROR_INVALID_PARAMETER);
	*incom_call = call_event_data->incom;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_event_data_get_active_call(cm_call_event_data_t *call_event_data, cm_call_data_t **active_call)
{
	CM_RETURN_VAL_IF_FAIL(call_event_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(active_call, CM_ERROR_INVALID_PARAMETER);
	*active_call = call_event_data->active_dial;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_call_event_data_get_held_call(cm_call_event_data_t *call_event_data, cm_call_data_t **held_call)
{
	CM_RETURN_VAL_IF_FAIL(call_event_data, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(held_call, CM_ERROR_INVALID_PARAMETER);
	*held_call = call_event_data->held;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_conf_call_data_get_call_id(cm_conf_call_data_t *conf_call, unsigned int *call_id)
{
	CM_RETURN_VAL_IF_FAIL(conf_call, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_id, CM_ERROR_INVALID_PARAMETER);

	*call_id = conf_call->call_id;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_conf_call_data_get_call_number(cm_conf_call_data_t *conf_call, char **call_number)
{
	CM_RETURN_VAL_IF_FAIL(conf_call, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_number, CM_ERROR_INVALID_PARAMETER);

	*call_number = conf_call->call_number;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_conf_call_data_get_person_id(cm_conf_call_data_t *conf_call, int *person_id)
{
	CM_RETURN_VAL_IF_FAIL(conf_call, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(person_id, CM_ERROR_INVALID_PARAMETER);

	*person_id = conf_call->person_id;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_conf_call_data_get_name_mode(cm_conf_call_data_t *conf_call, cm_name_mode_e *name_mode)
{
	CM_RETURN_VAL_IF_FAIL(conf_call, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(name_mode, CM_ERROR_INVALID_PARAMETER);

	*name_mode = conf_call->name_mode;
	return CM_ERROR_NONE;
}

EXPORT_API int cm_conf_call_data_free(cm_conf_call_data_t *conf_call)
{
	CM_RETURN_VAL_IF_FAIL(conf_call, CM_ERROR_INVALID_PARAMETER);
	g_free(conf_call->call_number);
	g_free(conf_call);

	return CM_ERROR_NONE;
}

EXPORT_API int cm_set_lcd_state(cm_lcd_control_state_e state)
{
	dbg("[%d]", state);
	int result = -1;
	switch (state) {
	case CM_LCD_OFF:
		result = device_display_change_state(DISPLAY_STATE_SCREEN_OFF);
		break;

	case CM_LCD_ON:
		result = device_display_change_state(DISPLAY_STATE_NORMAL);
		break;

	case CM_LCD_ON_LOCK:
		result = device_display_change_state(DISPLAY_STATE_NORMAL);
		result = device_power_request_lock(POWER_LOCK_DISPLAY, 0);
		break;

	case CM_LCD_ON_UNLOCK:
		result = device_display_change_state(DISPLAY_STATE_NORMAL);
		result = device_power_release_lock(POWER_LOCK_DISPLAY);
		result = device_power_release_lock(POWER_LOCK_CPU);
		break;

	case CM_LCD_OFF_SLEEP_LOCK:
		result = device_power_request_lock(POWER_LOCK_CPU, 0);
		break;

	case CM_LCD_OFF_SLEEP_UNLOCK:
		result = device_power_release_lock(POWER_LOCK_DISPLAY);
		result = device_power_release_lock(POWER_LOCK_CPU);
		break;

	default:
		break;
	}
	if (result != DEVICE_ERROR_NONE) {
		warn("error during change lcd state");
		return CM_ERROR_OPERATION_FAILED;
	}

	return CM_ERROR_NONE;
}

EXPORT_API int cm_start_voice_record(cm_client_h handle, char *call_num)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_start_voice_record()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(call_num, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_START_VOICE_RECORD,
			g_variant_new("(s)", call_num), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_stop_voice_record(cm_client_h handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = 0;
	dbg("cm_stop_voice_record()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_STOP_VOICE_RECORD,
			g_variant_new("()"), NULL, G_DBUS_CALL_FLAGS_NONE,
			CM_DEFAULT_TIMEOUT, handle->ca, &error);

	if (error) {
		err("Failed: %s", error->message);
		g_error_free(error);
		return CM_ERROR_OPERATION_FAILED;
	}

	if (!dbus_result) {
		err("no result");
		return CM_ERROR_OPERATION_FAILED;
	}

	 g_variant_get(dbus_result, "(i)", &value);
	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_set_voice_record_status_cb(cm_client_h handle, cm_voice_record_status_cb cb, void *user_data)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(cb, CM_ERROR_INVALID_PARAMETER);
	return __cm_set_cb(handle, DBUS_CALL_MANAGER_SIGNAL_VOICE_RECORD_STATUS, cb, user_data);
}

EXPORT_API int cm_unset_voice_record_status_cb(cm_client_h handle)
{
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	return __cm_unset_cb(handle, DBUS_CALL_MANAGER_SIGNAL_VOICE_RECORD_STATUS);
}

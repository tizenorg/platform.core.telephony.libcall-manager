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

#include "call-manager-product.h"
#include "call-manager-extension.h"
#include "call-manager-common.h"


EXPORT_API 	int cm_dial_emergency_contact_call(cm_client_h handle, char*number)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gint value = CM_ERROR_NONE;
	dbg("cm_dial_emergency_contact_call()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);
	CM_RETURN_VAL_IF_FAIL(number, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_DIAL_CALL,
			g_variant_new("(siiib)", number, CM_CALL_TYPE_VOICE, CM_SIM_SLOT_DEFAULT_E, 0, TRUE/*is_emergency_contact*/), NULL, G_DBUS_CALL_FLAGS_NONE,
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
			LOGE("PERMISSION_DENIED");
			value = CM_ERROR_PERMISSION_DENIED;
		}
		g_error_free(error);
	}

	 g_variant_unref(dbus_result);

	 return value;
}

EXPORT_API int cm_get_answering_machine_status(cm_client_h handle, gboolean *is_answering_machine_on)
{
	GVariant *dbus_result;
	GError *error = NULL;
	gboolean value = 0;
	dbg("cm_get_audio_state()");
	CM_RETURN_VAL_IF_FAIL(handle, CM_ERROR_INVALID_PARAMETER);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_conn, DBUS_CALL_MANAGER,
			DBUS_CALL_MANAGER_PATH, DBUS_CALL_MANAGER_DEFAULT_INTERFACE, DBUS_CALL_MANAGER_METHOD_GET_ANSWERING_MACHINE_STATUS,
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

	g_variant_get(dbus_result, "(b)", &value);
	*is_answering_machine_on = value;
	g_variant_unref(dbus_result);

	return CM_ERROR_NONE;
}


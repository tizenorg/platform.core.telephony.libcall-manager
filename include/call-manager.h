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

#ifndef __TIZEN_CALL_MANAGER_H__
#define __TIZEN_CALL_MANAGER_H__

/**
 * @file call_manager.h
 * @brief This file contains Call Manager APIs and related enumerations.
 */

#include <tizen.h>

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @file call_manager.h
 * @brief This file contains the Call Manager API.
 */

/**
 * @addtogroup CAPI_CALL_MANAGER_MODULE
 * @{
 */

/**
 * @brief The Call Manager API handle.
 * @since_tizen 2.4
 */
typedef struct __cm_client *cm_client_h;

// TODO:
// Below define will be removed when Call Manager error included in tizen_error.h during ACR
#ifndef TIZEN_ERROR_CALL_MANAGER
#define TIZEN_ERROR_CALL_MANAGER       -0x00000000
#endif

/**
 * @brief Enumeration for Call Manager error.
 * @since_tizen 2.4
 */
typedef enum {
 CM_ERROR_NONE = TIZEN_ERROR_NONE,                            /**< Successful */
 CM_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,          /**< Out of memory */
 CM_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,  /**< Invalid parameter */
 CM_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED,  /**< Permission denied */
 CM_ERROR_NOT_SUPPORTED = TIZEN_ERROR_NOT_SUPPORTED,          /**< Not supported */
 CM_ERROR_NOT_REGISTERED = TIZEN_ERROR_CALL_MANAGER | 0x01,  /**< Callback not registered */
 CM_ERROR_ALREADY_REGISTERED = TIZEN_ERROR_CALL_MANAGER | 0x02,  /**< Callback already registered */
 CM_ERROR_OPERATION_FAILED = TIZEN_ERROR_CALL_MANAGER | 0x03,  /**< Operation failed */
} cm_error_e;

/**
 * @brief Enumeration for Call status.
 * @since_tizen 2.4
 */
typedef enum {
	CM_CALL_STATUS_IDLE, 	    /**< Call is in idle state */
	CM_CALL_STATUS_RINGING,		/**< A new call arrived and is ringing or waiting */
	CM_CALL_STATUS_OFFHOOK,		/**< At least one call exist that is in dialing, alerting, active, on hold state */
	CM_CALL_STATUS_MAX
} cm_call_status_e;

/**
 * @brief Enumeration for Mute status.
 * @since_tizen 2.4
 */
typedef enum {
	CM_MUTE_STATUS_OFF, 	   	 /**< Mute state is off */
	CM_MUTE_STATUS_ON,		/**<Mute state is on */
	CM_MUTE_STATUS_MAX
} cm_mute_status_e;


/**
 * @brief Called when the call status changes.
 * @since_tizen 2.4
 * @param[in]   call_status	The changed call status
 * @param[in]   call_num	The call number with changed call status
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre You should register this callback using cm_set_call_status_cb().
 * @see cm_set_call_status_cb()
 * @see cm_unset_call_status_cb()
 */
typedef void (*cm_call_status_cb) (cm_call_status_e call_status, const char *call_num, void *user_data);


/**
 * @brief Called when the mute status changes.
 * @since_tizen 2.4
 * @param[in]   mute_status	The changed mute status
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre You should register this callback using cm_set_mute_status_cb().
 * @see cm_set_mute_status_cb()
 * @see cm_unset_mute_status_cb()
 */
typedef void (*cm_mute_status_cb) (cm_mute_status_e mute_status, void *user_data);


/**
 * @brief Acquires the handle to use the Call Manager API.
 *
 * @since_tizen 2.4
 *
 * @param[out] handle The handle to use the Call Manager API
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_deinit()
 */
int cm_init (cm_client_h *handle);

/**
 * @brief Deinitializes the Call Manager handle.
 *
 * @since_tizen 2.4
 *
 * @param[in] handle The handle to be deinitialized
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_init()
 */
int cm_deinit (cm_client_h handle);

/**
 * @brief Rejects the incoming call.
 *
 * @since_tizen 2.4
 * @privlevel platform
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
int cm_reject_call(cm_client_h handle);

/**
 * @brief Stops incoming call alert ringtone.
 *
 * @since_tizen 2.4
 * @privlevel platform
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
int cm_stop_alert(cm_client_h handle);

/**
 * @brief Gets the call status.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege TBD
 *
 * @param[in] handle The handle from #cm_init()
 * @param[out] call_status The status of the call (#cm_call_status_e)
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
int cm_get_call_status (cm_client_h handle, cm_call_status_e *call_status);

/**
 * @brief Sets a callback function to be invoked when the call status changes.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege TBD
 *
 * @param[in] handle  The handle to use the Call Manager API
 * @param[in] cb The callback to be invoked when the call status changes
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_ALREADY_REGISTERED Callback already registered
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_call_status_cb() will be invoked.
 *
 * @see cm_unset_call_status_cb()
 */
int cm_set_call_status_cb (cm_client_h handle, cm_call_status_cb cb, void *user_data);

/**
 * @brief Unsets a callback function.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege TBD
 *
 * @param[in] handle  The handle to use the Call Manager API
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_SUPPORTED     Not supported
 * @retval #CM_ERROR_NOT_REGISTERED Callback not registered
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_call_status_cb()
 */
int cm_unset_call_status_cb (cm_client_h handle);

/**
 * @brief Gets the mute status.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from #cm_init()
 * @param[out] mutel_status The status of the call (#cm_mute_status_e)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @pre The handle should be obtained from #cm_init()
 * @post The should be released by using #cm_deinit()
 */
int cm_get_mute_status (cm_client_h handle, cm_mute_status_e *mutel_status);

/**
 * @brief Sets a callback function to be invoked when the mute status changes.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle  The handle to use the Call Manager API
 * @param[in] cb The callback to be invoked when the mute status changes
 * @param[in] user_data The user data passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_ALREADY_REGISTERED Callback already registered
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @post cm_mute_status_cb() will be invoked.
 *
 * @see cm_unset_mute_status_cb()
 */
int cm_set_mute_status_cb (cm_client_h handle, cm_mute_status_cb cb, void *user_data);

/**
 * @brief Unsets a callback function.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle  The handle to use the Call Manager API
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #CM_ERROR_NONE              Successful
 * @retval #CM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CM_ERROR_PERMISSION_DENIED Permission denied
 * @retval #CM_ERROR_NOT_REGISTERED Callback not registered
 * @retval #CM_ERROR_OPERATION_FAILED  Operation failed
 *
 * @see cm_set_mute_status_cb()
 */
int cm_unset_mute_status_cb (cm_client_h handle);


/**
 * @}
 */

#ifdef __cplusplus
 }
#endif

#endif /* __TIZEN_CALL_MANAGER_H__ */

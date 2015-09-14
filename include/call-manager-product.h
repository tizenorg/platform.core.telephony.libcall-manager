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

#ifndef __CALL_MANAGER_PRODUCT_H__
#define __CALL_MANAGER_PRODUCT_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <glib.h>

#include "call-manager.h"

int cm_dial_emergency_contact_call(cm_client_h handle, char*number);
int cm_get_answering_machine_status(cm_client_h handle, gboolean *is_answering_machine_on);


#ifdef __cplusplus
 }
#endif

#endif /* __CALL_MANAGER_PRODUCT_H__ */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <glib.h>
#include <glib-object.h>

#include <call-manager.h>
#include <call-manager-extension.h>

static GMainLoop *gloop = NULL;
static gchar *keybuf = NULL;
GIOChannel *ioChannel = NULL;
cm_client_h cm_handle = NULL;

static void __call_status_changed_cb(cm_call_status_e call_status, const char* call_num, void *user_data)
{
	g_print("====> Call Status changed!!!!<====\n");
	g_print("====> Call status: %d\n", call_status);

	switch (call_status) {
	case CM_CALL_STATUS_IDLE:
		g_print("Call status is idle!\n");
		break;
	case CM_CALL_STATUS_RINGING:
		{
			g_print("Call status is ringing!\n");
			if (call_num) {
				g_print("call num is %s\n", call_num);
			}
		}
		break;
	case CM_CALL_STATUS_OFFHOOK:
		g_print("Call status is offhook!\n");
		break;
	default:
		break;
	}

	g_print("Call status changed cb done\n");
	return;
}

static void __mute_status_changed_cb(cm_mute_status_e mute_status, void *user_data)
{
	g_print("====> Mute Status changed!!!!<====\n");
	g_print("====> Mute status: %d\n", mute_status);

	switch (mute_status) {
	case CM_MUTE_STATUS_OFF:
		g_print("Mute OFF !\n");
		break;
	case CM_MUTE_STATUS_ON:
		g_print("Mute ON !\n");
		break;
	default:
		break;
	}

	g_print("Mute status changed cb done\n");
	return;
}

static void __call_event_changed_cb(cm_call_event_e call_event, cm_call_event_data_t* event_data, void *user_data)
{
	g_print("====> Call event changed!!!!<====\n");
	g_print("====>Event_id: %d\n", call_event);

	switch (call_event) {
		case CM_CALL_EVENT_ACTIVE:
		{
			cm_multi_sim_slot_type_e sim_slot = 0;
			unsigned int call_id = 0;
			cm_call_data_t *incom_call = NULL;
			cm_call_data_t *active_call = NULL;
			cm_call_data_t *held_call = NULL;

			cm_call_event_data_get_sim_slot(event_data, &sim_slot);
			g_print("sim_slot: %d\n", sim_slot);
			cm_call_event_data_get_call_id(event_data, &call_id);
			g_print("call_id: %d\n", call_id);
			cm_call_event_data_get_active_call(event_data, &active_call);
			cm_call_event_data_get_incom_call(event_data, &incom_call);
			cm_call_event_data_get_held_call(event_data, &held_call);
			break;
		}
		case CM_CALL_EVENT_IDLE:
		{
			cm_call_end_cause_type_e end_cause = CM_CALL_ENDCAUSE_MAX;
			cm_call_event_data_get_end_cause(event_data, &end_cause);
			g_print("end_cause: %d\n", end_cause);
		}
		case CM_CALL_EVENT_INCOMING:
		case CM_CALL_EVENT_ALERT:
		case CM_CALL_EVENT_DIALING:
		case CM_CALL_EVENT_HELD:
		case CM_CALL_EVENT_RETRIEVED:
		case CM_CALL_EVENT_SWAPPED:
		case CM_CALL_EVENT_WAITING:
		case CM_CALL_EVENT_JOIN:
		case CM_CALL_EVENT_SPLIT:
			break;
		default:
			break;
	}
	return;
}

static void __dtmf_indication_cb(cm_dtmf_indi_type_e indi_type, char* dtmf_number, void *user_data)
{
	g_print("====> __dtmf_indication_cb!!!!<====\n");
	g_print("====>indi type: %d\n", indi_type);
	if (dtmf_number)
		g_print("====>dtmf number: %s\n", dtmf_number);

	return;
}

static void __audio_state_changed_cb(cm_audio_state_type_e audio_status, void *user_data)
{
	g_print("====> __audio_state_changed_cb!!!!<====\n");
	g_print("====>audio status: %d\n", audio_status);

	return;
}

static void __go_foreground_cb(void *user_data)
{
	g_print("====> __go_foreground_cb!!!!<====\n");

	return;
}

static void __print_conf_call_data(cm_conf_call_data_t* call_data)
{
	if (call_data == NULL) {
		g_print("conf call data is NULL\n");
		return;
	}

	unsigned int call_id;
	char *call_number = NULL;
	int person_id;
	cm_name_mode_e name_mode;
	cm_conf_call_data_get_call_id(call_data, &call_id);
	cm_conf_call_data_get_call_number(call_data, &call_number);
	cm_conf_call_data_get_person_id(call_data, &person_id);
	cm_conf_call_data_get_name_mode(call_data, &name_mode);
	g_print("conf call call_id : %d\n", call_id);
	g_print("conf call person_id : %d\n", person_id);
	g_print("conf call call_number : %s\n", call_number ? call_number : "");
	g_print("conf call name_mode : %d\n", name_mode);

	return;
}

static void __print_call_data(cm_call_data_t* call_data)
{
	if (call_data == NULL) {
		g_print("call data is NULL\n");
		return;
	}

	unsigned int call_id;
	cm_call_direction_e call_direction;
	char *call_number = NULL;
	char *calling_name = NULL;
	cm_call_type_e call_type;
	cm_call_state_e call_state;
	cm_call_domain_e call_domain;
	int person_id;
	int member_count;
	gboolean is_emergency;
	long start_time;

	cm_call_data_get_call_id(call_data, &call_id);
	cm_call_data_get_call_number(call_data, &call_number);
	cm_call_data_get_calling_name(call_data, &calling_name);
	cm_call_data_get_call_member_count(call_data, &member_count);
	cm_call_data_get_call_direction(call_data, &call_direction);
	cm_call_data_get_call_domain(call_data, &call_domain);
	cm_call_data_get_call_state(call_data, &call_state);
	cm_call_data_get_call_type(call_data, &call_type);
	cm_call_data_is_emergency_call(call_data, &is_emergency);
	cm_call_data_get_start_time(call_data, &start_time);
	cm_call_data_get_person_id(call_data, &person_id);

	g_print("call_id : %d\n", call_id);
	g_print("call_direction : %d\n", call_direction);
	g_print("call_number : %s\n", call_number ? call_number : "");
	g_print("calling_name : %s\n", calling_name ? calling_name : "");
	g_print("call_type : %d\n", call_type);
	g_print("call_state : %d\n", call_state);
	g_print("member_count : %d\n", member_count);
	g_print("is_ecc : %d\n", is_emergency);
	g_print("call_domain : %d\n", call_domain);
	g_print("person_id : %d\n", person_id);
	g_print("start_time : %ld\n", start_time);

	return;
}


static void __print_menu(void)
{
	g_print("+++++++++++++++++++++++++++++++++++++\n");
	g_print("Operations\n");
	g_print("%-40s: exit this program\n", "0. exit");
	g_print("%-40s: print this message\n", "1. help");
	g_print("=====================================\n");
	g_print("%-40s: CM Initialize\n", "2. init");
	g_print("%-40s: CM Deinitialize\n", "3. deinit");
	g_print("\n");
	g_print("%-40s: Set Call Event Changed Cb\n", "4. setcb");
	g_print("%-40s: Unset Call Event Changed Cb\n", "5. unsetcb");
	g_print("\n");
	g_print("%-40s: Dial Voice Call\n", "6. dial_voicecall");
	g_print("%-40s: Dial Video Call\n", "7. dial_videocall");
	g_print("%-40s: End Call\n", "8. end");
	g_print("%-40s: Swap Call\n", "9. swap");
	g_print("%-40s: Hold Call\n", "10. hold");
	g_print("%-40s: Unhold Call\n", "11.unhold");
	g_print("%-40s: Join Call\n", "12.join");
	g_print("%-40s: Split Call\n", "13.split");
	g_print("%-40s: Answer Call\n", "14.answer");
	g_print("\n");
	g_print("%-40s: Set Extra volume on/off\n", "15.extra_vol");
	g_print("%-40s: Set Mute state on/off\n", "16.mute_state");
	g_print("%-40s: SPK on/off\n", "17.spk");
	g_print("%-40s: bluetooth on/off\n", "18.bluetooth");
	g_print("%-40s: Set DTMF indi handler\n", "19.setdtmfcb");
	g_print("%-40s: Unset DTMF indi handler\n", "20.unsetdtmfcb");
	g_print("%-40s: Send DTMF response\n", "21.senddtmfrsp");
	g_print("%-40s: Set Audio State Changed Cb\n", "22.setaudiocb");
	g_print("%-40s: Unset Audio State Changed Cb\n", "23.unsetaudiocb");
	g_print("%-40s: Get Audio State\n", "24.getaudiostate");
	g_print("\n");
	g_print("%-40s: Set Foreground Cb\n", "25.setfg");
	g_print("%-40s: Unset Foreground Cb\n", "26.unsetfg");
	g_print("%-40s: Activate UI\n", "27.activate");
	g_print("%-40s: Set Call Status Changed Cb\n", "28.setstatuscb");
	g_print("%-40s: Unset Call Status Changed Cb\n", "29.unsetstatuscb");
	g_print("%-40s: Transfer call\n", "30.transfercall");
	g_print("%-40s: Start dtmf\n", "31.startdtmf");
	g_print("%-40s: Stop dtmf\n", "32.stopdtmf");
	g_print("%-40s: Burst dtmf\n", "33.burstdtmf");
	g_print("%-40s: Stop alert\n", "34.stopalert");
	g_print("%-40s: Start alert\n", "35.startalert");
	g_print("\n");
	g_print("%-40s: Get all call list \n", "36.getallcalllist");
	g_print("%-40s: Get conference call list\n", "37.getconfcalllist");
	g_print("%-40s: Get all call data\n", "38.getallcalldata");
	g_print("%-40s: Reject call\n", "39.reject");
	g_print("%-40s: Get call status\n", "40.get_callstatus");
	g_print("%-40s: Set Mute Status Changed Cb\n", "41.setmutestatuscb");
	g_print("%-40s: Unset Mute Status Changed Cb\n", "42.unsetmutestatuscb");
	g_print("%-40s: Get Mute State\n", "43.getmutestatus");
	g_print("+++++++++++++++++++++++++++++++++++++\n");
}

static gboolean __process_input(gchar * buffer)
{
	gchar **strsplits = NULL;

	g_strchomp(buffer);
	strsplits = g_strsplit_set(buffer, " ,\t\n", 2);

	if (strsplits[0] == NULL) {
		g_print("Input > ");
		g_strfreev(strsplits);
		return TRUE;
	} else if ((g_strcmp0(strsplits[0], "1") == 0) || (g_strcmp0(strsplits[0], "help") == 0)) {
		__print_menu();
	} else if ((g_strcmp0(strsplits[0], "0") == 0) || (g_strcmp0(strsplits[0], "exit") == 0)) {
		g_main_loop_quit(gloop);
	} else if ((g_strcmp0(strsplits[0], "2") == 0) || (g_strcmp0(strsplits[0], "init") == 0)) {
		if (cm_handle) {
			g_print("!!!!!!!!!!!!!!!!Already initialized!!!!!!!!!!!!!!!!!!!!!\n");
		} else {
			cm_init(&cm_handle);
			cm_enable_recovery(cm_handle, "org.tizen.cmtest");
		}
	} else if ((g_strcmp0(strsplits[0], "3") == 0) || (g_strcmp0(strsplits[0], "deinit") == 0)) {
		if (cm_handle) {
			cm_deinit(cm_handle);
			cm_handle = NULL;
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "4") == 0) || (g_strcmp0(strsplits[0], "setcb") == 0)) {
		if (cm_handle) {
			g_print(">>>>>>Monitor Call Event Change\n");
			cm_set_call_event_cb(cm_handle, __call_event_changed_cb, NULL);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "5") == 0) || (g_strcmp0(strsplits[0], "unsetcb") == 0)) {
		if (cm_handle) {
			g_print(">>>>>>Monitor Call Event Change\n");
			cm_unset_call_event_cb(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "6") == 0) || (g_strcmp0(strsplits[0], "dial_voicecall") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter dial number!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				g_print(">>>>>>Dial to %s\n", strsplits[1]);
				cm_dial_call(cm_handle, strsplits[1], CM_CALL_TYPE_VOICE, CM_SIM_SLOT_DEFAULT_E);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "7") == 0) || (g_strcmp0(strsplits[0], "dial_videocall") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter dial number!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				g_print(">>>>>>Dial to %s\n", strsplits[1]);
				cm_dial_call(cm_handle, strsplits[1], CM_CALL_TYPE_VIDEO, CM_SIM_SLOT_DEFAULT_E);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "8") == 0) || (g_strcmp0(strsplits[0], "end") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter Call ID!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				unsigned int call_id = strtoul(strsplits[1], NULL, 10);
				g_print(">>>>>>End call %d\n", call_id);
				cm_end_call(cm_handle, call_id, CALL_RELEASE_TYPE_BY_CALL_HANDLE);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "9") == 0) || (g_strcmp0(strsplits[0], "swap") == 0)) {
		if (cm_handle) {
			cm_swap_call(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "10") == 0) || (g_strcmp0(strsplits[0], "hold") == 0)) {
		if (cm_handle) {
			cm_hold_call(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "11") == 0) || (g_strcmp0(strsplits[0], "unhold") == 0)) {
		if (cm_handle) {
			cm_unhold_call(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "12") == 0) || (g_strcmp0(strsplits[0], "join") == 0)) {
		if (cm_handle) {
			cm_join_call(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "13") == 0) || (g_strcmp0(strsplits[0], "split") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter Call ID!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				unsigned int call_id = strtoul(strsplits[1], NULL, 10);
				g_print(">>>>>>Split call %d\n", call_id);
				cm_split_call(cm_handle, call_id);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "14") == 0) || (g_strcmp0(strsplits[0], "answer") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter Answer Type!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				int ans_type = strtoul(strsplits[1], NULL, 10);
				g_print(">>>>>>Answer call %d\n", ans_type);
				cm_answer_call(cm_handle, ans_type);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "15") == 0) || (g_strcmp0(strsplits[0], "extra_vol") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter \"on\" or \"off\"!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				g_print(">>>>>>Extra volume %s\n", strsplits[1]);
				if (g_strcmp0(strsplits[1], "on") == 0) {
					cm_set_extra_vol(cm_handle, 1);
				} else {
					cm_set_extra_vol(cm_handle, 0);
				}
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "16") == 0) || (g_strcmp0(strsplits[0], "mute_state") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter \"on\" or \"off\"!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				g_print(">>>>>>Set Mute state %s\n", strsplits[1]);
				if (g_strcmp0(strsplits[1], "on") == 0) {
					cm_set_mute_state(cm_handle, 1);
				} else {
					cm_set_mute_state(cm_handle, 0);
				}
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "17") == 0) || (g_strcmp0(strsplits[0], "spk") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter \"on\" or \"off\"!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				g_print(">>>>>>Speaker %s\n", strsplits[1]);
				if (g_strcmp0(strsplits[1], "on") == 0) {
					cm_speaker_on(cm_handle);
				} else {
					cm_speaker_off(cm_handle);
				}
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "18") == 0) || (g_strcmp0(strsplits[0], "bluetooth") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter \"on\" or \"off\"!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				g_print(">>>>>>Bluetooth %s\n", strsplits[1]);
				if (g_strcmp0(strsplits[1], "on") == 0) {
					cm_bluetooth_on(cm_handle);
				} else {
					cm_bluetooth_off(cm_handle);
				}
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "19") == 0) || (g_strcmp0(strsplits[0], "setdtmfcb") == 0)) {
		if (cm_handle) {
			cm_set_dtmf_indication_cb(cm_handle, __dtmf_indication_cb, NULL);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "20") == 0) || (g_strcmp0(strsplits[0], "unsetdtmfcb") == 0)) {
		if (cm_handle) {
			cm_unset_dtmf_indication_cb(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "21") == 0) || (g_strcmp0(strsplits[0], "senddtmfrsp") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter response type!!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				int resp_type = strtoul(strsplits[1], NULL, 10);
				g_print(">>>>>>resp type %d\n", resp_type);
				cm_send_dtmf_resp(cm_handle, resp_type);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "22") == 0) || (g_strcmp0(strsplits[0], "setaudiocb") == 0)) {
		if (cm_handle) {
			cm_set_audio_state_changed_cb(cm_handle, __audio_state_changed_cb, NULL);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "23") == 0) || (g_strcmp0(strsplits[0], "unsetaudiocb") == 0)) {
		if (cm_handle) {
			cm_unset_audio_state_changed_cb(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "24") == 0) || (g_strcmp0(strsplits[0], "getaudiostate") == 0)) {
		if (cm_handle) {
			cm_audio_state_type_e audio_state;
			cm_get_audio_state(cm_handle, &audio_state);
			g_print("current audio state: %d\n", audio_state);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "25") == 0) || (g_strcmp0(strsplits[0], "setfg") == 0)) {
		if (cm_handle) {
			cm_set_foreground_cb(cm_handle, __go_foreground_cb, NULL);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "26") == 0) || (g_strcmp0(strsplits[0], "unsetfg") == 0)) {
		if (cm_handle) {
			cm_unset_foreground_cb(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "27") == 0) || (g_strcmp0(strsplits[0], "activate") == 0)) {
		if (cm_handle) {
			cm_activate_ui(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "28") == 0) || (g_strcmp0(strsplits[0], "setstatuscb") == 0)) {
		if (cm_handle) {
			g_print(">>>>>>Monitor Call Status Change\n");
			cm_set_call_status_cb(cm_handle, __call_status_changed_cb, NULL);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "29") == 0) || (g_strcmp0(strsplits[0], "unsetstatuscb") == 0)) {
		if (cm_handle) {
			g_print(">>>>>>Monitor Call Status Change\n");
			cm_unset_call_status_cb(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "30") == 0) || (g_strcmp0(strsplits[0], "transfercall") == 0)) {
		if (cm_handle) {
			cm_transfer_call(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "31") == 0) || (g_strcmp0(strsplits[0], "startdtmf") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter dtmf_digit !!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				cm_start_dtmf(cm_handle, strsplits[1][0]);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "32") == 0) || (g_strcmp0(strsplits[0], "stopdtmf") == 0)) {
		if (cm_handle) {
			cm_stop_dtmf(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "33") == 0) || (g_strcmp0(strsplits[0], "burstdtmf") == 0)) {
		if (cm_handle) {
			if (strsplits[1] == NULL) {
				g_print("!!!!!!!!!!!!!!!!Enter dtmf_digit !!!!!!!!!!!!!!!!!!!!!\n");
			} else {
				cm_burst_dtmf(cm_handle, strsplits[1]);
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "34") == 0) || (g_strcmp0(strsplits[0], "stopalert") == 0)) {
		if (cm_handle) {
			cm_stop_alert(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "35") == 0) || (g_strcmp0(strsplits[0], "startalert") == 0)) {
		if (cm_handle) {
			cm_start_alert(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "36") == 0) || (g_strcmp0(strsplits[0], "getallcalllist") == 0)) {
		if (cm_handle) {
			GSList *list = NULL;
			cm_get_all_call_list(cm_handle, &list);
			int list_len = g_slist_length(list);
			if (list == NULL)
				g_print("all call list is NULL!\n");
			else {
				int idx;
				cm_call_data_t *callData = NULL;
				for (idx = 0; idx < list_len; idx++) {
					callData = (cm_call_data_t *)g_slist_nth_data(list, idx);
					__print_call_data(callData);
					cm_call_data_free(callData);
				}
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "37") == 0) || (g_strcmp0(strsplits[0], "getconfcalllist") == 0)) {
		if (cm_handle) {
			GSList *list = NULL;
			cm_conf_call_data_t *call_data = NULL;
			cm_get_conference_call_list(cm_handle, &list);
			if (list == NULL)
				g_print("conference_call_list is NULL!\n");
			else{
				int list_len = g_slist_length(list);
				int idx;
				for (idx = 0; idx < list_len; idx++) {
					call_data = (cm_conf_call_data_t *)g_slist_nth_data(list, idx);
					__print_conf_call_data(call_data);
					cm_conf_call_data_free(call_data);
				}
			}
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "38") == 0) || (g_strcmp0(strsplits[0], "getallcalldata") == 0)) {
		if (cm_handle) {
			cm_call_data_t *incom = NULL;
			cm_call_data_t *active = NULL;
			cm_call_data_t *held = NULL;
			cm_get_all_calldata(cm_handle, &incom, &active, &held);
			g_print("incom call data:\n");
			__print_call_data(incom);
			g_print("active call data:\n");
			__print_call_data(active);
			g_print("held call data:\n");
			__print_call_data(held);
			cm_call_data_free(incom);
			cm_call_data_free(active);
			cm_call_data_free(held);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "39") == 0) || (g_strcmp0(strsplits[0], "reject") == 0)) {
		if (cm_handle) {
			cm_reject_call(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "40") == 0) || (g_strcmp0(strsplits[0], "get_callstatus") == 0)) {
		if (cm_handle) {
			cm_call_status_e call_status = CM_CALL_STATUS_IDLE;
			cm_get_call_status(cm_handle, &call_status);
			g_print("Call Status: %d\n", call_status);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "41") == 0) || (g_strcmp0(strsplits[0], "setmutestatuscb") == 0)) {
		if (cm_handle) {
			g_print(">>>>>>Monitor Mute Status Change\n");
			cm_set_mute_status_cb(cm_handle, __mute_status_changed_cb, NULL);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "42") == 0) || (g_strcmp0(strsplits[0], "unsetmutestatuscb") == 0)) {
		if (cm_handle) {
			g_print(">>>>>>Stop monitor Mute Status Change\n");
			cm_unset_mute_status_cb(cm_handle);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	} else if ((g_strcmp0(strsplits[0], "43") == 0) || (g_strcmp0(strsplits[0], "getmutestatus") == 0)) {
		if (cm_handle) {
			cm_mute_status_e mute_status;
			cm_get_mute_status(cm_handle, &mute_status);
			g_print("current mute status: %d\n", mute_status);
		} else {
			g_print("!!!!!!!!!!!!!!!!Not initialized!!!!!!!!!!!!!!!!!!!!!\n");
		}
	}

	g_print("Input > ");
	g_strfreev(strsplits);
	return TRUE;
}

static gboolean __keyboard_event(GIOChannel * source, GIOCondition condition, gpointer data)
{
	GError *error = NULL;
	g_io_channel_read_line(source, &keybuf, NULL, NULL, &error);
	return __process_input(keybuf);
}

static void __init_keyboard(gpointer user_data)
{
	GIOChannel *ioChannel = NULL;
	ioChannel = g_io_channel_unix_new(fileno(stdin));
	if (ioChannel == NULL) {
		g_error("Fail to Create keyboard");
	}
	g_io_add_watch(ioChannel, G_IO_IN, __keyboard_event, user_data);
}

static void __interrupt_cb(int signal)
{
	g_print("Interrupt\n");
	g_main_loop_quit(gloop);
}

int main(int argc, char **argv)
{
	struct sigaction sa = {.sa_handler = 0 };

#if !GLIB_CHECK_VERSION(2, 35, 0)
	g_type_init();
#endif

	/* set up a signal handler */
	sa.sa_handler = __interrupt_cb;
	sigaction(SIGINT, &sa, NULL);

	gloop = g_main_loop_new(NULL, TRUE);
	__init_keyboard(NULL);
	__print_menu();
	g_print("Input > ");
	g_main_loop_run(gloop);

	if (gloop)
		g_main_loop_unref(gloop);
	if (ioChannel)
		g_io_channel_unref(ioChannel);
	if (keybuf)
		g_free(keybuf);
	return 0;
}


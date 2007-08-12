/*
 * Copyright (c) 2004-2007 Voltaire, Inc. All rights reserved.
 * Copyright (c) 2002-2005 Mellanox Technologies LTD. All rights reserved.
 * Copyright (c) 1996-2003 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/*
 * Abstract:
 *	Declaration of osm_subn_t.
 *	This object represents an IBA subnet.
 *	This object is part of the OpenSM family of objects.
 *
 * Environment:
 *	Linux User Mode
 *
 * $Revision: 1.12 $
 */

#ifndef _OSM_SUBNET_H_
#define _OSM_SUBNET_H_

#include <iba/ib_types.h>
#include <complib/cl_qmap.h>
#include <complib/cl_map.h>
#include <complib/cl_ptr_vector.h>
#include <complib/cl_list.h>
#include <opensm/osm_base.h>

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif /* __cplusplus */

BEGIN_C_DECLS

#define OSM_SUBNET_VECTOR_MIN_SIZE			0
#define OSM_SUBNET_VECTOR_GROW_SIZE			1
#define OSM_SUBNET_VECTOR_CAPACITY			256

struct _osm_opensm_t;

/****h* OpenSM/Subnet
* NAME
*	Subnet
*
* DESCRIPTION
*	The Subnet object encapsulates the information needed by the
*	OpenSM to manage a subnet.  The OpenSM allocates one Subnet object
*	per IBA subnet.
*
*	The Subnet object is not thread safe, thus callers must provide
*	serialization.
*
*	This object is essentially a container for the various components
*	of a subnet.  Callers may directly access the member variables.
*
* AUTHOR
*	Steve King, Intel
*
*********/

/****f* OpenSM: Subnet/osm_pfn_ui_extension
* NAME
*	osm_pfn_ui_extension
*
* DESCRIPTION
*	This typedef defines the prototype for UI extension functions.
*	They might be registered in the subnet options to handle pre_lid_assign,
*  and ui_ucast_fwd_assign.
*
* SYNOPSIS
*/
typedef int
(*osm_pfn_ui_extension_t)(
	IN	void*				context );
/*
* PARAMETERS
*	context
*		[in] Client specific context specified in the subnet opt
*          Same prefix as the UI function (suffixed by ctx)
*
* RETURN VALUE
*	This function returns an int (the semantic is different between
*  the different calls)
*
* SEE ALSO
*
*********/

/****f* OpenSM: Subnet/osm_pfn_ui_mcast_extension
* NAME
*	osm_pfn_ui_mcast_extension
*
* DESCRIPTION
*	This typedef defines the prototype for UI extension functions.
*	They might be registered in the subnet options to handle ui_mcast_fwd_assign
*
* SYNOPSIS
*/
typedef void
(*osm_pfn_ui_mcast_extension_t)(
  IN void                 *context,
  IN ib_net16_t            mlid,
  IN osm_mcast_req_type_t  request_type,
  IN ib_net64_t            port_guid );
/*
* PARAMETERS
*  context
*     [in] Client specific context specified in the subnet opt
*          Same prefix as the UI function (suffixed by ctx)
*
*  mlid
*     [in] multicast lid of the group handled.
*
*  request_type
*     [in] Type of MC request being handled (create/join/leave)
*
*  port_guid
*     [in] port gui of the port that was added/removed from the
*          multicast group handled.
*
* RETURN VALUE
*	This function does not return a value.
*
* SEE ALSO
*
*********/

/****d* OpenSM: Subnet/osm_testability_modes_t
* NAME
*	osm_testability_modes_t
*
* DESCRIPTION
*	Enumerates the possible testability modes.
*
* SYNOPSIS
*/
typedef enum _osm_testability_modes
{
  OSM_TEST_MODE_NONE = 0,
  OSM_TEST_MODE_EXIT_BEFORE_SEND_HANDOVER,
  OSM_TEST_MODE_MAX
} osm_testability_modes_t;
/***********/

/****s* OpenSM: Subnet/osm_qos_options_t
* NAME
*	osm_qos_options_t
*
* DESCRIPTION
*	Subnet QoS options structure.  This structure contains the various
*	QoS specific configuration parameters for the subnet.
*
* SYNOPSIS
*/
typedef struct _osm_qos_options_t {
  unsigned max_vls;
  unsigned high_limit;
  char *vlarb_high;
  char *vlarb_low;
  char *sl2vl;
} osm_qos_options_t;
/*
* FIELDS
*
*	max_vls
*		The number of maximum VLs on the Subnet
*
*	high_limit
*		The limit of High Priority component of VL Arbitration
*		table (IBA 7.6.9)
*
*	vlarb_high
*		High priority VL Arbitration table template.
*
*	vlarb_low
*		Low priority VL Arbitration table template.
*
*	sl2vl
*		SL2VL Mapping table (IBA 7.6.6) template.
*
*********/

/****s* OpenSM: Subnet/osm_subn_opt_t
* NAME
*	osm_subn_opt_t
*
* DESCRIPTION
*	Subnet options structure.  This structure contains the various
*	site specific configuration parameters for the subnet.
*
* SYNOPSIS
*/
typedef struct _osm_subn_opt
{
  ib_net64_t               guid;
  ib_net64_t		   m_key;
  ib_net64_t               sm_key;
  ib_net64_t		   subnet_prefix;
  ib_net16_t		   m_key_lease_period;
  uint32_t		   sweep_interval;
  uint32_t		   max_wire_smps;
  uint32_t		   transaction_timeout;
  uint8_t		   sm_priority;
  uint8_t		   lmc;
  boolean_t		   lmc_esp0;
  uint8_t		   max_op_vls;
  uint8_t		   force_link_speed;
  boolean_t		   reassign_lids;
  boolean_t		   ignore_other_sm;
  boolean_t		   single_thread;
  boolean_t		   no_multicast_option;
  boolean_t		   disable_multicast;
  boolean_t		   force_log_flush;
  uint8_t                  subnet_timeout;
  uint8_t                  packet_life_time;
  uint8_t                  vl_stall_count;
  uint8_t                  leaf_vl_stall_count;
  uint8_t                  head_of_queue_lifetime;
  uint8_t                  leaf_head_of_queue_lifetime;
  uint8_t                  local_phy_errors_threshold;
  uint8_t                  overrun_errors_threshold;
  uint32_t                 sminfo_polling_timeout;
  uint32_t                 polling_retry_number;
  uint32_t		   max_msg_fifo_timeout;
  boolean_t                force_heavy_sweep;
  uint8_t                  log_flags;
  char *                   dump_files_dir;
  char *                   log_file;
  unsigned long            log_max_size;
  char *                   partition_config_file;
  boolean_t                no_partition_enforcement;
  boolean_t                no_qos;
  boolean_t                accum_log_file;
  char *                   console;
  uint16_t                 console_port;
  cl_map_t                 port_prof_ignore_guids;
  boolean_t                port_profile_switch_nodes;
  osm_pfn_ui_extension_t   pfn_ui_pre_lid_assign;
  void *                   ui_pre_lid_assign_ctx;
  osm_pfn_ui_mcast_extension_t pfn_ui_mcast_fdb_assign;
  void *                   ui_mcast_fdb_assign_ctx;
  boolean_t                sweep_on_trap;
  osm_testability_modes_t  testability_mode;
  char *                   routing_engine_name;
  boolean_t                connect_roots;
  char *                   lid_matrix_dump_file;
  char *                   ucast_dump_file;
  char *                   root_guid_file;
  char *                   cn_guid_file;
  char *                   sa_db_file;
  boolean_t                exit_on_fatal;
  boolean_t                honor_guid2lid_file;
  boolean_t                daemon;
  boolean_t                sm_inactive;
  boolean_t                babbling_port_policy;
  osm_qos_options_t        qos_options;
  osm_qos_options_t        qos_ca_options;
  osm_qos_options_t        qos_sw0_options;
  osm_qos_options_t        qos_swe_options;
  osm_qos_options_t        qos_rtr_options;
  boolean_t                enable_quirks;
  boolean_t                no_clients_rereg;
#ifdef ENABLE_OSM_PERF_MGR
  boolean_t                perfmgr;
  boolean_t                perfmgr_redir;
  uint16_t                 perfmgr_sweep_time_s;
  uint32_t                 perfmgr_max_outstanding_queries;
  char *                   event_db_dump_file;
#endif /* ENABLE_OSM_PERF_MGR */
  char *                   event_plugin_name;
} osm_subn_opt_t;
/*
* FIELDS
*
*	guid
*		The port guid that the SM is binding to.
*
*	m_key
*		M_Key value sent to all ports qualifing all Set(PortInfo).
*
*	sm_key
*		SM_Key value of the SM to qualify rcv SA queries as "trusted".
*
*	subnet_prefix
*		Subnet prefix used on this subnet.
*
*	m_key_lease_period
*		The lease period used for the M_Key on this subnet.
*
*	sweep_interval
*		The number of seconds between subnet sweeps.  A value of 0
*		disables sweeping.
*
*	sm_priority
*		The priority of this SM as specified by the user.  This
*		value is made available in the SMInfo attribute.
*
*	lmc
*		The LMC value used on this subnet.
*
*	lmc_esp0
*		Whether LMC value used on subnet should be used for
*		enhanced switch port 0 or not.  If TRUE, it is used.
*		Otherwise (the default), LMC is set to 0 for ESP0.
*
*	max_op_vls
*		Limit the maximal operational VLs. default is 1.
*
*	reassign_lids
*		If TRUE cause all lids to be re-assigend.
*		Otherwise (the default),
*		OpenSM always tries to preserve as LIDs as much as possible.
*
*	ignore_other_sm_option
*		This flag is TRUE if other SMs on the subnet should be ignored.
*
*	no_multicast_option
*		This flag is TRUE if OpenSM should disable multicast support.
*
*	max_msg_fifo_timeout
*		The maximal time a message can stay in the incoming message queue.
*		If there is more than one message in the queue and the last
*		message stayed in the queue more than this value the SA request
*		will be immediately returned with a BUSY status.
*
*	subnet_timeout
*		The subnet_timeout that will be set for all the ports in the
*		design SubnSet(PortInfo.vl_stall_life))
*
*	vl_stall_count
*		The number of sequential packets dropped that cause the port
*		to enter the VLStalled state.
*
*	leaf_vl_stall_count
*		The number of sequential packets dropped that cause the port
*		to enter the VLStalled state. This is for switch ports driving
*		a CA or router port.
*
*	head_of_queue_lifetime
*		The maximal time a packet can live at the head of a VL queue
*		on any port not driving a CA or router port.
*
*	leaf_head_of_queue_lifetime
*		The maximal time a packet can live at the head of a VL queue
*		on switch ports driving a CA or router.
*
*	local_phy_errors_threshold
*		Threshold of local phy errors for sending Trap 129
*
*	overrun_errors_threshold
*		Threshold of credits overrun errors for sending Trap 129
*
*	sminfo_polling_timeout
*		Specifies the polling timeout (in milliseconds) - the timeout
*		between one poll to another.
*
*	packet_life_time
*		The maximal time a packet can stay in a switch.
*		The value is send to all switches as SubnSet(SwitchInfo.life_state)
*
*	dump_files_dir
*		The directory to be used for opensm-subnet.lst, opensm.fdbs,
*		opensm.mcfdbs, and default log file (the latter for Windows,
*		not Linux).
*
*	log_file
*		Name of the log file (or NULL) for stdout.
*
*	log_max_size
*		This option defines maximal log file size in MB. When
*		specified the log file will be truncated upon reaching
*		this limit.
*
*	accum_log_file
*		If TRUE (default) - the log file will be accumulated.
*		If FALSE - the log file will be erased before starting current opensm run.
*
*	port_prof_ignore_guids
*		A map of guids to be ignored by port profiling.
*
*	port_profile_switch_nodes
*		If TRUE will count the number of switch nodes routed through
*		the link. If FALSE - only CA/RT nodes are counted.
*
*	pfn_ui_pre_lid_assign
*		A UI function to be invoked prior to lid assigment. It should
*		return 1 if any change was made to any lid or 0 otherwise.
*
*	ui_pre_lid_assign_ctx
*		A UI context (void *) to be provided to the pfn_ui_pre_lid_assign
*
*	pfn_ui_mcast_fdb_assign
*		A UI function to be called inside the mcast manager instead of
*		the call for the build spanning tree. This will be called on
*		every multicast call for create, join and leave, and is
*		responsible for the mcast FDB configuration.
*
*	ui_mcast_fdb_assign_ctx
*		A UI context (void *) to be provided to the pfn_ui_mcast_fdb_assign
*
*	sweep_on_trap
*		Received traps will initiate a new sweep.
*
*	testability_mode
*		Object that indicates if we are running in a special testability mode.
*
*	routing_engine_name
*		Name of used routing engine
*		(other than default Min Hop Algorithm)
*
*	connect_roots
*		The option which will enforce root to root connectivity with
*		up/down routing engine (even if this violates "pure" deadlock
*		free up/down algorithm)
*
*	lid_matrix_dump_file
*		Name of the lid matrix dump file from where switch
*		lid matrices (min hops tables) will be loaded
*
*	ucast_dump_file
*		Name of the unicast routing dump file from where switch
*		forwarding tables will be loaded
*
*	root_guid_file
*		Name of the file that contains list of root guids that
*		will be used by fat-tree or up/dn routing (provided by User)
*
*	cn_guid_file
*		Name of the file that contains list of compute node guids that
*		will be used by fat-tree routing (provided by User)
*
*	sa_db_file
*		Name of the SA database file.
*
*	exit_on_fatal
*		If TRUE (default) - SM will exit on fatal subnet initialization issues.
*		If FALSE - SM will not exit.
*		Fatal initialization issues:
*		a. SM recognizes 2 different nodes with the same guid, or
*		   12x link with lane reversal badly configured.
*
*	honor_guid2lid_file
*		Always honor the guid2lid file if it exists and is valid. This
*		means that the file will be honored when SM is coming out of
*		STANDBY. By default this is FALSE.
*
*	daemon
*		OpenSM will run in daemon mode.
*
*	sm_inactive
*		OpenSM will start with SM in not active state.
*
*	babbling_port_policy
*		OpenSM will enforce its "babbling" port policy.
*
*	perfmgr
*		Enable or disable the performance manager
*
*	perfmgr_redir
*		Enable or disable the saving of redirection by PerfMgr
*
*	perfmgr_sweep_time_s
*		Define the period (in seconds) of PerfMgr sweeps
*
*       event_db_dump_file
*               File to dump the event database to
*
*       event_db_plugin
*               Specify the name of the event plugin
*
*	qos_options
*		Default set of QoS options
*
*	qos_ca_options
*		QoS options for CA ports
*
*	qos_sw0_options
*		QoS options for switches' port 0
*
*	qos_swe_options
*		QoS options for switches' external ports
*
*	qos_rtr_options
*		QoS options for router ports
*
*	enable_quirks
*		Enable high risk new features and not fully qualified
*		hardware specific work arounds
*
*	no_clients_rereg
*		When TRUE disables clients reregistration request.
*
* SEE ALSO
*	Subnet object
*********/

/****s* OpenSM: Subnet/osm_subn_t
* NAME
*	osm_subn_t
*
* DESCRIPTION
*	Subnet structure.  Callers may directly access member components,
*	after grabbing a lock.
*
* TO DO
*	This structure should probably be volatile.
*
* SYNOPSIS
*/
typedef struct _osm_subn
{
  struct _osm_opensm_t    *p_osm;
  cl_qmap_t		   sw_guid_tbl;
  cl_qmap_t		   node_guid_tbl;
  cl_qmap_t		   port_guid_tbl;
  cl_qmap_t		   rtr_guid_tbl;
  cl_qmap_t		   prtn_pkey_tbl;
  cl_qmap_t		   mgrp_mlid_tbl;
  cl_qmap_t		   sm_guid_tbl;
  cl_list_t                light_sweep_physp_list;
  cl_qlist_t		   sa_sr_list;
  cl_qlist_t		   sa_infr_list;
  cl_ptr_vector_t	   port_lid_tbl;
  ib_net16_t		   master_sm_base_lid;
  ib_net16_t		   sm_base_lid;
  ib_net64_t		   sm_port_guid;
  uint8_t		   sm_state;
  osm_subn_opt_t	   opt;
  uint16_t		   max_unicast_lid_ho;
  uint16_t		   max_multicast_lid_ho;
  uint8_t                  min_ca_mtu;
  uint8_t                  min_ca_rate;
  boolean_t                ignore_existing_lfts;
  boolean_t                subnet_initialization_error;
  boolean_t                force_immediate_heavy_sweep;
  boolean_t                force_delayed_heavy_sweep;
  boolean_t                in_sweep_hop_0;
  boolean_t                moved_to_master_state;
  boolean_t                first_time_master_sweep;
  boolean_t                coming_out_of_standby;
  unsigned                 need_update;
} osm_subn_t;
/*
* FIELDS
*	sw_guid_tbl
*		Container of pointers to all Switch objects in the subent.
*		Indexed by node GUID.
*
*	node_guid_tbl
*		Container of pointers to all Node objects in the subent.
*		Indexed by node GUID.
*
*	port_guid_tbl
*		Container of pointers to all Port objects in the subent.
*		Indexed by port GUID - network order!
*
*	rtr_guid_tbl
*		Container of pointers to all Router objects in the subent.
*		Indexed by node GUID.
*
*	prtn_pkey_tbl
*		Container of pointers to all Partition objects in the subnet.
*		Indexed by P_KEY.
*
*	mgrp_mlid_tbl
*		Container of pointers to all Multicast Group objects in the subnet.
*		Indexed by MLID.
*
*	sm_guid_tbl
*		Container of pointers to SM objects representing other SMs
*		on the subnet.
*
*	light_sweep_physp_list
*		A list of all phys ports to scan for a change in remote
*		side state in next light sweep. These ports are not down
*		but for some reason the remote side did not answer.
*
*	node_lid_tbl
*		Container of pointers to all Node objects in the subent.
*		Indexed by node LID.
*
*	port_ptr_tbl
*		Container of pointers to all Port objects in the subent.
*		Indexed by port LID.
*
*	master_sm_base_lid
*		The base LID owned by the subnet's master SM.
*
*	sm_base_lid
*		The base LID of the local port where the SM is.
*
*	sm_port_guid
*		This SM's own port GUID.
*
*	sm_state
*		The high-level state of the SM.  This value is made available
*		in the SMInfo attribute.
*
*	opt
*		Subnet options structure contains site specific configuration.
*
*	max_unicast_lid_ho
*		The minimal max unicast lid reported by all switches
*
*	max_multicast_lid_ho
*		The minimal max multicast lid reported by all switches
*
*	min_ca_mtu
*		The minimal MTU reported by all CAs ports on the subnet
*
*	min_ca_rate
*		The minimal rate reported by all CA ports on the subnet
*
*  ignore_existing_lfts
*     This flag is a dynamic flag to instruct the LFT assignment to
*     ignore existing legal LFT settings.
*     The value will be set according to :
*     - Any change to the list of switches will set it to high
*     - Coming out of STANDBY it will be cleared (other SM worked)
*     - Set to FALSE upon end of all lft assignments.
*
*  subnet_initalization_error
*     Similar to the force_immediate_heavy_sweep flag. If TRUE -
*     means that we had errors during initialization (due to SubnSet requests
*     that failed). We want to declare the subnet as unhealthy, and force
*     another heavy sweep.
*
*  force_immediate_heavy_sweep
*     If TRUE - we want to force a heavy sweep. This can be done either
*     due to receiving of trap - meaning there is some change on the subnet,
*     or we received a handover from a remote sm.
*     In this case we want to sweep and reconfigure the entire subnet.
*     This will cause another heavy sweep to occure when the current sweep
*     is done.
*
*  force_delayed_heavy_sweep
*     In some means - similar to the force_immediate_heavy_sweep flag, only
*     it'll cause a heavy sweep in the next sweep. Note that this means that
*     if we are running with -s 0 (no sweeps) - then this forced heavy sweep
*     will not occur.
*     If we had some trouble on the subnet, that caused a strange dropping
*     of ports - we will try to do another heavy sweep on our next sweep.
*
*  in_sweep_hop_0
*     When in_sweep_hop_0 flag is set to TRUE - this means we are
*     in sweep_hop_0 - meaning we do not want to continue beyond
*     the current node.
*     This is relevant for the case of SM on switch, since in the
*     switch info we need to signal somehow not to continue
*     the sweeping.
*
*  moved_to_master_state
*     Used for the writing of "SUBNET UP" into /var/log/messages.
*     Will be TRUE when the SM switches to Master state, and returned
*     to FALSE once the sunbet is up.
*
*  first_time_master_sweep
*     This flag is used for the PortInfo setting. On the first sweep as master
*     (meaning after moving from Standby|Discovering state), the SM must send
*     a PortInfoSet to all ports. After that - we want to minimize the number of
*     PortInfoSet requests sent, and to send only requests that change the value
*     from what is updated in the port (or send a first request if this is a new
*     port). We will set this flag to TRUE when entering the master state, and
*     set it back to FALSE at the end of the drop manager. This is done since at
*     the end of the drop manager we have updated all the ports that are
*     reachable, and from now on these are the only ports we have data of. We
*     don't want to send extra set requests to these ports anymore.
*
*  coming_out_of_standby
*     TRUE on the first sweep after the SM was in standby.
*     Used for nulling any cache of LID and Routing.
*     The flag is set true if the SM state was standby and now changed to MASTER
*     it is reset at the end of the sweep.
*
*  need_update
*     This flag should be on during first non-master heavy (including
*     pre-master discovery stage)
*
* SEE ALSO
*	Subnet object
*********/

/****f* OpenSM: Subnet/osm_subn_construct
* NAME
*	osm_subn_construct
*
* DESCRIPTION
*	This function constructs a Subnet object.
*
* SYNOPSIS
*/
void
osm_subn_construct(
	IN osm_subn_t* const p_subn );
/*
* PARAMETERS
*	p_subn
*		[in] Pointer to a Subnet object to construct.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Allows calling osm_subn_init, and osm_subn_destroy.
*
*	Calling osm_subn_construct is a prerequisite to calling any other
*	method except osm_subn_init.
*
* SEE ALSO
*	Subnet object, osm_subn_init, osm_subn_destroy
*********/

/****f* OpenSM: Subnet/osm_subn_destroy
* NAME
*	osm_subn_destroy
*
* DESCRIPTION
*	The osm_subn_destroy function destroys a subnet, releasing
*	all resources.
*
* SYNOPSIS
*/
void
osm_subn_destroy(
	IN osm_subn_t* const p_subn );
/*
* PARAMETERS
*	p_subn
*		[in] Pointer to a Subnet object to destroy.
*
* RETURN VALUE
*	This function does not return a value.
*
* NOTES
*	Performs any necessary cleanup of the specified Subnet object.
*	Further operations should not be attempted on the destroyed object.
*	This function should only be called after a call to osm_subn_construct or
*	osm_subn_init.
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_init
*********/

/****f* OpenSM: Subnet/osm_subn_init
* NAME
*	osm_subn_init
*
* DESCRIPTION
*	The osm_subn_init function initializes a Subnet object for use.
*
* SYNOPSIS
*/
ib_api_status_t
osm_subn_init(
	IN osm_subn_t* const p_subn,
	IN struct _osm_opensm_t * const p_osm,
	IN const osm_subn_opt_t* const p_opt );
/*
* PARAMETERS
*	p_subn
*		[in] Pointer to an osm_subn_t object to initialize.
*
*	p_opt
*		[in] Pointer to the subnet options structure.
*
* RETURN VALUES
*	IB_SUCCESS if the Subnet object was initialized successfully.
*
* NOTES
*	Allows calling other Subnet methods.
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy
*********/

/*
  Forward references.
*/
struct _osm_mad_addr;
struct _osm_log;
struct _osm_switch;
struct _osm_physp;
struct _osm_port;

/****f* OpenSM: Helper/osm_get_gid_by_mad_addr
* NAME
*	osm_get_gid_by_mad_addr
*
* DESCRIPTION
*	Looks for the requester gid in the mad address.
*
* Note: This code is not thread safe. Need to grab the lock before
* calling it.
*
* SYNOPSIS
*/
ib_api_status_t
osm_get_gid_by_mad_addr(
   IN struct _osm_log      *p_log,
   IN const osm_subn_t     *p_subn,
   IN const struct _osm_mad_addr *p_mad_addr,
   OUT ib_gid_t            *p_gid);
/*
* PARAMETERS
*  p_log
*     [in] Pointer to a log object.
*
*  p_subn
*     [in] Pointer to subnet object.
*
*  p_mad_addr
*     [in] Pointer to mad address object.
*
*  p_gid
*     [out] Pointer to the GID structure to fill in.
*
* RETURN VALUES
*     IB_SUCCESS if able to find the GID by address given.
*
* NOTES
*
* SEE ALSO
*********/

/****f* OpenSM: Helper/osm_get_physp_by_mad_addr
* NAME
*	osm_get_physp_by_mad_addr
*
* DESCRIPTION
*	Looks for the requester physical port in the mad address.
*
* Note: This code is not thread safe. Need to grab the lock before
* calling it.
*
* SYNOPSIS
*/
struct _osm_physp *
osm_get_physp_by_mad_addr(
   IN struct _osm_log      *p_log,
   IN const osm_subn_t     *p_subn,
   IN struct _osm_mad_addr *p_mad_addr );
/*
* PARAMETERS
*  p_log
*     [in] Pointer to a log object.
*
*  p_subn
*     [in] Pointer to subnet object.
*
*  p_mad_addr
*     [in] Pointer to mad address object.
*
* RETURN VALUES
*	Pointer to requester physical port object if found. Null otherwise.
*
* NOTES
*
* SEE ALSO
*********/

/****f* OpenSM: Helper/osm_get_port_by_mad_addr
* NAME
*	osm_get_port_by_mad_addr
*
* DESCRIPTION
*	Looks for the requester port in the mad address.
*
* Note: This code is not thread safe. Need to grab the lock before
* calling it.
*
* SYNOPSIS
*/
struct _osm_port *
osm_get_port_by_mad_addr(
   IN struct _osm_log      *p_log,
   IN const osm_subn_t     *p_subn,
   IN struct _osm_mad_addr *p_mad_addr );
/*
* PARAMETERS
*  p_log
*     [in] Pointer to a log object.
*
*  p_subn
*     [in] Pointer to subnet object.
*
*  p_mad_addr
*     [in] Pointer to mad address object.
*
* RETURN VALUES
*	Pointer to requester port object if found. Null otherwise.
*
* NOTES
*
* SEE ALSO
*********/

/****f* OpenSM: Subnet/osm_get_switch_by_guid
* NAME
*	osm_get_switch_by_guid
*
* DESCRIPTION
*	Looks for the given switch guid in the subnet table of switches by guid.
*  NOTE: this code is not thread safe. Need to grab the lock before
*  calling it.
*
* SYNOPSIS
*/
struct _osm_switch *
osm_get_switch_by_guid(
   IN const osm_subn_t *p_subn,
   IN uint64_t          guid );
/*
* PARAMETERS
*	p_subn
*		[in] Pointer to an osm_subn_t object
*
*	guid
*		[in] The node guid in host order
*
* RETURN VALUES
*	The switch structure pointer if found. NULL otherwise.
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy,
*	osm_switch_t
*********/

/****f* OpenSM: Subnet/osm_get_node_by_guid
* NAME
*	osm_get_node_by_guid
*
* DESCRIPTION
*	The looks for the given node giud in the subnet table of nodes by guid.
*  NOTE: this code is not thread safe. Need to grab the lock before
*  calling it.
*
* SYNOPSIS
*/
struct _osm_node *
osm_get_node_by_guid(
   IN osm_subn_t  const *p_subn,
   IN uint64_t    guid );
/*
* PARAMETERS
*	p_subn
*		[in] Pointer to an osm_subn_t object
*
*	guid
*		[in] The node guid in host order
*
* RETURN VALUES
*	The node structure pointer if found. NULL otherwise.
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy,
*	osm_node_t
*********/

/****f* OpenSM: Subnet/osm_get_port_by_guid
* NAME
*	osm_get_port_by_guid
*
* DESCRIPTION
*	The looks for the given port guid in the subnet table of ports by guid.
*  NOTE: this code is not thread safe. Need to grab the lock before
*  calling it.
*
* SYNOPSIS
*/
struct _osm_port *
osm_get_port_by_guid(
   IN osm_subn_t  const *p_subn,
   IN uint64_t    guid );
/*
* PARAMETERS
*	p_subn
*		[in] Pointer to an osm_subn_t object
*
*	guid
*		[in] The port guid in host order
*
* RETURN VALUES
*	The port structure pointer if found. NULL otherwise.
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy,
*	osm_port_t
*********/

/****f* OpenSM: Helper/osm_get_physp_by_mad_addr
* NAME
*	osm_get_physp_by_mad_addr
*
* DESCRIPTION
*	Looks for the requester physical port in the mad address.
*
* Note: This code is not thread safe. Need to grab the lock before
* calling it.
*
* SYNOPSIS
*/
struct _osm_physp *
osm_get_physp_by_mad_addr(
   IN struct _osm_log      *p_log,
   IN const osm_subn_t     *p_subn,
   IN struct _osm_mad_addr *p_mad_addr );
/*
* PARAMETERS
*  p_log
*     [in] Pointer to a log object.
*
*  p_subn
*     [in] Pointer to subnet object.
*
*  p_mad_addr
*     [in] Pointer to mad address object.
*
* RETURN VALUES
*	Pointer to requester physical port object if found. Null otherwise.
*
* NOTES
*
* SEE ALSO
*********/

/****f* OpenSM: Subnet/osm_subn_set_default_opt
* NAME
*	osm_subn_set_default_opt
*
* DESCRIPTION
*	The osm_subn_set_default_opt function sets the default options.
*
* SYNOPSIS
*/
void
osm_subn_set_default_opt(
	IN osm_subn_opt_t* const p_opt );
/*
* PARAMETERS
*
*	p_opt
*		[in] Pointer to the subnet options structure.
*
* RETURN VALUES
*	None
*
* NOTES
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy
*********/

/****f* OpenSM: Subnet/osm_subn_set_default_opt
* NAME
*	osm_subn_set_default_opt
*
* DESCRIPTION
*	The osm_subn_set_default_opt function sets the default options.
*
* SYNOPSIS
*/
void
osm_subn_set_default_opt(
	IN osm_subn_opt_t* const p_opt );
/*
* PARAMETERS
*
*	p_opt
*		[in] Pointer to the subnet options structure.
*
* RETURN VALUES
*	None
*
* NOTES
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy
*********/

/****f* OpenSM: Subnet/osm_subn_parse_conf_file
* NAME
*	osm_subn_parse_conf_file
*
* DESCRIPTION
*	The osm_subn_parse_conf_file function parses the configuration file
*	and sets the defaults accordingly.
*
* SYNOPSIS
*/
ib_api_status_t
osm_subn_parse_conf_file(
  IN osm_subn_opt_t* const p_opt );
/*
* PARAMETERS
*
*	p_opt
*		[in] Pointer to the subnet options structure.
*
* RETURN VALUES
*	IB_SUCCESS, IB_ERROR
*
* NOTES
*  Assumes the conf file is part of the cache dir which defaults to
*  OSM_DEFAULT_CACHE_DIR or OSM_CACHE_DIR the name is opensm.opts
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy
*********/

/****f* OpenSM: Subnet/osm_subn_parse_conf_file
* NAME
*	osm_subn_rescan_conf_file
*
* DESCRIPTION
*	The osm_subn_rescan_conf_file function parses the configuration
*	file and update selected subnet options
*
* SYNOPSIS
*/
ib_api_status_t
osm_subn_rescan_conf_file(
  IN osm_subn_opt_t* const p_opts );
/*
* PARAMETERS
*
*	p_opt
*		[in] Pointer to the subnet options structure.
*
* RETURN VALUES
*	IB_SUCCESS, IB_ERROR
*
* NOTES
*  This uses the same file as osm_subn_parse_conf_file()
*
*********/

/****f* OpenSM: Subnet/osm_subn_write_conf_file
* NAME
*	osm_subn_write_conf_file
*
* DESCRIPTION
*  Write the configuration file into the cache
*
* SYNOPSIS
*/
ib_api_status_t
osm_subn_write_conf_file(
  IN osm_subn_opt_t* const p_opt );
/*
* PARAMETERS
*
*	p_opt
*		[in] Pointer to the subnet options structure.
*
* RETURN VALUES
*	IB_SUCCESS, IB_ERROR
*
* NOTES
*  Assumes the conf file is part of the cache dir which defaults to
*  OSM_DEFAULT_CACHE_DIR or OSM_CACHE_DIR the name is opensm.opts
*
* SEE ALSO
*	Subnet object, osm_subn_construct, osm_subn_destroy
*********/

END_C_DECLS

#endif /* _OSM_SUBNET_H_ */

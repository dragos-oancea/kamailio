/*
 * Copyright (C) 2015 Olle E. Johansson, Edvina AB
 * RPC functions
 *
 * This file is part of kamailio, a free SIP server.
 *
 * Kamailio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * Kamailio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


/*! \file
 * \brief  Kamailio curl :: RPC functions
 * \ingroup curl
 */

#include "../../dprint.h"
#include "../../rpc.h"
#include "../../rpc_lookup.h"
#include "curl.h"
#include "curlcon.h"


static const char* curl_rpc_listcon_doc[2] = {
	"List all CURL connection definitions",
	0
};


/*
 * RPC command to print curl destination sets
 */
static void curl_rpc_listcon(rpc_t* rpc, void* ctx)
{
	void* th;
	void* ih;
	void* rh;
	void* sh;
	int j;
	char c[3];
	str data = {"", 0};
	curl_con_t *cc;

	cc = _curl_con_root;
	if(cc==NULL)
	{
		LM_ERR("no connection definitions\n");
		rpc->fault(ctx, 500, "No Connection Definitions");
		return;
	}

	/* add entry node */
	if (rpc->add(ctx, "{", &th) < 0)
	{
		rpc->fault(ctx, 500, "Internal error root reply");
		return;
	}

        while(cc)
        {
		if (rpc->struct_add(th, "{", "CONNECTION", &rh) < 0)
		{
			rpc->fault(ctx, 500, "Internal error set structure");
			return;
		}

		if(rpc->struct_add(rh, "SSSSS", 
				"NAME", &cc->name,
				"SCHEMA", &cc->schema,
				"URI", &cc->url,
				"USERNAME", &cc->username,
				"PASSWORD", &cc->password
				) < 0) {
			rpc->fault(ctx, 500, "Internal error set structure");
			return;
		}
                cc = cc->next;
        }
	return;
}

rpc_export_t curl_rpc_cmds[] = {
	{"curl.listcon",   curl_rpc_listcon,
		curl_rpc_listcon_doc,   0},
	{0, 0, 0, 0}
};

/**
 * register RPC commands
 */
int curl_init_rpc(void)
{
	if (rpc_register_array(curl_rpc_cmds)!=0)
	{
		LM_ERR("failed to register RPC commands\n");
		return -1;
	}
	return 0;
}
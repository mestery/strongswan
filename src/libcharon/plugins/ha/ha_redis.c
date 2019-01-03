/*
 * Copyright (c) 2019 Cisco and/or its affiliates.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <sa/ikev2/keymat_v2.h>
#include <sa/ikev1/keymat_v1.h>

#include "ha_redis.h"

typedef struct private_ha_redis_t private_ha_redis_t;

/**
 * Redis variables
 */
struct private_ha_redis_t {

	/**
	 * Public ha_redis_t interface.
	 */
	ha_redis_t public;

	/**
	 * Redis connection context.
	 */
	redisContext *ctx;

	/**
	 * Hostname to connect to. Clearly should not be hardcoded.
	 * .FIXME: Do not hardcode this.
	 */
	char hostname[128];

	/**
	 * Port to connect to. Clearly should not be hardcoded.
	 * .FIXME: Do not hardcode this.
	 */
	int port;

	/**
	 * Connection timeout.
	 */
	struct timeval timeout;
};

METHOD(listener_t, alert, bool,
	private_ha_redis_t *this, ike_sa_t *ike_sa, alert_t alert, va_list args)
{
	DBG1(DBG_CFG, "ha_redis received alert");
	return TRUE;
}

METHOD(listener_t, ike_keys, bool,
	private_ha_redis_t *this, ike_sa_t *ike_sa, diffie_hellman_t *dh,
	chunk_t dh_other, chunk_t nonce_i, chunk_t nonce_r, ike_sa_t *rekey,
	shared_key_t *shared, auth_method_t method)
{
	DBG1(DBG_CFG, "ha_redis received ike_keys");
	return TRUE;
}

METHOD(listener_t, ike_updown, bool,
	private_ha_redis_t *this, ike_sa_t *ike_sa, bool up)
{
	DBG1(DBG_CFG, "ha_redis received ike_updown");
	return TRUE;
}

METHOD(listener_t, ike_rekey, bool,
	private_ha_redis_t *this, ike_sa_t *old, ike_sa_t *new)
{
	DBG1(DBG_CFG, "ha_redis received ike_rekey");
	return TRUE;
}

METHOD(listener_t, ike_state_change, bool,
	private_ha_redis_t *this, ike_sa_t *ike_sa, ike_sa_state_t new)
{
	DBG1(DBG_CFG, "ha_redis received ike_state_change");
	return TRUE;
}

METHOD(listener_t, message_hook, bool,
	private_ha_redis_t *this, ike_sa_t *ike_sa, message_t *message,
	bool incoming, bool plain)
{
	DBG1(DBG_CFG, "ha_redis received message_hook");
	return TRUE;
}

/**
 * Tear-down redis parameters.
 */
METHOD(ha_redis_t, destroy, void,
        private_ha_redis_t *this)
{
	redisFree(this->ctx);

	free(this);
}

/**
 * Setup redis parameters.
 */
ha_redis_t *ha_redis_create(void)
{
	private_ha_redis_t *this;

	INIT(this,
		.public = {
			.listener = {
				.alert = _alert,
				.ike_keys = _ike_keys,
				.ike_updown = _ike_updown,
				.ike_rekey = _ike_rekey,
				.ike_state_change = _ike_state_change,
				.message = _message_hook,
			},
			.destroy = _destroy,
                },
        );

	/**
	 * .FIXME: Hard coding is bad
	 */
	strncpy(this->hostname, "172.17.0.2", sizeof("172.17.0.2"));

	/**
	 * .FIXME: Hard coding is bad
	 */
	this->port = 6379;

	this->timeout.tv_sec = 1;
	this->timeout.tv_usec = 500000;

	this->ctx = redisConnectWithTimeout(this->hostname, this->port, this->timeout);
	if (this->ctx == NULL || this->ctx->err) {
		if (this->ctx) {
			DBG1(DBG_CFG, "Connection error: %s", this->ctx->errstr);
			redisFree(this->ctx);
		} else {
			DBG1(DBG_CFG, "Connection error: cannot allocate redis context");
		}

		return NULL;
	}

	return &this->public;
}

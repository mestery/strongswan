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

/**
 * @defgroup ha_redis ha_redis
 * @{ @ingroup ha
 */

#ifndef HA_REDIS_H_
#define HA_REDIS_H_

#include <daemon.h>
#include <hiredis/hiredis.h>

typedef struct ha_redis_t ha_redis_t;

/**
 * Access to redis
 */
struct ha_redis_t {

	/**
	 * Implements bus listener interface.
	 */
	listener_t listener;

        /**
         * Destroy a ha_redis_t.
         */
        void (*destroy)(ha_redis_t *this);
};

/**
 * Create a ha_redis_t instance.
 *
 * @return            Listens for IKE messages and plumbs them into redis.
 */
ha_redis_t *ha_redis_create(void);

#endif /** HA_REDIS_H_ @}*/
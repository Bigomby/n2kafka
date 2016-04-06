/*
**
** Copyright (c) 2014, Eneo Tecnologia
** Author: Eugenio Perez <eupm90@gmail.com>
** All rights reserved.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU Affero General Public License as
** published by the Free Software Foundation, either version 3 of the
** License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Affero General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "rb_database.h"

#include "util/pair.h"

#include <librdkafka/rdkafka.h>
#include <librd/rdavl.h>
#include <librd/rdsysqueue.h>
#include <jansson.h>

#include <stdint.h>
#include <string.h>
#include <pthread.h>

#ifndef NDEBUG
/// MAGIC to check rb_config between void * conversions
#define RB_CONFIG_MAGIC 0xbc01a1cbc01a1cL
#endif

/* All functions are thread-safe here, excepting free_valid_mse_database */
struct json_t;
struct rb_config {
#ifdef RB_CONFIG_MAGIC
	/// This value always have to be RB_CONFIG_MAGIC
	uint64_t magic;
#endif
	struct rb_database database;
};

#ifdef RB_CONFIG_MAGIC
/// Checks that rb_config magic field has the right value
#define assert_rb_config(cfg) do{ \
	assert(RB_CONFIG_MAGIC==(cfg)->magic);} while(0)
#else
#define assert_rb_config(cfg)
#endif

int parse_rb_config(void *_db,const struct json_t *rb_config);
/** Does nothing, since this decoder does not save anything related to 
    listener
    */
int rb_decoder_reload(void *_db, const struct json_t *rb_config);

int rb_opaque_creator(struct json_t *config,void **opaque);
int rb_opaque_reload(struct json_t *config,void *opaque);
void rb_opaque_done(void *opaque);
void rb_decode(char *buffer,size_t buf_size,const keyval_list_t *props,
                void *listener_callback_opaque,void **decoder_sessionp);

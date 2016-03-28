/*
** Copyright (C) 2015 Eneo Tecnologia S.L.
** Author: Eugenio Perez <eupm90@gmail.com>
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

#include "config.h"

#include "mse/rb_mse.h"
#include "meraki/rb_meraki.h"
#include "rb_http2k/rb_http2k_decoder.h"
#include "kafka.h"
#include "in_addr_list.h"
#include "pair.h"

#include <stdint.h>
#include <stdbool.h>
#include <sys/queue.h>
#include <librdkafka/rdkafka.h>

struct json_t;
struct listener;
typedef void (*decoder_callback)(char *buffer,size_t buf_size,
    const keyval_list_t *props,void *listener_callback_opaque,
    void **sessionp);
typedef struct listener* (*listener_creator)(struct json_t *config,
                        decoder_callback cb,int cb_flags,void *cb_opaque);
typedef void (*listener_join)(void *listener_private);
typedef int (*decoder_listener_opaque_creator)(struct json_t *config,void **opaque);
typedef int (*decoder_listener_opaque_reload)(struct json_t *config,void *opaque);
typedef void (*decoder_listener_opaque_destructor)(void *opaque);
// @TODO we need this callback to split data acquiring || data processing
// typedef void (*data_process)(void *data_process_private,const char *buffer,size_t bsize);
typedef void (*listener_reload)(struct json_t *new_config,
    decoder_listener_opaque_reload opaque_reload,
    void *cb_opaque,void *listener_private);
struct listener{
    uint16_t port; // as listener ID
    void *private;
    
    struct{
        void *cb_opaque;
        decoder_callback callback;
        decoder_listener_opaque_destructor cb_opaque_destructor;
        decoder_listener_opaque_reload cb_opaque_reload;
        int flags;
    }cb;
    listener_creator create;
    listener_join join;
    listener_reload reload;
    LIST_ENTRY(listener) entry;
};

enum {
/**
    Tells if the listener support straming API, instead of wait to all data
    to be available
*/
    DECODER_F_SUPPORT_STREAMING = 0x01,
};

typedef LIST_HEAD(,listener) listener_list;

struct n2kafka_config{
#ifdef HAVE_LIBMICROHTTPD
#define N2KAFKA_HTTP 3
#endif

    /// Path to reload
    char *config_path;

    char *format;

    char *topic;
    char *brokers;

    rd_kafka_conf_t *kafka_conf;
    rd_kafka_topic_conf_t *kafka_topic_conf;rd_kafka_t *rk;

    in_addr_list_t *blacklist;

    /// @TODO this should belong to decoders, not here.
    struct mse_config mse;
    struct meraki_config meraki;
    struct rb_config rb;

    char *response;
    int response_len;

    listener_list listeners;

    struct json_t *stream_enrichment;

    bool debug;
};

extern struct n2kafka_config global_config;

static inline bool only_stdout_output(){
	return global_config.debug && !global_config.brokers;
}

void init_global_config();

void parse_config(const char *config_file_path);

void reload_config(struct n2kafka_config *config);

void free_global_config();

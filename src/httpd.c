// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "httpd.h"
#include "centaur.h"
#include "chess/chess.h"
#include "list.h"
#include "screen.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <gc/gc.h>
#include <pcre2.h>
#include <pthread.h>

//
// Types
//

struct key_value {
    struct key_value *next;
    const char       *key;
    const char       *value;
};

struct HttpdRequest {
    struct MHD_Connection *mhd_connection;
    const char            *method;
    const char            *url;
    HttpdRequestHandler    handler;
    uint8_t               *body;
    size_t                 body_allocated;
    size_t                 body_used;
    struct key_value      *post_vars;
    void                  *userdata;
};

struct HttpdResponse {
    struct MHD_Response *mhd_response;
    int                  status_code;
};

//
// Request
//

void httpd_request_free(struct HttpdRequest *request) {
    assert(request);
    if (request->body) {
        free(request->body);
    }
    free(request);
}

struct HttpdRequest*
httpd_request_new(
    struct MHD_Connection *connection,
    const char            *method,
    const char            *url,
    HttpdRequestHandler    handler)
{
    struct HttpdRequest *request = malloc(sizeof *request);
    request->mhd_connection = connection;
    request->method         = method;
    request->url            = url;
    request->handler        = handler;
    request->body           = NULL;
    request->body_allocated = 0;
    request->body_used      = 0;
    request->post_vars      = NULL;
    request->userdata       = NULL;
    return request;
}

void *httpd_request_userdata(const struct HttpdRequest *request) {
    return request->userdata;
}

void httpd_request_set_userdata(struct HttpdRequest *request, void *userdata) {
    request->userdata = userdata;
}

const uint8_t *httpd_request_body(const struct HttpdRequest *request) {
    return request->body;
}

size_t httpd_request_body_length(const struct HttpdRequest *request) {
    return request->body_used;
}

void
httpd_request_accumulate_body(
    struct HttpdRequest *request,
    const char           *data,
    size_t                len)
{
    if (request->body_used + len + 1 > request->body_allocated) {
        size_t desired = 2 * request->body_allocated;
        if (request->body_used + len + 1 > desired) {
            desired = request->body_used + len + 1;
        }
        if (request->body == NULL) {
            request->body = malloc(desired);
        } else {
            request->body = realloc(request->body, desired);
        }
        request->body_allocated = desired;
    }

    memcpy(&request->body[request->body_used], data, len);
    request->body_used += len;
    request->body[request->body_used] = 0;
}

HttpdRequestHandler
httpd_request_get_handler(const struct HttpdRequest *request) {
    return request->handler;
}

const char *httpd_request_url(const struct HttpdRequest *request) {
    return request->url;
}

const char *httpd_request_method(const struct HttpdRequest *request) {
    return request->method;
}

const char*
httpd_request_header(const struct HttpdRequest *request, const char *name) {
   return MHD_lookup_connection_value(request->mhd_connection, MHD_HEADER_KIND, name);
}

const char *httpd_request_content_type(const struct HttpdRequest *request) {
   return httpd_request_header(request, "Content-Type");
}

const char*
httpd_request_query_var(const struct HttpdRequest *request, const char *name) {
    return MHD_lookup_connection_value(request->mhd_connection, MHD_GET_ARGUMENT_KIND, name);
}

static enum MHD_Result
query_iterator_cb(void *cls, enum MHD_ValueKind kind, const char *name, const char *value)
{
    assert(MHD_GET_ARGUMENT_KIND == kind);

    char **query = cls;

    char *prev = *query;
    char *curr = NULL;

    if (prev != NULL) {
        if (value != NULL) {
            asprintf(&curr, "%s&%s=%s", prev, name, value);
        } else {
            asprintf(&curr, "%s&%s", prev, name);
        }
    } else {
        if (value != NULL) {
            asprintf(&curr, "%s=%s", name, value);
        } else {
            asprintf(&curr, "%s", name);
        }
    }

    *query = curr;
    return MHD_YES;
}

void
httpd_request_query_string(const struct HttpdRequest *request, char **query) {
    MHD_get_connection_values(
        request->mhd_connection,
        MHD_GET_ARGUMENT_KIND,
        query_iterator_cb,
        query);
}

static bool
is_urlencoded(const struct HttpdRequest *request)
{
    static const char *urlencoded_type = "application/x-www-form-urlencoded";

    const char *content_type = httpd_request_content_type(request);

    bool is_urlencoded = false;
    if (content_type != NULL) {
        is_urlencoded = (strncmp(content_type, urlencoded_type, strlen(urlencoded_type)) == 0);
    }

    return is_urlencoded;
}

static void
MHD_unescape_plus(char *arg)
{
    char *p;
    for (p = strchr(arg, '+'); p; p = strchr(p + 1, '+')) {
        *p = ' ';
    }
}

static void
parse_post_vars(struct HttpdRequest *request)
{
    if (request->post_vars && is_urlencoded(request)) {
        char *kvs = (char*)request->body;

        struct key_value *tail = malloc(sizeof *tail);
        request->post_vars = tail;

        for (char *kv = strsep(&kvs, "&"); kv; kv = strsep(&kvs, "&")) {
            tail->key = strsep(&kv, "=");
            (void)MHD_http_unescape((char*)tail->key);

            tail->value = kv;
            if (tail->value != NULL) {
                (void)MHD_unescape_plus((char*)tail->value);
                (void)MHD_http_unescape((char*)tail->value);
            }

            tail->next = malloc(sizeof *tail);
            *tail->next =(struct key_value){0};
            tail = tail->next;
        }
    }
}

const char*
httpd_request_post_var(const struct HttpdRequest *request, const char *name) {
    parse_post_vars((struct HttpdRequest*)request);

    struct key_value *kvs = request->post_vars;
    while (kvs && kvs->key && strcmp(kvs->key, name) != 0) {
        kvs = kvs->next;
    }
    return kvs ? kvs->value : NULL;
}

//
// Response
//

void httpd_response_free(struct HttpdResponse *response) {
    assert(response);
    if (response->mhd_response) {
        MHD_destroy_response(response->mhd_response);
    }
    free(response);
}

static struct HttpdResponse*
httpd_response_new(struct MHD_Response *mhd_response, int status_code) {
    if (mhd_response) {
        MHD_add_response_header(mhd_response, "Cache-Control", "no-store");
    }
    struct HttpdResponse *response = malloc(sizeof *response);
    response->mhd_response = mhd_response;
    response->status_code  = status_code;
    return response;
}

//
// Handlers
//

struct EventStream {
    pthread_cond_t  cond;
    pthread_mutex_t mutex;
    struct List    *events;
};

static void observe_game(struct Model *model, struct EventStream *stream) {
    (void)model;
    pthread_mutex_lock(&stream->mutex);
    list_push(stream->events, "game_changed");
    pthread_cond_signal(&stream->cond);
    pthread_mutex_unlock(&stream->mutex);
}

static void observe_screen(struct Model *model, struct EventStream *stream) {
    (void)model;
    pthread_mutex_lock(&stream->mutex);
    list_push(stream->events, "screen_changed");
    pthread_cond_signal(&stream->cond);
    pthread_mutex_unlock(&stream->mutex);
}

void stream_free(struct EventStream *stream) {
    pthread_mutex_lock(&stream->mutex);
    MODEL_UNOBSERVE(centaur.game, observe_game, stream);
    MODEL_UNOBSERVE(&screen, observe_screen, stream);
    pthread_mutex_unlock(&stream->mutex);
    pthread_mutex_destroy(&stream->mutex);

    pthread_cond_destroy(&stream->cond);
}

static ssize_t
stream_events(struct EventStream *stream, uint64_t pos, char *buf, size_t max)
{
    (void)pos;

    // Keepalive
    int rc = snprintf(buf, max, ":\n\n");

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 25;

    pthread_mutex_lock(&stream->mutex);
    int err = pthread_cond_timedwait(&stream->cond, &stream->mutex, &ts);
    if (!err) {
        clock_gettime(CLOCK_REALTIME, &ts);
        const char *event = list_shift(stream->events);
        if (event) {
            rc = snprintf(
                buf, max, "event: %s\ndata: {\"timestamp\": %ld}\n\n", event, ts.tv_sec);
        }
    }
    pthread_mutex_unlock(&stream->mutex);

    return rc;
}

static struct HttpdResponse*
get_events(struct HttpdRequest *request) {
    (void)request;

    struct EventStream *stream = GC_MALLOC(sizeof *stream);
    pthread_cond_init(&stream->cond, NULL);
    pthread_mutex_init(&stream->mutex, NULL);

    MODEL_OBSERVE(centaur.game, observe_game, stream);
    MODEL_OBSERVE(&screen, observe_screen, stream);

    struct MHD_Response *mhd_response = MHD_create_response_from_callback(
        MHD_SIZE_UNKNOWN,
        1024,
        (MHD_ContentReaderCallback)stream_events,
        stream,
        (MHD_ContentReaderFreeCallback)stream_free);
    MHD_add_response_header(mhd_response, "Content-Type", "text/event-stream");

    return httpd_response_new(mhd_response, 200);
}

static struct HttpdResponse*
get_fen(struct HttpdRequest *request) {
    (void)request;

    const char *fen = position_fen(game_current(centaur.game));
    size_t      len = strlen(fen);

    struct MHD_Response *mhd_response =
        MHD_create_response_from_buffer(len, (void*)fen, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(mhd_response, "Content-Type", "text/plain");

    return httpd_response_new(mhd_response, 200);
}

static struct HttpdResponse*
get_pgn(struct HttpdRequest *request) {
    (void)request;

    const char *pgn = game_pgn(centaur.game);
    size_t      len = strlen(pgn);

    struct MHD_Response *mhd_response =
        MHD_create_response_from_buffer(len, (void*)pgn, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(mhd_response, "Content-Type", "text/plain");

    return httpd_response_new(mhd_response, 200);
}

static struct HttpdResponse*
get_screen(struct HttpdRequest *request) {
    (void)request;

    uint8_t *png  = NULL;
    size_t   size = 0;
    screen_png(&png, &size);

    struct MHD_Response *mhd_response =
        MHD_create_response_from_buffer(size, png, MHD_RESPMEM_MUST_FREE);
    MHD_add_response_header(mhd_response, "Content-Type", "image/png");

    return httpd_response_new(mhd_response, 200);
}

//
// Daemon
//

enum Match {
    MATCH_PREFIX,
    MATCH_REGEX,
};

enum Method {
    METHOD_UNDEFINED = 0x00,
    METHOD_DELETE    = 0x01,
    METHOD_GET       = 0x02,
    METHOD_HEAD      = 0x04,
    METHOD_OPTIONS   = 0x08,
    METHOD_PATCH     = 0x10,
    METHOD_POST      = 0x20,
    METHOD_PUT       = 0x40,
};

struct Endpoint {
    const char *pattern;
    enum Match  match;
    enum Method method;
    HttpdRequestHandler handler;
};

#define NUM_ENDPOINTS 4

static const struct Endpoint
endpoints[NUM_ENDPOINTS] = {
    {"/api/events", MATCH_PREFIX, METHOD_GET, get_events},
    {"/api/fen",    MATCH_PREFIX, METHOD_GET, get_fen},
    {"/api/pgn",    MATCH_PREFIX, METHOD_GET, get_pgn},
    {"/api/screen", MATCH_PREFIX, METHOD_GET, get_screen},
};

static enum Method
encode_method(const char *method)
{
    static const struct {
        const char *name;
        enum Method value;
    } methods[7] = {
        {MHD_HTTP_METHOD_DELETE,  METHOD_DELETE},
        {MHD_HTTP_METHOD_GET,     METHOD_GET},
        {MHD_HTTP_METHOD_HEAD,    METHOD_HEAD},
        {MHD_HTTP_METHOD_OPTIONS, METHOD_OPTIONS},
        {MHD_HTTP_METHOD_PATCH,   METHOD_PATCH},
        {MHD_HTTP_METHOD_POST,    METHOD_POST},
        {MHD_HTTP_METHOD_PUT,     METHOD_PUT},
    };

    for (int i = 0; i != 7; ++i) {
        if (strcmp(method, methods[i].name) == 0) {
            return methods[i].value;
        }
    }

    return METHOD_UNDEFINED;
}

static bool
match_prefix(const char *pattern, const char *url)
{
    return strncmp(pattern, url, strlen(pattern)) == 0;
}

static bool
match_regex(const char *pattern, const char *url)
{
    int err_code;
    PCRE2_SIZE err_offset;

    pcre2_code *re = pcre2_compile(
        (PCRE2_SPTR)pattern,
        PCRE2_ZERO_TERMINATED,
        0,
        &err_code,
        &err_offset,
        NULL);

    pcre2_match_data *md = NULL;
    if (re) {
        md = pcre2_match_data_create_from_pattern(re, NULL);
    }

    int rc = 0;
    if (md) {
        rc = pcre2_match(re, (PCRE2_SPTR)url, PCRE2_ZERO_TERMINATED, 0, 0, md, NULL);
    }

    if (md) {
        pcre2_match_data_free(md);
    }

    if (re) {
        pcre2_code_free(re);
    }

    return rc > 0;
}

static bool
match_endpoint(const struct Endpoint *endpoint, const char *url)
{
    bool result = false;

    switch (endpoint->match) {
    case MATCH_PREFIX:
        result = match_prefix(endpoint->pattern, url);
        break;
    case MATCH_REGEX:
        result = match_regex(endpoint->pattern, url);
        break;
    default:
        assert(0);
    }

    return result;
}


static void
lookup_endpoint(
    const struct Endpoint  *lookup_table,
    int                     lookup_table_size,
    const char             *url,
    const char             *method,
    const struct Endpoint **endpoint)
{
    const enum Method meth = encode_method(method);

    for (int i = 0; i != lookup_table_size; ++i) {
        if ((lookup_table[i].method & meth) == meth &&
            match_endpoint(&lookup_table[i], url))
        {
            *endpoint = &lookup_table[i];
            return;
        }
    }
}

static enum MHD_Result
handle_headers(
    void                  *cls,
    struct MHD_Connection *connection,
    const char            *url,
    const char            *method,
    const char            *version,
    const char            *upload_data,
    size_t                *upload_data_size,
    void                 **con_cls)
{
    (void)cls;
    (void)version;
    (void)upload_data;
    (void)upload_data_size;

    const struct Endpoint *endpoint = NULL;
    lookup_endpoint(endpoints, NUM_ENDPOINTS, url, method, &endpoint);

    struct HttpdRequest *request = NULL;
    if (endpoint) {
        request = httpd_request_new(
            connection,
            method,
            url,
            endpoint->handler);
        *con_cls = request;
    }
    if (request) {
        return MHD_YES;
    }

    enum MHD_Result result = MHD_NO;
    struct MHD_Response *response =
        MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
    if (response) {
        result = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
        MHD_destroy_response(response);
    }

    return result;
}

static enum MHD_Result
handle_body(
    void                  *cls,
    struct MHD_Connection *connection,
    const char            *url,
    const char            *method,
    const char            *version,
    const char            *upload_data,
    size_t                *upload_data_size,
    void                 **con_cls)
{
    (void)cls;
    (void)url;
    (void)method;
    (void)version;

    struct HttpdRequest *request = *con_cls;

    if (*upload_data_size) {
        // Handle incoming request data.
        httpd_request_accumulate_body(request, upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    }

    // Have received entire request.
    struct HttpdResponse *response = NULL;
    HttpdRequestHandler  handler = httpd_request_get_handler(request);
    if (handler) {
        response = handler(request);
    }

    enum MHD_Result result = MHD_NO;
    if (response) {
        result = MHD_queue_response(connection, response->status_code, response->mhd_response);
        httpd_response_free(response);
        return result;
    }

    struct MHD_Response *res =
        MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
    if (res) {
        result = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, res);
        MHD_destroy_response(res);
        return result;
    }

    return MHD_NO;
}

static enum MHD_Result
handle_connection(
    void                  *cls,
    struct MHD_Connection *connection,
    const char            *url,
    const char            *method,
    const char            *version,
    const char            *upload_data,
    size_t                *upload_data_size,
    void                 **con_cls)
{
    (void)cls;
    (void)connection;

    if (*con_cls == NULL) {
        // First callback, request has not yet been created.
        return handle_headers(
            cls,
            connection,
            url,
            method,
            version,
            upload_data,
            upload_data_size,
            con_cls);
    }

    // Subsequent callbacks, request has been created.
    return handle_body(
        cls,
        connection,
        url,
        method,
        version,
        upload_data,
        upload_data_size,
        con_cls);
}

static void
handle_completed(
    void                           *cls,
    struct MHD_Connection          *connection,
    void                          **con_cls,
    enum MHD_RequestTerminationCode reason)
{
    (void)cls;
    (void)connection;
    (void)reason;

    struct HttpdRequest *request = *con_cls;
    if (request) {
        httpd_request_free(request);
    }
}

static struct MHD_Daemon *httpd_daemon = NULL;

void httpd_stop() {
    if (httpd_daemon) {
        MHD_stop_daemon(httpd_daemon);
        httpd_daemon = NULL;
    }
}

int httpd_start() {
    const char *s_port = getenv("PORT");
    const int   port   = s_port ? atoi(s_port) : 80;
    httpd_daemon = MHD_start_daemon(
        MHD_USE_THREAD_PER_CONNECTION,
        port,
        NULL,
        NULL,
        handle_connection,
        NULL,
        MHD_OPTION_NOTIFY_COMPLETED,
        handle_completed,
        NULL,
        MHD_OPTION_END);
    return httpd_daemon ? 0 : 1;
}

// This file is part of the Raccoon's Centaur Mods (RCM).
//
// RCM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

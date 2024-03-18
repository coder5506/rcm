// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_HTTPD_H
#define RCM_HTTPD_H

#include <stddef.h>
#include <stdint.h>

#include <microhttpd.h>

//
// Types
//

struct HttpdRequest;
struct HttpdResponse;

typedef struct HttpdResponse *(*HttpdRequestHandler)(struct HttpdRequest*);

//
// Request
//

void httpd_request_free(struct HttpdRequest *request);

struct HttpdRequest*
httpd_request_new(
    struct MHD_Connection *connection,
    const char            *method,
    const char            *url,
    HttpdRequestHandler    handler);

void *httpd_request_userdata(const struct HttpdRequest *request);
void httpd_request_set_userdata(struct HttpdRequest *request, void *userdata);

const uint8_t *httpd_request_body(const struct HttpdRequest*);
size_t httpd_request_body_length(const struct HttpdRequest*);

void
httpd_request_accumulate_body(
    struct HttpdRequest  *request,
    const char           *data,
    size_t                len);

HttpdRequestHandler httpd_request_get_handler(const struct HttpdRequest*);

const char *httpd_request_url(const struct HttpdRequest*);

const char *httpd_request_method(const struct HttpdRequest*);

const char*
httpd_request_header(const struct HttpdRequest *request, const char *name);

const char *httpd_request_content_type(const struct HttpdRequest*);

const char*
httpd_request_query_var(const struct HttpdRequest*, const char *name);

void
httpd_request_query_string(const struct HttpdRequest*, char**);

const char*
httpd_request_post_var(const struct HttpdRequest*, const char *name);

//
// Response
//

void httpd_response_free(struct HttpdResponse *response);

//
// Daemon
//

void httpd_stop(void);
int httpd_start(void);

#endif

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

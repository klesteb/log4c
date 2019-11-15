
/*
 * appender_type_domain.c
 *
 * Copyright 2019, Kevin L. Esteb (kevin@kesteb.us). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <log4c/appender.h>
#include <log4c/appender_type_domain.h>
#include <sd/malloc.h>
#include <sd/error.h>
#include <sd/domnode.h>
#include <sd/sd_xplatform.h>

/* Internal structs that defines the conf and the state info
 * for an instance of the appender_type_domain type.
 */    

struct __domain_udata {
    char *path;
    int sfd;
    int timeout;
};

/***************************************************************************
                Appender Interface functions: open, append, close, parser

****************************************************************************/
static int domain_open(log4c_appender_t *this) {

    int rc = 0; 
    int sfd = 0;
    struct sockaddr_un saddr;
    domain_udata_t *udata = log4c_appender_get_udata(this);  

    sd_debug("domain_appender_open[");

    memset(&saddr, 0, sizeof(struct sockaddr_un));

    if (udata->sfd == 0) {

        errno = 0;
        sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (sfd == -1) {

            rc = -1;
            sd_error("domain_open(): %d, %s", errno, strerror(errno));
            goto fini;

        }

        saddr.sun_family = AF_LOCAL;
        strcpy(saddr.sun_path, udata->path);

        errno = 0;
        rc = connect(sfd, (struct sockaddr *)&saddr, SUN_LEN(&saddr));
        if (rc == -1) {

            sd_error("domain_open(): %d, %s", errno, strerror(errno));
            goto fini;

        }

        domain_udata_set_fd(udata, sfd);

    }

    fini:
    sd_debug("]");

    return rc;

}

/***************************************************************************/
static int domain_append(log4c_appender_t* this, 
			 const log4c_logging_event_t* a_event) {

    int len = 0;
    int rc = -1;
    int count = 0;
    int bufsize = 0;
    char *buffer = NULL;
    char *fmt = "%s\r\n";
    domain_udata_t *udata = log4c_appender_get_udata(this); 

    sd_debug("domain_append[");

    if (udata != NULL) {

        len = strlen(a_event->evt_rendered_msg) + 3;
        buffer = sd_calloc(1, len);
        snprintf(buffer, len, fmt, a_event->evt_rendered_msg);
        bufsize = len - 1;

        while (bufsize > 0) {

            errno = 0;

            if ((count = write(udata->sfd, buffer, bufsize))) {

                if (errno == EINTR) {

                    count = 0;

                } else {

                    sd_error("domain_append(): %d, %s", errno, strerror(errno));
                    goto fini;

                }

                bufsize -= count;
                buffer += count;

            }

        }

    }

    fini:
    free(buffer);

    sd_debug("]");

    return rc;

}

/****************************************************************************/
static int domain_close(log4c_appender_t* this) {  

    int rc = 0;
    domain_udata_t *udata = log4c_appender_get_udata(this); 

    sd_debug("domain_close[");

    if (!this) {

        rc = -1; 
        goto fini;

    }
 
    if (udata == NULL) {

        rc = -1;
        goto fini;

    }

    free(udata->path);
    close(udata->sfd);

    fini:
    sd_debug("]");

    return rc;

}

/*******************************************************************************/
static int domain_parse(log4c_appender_t *this, void *a_node) {

    int timeout = 10;
    struct sockaddr_un saddr;
    sd_domnode_t *dom = NULL;
    domain_udata_t *udata = NULL;
    char *path = "/tmp/logger-socket";
    int len = (sizeof(saddr.sun_path) - 1);
    sd_domnode_t *anode = (sd_domnode_t *)a_node;

    dom = sd_domnode_attrs_get(anode, "path");
    if (dom && dom->value) {

        path = dom->value;
        if (strlen(path) > len) {

            path[len + 1] = '\0';

        }

    }

    dom = sd_domnode_attrs_get(anode, "timeout");
    if (dom && dom->value) {

        timeout = atoi(dom->value);

    }

    sd_debug("path='%s', timeout='%d'", path, timeout);

    udata = domain_make_udata();
    domain_udata_set_path(udata, path);
    domain_udata_set_timeout(udata, timeout);

    log4c_appender_set_udata(this, udata);

    return 0;
    
}

/*******************************************************************************/
domain_udata_t *domain_make_udata(void) {

    domain_udata_t *udata = NULL;
    udata = (domain_udata_t *)sd_calloc(1, sizeof(domain_udata_t));

    return udata;

}

/*******************************************************************************/
LOG4C_API int domain_udata_set_path(domain_udata_t *udata, const char *path) {

    struct sockaddr_un saddr;
    int len = (sizeof(saddr.sun_path) - 1);

    udata->path = strndup(path, len);

    return 0;

}

/*******************************************************************************/
LOG4C_API const char * domain_udata_get_path(domain_udata_t *udata){

    return udata->path; 

}

/*******************************************************************************/
LOG4C_API int domain_udata_set_fd(domain_udata_t *udata, int fd) {

    udata->sfd = fd;

    return 0;

}

/*******************************************************************************/
LOG4C_API int  domain_udata_get_fd(domain_udata_t *udata){

    return udata->sfd; 

}

/*******************************************************************************/
LOG4C_API int domain_udata_set_timeout(domain_udata_t *udata, int timeout) {

    udata->timeout = timeout;

    return 0;

}

/*******************************************************************************/
LOG4C_API int domain_udata_get_timeout(domain_udata_t *udata){

    return udata->timeout; 

}

/****************************************************************************/
const log4c_appender_type_t log4c_appender_type_domain = {
    "domain",
    domain_open,
    domain_append,
    domain_close,
    domain_parse
};


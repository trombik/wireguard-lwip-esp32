/*
 * Copyright (c) 2022 Tomoyuki Sakurai <y@trombik.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 * 3. Neither the name of "Floorsense Ltd", "Agile Workspace Ltd" nor the names of
 *  its contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#if !defined(__ESP_WIREGUARD__H__)
#define __ESP_WIREGUARD__H__

#include <stdint.h>
#include <esp_err.h>
#include <lwip/netif.h>

typedef struct {
    /* interface config */
    char*       private_key;            /**< a base64 private key generated by wg genkey. Required. */
    int         listen_port;            /**< a 16-bit port for listening */
    uint32_t    fw_mark;                /**< a 32-bit fwmark for outgoing packets */
    /* peer config */
    char*       public_key;             /**< a base64 public key calculated by wg pubkey from a private key. Required. */
    char*       preshared_key;          /**< a base64 preshared key generated by wg genpsk. */
    char*       allowed_ip;             /**< a local IP address. */
    char*       allowed_ip_mask;        /**< a subnet mask of the local IP address. */
    char*       endpoint;               /**< an endpoint IP address or hostname. */
    int         port;                   /**< a port number of remote endpoint. */
    int         persistent_keepalive;   /**< a seconds interval, between 1 and 65535 inclusive, of how often to send an
                                             authenticated empty packet to the peer for the purpose of keeping a stateful
                                             firewall or NAT mapping valid persistently. */
} wireguard_config_t;

typedef struct {
    wireguard_config_t* config;        /**< a pointer to wireguard config */
    struct netif*       netif;         /**< a pointer to configured netif */
    struct netif*       netif_default; /**< a pointer to the default netif. */
} wireguard_ctx_t;

/**
 * @brief Initialize WireGuard
 *
 * @param       config WireGuard configuration.
 * @param[out]  ctx Context of WireGuard.
 *
 * @return
 *      - ESP_OK: Successfully initilized WireGuard interface.
 *      - ESP_ERR_INVALID_ARG: given argument is invalid.
 *      - ESP_FAIL: Other error.
 */
esp_err_t esp_wireguard_init(wireguard_config_t *config, wireguard_ctx_t *ctx);

/**
 * @brief Create a WireGuard interface and establish the connection to the
 *        peer.
 *
 * @param       ctx Context of WireGuard.
 * @return
 *      - ESP_OK on success.
 *      - ESP_FAIL on failure.
 */
esp_err_t esp_wireguard_connect(wireguard_ctx_t *ctx);

/**
 * @brief Set the default gateway to the peer.
 * @param ctx Context of WireGuard
 */
void esp_wireguard_set_default(wireguard_ctx_t *ctx);

/**
 * @brief Test if the peer is up.
 */
esp_err_t esp_wireguardif_peer_is_up(wireguard_ctx_t *ctx);

/**
 * @brief Disconnect from the peer
 *
 * @param ctx Context of WireGuard.
 */
void esp_wireguard_disconnect(wireguard_ctx_t *ctx);

#endif
// vim: expandtab

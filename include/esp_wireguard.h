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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <esp_err.h>
#include <lwip/netif.h>

#define ESP_WIREGUARD_CONFIG_DEFAULT() { \
    .private_key = NULL, \
    .listen_port = 0, \
    .fw_mark = 0, \
    .base_ip = NULL, \
    .net_mask = NULL \
}

#define ESP_WIREGUARD_PEER_CONFIG_DEFAULT() { \
    .public_key = NULL, \
    .preshared_key = NULL, \
    .allowed_ip = NULL, \
    .allowed_ip_mask = NULL, \
    .endpoint = NULL, \
    .port = 51820, \
    .persistent_keepalive = 0, \
}

typedef struct {
    /* peer config */
    char*       public_key;             /**< a base64 public key calculated by wg pubkey from a private key. Required. */
    char*       preshared_key;          /**< a base64 preshared key generated by wg genpsk. */
    char*       allowed_ip;             /**< a local IP address. */
    char*       allowed_ip_mask;        /**< a subnet mask of the local IP address. */
    char*       endpoint;               /**< an endpoint IP address or hostname. */
    int         port;                   /**< a port number of remote endpoint. Default is 51820. */
    int         persistent_keepalive;   /**< a seconds interval, between 1 and 65535 inclusive, of how often to send an
                                             authenticated empty packet to the peer for the purpose of keeping a stateful
                                             firewall or NAT mapping valid persistently. Set zero to disable the feature.
                                             Default is zero. */
} wireguard_peer_config_t;

typedef struct {
    /* interface config */
    char *private_key; /**< a base64 private key generated by wg genkey. Required. */
    int listen_port;   /**< a 16-bit port for listening */
    uint32_t fw_mark;  /**< a 32-bit fwmark for outgoing packets */
    char *base_ip;     /**< a 32-bit fwmark for outgoing packets */
    char *net_mask;     /**< a 32-bit fwmark for outgoing packets */
} wireguard_config_t;

typedef struct {
    wireguard_config_t* config;        /**< a pointer to wireguard config */
    struct netif*       netif;         /**< a pointer to configured netif */
    struct netif*       netif_default; /**< a pointer to the default netif. */
} wireguard_ctx_t;

/**
 * @brief Initialize WireGuard
 *
 * Call this function to initilize the context of WireGuard.
 *
 * Do not call this function multiple times.
 *
 * To connect to other peer, use `esp_wireguard_disconnect()`, and
 * `esp_wireguard_init()` with a new configuration.
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
 * @brief Create a WireGuard interface and start establishing the connection
 *        to the peer.
 *
 * Call the funtion to start establishing the connection. Note that `ESP_OK`
 * does not mean the connection is established. To see if the connection is
 * established, or the peer is up, use `esp_wireguardif_peer_is_up()`.
 *
 * Do not call this function multiple times.
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
 * @return
 *      - ESP_OK on success.
 */
esp_err_t esp_wireguard_set_default(wireguard_ctx_t *ctx);

/**
 * @brief Test if the peer is up.
 */
esp_err_t esp_wireguardif_peer_is_up(wireguard_ctx_t *ctx, const char *pubkey);

/**
 * @brief Disconnect from the peer
 *
 * @param ctx Context of WireGuard.
 * @return
 *      - ESP_OK on success.
 */
esp_err_t esp_wireguard_disconnect(wireguard_ctx_t *ctx);

/**
 * @brief Add and connect a peer to wireguard
 *
 * @param peer_config Configuration of a peer.
 * @param wireguard_peer_index Index of the peer.
 * @return
 *      - ESP_OK on success.
 */
esp_err_t esp_wireguard_add_peer(wireguard_ctx_t *ctx, wireguard_peer_config_t* peer_config);

/**
 * @brief Remove a peer from wireguard
 *
 * @param ctx Context of WireGuard.
 * @return
 *      - ESP_OK on success.
 */
esp_err_t esp_wireguard_remove_peer(wireguard_ctx_t* ctx, const char *pubkey);

/**
 * @brief Update a peer in wireguard
 *
 * @param peer_config New config of peer.
 * @param wireguard_peer_index Index of the peer.
 * @return
 *      - ESP_OK on success.
 */
esp_err_t esp_wireguard_update_peer(wireguard_ctx_t* ctx, wireguard_peer_config_t *peer_config);

#ifdef __cplusplus
}
#endif

#endif
// vim: expandtab

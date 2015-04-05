/*
 * nicookie_osx_chrome.c
 * Copyright (c) 2015 IGARASHI Makoto (raccy)
 * This software is released under the MIT License, see LICENSE.
 */

#include <stdio.h>
#include <string.h>

#include <sqlite3.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/bio.h>


#include "nicookie.h"
#include "nicookie_config.h"
#include "nicookie_util.h"
#include "nicookie_osx.h"
#include "nicookie_chrome.h"

static unsigned char *nicookie_osx_chrome_key(void);
static char *nicookie_osx_chrome_decryt(const char *encrypted_value,
    int encrypted_value_size);

// http://n8henrie.com/2014/05/decrypt-chrome-cookies-with-python/

char *nicookie_osx_chrome(char *buf, size_t size) {
  char *cookies_path = nicookie_str_with_env(
      "%s/Library/Application Support/Google/Chrome/Default/Cookies",
      "HOME");
  if (cookies_path == NULL) return NULL;

  char *result_buf = nicookie_chrome_cookies(buf, size, cookies_path,
      nicookie_osx_chrome_decryt);
  free(cookies_path);
  return result_buf;
}

static unsigned char *nicookie_osx_chrome_key(void) {
  char *cmd = "security find-generic-password -w -a Chrome -s 'Chrome Safe Storage'";
  FILE *cmd_out = popen(cmd, "r");
  char sec_key[256] = {'\0'};

  if (fgets(sec_key, 256, cmd_out) == NULL) {
    errno = EACCES;
    return NULL;
  }

  if (strlen(sec_key) == 0) {
    errno = EACCES;
    return NULL;
  }

  nicookie_str_chomp(sec_key);
  nicookie_debug_str(sec_key);

  // 16
  unsigned char *enc_key = malloc(16);
  if (enc_key == NULL) {
    return NULL;
  }

  int iterations = 1003;
  char *salt = "saltysalt";
  int pbkdf2_r = PKCS5_PBKDF2_HMAC_SHA1(sec_key, strlen(sec_key),
      (unsigned char *)salt, strlen(salt),
      iterations, 16, enc_key);
  nicookie_debug_int(pbkdf2_r);
  return enc_key;
}

static char *nicookie_osx_chrome_decryt(const char *encrypted_value,
    int encrypted_value_size) {
  unsigned char *enc_key = nicookie_osx_chrome_key();
  if (enc_key == NULL) {
    return NULL;
  }

  unsigned char iv[16];
  for (int i = 0; i < 16; i++) iv[i] = ' ';

  // alwayes enc size >= dec size
  int plain_value_size = encrypted_value_size;
  char *plain_value = malloc(plain_value_size);
  if (plain_value == NULL) {
    free(enc_key);
    return NULL;
  }

  int result = 1;
  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);

  result = EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, enc_key, iv);
  if (!result) {
    EVP_CIPHER_CTX_cleanup(&ctx);
    free(enc_key);
    free(plain_value);
    errno = EACCES;
    return NULL;
  }

  result = EVP_DecryptUpdate(&ctx,
      (unsigned char *)plain_value, &plain_value_size,
      (unsigned char *)(encrypted_value + 3), encrypted_value_size - 3);
  if (!result) {
    EVP_CIPHER_CTX_cleanup(&ctx);
    free(enc_key);
    free(plain_value);
    errno = EACCES;
    return NULL;
  }

  int fin_size = 0;
  result = EVP_DecryptFinal_ex(&ctx,
      (unsigned char *)(plain_value + plain_value_size), &fin_size);
  if (!result) {
    EVP_CIPHER_CTX_cleanup(&ctx);
    free(enc_key);
    free(plain_value);
    errno = EACCES;
    return NULL;
  }

  EVP_CIPHER_CTX_cleanup(&ctx);
  free(enc_key);

  nicookie_debug_int(plain_value_size);
  nicookie_debug_int(fin_size);
  plain_value_size += fin_size;
  plain_value[plain_value_size] = '\0';
  nicookie_debug_str(plain_value);

  return plain_value;
}

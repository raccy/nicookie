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

  char *value = NULL;
  void *encrypted_value = NULL;
  int encrypted_value_size = 0;
  int result = nicookie_sqlite3(cookies_path,
      "SELECT value,encrypted_value FROM cookies WHERE"
      " host_key = '"  NICOOKIE_COOKIE_HOST
      "' AND name = '" NICOOKIE_COOKIE_NAME
      "' AND path = '" NICOOKIE_COOKIE_PATH "';",
      &value, &encrypted_value, &encrypted_value_size);
  if (result != SQLITE_OK || value == NULL) {
    if (errno == 0) errno = ENOENT;
    return NULL;
  }
  nicookie_debug_str(value);
  nicookie_debug_int(encrypted_value_size);
  free(cookies_path);

  unsigned char *enc_key = nicookie_osx_chrome_key();
  if (enc_key == NULL) {
    return NULL;
  }

  // char *cmd = "security find-generic-password -w -a Chrome -s 'Chrome Safe Storage'";
  // FILE *cmd_out = popen(cmd, "r");
  // char *key = malloc(256);
  // fgets(key, 256, cmd_out);
  // nicookie_debug_int(key);
  // nicookie_debug_str(key);
  //
  // unsigned char enc_key[16];
  // int iterations = 1003;
  //
  // char *salt = "saltysalt";
  // // 改行！
  // int pbkdf2_r = PKCS5_PBKDF2_HMAC_SHA1(key, strlen(key) - 1, salt, strlen(salt), iterations, 16, enc_key);
  // nicookie_debug_int(pbkdf2_r);

  unsigned char iv[16];
  for (int i = 0; i < 16; i++) iv[i] = ' ';
  int plain_size = 256;
  char *plain = NULL;
  plain = malloc(plain_size);
  nicookie_debug_int(plain);

  EVP_CIPHER_CTX ctx;
  EVP_CIPHER_CTX_init(&ctx);
  nicookie_debug_int(&ctx);
  EVP_DecryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, enc_key, iv);
  nicookie_debug_int(&iv);

  EVP_DecryptUpdate(&ctx, (unsigned char *)plain, &plain_size, (unsigned char *)(encrypted_value + 3), encrypted_value_size - 3);
  int fin_size = 0;
  EVP_DecryptFinal_ex(&ctx, (unsigned char *)(plain + plain_size), &fin_size);
  nicookie_debug_int(plain_size);
  nicookie_debug_int(fin_size);
  plain[plain_size] = '\0';

  nicookie_debug_str(plain);
  EVP_CIPHER_CTX_cleanup(&ctx);

  free(enc_key);

  return plain;

}

static unsigned char *nicookie_osx_chrome_key(void) {
  char *cmd = "security find-generic-password -w -a Chrome -s 'Chrome Safe Storage'";
  FILE *cmd_out = popen(cmd, "r");
  char sec_key[256];
  if (fgets(sec_key, 256, cmd_out) == NULL) {
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
      salt, strlen(salt),
      iterations, 16, enc_key);
  nicookie_debug_int(pbkdf2_r);
  return enc_key;
}

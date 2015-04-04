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
// #include "nicookie_general_chrome.h"

// #define
// OSX_FIREFOX_PROFILE_COOKIE_FILE_SUFFIX
// "${HOME}/Library/Application Support/Firefox/profiles.ini"
// "${HOME}/.mozilla/firefox/profiles.ini"
// "${APPDATA}\\Mozilla\\Firefox\\profiles.ini"

// http://n8henrie.com/2014/05/decrypt-chrome-cookies-with-python/

char *nicookie_osx_chrome(char *buf, size_t size) {
  char *cookies_path = nicookie_str_with_env("%s/Library/Application Support/Google/Chrome/Default/Cookies", "HOME");

  sqlite3 *db = NULL;
  if (sqlite3_open_v2(cookies_path, &db, SQLITE_OPEN_READONLY, NULL)
      != SQLITE_OK) {
    sqlite3_close(db);
    return NULL;
  }
  nicookie_debug_int(db);

  char *errmsg = NULL;
  sqlite3_stmt *stmp;

  int result = sqlite3_prepare(db, "select value,encrypted_value from cookies where host_key = '.nicovideo.jp' and name = 'user_session' and path = '/';", -1, &stmp, NULL);
  int rc = sqlite3_step(stmp);
  const char *text = (const char *)sqlite3_column_text(stmp, 0);
  char *value = malloc(strlen(text) + 1);
  strcpy(value, text);

  int blob_size = sqlite3_column_bytes(stmp, 1);
  char *blob = malloc(blob_size);
  memcpy(blob, sqlite3_column_blob(stmp, 1), blob_size);

  nicookie_debug_int(blob_size);

  sqlite3_finalize(stmp);

  char *cmd = "security find-generic-password -w -a Chrome -s 'Chrome Safe Storage'";
  FILE *cmd_out = popen(cmd, "r");
  char *key = malloc(256);
  fgets(key, 256, cmd_out);
  nicookie_debug_int(key);
  nicookie_debug_str(key);

  nicookie_debug_int(result);
  nicookie_debug_int(value);
  nicookie_debug_str(value);
  // nicookie_debug_str(blob);

  unsigned char enc_key[16];
  int iterations = 1003;

  char *salt = "saltysalt";
  // 改行！
  int pbkdf2_r = PKCS5_PBKDF2_HMAC_SHA1(key, strlen(key) - 1, salt, strlen(salt), iterations, 16, enc_key);
  nicookie_debug_int(pbkdf2_r);

  // unsigned char *enc_key = nicookie_base64_decode(key);

  // for (int i = 0; i < 128 / 8; i++) {
  //   printf("%02x", enc_key[i]);
  // }
  // printf("\n");


  // TODO base64をばらして、AESで複合化？
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

  EVP_DecryptUpdate(&ctx, (unsigned char *)plain, &plain_size, (unsigned char *)(blob + 3), blob_size - 3);
  int fin_size = 0;
  EVP_DecryptFinal_ex(&ctx, (unsigned char *)(plain + plain_size), &fin_size);
  nicookie_debug_int(plain_size);
  nicookie_debug_int(fin_size);
  plain[plain_size] = '\0';

  nicookie_debug_str(plain);
  EVP_CIPHER_CTX_cleanup(&ctx);


  sqlite3_close(db);
  free(cookies_path);

  return plain;

}

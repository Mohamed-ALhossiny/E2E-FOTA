/**
  ******************************************************************************
  * @file    crypto.h
  * @author  MCD Application Team
  * @brief   Main header file of helper for migration of cryptographics
  *          library V3 to V4
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef CMOX_HELPER_H
#define CMOX_HELPER_H

/* common headers */
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_cbc.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_ccm.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_cfb.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_ctr.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_ecb.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_gcm.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_keywrap.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_ofb.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_aes_xts.h"
#include "../../../legacy_v3/include/cipher/legacy_v3_chachapoly.h"
#include "../../../legacy_v3/include/drbg/legacy_v3_ctr_drbg.h"
#include "../../../legacy_v3/include/ecc/legacy_v3_c25519.h"
#include "../../../legacy_v3/include/ecc/legacy_v3_ecc.h"
#include "../../../legacy_v3/include/ecc/legacy_v3_ed25519.h"
#include "../../../legacy_v3/include/err_codes.h"
#include "../../../legacy_v3/include/hash/legacy_v3_sha1.h"
#include "../../../legacy_v3/include/hash/legacy_v3_sha224.h"
#include "../../../legacy_v3/include/hash/legacy_v3_sha256.h"
#include "../../../legacy_v3/include/hash/legacy_v3_sha384.h"
#include "../../../legacy_v3/include/hash/legacy_v3_sha512.h"
#include "../../../legacy_v3/include/mac/legacy_v3_cmac.h"
#include "../../../legacy_v3/include/mac/legacy_v3_hmac_sha1.h"
#include "../../../legacy_v3/include/mac/legacy_v3_hmac_sha224.h"
#include "../../../legacy_v3/include/mac/legacy_v3_hmac_sha256.h"
#include "../../../legacy_v3/include/mac/legacy_v3_hmac_sha384.h"
#include "../../../legacy_v3/include/mac/legacy_v3_hmac_sha512.h"
#include "../../../legacy_v3/include/rsa/legacy_v3_rsa.h"

#endif /* CMOX_HELPER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

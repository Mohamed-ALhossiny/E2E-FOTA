/**
  *******************************************************************************
  * @file    legacy_v3_aes_ccm.h
  * @author  MCD Application Team
  * @brief   Header file of AES CCM helper for migration of cryptographics
  *          library V3 to V4
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *******************************************************************************
  */

#ifndef LEGACY_V3_AES_CCM_H
#define LEGACY_V3_AES_CCM_H

#include "cipher/cmox_ccm.h"

#include "../../../../legacy_v3/include/cipher/legacy_v3_aes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct
{
  cmox_ccm_handle_t ccm_handle;
  cmox_cipher_handle_t *cipher_handle;
  uint32_t   mContextId;  /*!< Unique ID of this context. \b Not \b used in current implementation. */
  int32_t mAssDataSize;   /*!< Size of the associated data to be processed yet.
                               This must be set by the caller prior to calling Init. */
  int32_t mPayloadSize;   /*!< Size of the payload data to be processed yet size.
                               This must be set by the caller prior to calling Init. */
  SKflags_et mFlags;      /*!< 32 bit mFlags, used to perform keyschedule */
  int32_t   mNonceSize;   /*!< Size of the Initialization Vector in bytes */
  int32_t   mKeySize;     /*!< Key length in bytes */
  int32_t   mTagSize;     /*!< Tag length in bytes */
  uint8_t   *pmTag;       /*!< Pointer to Authentication TAG. This value must be set in decryption,
                               and this TAG will be verified */
} AESCCMctx_stt; /*!< AES context structure for CBC mode */


/**
  * @brief Initialization for AES CCM Encryption
  * @param[in,out]    *P_pAESCCMctx  AES CCM context
  * @param[in]        *P_pKey        Buffer with the Key
  * @param[in]        *P_pNonce      Buffer with the Nonce
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
  * @note \c P_pAESCCMctx.mKeySize (see \ref AESCCMctx_stt) must be set with the size of the key prior to calling this
  *           function.
  *           Otherwise the following predefined values can be used:
  *         - \ref CRL_AES128_KEY
  *         - \ref CRL_AES192_KEY
  *         - \ref CRL_AES256_KEY
  * @note \c P_pAESCCMctx.mFlags must be set prior to calling this function. Default value is E_SK_DEFAULT.
  *          See \ref SKflags_et for details.
  * @note \c P_pAESCCMctx.mNonceSize must be set with the size of the CCM Nonce. Possible values are {7,8,9,10,11,12,13}
  * @note \c P_pAESCCMctx.mTagSize must be set with the size of authentication TAG that will be generated by the
  *          \ref AES_CCM_Encrypt_Finish. Possible values are values are {4,6,8,10,12,14,16}
  * @note \c P_pAESCCMctx.mAssDataSize must be set with the size of the Associated Data (i.e. Header or any data that
  *          will be authenticated but not encrypted)
  * @note \c P_pAESCCMctx.mPayloadSize must be set with the size of the Payload (i.e. Data that will be authenticated
  *          and encrypted)
  * @remark In CCM standard the TAG is appended to the Ciphertext. In this implementation, for API compatibility with
  *         GCM, the user must supply a pointer to \ref AES_CCM_Encrypt_Finish that will be used to output the
  *         authentication TAG.
  */
int32_t AES_CCM_Encrypt_Init(AESCCMctx_stt *P_pAESCCMctx, const uint8_t *P_pKey, const uint8_t *P_pNonce);


/**
  * @brief AES CCM Header processing function
  * @param[in,out]    *P_pAESCCMctx     AES CCM, already initialized, context
  * @param[in]    *P_pInputBuffer   Input buffer
  * @param[in]     P_inputSize      Size of input data, expressed in bytes
  * @retval   AES_SUCCESS Operation Successful
  * @retval   AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval   AES_ERR_BAD_OPERATION   Append not allowed
  * @note   This function can be called multiple times, provided that \c P_inputSize is a multiple of 16. \n
  *         A single, final, call with \c P_inputSize not multiple of 16 is allowed.
  */
int32_t AES_CCM_Header_Append(AESCCMctx_stt *P_pAESCCMctx, const uint8_t *P_pInputBuffer, int32_t P_inputSize);

/**
  * @brief AES CCM Encryption function
  * @param[in,out] *P_pAESCCMctx     AES CCM, already initialized, context
  * @param[in]    *P_pInputBuffer   Input buffer
  * @param[in]     P_inputSize      Size of input data, expressed in bytes
  * @param[out]   *P_pOutputBuffer  Output buffer
  * @param[out]   *P_pOutputSize    Pointer to integer that will contain the size of written output data, expressed in
  *                                 bytes
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_OPERATION   Append not allowed
  * @note   This function can be called multiple times, provided that \c P_inputSize is a multiple of 16. \n
  *         A single, final, call with \c P_inputSize not multiple of 16 is allowed.
  */
int32_t AES_CCM_Encrypt_Append(AESCCMctx_stt *P_pAESCCMctx,
                               const uint8_t *P_pInputBuffer,
                               int32_t        P_inputSize,
                               uint8_t       *P_pOutputBuffer,
                               int32_t       *P_pOutputSize);

/**
  * @brief AES CCM Finalization during Encryption, this will create the Authentication TAG
  * @param[in,out] *P_pAESCCMctx     AES CCM, already initialized, context
  * @param[out]    *P_pOutputBuffer  Output Authentication TAG
  * @param[out]    *P_pOutputSize    Size of returned TAG
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @note This function \b requires:  \c P_pAESCCMctx->mTagSize to contain a valid value in the set {4,6,8,10,12,14,16}
  */
int32_t AES_CCM_Encrypt_Finish(AESCCMctx_stt *P_pAESCCMctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);


/**
  * @brief Initialization for AES CCM Decryption
  * @param[in,out]    *P_pAESCCMctx  AES CCM context
  * @param[in]        *P_pKey        Buffer with the Key
  * @param[in]        *P_pNonce      Buffer with the Nonce
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
  * @note \c P_pAESCCMctx.mKeySize (see \ref AESCCMctx_stt) must be set with the size of the key prior to calling this
  *           function.
  *           Otherwise the following predefined values can be used:
  *         - \ref CRL_AES128_KEY
  *         - \ref CRL_AES192_KEY
  *         - \ref CRL_AES256_KEY
  * @note \c P_pAESCCMctx.mFlags must be set prior to calling this function. Default value is E_SK_DEFAULT.
  *          See \ref SKflags_et for details.
  * @note \c P_pAESCCMctx.mNonceSize must be set with the size of the CCM Nonce. Possible values are {7,8,9,10,11,12,13}
  * @note \c P_pAESCCMctx.pmTag must be set with a pointer to the authentication TAG that will be checked during
  *          \ref AES_CCM_Decrypt_Finish
  * @note \c P_pAESCCMctx.mTagSize must be set with the size of authentication TAG that will be checked by the
  *          \ref AES_CCM_Decrypt_Finish. Possible values are values are {4,6,8,10,12,14,16}
  * @note \c P_pAESCCMctx.mAssDataSize must be set with the size of the Associated Data (i.e. Header or any data that
  *          will be authenticated but not encrypted)
  * @note \c P_pAESCCMctx.mPayloadSize must be set with the size of the Payload (i.e. Data that will be authenticated
  *          and encrypted)
  * @remark CCM standard expects the authentication TAG to be passed as part of the ciphertext. In this implementations
  *         the tag should be \b not be passed to \ref AES_CCM_Decrypt_Append.
  *         Instead a pointer to the TAG must be set in \c P_pAESCCMctx.pmTag and this will be checked by
  *         \ref AES_CCM_Decrypt_Finish
  * @remark This function is just a helper for \ref AES_CCM_Encrypt_Init
  */
int32_t AES_CCM_Decrypt_Init(AESCCMctx_stt *P_pAESCCMctx, const uint8_t *P_pKey, const uint8_t *P_pNonce);

/**
  * @brief AES CCM Decryption function
  * @param[in,out] *P_pAESCCMctx     AES CCM, already initialized, context
  * @param[in]    *P_pInputBuffer   Input buffer
  * @param[in]     P_inputSize      Size of input data, expressed in bytes
  * @param[out]   *P_pOutputBuffer  Output buffer
  * @param[out]   *P_pOutputSize    Pointer to integer that will contain the size of written output data, expressed in
  *                                 bytes
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_OPERATION   Append not allowed
  * @note   This function can be called multiple times, provided that \c P_inputSize is a multiple of 16. \n
  *         A single, final, call with \c P_inputSize not multiple of 16 is allowed.
  * @remark This function shouldn't process the TAG, which is part of the ciphertext according to CCM standard.
  */
int32_t AES_CCM_Decrypt_Append(AESCCMctx_stt *P_pAESCCMctx,
                               const uint8_t *P_pInputBuffer,
                               int32_t        P_inputSize,
                               uint8_t       *P_pOutputBuffer,
                               int32_t       *P_pOutputSize);

/**
  * @brief AES CCM Finalization during Decryption, the authentication TAG will be checked
  * @param[in,out] *P_pAESCCMctx     AES CCM, already initialized, context
  * @param[out]    *P_pOutputBuffer  Won't be used
  * @param[out]    *P_pOutputSize    Will contain zero
  * @return    Result of Authentication or error code
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT  pmTag should be set and mTagSize must be valid
  * @retval    AUTHENTICATION_SUCCESSFUL if the TAG is verified
  * @retval    AUTHENTICATION_FAILED if the TAG is not verified
  * @note This function \b requires:
  *        - \c P_pAESCCMctx->pmTag to be set to a valid pointer to the tag to be checked
  *        - \c P_pAESCCMctx->mTagSize to contain a valid value in the set {4,6,8,10,12,14,16}
  */
int32_t AES_CCM_Decrypt_Finish(AESCCMctx_stt *P_pAESCCMctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* LEGACY_V3_AES_CCM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
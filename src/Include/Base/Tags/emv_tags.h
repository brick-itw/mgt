#ifndef _U2_EMV_TAGS_H
#define _U2_EMV_TAGS_H

#define TAG_ADF_NAME       0x4FUL
#define TAG_APP_LABEL      0x50UL
#define TAG_TRACK2         0x57UL
#define TAG_PAN            0x5AUL
#define TAG_DIR_ENTRY      0x61UL
#define TAG_REC_TEMPLATE   0x70UL
#define TAG_SCR_BEFORE     0x71UL
#define TAG_SCR_AFTER      0x72UL
#define TAG_RESP_TMP2      0x77UL
#define TAG_RESP_TEMPLATE  0x80UL
#define TAG_AMOUNT_AUTHORISED_BINARY 0x81UL
#define TAG_AIP            0x82UL
#define TAG_PDOL_DATA      0x83UL
#define TAG_DFNAME         0x84UL
#define TAG_SCR_CMD        0x86UL
#define TAG_APP_PRT        0x87UL
#define TAG_DIRECTORY_SFI  0x88UL

#define TAG_AUTH_RESP_CODE 0x8AUL
#define TAG_CDOL_BASE      0x8CUL
#define TAG_CDOL1          0x8CUL
#define TAG_CDOL2          0x8DUL
#define TAG_APP_CVMLIST    0x8EUL
#define TAG_KEY_IDX        0x8FUL
#define TAG_ISS_PK_CERT    0x90UL
#define TAG_ISS_AUTH_DATA  0x91UL
#define TAG_ISS_PK_REM     0x92UL
#define TAG_SIGNED_DATA    0x93UL
#define TAG_AFL            0x94UL
#define TAG_TVR            0x95UL
#define TAG_TDOL           0x97UL
#define TAG_TC_HASH        0x98UL
#define TAG_TRX_DATE       0x9AUL
#define TAG_TSI            0x9BUL
#define TAG_TRX_TYPE       0x9CUL
#define TAG_DDF_NAME       0x9DUL

#define TAG_HOLDER_NAME  0x5F20UL
#define TAG_APP_EXP_DATE 0x5F24UL
#define TAG_APP_EFF_DATE 0x5F25UL
#define TAG_ISSUER_CNTRY 0x5F28UL
#define TAG_CUR_ID       0x5F2AUL
#define TAG_APP_LANGUAGE 0x5F2DUL
#define TAG_PAN_SEQNUM   0x5F34UL
#define TAG_CUR_EXP      0x5F36UL

#define TAG_TRX_AMOUNT   0x9F02UL
#define TAG_AMOUNT_OTHER 0x9F03UL
#define TAG_AID          0x9F06UL
#define TAG_AUC          0x9F07UL
#define TAG_APP_VERSION  0x9F08UL
#define TAG_TRM_VERSION  0x9F09UL
#define TAG_IAC_DEFAULT  0x9F0DUL
#define TAG_IAC_DENY     0x9F0EUL
#define TAG_IAC_ONLINE   0x9F0FUL
#define TAG_IAPP_DATA    0x9F10UL
#define TAG_APP_PREF_NAME  0x9F12UL
#define TAG_LAST_ONLINE  0x9F13UL
#define TAG_LOWER_C_LIM  0x9F14UL
#define TAG_MCC_CODE     0x9F15UL
#define TAG_MERCH_ID     0x9F16UL
#define TAG_PIN_TRIES    0x9F17UL
#define TAG_SCRIPT_ID    0x9F18UL
#define TAG_TRM_CNTRY    0x9F1AUL
#define TAG_FLOOR_LIMIT  0x9F1BUL
#define TAG_TERM_ID      0x9F1CUL
#define TAG_RM_DATA      0x9F1DUL   // 8 bytes
#define TAG_TERM_SN      0x9F1EUL   // 8 bytes
#define TAG_TRX_TIME     0x9F21UL
#define TAG_UPPER_C_LIM  0x9F23UL
#define TAG_AC           0x9F26UL
#define TAG_AC_INFO      0x9F27UL
#define TAG_ICC_PIN_CERT 0x9F2DUL
#define TAG_ICC_PIN_EXP  0x9F2EUL
#define TAG_ICC_PIN_REM  0x9F2FUL
#define TAG_ISS_PK_EXP   0x9F32UL
#define TAG_TERM_CAPS    0x9F33UL
#define TAG_CVM_RESULTS  0x9F34UL
#define TAG_TERM_TYPE    0x9F35UL
#define TAG_ATC          0x9F36UL
#define TAG_UNPRED_NUM   0x9F37UL
#define TAG_PDOL         0x9F38UL
#define TAG_POS_ENTRY    0x9F39UL
#define TAG_TERM_EX_CAPS 0x9F40UL
#define TAG_APP_CURRENCY 0x9F42UL
#define TAG_APP_EXP      0x9F44UL
#define TAG_DAC          0x9F45UL
#define TAG_ICC_PK_CERT  0x9F46UL
#define TAG_ICC_PK_EXP   0x9F47UL
#define TAG_ICC_PK_REM   0x9F48UL
#define TAG_DDOL         0x9F49UL
#define TAG_SDOL         0x9F4AUL
#define TAG_DYNAMIC_DATA 0x9F4BUL
#define TAG_ICC_DYN_NUM  0x9F4CUL
#define TAG_LOG_ENTRY    0x9F4DUL
#define TAG_LOG_FORMAT   0x9F4FUL
#define TAG_OFFLINE_BAL  0x9F50UL
#define TAG_DRDOL        0x9F51UL
#define TAG_CVR_RESULT   0x9F52UL
#define TAG_SCRIPT_RES   0x9F5BUL      /* 1 + 4 bytes */
#define TAG_APP_CAPS     0x9F5DUL      /* 3 */
#define TAG_DS_ID        0x9F5EUL      /* 3 */
#define TAG_CVC3         0x9F61UL      /* 2 */
#define TAG_PCVC3        0x9F65UL      /* 2 */
#define TAG_P_UN_ATC     0x9F66UL      /* 2 or 4 */
#define TAG_TTQ          0x9F66UL      /* 4 */
#define TAG_N_ATC        0x9F67UL
#define TAG_UDOL         0x9F69UL
#define TAG_PP_UN        0x9F6AUL
#define TAG_MAGSTRIPE2   0x9F6BUL
#define TAG_CTQ          0x9F6CUL
#define TAG_MS_VERSION   0x9F6DUL      /* 2 */
#define TAG_FORM_FACTOR  0x9F6EUL      /* 4 */
#define TAG_MOBILE_SUPP  0x9F7EUL      /* 1   was 9F55, but EMV Ctls part 2 says it must be 9F7E */
#define TAG_LC_DATA      0x9F7FUL 
#define TAG_ALL_DIR      0xBF0CUL 
#define TAG_POS_CV_INFO  0xDF4BUL

#define TAG_LOYALTY_KZHT 0x9F6EUL      /* 7 */

#endif


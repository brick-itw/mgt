#ifndef _MGT_SPECIAL_TAGS_H
#define _MGT_SPECIAL_TAGS_H

#define TAG_REQUEST_TYPE              0x01
  #define TRT_INITIAL_PURCH    0x01
  #define TRT_RECURRING_PURCH  0x02
  #define TRT_REFUND           0x03
  #define TRT_CANCEL           0x04
  #define TRT_ECHO_TEST        0x05
  #define TRT_REPLY            0x80

#define TAG_REQUEST_ID                0x02
#define TAG_AMOUNT                    0x03
#define TAG_CURRENCY                  0x04 
#define TAG_ENCRYPTED_CARD_DATA       0x05
#define TAG_RESULT_CODE               0x06
#define TAG_APPROVAL_CODE             0x07
#define TAG_RRN                       0x08
#define TAG_TERMINAL_ID               0x09
#define TAG_PAN                       0x0A
#define TAG_DETOKENIZED_PAN           0x0B
#define TAG_CARD_EXPIRY_DATE          0x0C

#endif


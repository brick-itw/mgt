#ifndef _U2_CONF_TAGS_H
#define _U2_CONF_TAGS_H

#define TAG_KEY_RID      0xDF01UL
#define TAG_KEY_MOD      0xDF02UL

#define TAG_CURR_LABEL   0xDF05UL

#define TAG_RND_THRESH   0xDF06UL
#define TAG_RND_TARGET   0xDF07UL
#define TAG_RND_MAX      0xDF08UL

#define TAG_STR_ID       0xDF0BUL          /* 4 */
#define TAG_BLOCK_TIME   0xDF0CUL          /* 4 bytes */
#define TAG_BLK_ID       0xDF0DUL          /* 1  byte */
#define TAG_STR_TEXT_RU  0xDF0EUL         
#define TAG_STR_TEXT_EN  0xDF5EUL         

#define TAG_CVM_LIMIT    0xDF8019UL  /* BCD 6 bytes */


#define TAG_CONTROL_TYPE   0xDF8041UL  /* 1 */             
  #define TCT_STATIC              0
  #define TCT_BUTTON              1
  #define TCT_EDITOR              2
  #define TCT_STSTUS              3
  #define TCT_CLOCK8              4
  #define TCT_CLOCK5              5
  #define TCT_BITMAP              6
  #define TCT_MENU                7
  #define TCT_ACTION              8

#define TAG_CONTROL_LEFT   0xDF8042UL  /* 2 */             
#define TAG_CONTROL_TOP    0xDF8043UL  /* 2 */             
#define TAG_CONTROL_RIGHT  0xDF8044UL  /* 2 */             
#define TAG_CONTROL_BOTTOM 0xDF8045UL  /* 2 */             
#define TAG_CONTROL_KEY    0xDF8046UL  /* 1 */             
#define TAG_CONTROL_TEXT   0xDF8047UL  /* str */
#define TAG_CONTROL_ACTION 0xDF8048UL  /* 2 */
#define TAG_SCREEN_NAME    0xDF8049UL  /* 4 */
#define TAG_CONTROL_TIMER  0xDF8050UL  /* 4 */
#define TAG_CONTROL_FLAGS  0xDF8051UL  /* 4 */
#define TAG_CONTROL_FONT   0xDF8052UL  /* str */


#define TAG_CONTROL_ALIGN  0xDF8053UL  /* 1 */
  #define TCA_LEFT                0
  #define TCA_RIGHT               1
  #define TCA_CENTER              2


#define TAG_APP_TEMPLATE  0xFF01UL
#define TAG_CUR_TEMPLATE  0xFF02UL
#define TAG_KEY_TEMPLATE  0xFF03UL
#define TAG_CON_TEMPLATE  0xFF04UL
#define TAG_STR_TEMPLATE  0xFF10UL
#define TAG_SCREEN        0xFF24UL
#define TAG_CONTROL       0xFF25UL


#define TAG_PIN_BLOCK     0xDF8803UL
#define TAG_MAC_SKEY_TMK  0xDF882AUL
#define TAG_PIN_SKEY_TMK  0xDF882BUL


#define TAG_TAG_TEMPLATE  0xFF8005UL
#define TAG_TAG_VALUE     0xDF9001UL
#define TAG_TAG_FORMAT    0xDF9002UL
#define TAG_TAG_MIN_LEN   0xDF9003UL
#define TAG_TAG_MAX_LEN   0xDF9004UL
#define TAG_TAG_SOURCE    0xDF9005UL

#define TAG_HOST_URL        0xDF900AUL
#define TAG_SELF_URL        0xDF900BUL
#define TAG_NETWORK_MASK    0xDF900CUL
#define TAG_NETWORK_GETAWAY 0xDF900DUL


#endif


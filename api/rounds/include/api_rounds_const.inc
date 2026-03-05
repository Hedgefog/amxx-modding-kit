#if defined _api_rounds_const_included
  #endinput
#endif
#define _api_rounds_const_included

#define API_ROUNDS_VERSION "2.1.0"

enum Round_CheckResult {
  Round_CheckResult_Continue,
  Round_CheckResult_Supercede
};

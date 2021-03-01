	.set noreorder

#include "pspimport.s"

#ifdef F_scePopsMan_0000
	IMPORT_START "scePopsMan",0x40090000
#endif
#ifdef F_scePopsMan_0001
	IMPORT_FUNC  "scePopsMan",0x0090B2C8,scePopsMan_0090B2C8
#endif
#ifdef F_scePopsMan_0002
	IMPORT_FUNC  "scePopsMan",0x03971322,scePopsMan_03971322
#endif
#ifdef F_scePopsMan_0003
	IMPORT_FUNC  "scePopsMan",0x04B14718,scePopsManEnd
#endif
#ifdef F_scePopsMan_0004
	IMPORT_FUNC  "scePopsMan",0x065C7F0B,scePopsManImposeGetParam
#endif
#ifdef F_scePopsMan_0005
	IMPORT_FUNC  "scePopsMan",0x29B3FB24,scePopsMan_29B3FB24
#endif
#ifdef F_scePopsMan_0006
	IMPORT_FUNC  "scePopsMan",0x2E18E4E9,scePopsMan_2E18E4E9
#endif
#ifdef F_scePopsMan_0007
	IMPORT_FUNC  "scePopsMan",0x53F9ABD2,scePopsMan_53F9ABD2
#endif
#ifdef F_scePopsMan_0008
	IMPORT_FUNC  "scePopsMan",0x56EEB969,scePopsMan_56EEB969
#endif
#ifdef F_scePopsMan_0009
	IMPORT_FUNC  "scePopsMan",0x6768B22F,scePopsMan_6768B22F
#endif
#ifdef F_scePopsMan_0010
	IMPORT_FUNC  "scePopsMan",0x875F4C05,scePopsMan_875F4C05
#endif
#ifdef F_scePopsMan_0011
	IMPORT_FUNC  "scePopsMan",0xB262B44F,scePopsManImposeChanges
#endif
#ifdef F_scePopsMan_0012
	IMPORT_FUNC  "scePopsMan",0xBF224726,scePopsManChkregGetPsCode
#endif
#ifdef F_scePopsMan_0013
	IMPORT_FUNC  "scePopsMan",0xEC94C50C,scePopsManImposeGetBatteryIconStatus
#endif
#ifdef F_scePopsMan_0014
	IMPORT_FUNC  "scePopsMan",0xEF8F686E,scePopsMan_EF8F686E
#endif
#ifdef F_scePopsMan_0015
	IMPORT_FUNC  "scePopsMan",0xF03F1227,scePopsManInit
#endif
#ifdef F_scePopsMan_0016
	IMPORT_FUNC  "scePopsMan",0xFC56480E,scePopsMan_FC56480E
#endif




extern int Dbg_Param;
#define TestAp_Printf(flag, msg...) ((Dbg_Param & flag)?printf(msg):flag)

#define TESTAP_DBG_USAGE	(1 << 0)
#define TESTAP_DBG_ERR		(1 << 1)
#define TESTAP_DBG_FLOW		(1 << 2)
#define TESTAP_DBG_FRAME	(1 << 3)
#define TESTAP_DBG_BW	    (1 << 4)

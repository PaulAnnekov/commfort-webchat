//---------------------------------------------------------------------------

#ifndef check_connH
#define check_connH
#include <ExtCtrls.hpp>
class TConnCheckTimer : public TObject
{
  public:
	TTimer *ConnCheckTimer;

	__fastcall TConnCheckTimer::TConnCheckTimer();
	__fastcall ~TConnCheckTimer(void);
	void __fastcall ConnCheckTimerOnTimer(TObject *Sender);
};
extern TConnCheckTimer *conn_check_timer;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef check_db_threadH
#define check_db_threadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "global.h"
//---------------------------------------------------------------------------
class TGetDBUpdateThread : public TThread
{
private:
	MYSQL_ROW row;
protected:
	void __fastcall Execute();
public:
	__fastcall TGetDBUpdateThread(bool CreateSuspended);
	void __fastcall GentleStop();
	bool check_new_mess();
	bool check_new_users();
	bool check_old_users();
	bool check_rows_limit();
	bool ping();
	volatile long stop;
	int d_file_pos;
	void terminate();
};
//---------------------------------------------------------------------------
#endif

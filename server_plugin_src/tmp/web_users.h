//---------------------------------------------------------------------------

#ifndef web_usersH
#define web_usersH
#include <vector>
#include <System.hpp>
//---------------------------------------------------------------------------
using namespace std;

struct USER {
	USER(String c_name);
	String name;
	String chnl;
	//String chnls[16];
};
//---------------------------------------------------------------------------

class WebUsers {
	private:
		vector<USER> users;
		vector<USER>::iterator search_user(String name);
	public:
		WebUsers();
		~WebUsers(void);

		void add_user(String name);
		//bool add_user_chnl(String name, String chnl);
		void del_user(String name);
		void change_user_chnl(String name, String chnl);
		//bool del_user_chnl(String name, String chnl);

		bool is_user_in_chnl(String name, String chnl);
		bool is_user_exists(String name);
		bool is_list_empty();

		void clear();
};
//---------------------------------------------------------------------------

#endif

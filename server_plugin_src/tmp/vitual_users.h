//---------------------------------------------------------------------------

#ifndef vitual_usersH
#define vitual_usersH
//---------------------------------------------------------------------------

void AddUser(String name, String ip);
VirtualUsers::iterator SearchUser(String name);
void SetUserAuthorizationStatus(String name, int status);
#endif

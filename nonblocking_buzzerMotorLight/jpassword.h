

class jpassword
{
  public:
    jpassword(char * password) : mMatch(false), mLen(strlen(password))
    {
      mPassword = new char[mLen+1];
      mUserEntry = new char[mLen+1];

      strlcpy(mPassword,password,mLen+1);
      erase();
    }
    ~jpassword() 
    {
      delete mPassword;
      delete mUserEntry;
    }

    void addchar(char c)
    {
      if (done())
        return;
      
      char s[2]={c,'\0'};
      strcat(mUserEntry,s);
    }

    bool correct() {return (0==strcmp(mPassword,mUserEntry));}
    bool done() {return strlen(mUserEntry)==mLen;}
    void erase() {mUserEntry[0]='\0';}

    char * userpassword() {return mUserEntry;}

  private:
    char * mUserEntry;
    char * mPassword;
    int mLen;
    bool mMatch;
};


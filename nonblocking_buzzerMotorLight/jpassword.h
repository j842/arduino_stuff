
char seps[] = " ,\t\n";
#define MAXPASSWORDS 5
#define MAXPASSLEN 20

class jpassword
{
  public:
    // space separated passwords.
    jpassword(char * passwords) : mMatch(0), mNumPasswords(0)
    {
      for (int i=0;i<MAXPASSWORDS;++i)
        mPasswords[i]=NULL;
      
      char * token;
      token = strtok(passwords, seps );
      while (token != NULL && mNumPasswords<MAXPASSWORDS)
      {
        mPasswords[mNumPasswords] = new char[strlen(token)+1];
        strlcpy(mPasswords[mNumPasswords],token,MAXPASSLEN-1);
        ++mNumPasswords;

        token = strtok( NULL, seps);
      }
      
      erase();
    }
    ~jpassword() 
    {
      for (int i=0;i<MAXPASSWORDS;++i)
        delete[] mPasswords[i];
    }

    void addchar(char c)
    {
      if (done())
        return;
      if (c=='#')
      {
        mDone = true;
        for (int i=0;i<MAXPASSWORDS;++i) 
            if (mPasswords[i]!=NULL) 
                if (0==strcmp(mPasswords[i],mUserEntry)) 
                  mMatch=i+1;
      }
      else
        {              
        char s[2]={c,'\0'};
        strcat(mUserEntry,s);
        }
    }
    

    int match() {return mMatch;}
    bool correct() {return mMatch>0;}
    bool done() {return mDone;}
    void erase() {mUserEntry[0]='\0'; mDone=false; mMatch=0;}
    const char * const userpassword() {return mUserEntry;}

  private:
    char mUserEntry[MAXPASSLEN];
    char * mPasswords[MAXPASSWORDS];
    int mNumPasswords;
    int mMatch;
    bool mDone;
};



// typedef enum 
// {   
//     kNone = 0,
//     kString = 1,
//     kBool = 2,
//     kInt = 3,
// } tmultitype;

// class multitype
// {
//     public:
//         multitype()

// };


typedef enum 
{
    kNone = 0,
    kMessage = 1,
    kInfo_SwitchInit = 2,    
    kInfo_SwitchChange = 3,    
} tbufbuf;

class bufbuf 
{
    public:
        bufbuf() 
        {
            mBytes[0]=kNone;
            _setLen(0);
            mBytes[2]=0;
        }

        tbufbuf _getType() {return (tbufbuf)(mBytes[0]);}
        void _setType(tbufbuf t) {mBytes[0]=t;}
        uint8_t _getLen() {return mBytes[1];}
        void _setLen(uint8_t l) {mBytes[1]=l;}
        const uint8_t * _getPayload() {return mBytes+2;}
        void _setPayload(const uint8_t * p, uint8_t l) 
        {
            if (l>kMaxLen-2) 
                l=kMaxLen-2;
            _setLen(l);
            memcpy(mBytes+2,p,l);
        }

        bufbuf(std::string s)
        {
            setMessage(s);
        } 

        uint8_t _set(tbufbuf type, uint8_t len, const uint8_t * Payload)
        {
            _setType(type);
            _setPayload(Payload,len);
            return _getLen();
        }

        bool _get(uint8_t & type, uint8_t & len, uint8_t * Payload, uint8_t maxlen)
        {
            type = _getType();
            len = _getLen();
            memcpy(Payload, _getPayload(), std::min(len,maxlen));
            return (maxlen>=len);
        }

        void setMessage(std::string s) 
        {
            _set(kMessage, s.length(), reinterpret_cast<const uint8_t *>(s.c_str()));
        }

        std::string getMessage()
        {
            if (_getType()!=kMessage) return "Error - not a message.";
            if (_getLen()==0) return "";
            char * s = new char[_getLen()+1];
            strncpy(s,reinterpret_cast<const char *>(_getPayload()),_getLen());
            s[_getLen()]=0;
            return s;
        }

        void setInfo_SwitchInit(bool switchOn)
        {
            _setType(kInfo_Switch);
            _setbool(switchOn);
        }
        bool getInfo_SwitchInit()
        {
           return _getbool();
        }

    private:
        static const uint8_t kMaxLen = 255;
        uint8_t mBytes[kMaxLen];
};
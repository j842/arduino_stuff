#ifndef __BUFBUF_H
#define __BUFBUF_H

#include <string>
#include <assert.h>

typedef enum 
{
    kNone = 0,
    kMessage,
    kCmd_Power, // server sets client's power state. bool for on/off.
    kStat_Power, // client reports current power state. bool for on/off.
    kReq_Power, // client requests power state from server. No payload.
    kCmd_Shutdown, // server requests client shuts down. No payload.
} tID;

typedef enum 
{   
    kNoPayload = 0,
    kString = 1,
    kBool = 2,
    kInt = 3,
} tPayloadType;

// Simple class to send and receive commands/info.
//
// [0] = type         (uint8_t)
// [1] = Payload type (uint8_t)
// [2] = Payload len  (uint8_t)
// [3,...] = Payload
class jbuf
{
    public:

        jbuf() {clear();}

        void clear()
        {
            setID(kNone);
            setPayloadType(kNoPayload);
            setPayloadLen(0);
        }
        
        void setString(tID bufID, std::string s)
        {
            setID(bufID);
            setPayloadType(kString);
            setPayloadLen((s.length()>kMaxPayloadLen) ? kMaxPayloadLen : static_cast<uint8_t>(s.length()));
            if (getPayloadLen()>0)
                memcpy(mBytes+kHeaderLen,reinterpret_cast<const uint8_t *>(s.c_str()), getPayloadLen());
        }
        std::string getString() const
        {
            if (getPayloadType()!=kString) return "Error - buffer payload is not a string.";
            if (getPayloadLen()==0) return "";

            char * s = new char[getPayloadLen()+1];
            strncpy(s,reinterpret_cast<const char *>(mBytes+kHeaderLen),getPayloadLen());
            s[getPayloadLen()]=0;
            return s;
        }

        void setBool(tID bufID, bool b)
        {
            setID(bufID);
            setPayloadType(kBool);
            setPayloadLen(1);
            mBytes[kHeaderLen] = (b ? 1 : 0);
        }
        bool getBool() const
        {
            assert(getPayloadType()==kBool);
            return mBytes[kHeaderLen];
        }

        void setInt(tID bufID, int i)
        {
            setID(bufID);
            setPayloadType(kInt);
            setPayloadLen(sizeof(int)); // normally 4 bytes.
            memcpy(mBytes+kHeaderLen,static_cast<uint8_t *>(static_cast<void*>(&i)),getPayloadLen());
        }
        int getInt() const
        {
            assert(getPayloadType()==kInt);
            assert(getPayloadLen()==sizeof(int)); // x-platform check.
            int i;
            memcpy(&i, &mBytes[kHeaderLen], sizeof(int));
            return i;
        }

        void setIDOnly(tID bufID)
        {
            setID(bufID);
            setPayloadType(kNoPayload);
            setPayloadLen(0);
        }

    public:
        // for udpbro class, not general use.
        uint8_t _getLen() const {return mLen;}
        void _setLen(uint8_t len) {mLen = len;}
        const uint8_t * _getBufR() const {return &(mBytes[0]);}
        uint8_t * _getBufW() {return &(mBytes[0]);} 
        uint8_t _getMaxLen() const {return kMaxLen;}

    public: 
        tID getID() const {return static_cast<tID>(mBytes[0]);}

    private:    
        void setID(tID t) {mBytes[0]=static_cast<uint8_t>(t);}

        tPayloadType getPayloadType() const {return static_cast<tPayloadType>(mBytes[1]);}
        void setPayloadType(tPayloadType t) {mBytes[1]=static_cast<uint8_t>(t);}
        
        uint8_t getPayloadLen() const {return mBytes[2];}
        void setPayloadLen(uint8_t l) 
        {
            assert(l<=kMaxPayloadLen); 
            mBytes[2]=l;
            _setLen(l+kHeaderLen);
        }

        static const uint8_t kMaxLen = 255;
        static const uint8_t kHeaderLen = 3;
        static const uint8_t kMaxPayloadLen = kMaxLen-kHeaderLen;
        uint8_t mBytes[kMaxLen];
        uint8_t mLen;
};

#endif
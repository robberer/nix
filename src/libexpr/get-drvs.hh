#ifndef __GET_DRVS_H
#define __GET_DRVS_H

#include "eval.hh"

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>


namespace nix {


struct MetaValue
{
    enum { tpNone, tpString, tpStrings, tpInt } type;
    string stringValue;
    Strings stringValues;
    int intValue;
};


typedef std::map<string, MetaValue> MetaInfo;


struct DrvInfo
{
private:
    string drvPath;
    string outPath;

    bool metaInfoRead;
    MetaInfo meta;

    bool failed; // set if we get an AssertionError
    
public:
    string name;
    string attrPath; /* path towards the derivation */
    string system;

    /* !!! make this private */
    Bindings * attrs;

    DrvInfo() : metaInfoRead(false), failed(false), attrs(0) { };

    string queryDrvPath(EvalState & state) const;
    string queryOutPath(EvalState & state) const;
    MetaInfo queryMetaInfo(EvalState & state) const;
    MetaValue queryMetaInfo(EvalState & state, const string & name) const;

    void setDrvPath(const string & s)
    {
        drvPath = s;
    }
    
    void setOutPath(const string & s)
    {
        outPath = s;
    }

    void setMetaInfo(const MetaInfo & meta);

    void setFailed() { failed = true; };
    bool hasFailed() { return failed; };
};


#if HAVE_BOEHMGC
typedef list<DrvInfo, traceable_allocator<DrvInfo> > DrvInfos;
#else
typedef list<DrvInfo> DrvInfos;
#endif


/* If value `v' denotes a derivation, store information about the
   derivation in `drv' and return true.  Otherwise, return false. */
bool getDerivation(EvalState & state, Value & v, DrvInfo & drv);

void getDerivations(EvalState & state, Value & v, const string & pathPrefix,
    Bindings & autoArgs, DrvInfos & drvs);

 
}


#endif /* !__GET_DRVS_H */

//
// Generated file, do not edit! Created by opp_msgc 4.3 from darknetmessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "darknetmessage_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("DarknetMessageType");
    if (!e) enums.getInstance()->add(e = new cEnum("DarknetMessageType"));
    e->insert(DM_UNKNOWN, "DM_UNKNOWN");
    e->insert(DM_REQUEST, "DM_REQUEST");
    e->insert(DM_RESPONSE, "DM_RESPONSE");
    e->insert(DM_CON_SYN, "DM_CON_SYN");
    e->insert(DM_CON_ACK, "DM_CON_ACK");
    e->insert(DM_FORWARD, "DM_FORWARD");
);

Register_Class(DarknetMessage);

DarknetMessage::DarknetMessage(const char *name, int kind) : cPacket(name,kind)
{
    this->destNodeID_var = 0;
    this->srcNodeID_var = 0;
    this->TTL_var = 32;
    this->type_var = 0;
    visitedNodes_arraysize = 0;
    this->visitedNodes_var = 0;
}

DarknetMessage::DarknetMessage(const DarknetMessage& other) : cPacket(other)
{
    visitedNodes_arraysize = 0;
    this->visitedNodes_var = 0;
    copy(other);
}

DarknetMessage::~DarknetMessage()
{
    delete [] visitedNodes_var;
}

DarknetMessage& DarknetMessage::operator=(const DarknetMessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void DarknetMessage::copy(const DarknetMessage& other)
{
    this->destNodeID_var = other.destNodeID_var;
    this->srcNodeID_var = other.srcNodeID_var;
    this->TTL_var = other.TTL_var;
    this->type_var = other.type_var;
    delete [] this->visitedNodes_var;
    this->visitedNodes_var = (other.visitedNodes_arraysize==0) ? NULL : new opp_string[other.visitedNodes_arraysize];
    visitedNodes_arraysize = other.visitedNodes_arraysize;
    for (unsigned int i=0; i<visitedNodes_arraysize; i++)
        this->visitedNodes_var[i] = other.visitedNodes_var[i];
}

void DarknetMessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->destNodeID_var);
    doPacking(b,this->srcNodeID_var);
    doPacking(b,this->TTL_var);
    doPacking(b,this->type_var);
    b->pack(visitedNodes_arraysize);
    doPacking(b,this->visitedNodes_var,visitedNodes_arraysize);
}

void DarknetMessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->destNodeID_var);
    doUnpacking(b,this->srcNodeID_var);
    doUnpacking(b,this->TTL_var);
    doUnpacking(b,this->type_var);
    delete [] this->visitedNodes_var;
    b->unpack(visitedNodes_arraysize);
    if (visitedNodes_arraysize==0) {
        this->visitedNodes_var = 0;
    } else {
        this->visitedNodes_var = new opp_string[visitedNodes_arraysize];
        doUnpacking(b,this->visitedNodes_var,visitedNodes_arraysize);
    }
}

const char * DarknetMessage::getDestNodeID() const
{
    return destNodeID_var.c_str();
}

void DarknetMessage::setDestNodeID(const char * destNodeID)
{
    this->destNodeID_var = destNodeID;
}

const char * DarknetMessage::getSrcNodeID() const
{
    return srcNodeID_var.c_str();
}

void DarknetMessage::setSrcNodeID(const char * srcNodeID)
{
    this->srcNodeID_var = srcNodeID;
}

int DarknetMessage::getTTL() const
{
    return TTL_var;
}

void DarknetMessage::setTTL(int TTL)
{
    this->TTL_var = TTL;
}

int DarknetMessage::getType() const
{
    return type_var;
}

void DarknetMessage::setType(int type)
{
    this->type_var = type;
}

void DarknetMessage::setVisitedNodesArraySize(unsigned int size)
{
    opp_string *visitedNodes_var2 = (size==0) ? NULL : new opp_string[size];
    unsigned int sz = visitedNodes_arraysize < size ? visitedNodes_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        visitedNodes_var2[i] = this->visitedNodes_var[i];
    for (unsigned int i=sz; i<size; i++)
        visitedNodes_var2[i] = 0;
    visitedNodes_arraysize = size;
    delete [] this->visitedNodes_var;
    this->visitedNodes_var = visitedNodes_var2;
}

unsigned int DarknetMessage::getVisitedNodesArraySize() const
{
    return visitedNodes_arraysize;
}

const char * DarknetMessage::getVisitedNodes(unsigned int k) const
{
    if (k>=visitedNodes_arraysize) throw cRuntimeError("Array of size %d indexed by %d", visitedNodes_arraysize, k);
    return visitedNodes_var[k].c_str();
}

void DarknetMessage::setVisitedNodes(unsigned int k, const char * visitedNodes)
{
    if (k>=visitedNodes_arraysize) throw cRuntimeError("Array of size %d indexed by %d", visitedNodes_arraysize, k);
    this->visitedNodes_var[k] = visitedNodes;
}

class DarknetMessageDescriptor : public cClassDescriptor
{
  public:
    DarknetMessageDescriptor();
    virtual ~DarknetMessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(DarknetMessageDescriptor);

DarknetMessageDescriptor::DarknetMessageDescriptor() : cClassDescriptor("DarknetMessage", "cPacket")
{
}

DarknetMessageDescriptor::~DarknetMessageDescriptor()
{
}

bool DarknetMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DarknetMessage *>(obj)!=NULL;
}

const char *DarknetMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DarknetMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int DarknetMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *DarknetMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destNodeID",
        "srcNodeID",
        "TTL",
        "type",
        "visitedNodes",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int DarknetMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destNodeID")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcNodeID")==0) return base+1;
    if (fieldName[0]=='T' && strcmp(fieldName, "TTL")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+3;
    if (fieldName[0]=='v' && strcmp(fieldName, "visitedNodes")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DarknetMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "int",
        "int",
        "string",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *DarknetMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 3:
            if (!strcmp(propertyname,"enum")) return "DarknetMessageType";
            return NULL;
        default: return NULL;
    }
}

int DarknetMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DarknetMessage *pp = (DarknetMessage *)object; (void)pp;
    switch (field) {
        case 4: return pp->getVisitedNodesArraySize();
        default: return 0;
    }
}

std::string DarknetMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DarknetMessage *pp = (DarknetMessage *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getDestNodeID());
        case 1: return oppstring2string(pp->getSrcNodeID());
        case 2: return long2string(pp->getTTL());
        case 3: return long2string(pp->getType());
        case 4: return oppstring2string(pp->getVisitedNodes(i));
        default: return "";
    }
}

bool DarknetMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DarknetMessage *pp = (DarknetMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestNodeID((value)); return true;
        case 1: pp->setSrcNodeID((value)); return true;
        case 2: pp->setTTL(string2long(value)); return true;
        case 3: pp->setType(string2long(value)); return true;
        case 4: pp->setVisitedNodes(i,(value)); return true;
        default: return false;
    }
}

const char *DarknetMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *DarknetMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DarknetMessage *pp = (DarknetMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}



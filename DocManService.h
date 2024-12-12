// DocManService.h
// Generated from file http://judith/DocMan/service.php?wsdl
//(c) 2008-2014 by CRESD GmbH, Munich, http://www.cresd.de

#ifndef SOAP_DocManService_H
#define SOAP_DocManService_H
#include <gak/soap.h>
class DocManService : public gak::net::SoapRequest {
public:
DocManService() : gak::net::SoapRequest("http://judith/DocMan/service.php?wsdl") {}
DocManService( const char *wsdlUrl ) : gak::net::SoapRequest(wsdlUrl) {}

typedef struct {
int id;
gak::STRING nachname;
gak::STRING vorname;
gak::STRING strasse;
gak::STRING postfach;
gak::STRING land;
gak::STRING plz;
gak::STRING ort;
gak::STRING email;
gak::STRING password;
gak::STRING guest;
gak::STRING loginEnabled;
gak::STRING administrator;
gak::STRING is_group;
}UserRecord_t;
void fillUserRecord( UserRecord_t &item, gak::xml::Element *xml );
void fillUserRecord( gak::Array<UserRecord_t> &item, gak::xml::Element *xml );

typedef struct {
UserRecord_t item;
}UserTable_t;
void fillUserTable( UserTable_t &item, gak::xml::Element *xml );
void fillUserTable( gak::Array<UserTable_t> &item, gak::xml::Element *xml );

typedef struct {
int id;
int parentID;
int item_type;
gak::STRING name;
gak::STRING description;
int ownerUser;
int ownerGroup;
int mode;
int createBy;
int createDate;
int modifiedDate;
int reservedBy;
}ItemRecord_t;
void fillItemRecord( ItemRecord_t &item, gak::xml::Element *xml );
void fillItemRecord( gak::Array<ItemRecord_t> &item, gak::xml::Element *xml );

typedef struct {
ItemRecord_t item;
}ItemContent_t;
void fillItemContent( ItemContent_t &item, gak::xml::Element *xml );
void fillItemContent( gak::Array<ItemContent_t> &item, gak::xml::Element *xml );

typedef struct {
int id;
int itemID;
int createBy;
gak::STRING mimeType;
gak::STRING description;
int createDate;
int modifiedDate;
}VersionRecord_t;
void fillVersionRecord( VersionRecord_t &item, gak::xml::Element *xml );
void fillVersionRecord( gak::Array<VersionRecord_t> &item, gak::xml::Element *xml );

typedef struct {
VersionRecord_t item;
}VersionTable_t;
void fillVersionTable( VersionTable_t &item, gak::xml::Element *xml );
void fillVersionTable( gak::Array<VersionTable_t> &item, gak::xml::Element *xml );

typedef struct {
}sGetUserTableRequestType_t;
void fillsGetUserTableRequestType( sGetUserTableRequestType_t &item, gak::xml::Element *xml );
void fillsGetUserTableRequestType( gak::Array<sGetUserTableRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
UserTable_t theUserTable;
}sGetUserTableResponseType_t;
void fillsGetUserTableResponseType( sGetUserTableResponseType_t &item, gak::xml::Element *xml );
void fillsGetUserTableResponseType( gak::Array<sGetUserTableResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
UserRecord_t theUser;
}sUpdateUserRequestType_t;
void fillsUpdateUserRequestType( sUpdateUserRequestType_t &item, gak::xml::Element *xml );
void fillsUpdateUserRequestType( gak::Array<sUpdateUserRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int theUserID;
}sUpdateUserResponseType_t;
void fillsUpdateUserResponseType( sUpdateUserResponseType_t &item, gak::xml::Element *xml );
void fillsUpdateUserResponseType( gak::Array<sUpdateUserResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int theUserID;
}sDeleteUserRequestType_t;
void fillsDeleteUserRequestType( sDeleteUserRequestType_t &item, gak::xml::Element *xml );
void fillsDeleteUserRequestType( gak::Array<sDeleteUserRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int theUserID;
}sDeleteUserResponseType_t;
void fillsDeleteUserResponseType( sDeleteUserResponseType_t &item, gak::xml::Element *xml );
void fillsDeleteUserResponseType( gak::Array<sDeleteUserResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
}sGetItemContentRequestType_t;
void fillsGetItemContentRequestType( sGetItemContentRequestType_t &item, gak::xml::Element *xml );
void fillsGetItemContentRequestType( gak::Array<sGetItemContentRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
ItemContent_t theItemContent;
}sGetItemContentResponseType_t;
void fillsGetItemContentResponseType( sGetItemContentResponseType_t &item, gak::xml::Element *xml );
void fillsGetItemContentResponseType( gak::Array<sGetItemContentResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
ItemRecord_t itemData;
}sUpdateItemRequestType_t;
void fillsUpdateItemRequestType( sUpdateItemRequestType_t &item, gak::xml::Element *xml );
void fillsUpdateItemRequestType( gak::Array<sUpdateItemRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int theItemID;
}sUpdateItemResponseType_t;
void fillsUpdateItemResponseType( sUpdateItemResponseType_t &item, gak::xml::Element *xml );
void fillsUpdateItemResponseType( gak::Array<sUpdateItemResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
}sDeleteItemRequestType_t;
void fillsDeleteItemRequestType( sDeleteItemRequestType_t &item, gak::xml::Element *xml );
void fillsDeleteItemRequestType( gak::Array<sDeleteItemRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int theItemID;
}sDeleteItemResponseType_t;
void fillsDeleteItemResponseType( sDeleteItemResponseType_t &item, gak::xml::Element *xml );
void fillsDeleteItemResponseType( gak::Array<sDeleteItemResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
gak::STRING mimeType;
gak::STRING description;
gak::STRING encodedData;
int modTime;
}sAddVersionRequestType_t;
void fillsAddVersionRequestType( sAddVersionRequestType_t &item, gak::xml::Element *xml );
void fillsAddVersionRequestType( gak::Array<sAddVersionRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int theVersionID;
}sAddVersionResponseType_t;
void fillsAddVersionResponseType( sAddVersionResponseType_t &item, gak::xml::Element *xml );
void fillsAddVersionResponseType( gak::Array<sAddVersionResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
int versionID;
gak::STRING mimeType;
gak::STRING description;
}sUpdateVersionRequestType_t;
void fillsUpdateVersionRequestType( sUpdateVersionRequestType_t &item, gak::xml::Element *xml );
void fillsUpdateVersionRequestType( gak::Array<sUpdateVersionRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int theVersionID;
}sUpdateVersionResponseType_t;
void fillsUpdateVersionResponseType( sUpdateVersionResponseType_t &item, gak::xml::Element *xml );
void fillsUpdateVersionResponseType( gak::Array<sUpdateVersionResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
int versionID;
}sDeleteVersionRequestType_t;
void fillsDeleteVersionRequestType( sDeleteVersionRequestType_t &item, gak::xml::Element *xml );
void fillsDeleteVersionRequestType( gak::Array<sDeleteVersionRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int theVersionID;
}sDeleteVersionResponseType_t;
void fillsDeleteVersionResponseType( sDeleteVersionResponseType_t &item, gak::xml::Element *xml );
void fillsDeleteVersionResponseType( gak::Array<sDeleteVersionResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
int versionID;
}sGetVersionsRequestType_t;
void fillsGetVersionsRequestType( sGetVersionsRequestType_t &item, gak::xml::Element *xml );
void fillsGetVersionsRequestType( gak::Array<sGetVersionsRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
VersionTable_t theItemVersions;
}sGetVersionsResponseType_t;
void fillsGetVersionsResponseType( sGetVersionsResponseType_t &item, gak::xml::Element *xml );
void fillsGetVersionsResponseType( gak::Array<sGetVersionsResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
}sCanReserveRequestType_t;
void fillsCanReserveRequestType( sCanReserveRequestType_t &item, gak::xml::Element *xml );
void fillsCanReserveRequestType( gak::Array<sCanReserveRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int canReserve;
}sCanReserveResponseType_t;
void fillsCanReserveResponseType( sCanReserveResponseType_t &item, gak::xml::Element *xml );
void fillsCanReserveResponseType( gak::Array<sCanReserveResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
}sCanUnreserveRequestType_t;
void fillsCanUnreserveRequestType( sCanUnreserveRequestType_t &item, gak::xml::Element *xml );
void fillsCanUnreserveRequestType( gak::Array<sCanUnreserveRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int canUnreserve;
}sCanUnreserveResponseType_t;
void fillsCanUnreserveResponseType( sCanUnreserveResponseType_t &item, gak::xml::Element *xml );
void fillsCanUnreserveResponseType( gak::Array<sCanUnreserveResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
}sReserveRequestType_t;
void fillsReserveRequestType( sReserveRequestType_t &item, gak::xml::Element *xml );
void fillsReserveRequestType( gak::Array<sReserveRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int reservedBy;
}sReserveResponseType_t;
void fillsReserveResponseType( sReserveResponseType_t &item, gak::xml::Element *xml );
void fillsReserveResponseType( gak::Array<sReserveResponseType_t> &item, gak::xml::Element *xml );

typedef struct {
int itemID;
}sUnreserveRequestType_t;
void fillsUnreserveRequestType( sUnreserveRequestType_t &item, gak::xml::Element *xml );
void fillsUnreserveRequestType( gak::Array<sUnreserveRequestType_t> &item, gak::xml::Element *xml );

typedef struct {
int error;
}sUnreserveResponseType_t;
void fillsUnreserveResponseType( sUnreserveResponseType_t &item, gak::xml::Element *xml );
void fillsUnreserveResponseType( gak::Array<sUnreserveResponseType_t> &item, gak::xml::Element *xml );

gak::xml::Element * sGetUserTable( );
gak::xml::Element * sGetUserTable( gak::xml::Element *theParameter );

int sUpdateUser( const UserRecord_t &theUser);
int sUpdateUser( gak::xml::Element *theParameter );

int sDeleteUser( int theUserID);
int sDeleteUser( gak::xml::Element *theParameter );

gak::xml::Element * sGetItemContent( int itemID);
gak::xml::Element * sGetItemContent( gak::xml::Element *theParameter );

int sUpdateItem( const ItemRecord_t &itemData);
int sUpdateItem( gak::xml::Element *theParameter );

int sDeleteItem( int itemID);
int sDeleteItem( gak::xml::Element *theParameter );

int sAddVersion( int itemID, const gak::STRING &mimeType, const gak::STRING &description, const gak::STRING &encodedData, int modTime);
int sAddVersion( gak::xml::Element *theParameter );

int sUpdateVersion( int itemID, int versionID, const gak::STRING &mimeType, const gak::STRING &description);
int sUpdateVersion( gak::xml::Element *theParameter );

int sDeleteVersion( int itemID, int versionID);
int sDeleteVersion( gak::xml::Element *theParameter );

gak::xml::Element * sGetVersions( int itemID, int versionID);
gak::xml::Element * sGetVersions( gak::xml::Element *theParameter );

int sCanReserve( int itemID);
int sCanReserve( gak::xml::Element *theParameter );

int sCanUnreserve( int itemID);
int sCanUnreserve( gak::xml::Element *theParameter );

int sReserve( int itemID);
int sReserve( gak::xml::Element *theParameter );

int sUnreserve( int itemID);
int sUnreserve( gak::xml::Element *theParameter );
};
#endif

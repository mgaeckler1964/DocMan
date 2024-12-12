// DocManService.cpp
// Generated from file http://judith/DocMan/service.php?wsdl
//(c) 2008-2014 by CRESD GmbH, Munich, http://www.cresd.de

#include <gak/numericString.h>
#include "DocManService.h"
void DocManService::fillUserRecord( UserRecord_t &item, gak::xml::Element *xml ) {
gak::xml::Element *id = xml->getElement( "id" );
if( id )
item.id = atoi( id->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *nachname = xml->getElement( "nachname" );
if( nachname )
item.nachname = nachname->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *vorname = xml->getElement( "vorname" );
if( vorname )
item.vorname = vorname->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *strasse = xml->getElement( "strasse" );
if( strasse )
item.strasse = strasse->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *postfach = xml->getElement( "postfach" );
if( postfach )
item.postfach = postfach->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *land = xml->getElement( "land" );
if( land )
item.land = land->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *plz = xml->getElement( "plz" );
if( plz )
item.plz = plz->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *ort = xml->getElement( "ort" );
if( ort )
item.ort = ort->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *email = xml->getElement( "email" );
if( email )
item.email = email->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *password = xml->getElement( "password" );
if( password )
item.password = password->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *guest = xml->getElement( "guest" );
if( guest )
item.guest = guest->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *loginEnabled = xml->getElement( "loginEnabled" );
if( loginEnabled )
item.loginEnabled = loginEnabled->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *administrator = xml->getElement( "administrator" );
if( administrator )
item.administrator = administrator->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *is_group = xml->getElement( "is_group" );
if( is_group )
item.is_group = is_group->getValue( gak::xml::PLAIN_MODE );
}
void DocManService::fillUserRecord( gak::Array<UserRecord_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillUserRecord( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillUserTable( UserTable_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillUserTable( gak::Array<UserTable_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillUserTable( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillItemRecord( ItemRecord_t &item, gak::xml::Element *xml ) {
gak::xml::Element *id = xml->getElement( "id" );
if( id )
item.id = atoi( id->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *parentID = xml->getElement( "parentID" );
if( parentID )
item.parentID = atoi( parentID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *item_type = xml->getElement( "item_type" );
if( item_type )
item.item_type = atoi( item_type->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *name = xml->getElement( "name" );
if( name )
item.name = name->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *description = xml->getElement( "description" );
if( description )
item.description = description->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *ownerUser = xml->getElement( "ownerUser" );
if( ownerUser )
item.ownerUser = atoi( ownerUser->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *ownerGroup = xml->getElement( "ownerGroup" );
if( ownerGroup )
item.ownerGroup = atoi( ownerGroup->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *mode = xml->getElement( "mode" );
if( mode )
item.mode = atoi( mode->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *createBy = xml->getElement( "createBy" );
if( createBy )
item.createBy = atoi( createBy->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *createDate = xml->getElement( "createDate" );
if( createDate )
item.createDate = atoi( createDate->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *modifiedDate = xml->getElement( "modifiedDate" );
if( modifiedDate )
item.modifiedDate = atoi( modifiedDate->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *reservedBy = xml->getElement( "reservedBy" );
if( reservedBy )
item.reservedBy = atoi( reservedBy->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillItemRecord( gak::Array<ItemRecord_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillItemRecord( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillItemContent( ItemContent_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillItemContent( gak::Array<ItemContent_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillItemContent( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillVersionRecord( VersionRecord_t &item, gak::xml::Element *xml ) {
gak::xml::Element *id = xml->getElement( "id" );
if( id )
item.id = atoi( id->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *createBy = xml->getElement( "createBy" );
if( createBy )
item.createBy = atoi( createBy->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *mimeType = xml->getElement( "mimeType" );
if( mimeType )
item.mimeType = mimeType->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *description = xml->getElement( "description" );
if( description )
item.description = description->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *createDate = xml->getElement( "createDate" );
if( createDate )
item.createDate = atoi( createDate->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *modifiedDate = xml->getElement( "modifiedDate" );
if( modifiedDate )
item.modifiedDate = atoi( modifiedDate->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillVersionRecord( gak::Array<VersionRecord_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillVersionRecord( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillVersionTable( VersionTable_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillVersionTable( gak::Array<VersionTable_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillVersionTable( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsGetUserTableRequestType( sGetUserTableRequestType_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillsGetUserTableRequestType( gak::Array<sGetUserTableRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsGetUserTableRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsGetUserTableResponseType( sGetUserTableResponseType_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillsGetUserTableResponseType( gak::Array<sGetUserTableResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsGetUserTableResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUpdateUserRequestType( sUpdateUserRequestType_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillsUpdateUserRequestType( gak::Array<sUpdateUserRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUpdateUserRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUpdateUserResponseType( sUpdateUserResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theUserID = xml->getElement( "theUserID" );
if( theUserID )
item.theUserID = atoi( theUserID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsUpdateUserResponseType( gak::Array<sUpdateUserResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUpdateUserResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsDeleteUserRequestType( sDeleteUserRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theUserID = xml->getElement( "theUserID" );
if( theUserID )
item.theUserID = atoi( theUserID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsDeleteUserRequestType( gak::Array<sDeleteUserRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsDeleteUserRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsDeleteUserResponseType( sDeleteUserResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theUserID = xml->getElement( "theUserID" );
if( theUserID )
item.theUserID = atoi( theUserID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsDeleteUserResponseType( gak::Array<sDeleteUserResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsDeleteUserResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsGetItemContentRequestType( sGetItemContentRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsGetItemContentRequestType( gak::Array<sGetItemContentRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsGetItemContentRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsGetItemContentResponseType( sGetItemContentResponseType_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillsGetItemContentResponseType( gak::Array<sGetItemContentResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsGetItemContentResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUpdateItemRequestType( sUpdateItemRequestType_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillsUpdateItemRequestType( gak::Array<sUpdateItemRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUpdateItemRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUpdateItemResponseType( sUpdateItemResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theItemID = xml->getElement( "theItemID" );
if( theItemID )
item.theItemID = atoi( theItemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsUpdateItemResponseType( gak::Array<sUpdateItemResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUpdateItemResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsDeleteItemRequestType( sDeleteItemRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsDeleteItemRequestType( gak::Array<sDeleteItemRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsDeleteItemRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsDeleteItemResponseType( sDeleteItemResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theItemID = xml->getElement( "theItemID" );
if( theItemID )
item.theItemID = atoi( theItemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsDeleteItemResponseType( gak::Array<sDeleteItemResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsDeleteItemResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsAddVersionRequestType( sAddVersionRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *mimeType = xml->getElement( "mimeType" );
if( mimeType )
item.mimeType = mimeType->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *description = xml->getElement( "description" );
if( description )
item.description = description->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *encodedData = xml->getElement( "encodedData" );
if( encodedData )
item.encodedData = encodedData->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *modTime = xml->getElement( "modTime" );
if( modTime )
item.modTime = atoi( modTime->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsAddVersionRequestType( gak::Array<sAddVersionRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsAddVersionRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsAddVersionResponseType( sAddVersionResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theVersionID = xml->getElement( "theVersionID" );
if( theVersionID )
item.theVersionID = atoi( theVersionID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsAddVersionResponseType( gak::Array<sAddVersionResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsAddVersionResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUpdateVersionRequestType( sUpdateVersionRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *versionID = xml->getElement( "versionID" );
if( versionID )
item.versionID = atoi( versionID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *mimeType = xml->getElement( "mimeType" );
if( mimeType )
item.mimeType = mimeType->getValue( gak::xml::PLAIN_MODE );
gak::xml::Element *description = xml->getElement( "description" );
if( description )
item.description = description->getValue( gak::xml::PLAIN_MODE );
}
void DocManService::fillsUpdateVersionRequestType( gak::Array<sUpdateVersionRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUpdateVersionRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUpdateVersionResponseType( sUpdateVersionResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theVersionID = xml->getElement( "theVersionID" );
if( theVersionID )
item.theVersionID = atoi( theVersionID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsUpdateVersionResponseType( gak::Array<sUpdateVersionResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUpdateVersionResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsDeleteVersionRequestType( sDeleteVersionRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *versionID = xml->getElement( "versionID" );
if( versionID )
item.versionID = atoi( versionID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsDeleteVersionRequestType( gak::Array<sDeleteVersionRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsDeleteVersionRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsDeleteVersionResponseType( sDeleteVersionResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *theVersionID = xml->getElement( "theVersionID" );
if( theVersionID )
item.theVersionID = atoi( theVersionID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsDeleteVersionResponseType( gak::Array<sDeleteVersionResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsDeleteVersionResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsGetVersionsRequestType( sGetVersionsRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
gak::xml::Element *versionID = xml->getElement( "versionID" );
if( versionID )
item.versionID = atoi( versionID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsGetVersionsRequestType( gak::Array<sGetVersionsRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsGetVersionsRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsGetVersionsResponseType( sGetVersionsResponseType_t &item, gak::xml::Element *xml ) {
}
void DocManService::fillsGetVersionsResponseType( gak::Array<sGetVersionsResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsGetVersionsResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsCanReserveRequestType( sCanReserveRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsCanReserveRequestType( gak::Array<sCanReserveRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsCanReserveRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsCanReserveResponseType( sCanReserveResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *canReserve = xml->getElement( "canReserve" );
if( canReserve )
item.canReserve = atoi( canReserve->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsCanReserveResponseType( gak::Array<sCanReserveResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsCanReserveResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsCanUnreserveRequestType( sCanUnreserveRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsCanUnreserveRequestType( gak::Array<sCanUnreserveRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsCanUnreserveRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsCanUnreserveResponseType( sCanUnreserveResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *canUnreserve = xml->getElement( "canUnreserve" );
if( canUnreserve )
item.canUnreserve = atoi( canUnreserve->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsCanUnreserveResponseType( gak::Array<sCanUnreserveResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsCanUnreserveResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsReserveRequestType( sReserveRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsReserveRequestType( gak::Array<sReserveRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsReserveRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsReserveResponseType( sReserveResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *reservedBy = xml->getElement( "reservedBy" );
if( reservedBy )
item.reservedBy = atoi( reservedBy->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsReserveResponseType( gak::Array<sReserveResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsReserveResponseType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUnreserveRequestType( sUnreserveRequestType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *itemID = xml->getElement( "itemID" );
if( itemID )
item.itemID = atoi( itemID->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsUnreserveRequestType( gak::Array<sUnreserveRequestType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUnreserveRequestType( itemArray.createElement(), xml->getElement( i ) );
} }
void DocManService::fillsUnreserveResponseType( sUnreserveResponseType_t &item, gak::xml::Element *xml ) {
gak::xml::Element *error = xml->getElement( "error" );
if( error )
item.error = atoi( error->getValue( gak::xml::PLAIN_MODE ) );
}
void DocManService::fillsUnreserveResponseType( gak::Array<sUnreserveResponseType_t> &itemArray, gak::xml::Element *xml ) {
for( size_t i=0; i<xml->getNumObjects(); i++ )
{
fillsUnreserveResponseType( itemArray.createElement(), xml->getElement( i ) );
} }

gak::xml::Element * DocManService::sGetUserTable( ) {
setOperation( "sGetUserTable" );
gak::xml::Element *response = execute();
if( response )
response = response->getElement(0);
return response;
}

gak::xml::Element * DocManService::sGetUserTable( gak::xml::Element *theParameter ) {
setOperation( "sGetUserTable" );
gak::xml::Element *response = execute( theParameter );
if( response )
response = response->getElement(0);
return response;
}

int DocManService::sUpdateUser( const UserRecord_t &theUser) {
setOperation( "sUpdateUser" );
setParameter( "theUser/id", theUser.id );
setParameter( "theUser/nachname", theUser.nachname );
setParameter( "theUser/vorname", theUser.vorname );
setParameter( "theUser/strasse", theUser.strasse );
setParameter( "theUser/postfach", theUser.postfach );
setParameter( "theUser/land", theUser.land );
setParameter( "theUser/plz", theUser.plz );
setParameter( "theUser/ort", theUser.ort );
setParameter( "theUser/email", theUser.email );
setParameter( "theUser/password", theUser.password );
setParameter( "theUser/guest", theUser.guest );
setParameter( "theUser/loginEnabled", theUser.loginEnabled );
setParameter( "theUser/administrator", theUser.administrator );
setParameter( "theUser/is_group", theUser.is_group );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sUpdateUser( gak::xml::Element *theParameter ) {
setOperation( "sUpdateUser" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sDeleteUser( int theUserID) {
setOperation( "sDeleteUser" );
setParameter( "theUserID", theUserID );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sDeleteUser( gak::xml::Element *theParameter ) {
setOperation( "sDeleteUser" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

gak::xml::Element * DocManService::sGetItemContent( int itemID) {
setOperation( "sGetItemContent" );
setParameter( "itemID", itemID );
gak::xml::Element *response = execute();
if( response )
response = response->getElement(0);
return response;
}

gak::xml::Element * DocManService::sGetItemContent( gak::xml::Element *theParameter ) {
setOperation( "sGetItemContent" );
gak::xml::Element *response = execute( theParameter );
if( response )
response = response->getElement(0);
return response;
}

int DocManService::sUpdateItem( const ItemRecord_t &itemData) {
setOperation( "sUpdateItem" );
setParameter( "itemData/id", itemData.id );
setParameter( "itemData/parentID", itemData.parentID );
setParameter( "itemData/item_type", itemData.item_type );
setParameter( "itemData/name", itemData.name );
setParameter( "itemData/description", itemData.description );
setParameter( "itemData/ownerUser", itemData.ownerUser );
setParameter( "itemData/ownerGroup", itemData.ownerGroup );
setParameter( "itemData/mode", itemData.mode );
setParameter( "itemData/createBy", itemData.createBy );
setParameter( "itemData/createDate", itemData.createDate );
setParameter( "itemData/modifiedDate", itemData.modifiedDate );
setParameter( "itemData/reservedBy", itemData.reservedBy );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sUpdateItem( gak::xml::Element *theParameter ) {
setOperation( "sUpdateItem" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sDeleteItem( int itemID) {
setOperation( "sDeleteItem" );
setParameter( "itemID", itemID );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sDeleteItem( gak::xml::Element *theParameter ) {
setOperation( "sDeleteItem" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sAddVersion( int itemID, const gak::STRING &mimeType, const gak::STRING &description, const gak::STRING &encodedData, int modTime) {
setOperation( "sAddVersion" );
setParameter( "itemID", itemID );
setParameter( "mimeType", mimeType );
setParameter( "description", description );
setParameter( "encodedData", encodedData );
setParameter( "modTime", modTime );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sAddVersion( gak::xml::Element *theParameter ) {
setOperation( "sAddVersion" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sUpdateVersion( int itemID, int versionID, const gak::STRING &mimeType, const gak::STRING &description) {
setOperation( "sUpdateVersion" );
setParameter( "itemID", itemID );
setParameter( "versionID", versionID );
setParameter( "mimeType", mimeType );
setParameter( "description", description );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sUpdateVersion( gak::xml::Element *theParameter ) {
setOperation( "sUpdateVersion" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sDeleteVersion( int itemID, int versionID) {
setOperation( "sDeleteVersion" );
setParameter( "itemID", itemID );
setParameter( "versionID", versionID );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sDeleteVersion( gak::xml::Element *theParameter ) {
setOperation( "sDeleteVersion" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

gak::xml::Element * DocManService::sGetVersions( int itemID, int versionID) {
setOperation( "sGetVersions" );
setParameter( "itemID", itemID );
setParameter( "versionID", versionID );
gak::xml::Element *response = execute();
if( response )
response = response->getElement(0);
return response;
}

gak::xml::Element * DocManService::sGetVersions( gak::xml::Element *theParameter ) {
setOperation( "sGetVersions" );
gak::xml::Element *response = execute( theParameter );
if( response )
response = response->getElement(0);
return response;
}

int DocManService::sCanReserve( int itemID) {
setOperation( "sCanReserve" );
setParameter( "itemID", itemID );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sCanReserve( gak::xml::Element *theParameter ) {
setOperation( "sCanReserve" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sCanUnreserve( int itemID) {
setOperation( "sCanUnreserve" );
setParameter( "itemID", itemID );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sCanUnreserve( gak::xml::Element *theParameter ) {
setOperation( "sCanUnreserve" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sReserve( int itemID) {
setOperation( "sReserve" );
setParameter( "itemID", itemID );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sReserve( gak::xml::Element *theParameter ) {
setOperation( "sReserve" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sUnreserve( int itemID) {
setOperation( "sUnreserve" );
setParameter( "itemID", itemID );
gak::STRING value;
gak::xml::Element *response = execute();
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

int DocManService::sUnreserve( gak::xml::Element *theParameter ) {
setOperation( "sUnreserve" );
gak::STRING value;
gak::xml::Element *response = execute( theParameter );
if( response ) {
response = response->getElement(0);
if( response )
value = response->getValue( gak::xml::PLAIN_MODE );
}
int iVal = value.getValueE<int>(); return iVal;
}

<?xml version="1.0" encoding="iso-8859-1" ?>
<setup>
<application title="Doc Manager" version="1.0.8.374" author="gak" />
<bde AliasName="DOCMAN" RESOURCE="0009" DataPath="" dbVersion="8" />
<icons Group="Doc Manager">
<icon title="Doc Manager" target="PROGRAM\DocMan.exe" main="true" dbUpd="" />
<icon title="BDE Administration" target="BDE\bdeadmin.exe" main="" dbUpd="" />
<icon title="DocMan DB Administration" target="PROGRAM\docManDB.exe" main="" dbUpd="true" />
<icon title="XML Editor" target="PROGRAM\xmlEdit.exe" main="" dbUpd="" />
<icon title="Manual (German)" target="PROGRAM\DocMan.pdf" main="" dbUpd="" />
<icon title="Data punp" target="BDE\datapump.exe" main="" dbUpd="" />
<icon title="Doc Man BG" target="PROGRAM\DocManBG.exe" main="" dbUpd="" />
</icons>
<source>
<group name="PROGRAM" destination="$ProgramFiles32">
<file source="..\..\Object\DocMan.exe" />
<file source="..\..\Object\docman.xsd" />
<file source="..\..\Object\xschema.xsd" />
<file source="..\..\Object\xslt.xsd" />
<file source="..\..\Object\docMandb.exe" />
<file source="..\..\Object\xmlEdit.exe" />
<file source="..\..\Object\midas.dll" />
<file source="reports" />
<file source="..\..\Object\srxViewer.DEU" />
<file source="..\..\Object\srxViewer.exe" />
<file source="..\..\Object\editor.css" />
<file source="..\..\Object\content.xsd" />
<file source="..\..\Object\libeay32.dll" />
<file source="..\..\Object\ssleay32.dll" />
<file source="DocMan.pdf" />
<file source="..\..\object\DocManBG.exe" />
</group>
</source>
</setup>
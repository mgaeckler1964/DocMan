<?xml version="1.0" encoding="iso-8859-1" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/TaskList">
<html>
<head>
<title>
<xsl:value-of select="@name" />
</title>
</head>
<body>
<h1>
<xsl:text>All Tasks: </xsl:text>
<xsl:value-of select="@name" />
</h1>
<table border="0" cellspacing="0" cellpadding="2">
<tr>
<th>Name</th>
<th>Status</th>
<th>Type</th>
<th>Module</th>
<th>Function</th>
<th>Release</th>
<th>Priority</th>
<th>Assigned To</th>
<th>Description</th>
</tr>
<xsl:for-each select="Task">
<tr style="border: 1px solid #000000; background-color:#EEEEEE;">
<td>
<xsl:value-of select="@name" />
</td>
<td>
<xsl:value-of select="@status" />
</td>
<td>
<xsl:value-of select="@type" />
</td>
<td>
<xsl:value-of select="@module" />
</td>
<td>
<xsl:value-of select="@badFunction" />
</td>
<td>
<xsl:value-of select="@release" />
</td>
<td>
<xsl:value-of select="@priority" />
</td>
<td>
<xsl:value-of select="@assignedTo" />
</td>
<td>
<xsl:value-of select="description" />
</td>
</tr>
<tr style="border: 1px solid #000000">
<td colspan="5">
<xsl:value-of select="internalRemarks" />
</td>
<td colspan="4">
<xsl:value-of select="externalRemarks" />
</td>
</tr>
</xsl:for-each>
</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
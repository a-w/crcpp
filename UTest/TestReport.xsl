<?xml version="1.0" encoding="utf-8"?>
<!--
 * $Id$
 * $Date$
 * 
 * Copyright (c) 2003 INTEC International GmbH, Hechingen, Germany
 * Author: Adrian Weiler
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method = "html" encoding="utf-8" />
	<xsl:strip-space elements="*"/>
	<xsl:param name="title" select="'Result of Unit Tests'"/>
	
	<xsl:template match="/TestRun">	
		<html>
			<head>
			<title>Results for <xsl:value-of select="$title"/></title>
			<style type="text/css"><![CDATA[ ]]>
BODY {
	font: x-small Verdana, Arial, Helvetica, sans-serif;
	background-color: #FFFFFF; }
	
h1, h2, h3, h4 {
	font-family: verdana, Arial, Helvetica, sans-serif;	
	font-weight: bold;
	color: #000000; }
	
h1 {
	font-size: 145%;
	margin-bottom: .5em; }
	
h2 {
	font-size: 125%;
	margin-top: 1.5em;
	margin-bottom: .5em; }
	
h3 {
	font-size: 115%;
	margin-top: 1.2em;
	margin-bottom: .5em}
	
h4 {
	font-size: 100%;
	margin-top: 1.2em;
	margin-bottom: .5em; }
			</style>
			</head>
			<body>
			<xsl:apply-templates select="Statistics"/>
			<xsl:if test="count(FailedTests/*) &gt; 0">
				<xsl:apply-templates select="FailedTests"/>
			</xsl:if>
			<xsl:if test="count(SucessfulTests/*) &gt; 0">
				<xsl:apply-templates select="SucessfulTests"/>
			</xsl:if>
			</body>
		</html>
	</xsl:template>
	
	<xsl:template match="Statistics">
		<h1>Summary of <xsl:value-of select="$title"/></h1>
		<table border="0">
		<xsl:apply-templates/>
		</table>
	</xsl:template>
	
	<xsl:template match="Tests">
	<tr><td>Total number of tests run:</td><td><xsl:value-of select="."/></td></tr>
	</xsl:template>

	<xsl:template match="FailuresTotal">
	<tr>
	  <td>
		<xsl:if test=". &gt; 0">
		  <xsl:attribute name="style">font-weight:bold; background-color:red;</xsl:attribute>
		</xsl:if>
	  Failed Tests:
	  </td>
	  <td>
		<xsl:if test=". &gt; 0">
		  <xsl:attribute name="style">font-weight:bold;</xsl:attribute>
		</xsl:if>
		<xsl:value-of select="."/>
	  </td>
	</tr>
	</xsl:template>

	<xsl:template match="FailedTests">
		<h1>Failed Tests</h1>
		<table border="1">
		<tr>
			<th>Module</th>
			<th>Test</th>
			<th>Type</th>
			<th>File</th>
			<th>Line</th>
			<th>Failed item</th>
		</tr>
			<xsl:apply-templates/>
		</table>
	</xsl:template>

	<xsl:template match="SucessfulTests">
		<h1>Successful Tests</h1>
		<table border="1">
		<tr>
			<th>Module</th>
			<th>Test</th>
		</tr>
			<xsl:apply-templates/>
		</table>
	</xsl:template>

	<xsl:template match="Test">
	  <tr>
	  	<xsl:apply-templates/>
	  </tr>
	</xsl:template>

	<xsl:template match="FailedTest">
	  <tr>
	  	<xsl:apply-templates/>
	  	<td><xsl:value-of select="FailureType"/></td>
	  	<td><xsl:choose>
	  	<xsl:when test="Location/File">
	  	<xsl:value-of select="Location/File"/>
	  	</xsl:when>
	  	<xsl:otherwise><xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text></xsl:otherwise>
	  	</xsl:choose></td>
	  	<td><xsl:choose>
	  	<xsl:when test="Location/Line">
	  	<xsl:value-of select="Location/Line"/>
	  	</xsl:when>
	  	<xsl:otherwise><xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text></xsl:otherwise>
	  	</xsl:choose></td>
	  	<td><xsl:value-of select="text()"/></td>
	  </tr>
	</xsl:template>

	<xsl:template match="Name">
	 	<td><xsl:value-of select="translate(substring-before(text(),'.'),'0123456789','')"/></td>
	 	<td><xsl:value-of select="substring-after(text(),'.')"/></td>
	</xsl:template>

	<xsl:template match="text()"/>
</xsl:stylesheet>
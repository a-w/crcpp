Dim oXMLDoc
Dim oXSLDoc
Dim oFS
Dim oHtmlFile
Dim oShell

'
' Try to use MXSML 4 if we have it
'
on error resume next
Set oXMLDoc = CreateObject ("MSXML2.DOMDocument.4.0")
if isempty(oXMLDoc) then  Set oXMLDoc = CreateObject ("MSXML2.DOMDocument")
Set oXSLDoc = CreateObject ("MSXML2.DOMDocument.4.0")
if isempty(oXSLDoc) then  Set oXSLDoc = CreateObject ("MSXML2.DOMDocument")
on error goto 0

Set iFS     = CreateObject ("Scripting.FileSystemObject")
Set oShell  = CreateObject ("WScript.Shell")

oXMLDoc.setProperty "SelectionLanguage", "XPath"
oXMLDoc.async = false
oXMLDoc.load "lasttest.xml"
oXSLDoc.async = false
oXSLDoc.load "TestReport.xsl"

Set oHtmlFile = iFs.CreateTextFile ("TestResult.html", true)

oHtmlFile.Write oXMLDoc.transformNode(oXSLDoc)
oHtmlFile.Close

oShell.Exec "Explorer TestResult.html"
'msgbox"Postprocessing done"
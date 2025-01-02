<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="html" indent="yes"/>

    <!-- 기본 템플릿: XML 노드를 HTML로 변환 -->
    <xsl:template match="/">
        <html>
            <head>
                <title>XML to HTML</title>
                <style>
                    body { font-family: Arial, sans-serif; margin: 20px; }
                    pre { background: #f4f4f4; padding: 10px; border-radius: 5px; }
                </style>
            </head>
            <body>
                <h1>XML Data</h1>
                <pre>
<xsl:copy-of select="."/>
                </pre>
            </body>
        </html>
    </xsl:template>
</xsl:stylesheet>

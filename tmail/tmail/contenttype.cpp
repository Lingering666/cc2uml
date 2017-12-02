#include "stdafx.h"

struct contenttype
{
	const wchar_t *ext, *value;
};

static struct contenttype static_contenttype[] = {
/* {NOTE:  All of these strings should never be translated
  because they are protocol specific and are important for some
  web-browsers
  } */
  // { Animation }
  {L".nml", L"animation/narrative"},    

  // { Audio }
  {L".aac", L"audio/mp4"},
  {L".aif", L"audio/x-aiff"},    
  {L".aifc", L"audio/x-aiff"},    
  {L".aiff", L"audio/x-aiff"},    

  {L".au", L"audio/basic"},    
  {L".gsm", L"audio/x-gsm"},    
  {L".kar", L"audio/midi"},    
  {L".m3u", L"audio/mpegurl"},    
  {L".m4a", L"audio/x-mpg"},    
  {L".mid", L"audio/midi"},    
  {L".midi", L"audio/midi"},    
  {L".mpega", L"audio/x-mpg"},    
  {L".mp2", L"audio/x-mpg"},    
  {L".mp3", L"audio/x-mpg"},    
  {L".mpga", L"audio/x-mpg"},    
  {L".m3u", L"audio/x-mpegurl"},    
  {L".pls", L"audio/x-scpls"},   
  {L".qcp", L"audio/vnd.qcelp"},    
  {L".ra", L"audio/x-realaudio"},    
  {L".ram", L"audio/x-pn-realaudio"},    
  {L".rm", L"audio/x-pn-realaudio"},    
  {L".sd2", L"audio/x-sd2"},    
  {L".sid", L"audio/prs.sid"},   
  {L".snd", L"audio/basic"},   
  {L".wav", L"audio/x-wav"},    
  {L".wax", L"audio/x-ms-wax"},    
  {L".wma", L"audio/x-ms-wma"},    

  {L".mjf", L"audio/x-vnd.AudioExplosion.MjuiceMediaFile"},    

  // { Image }
  {L".art", L"image/x-jg"},    
  {L".bmp", L"image/bmp"},    
  {L".cdr", L"image/x-coreldraw"},    
  {L".cdt", L"image/x-coreldrawtemplate"},    
  {L".cpt", L"image/x-corelphotopaint"},    
  {L".djv", L"image/vnd.djvu"},    
  {L".djvu", L"image/vnd.djvu"},    
  {L".gif", L"image/gif"},    
  {L".ief", L"image/ief"},    
  {L".ico", L"image/x-icon"},    
  {L".jng", L"image/x-jng"},    
  {L".jpg", L"image/jpeg"},    
  {L".jpeg", L"image/jpeg"},    
  {L".jpe", L"image/jpeg"},    
  {L".pat", L"image/x-coreldrawpattern"},   
  {L".pcx", L"image/pcx"},    
  {L".pbm", L"image/x-portable-bitmap"},    
  {L".pgm", L"image/x-portable-graymap"},    
  {L".pict", L"image/x-pict"},    
  {L".png", L"image/x-png"},    
  {L".pnm", L"image/x-portable-anymap"},    
  {L".pntg", L"image/x-macpaint"},    
  {L".ppm", L"image/x-portable-pixmap"},    
  {L".psd", L"image/x-psd"},    
  {L".qtif", L"image/x-quicktime"},    
  {L".ras", L"image/x-cmu-raster"},    
  {L".rf", L"image/vnd.rn-realflash"},    
  {L".rgb", L"image/x-rgb"},    
  {L".rp", L"image/vnd.rn-realpix"},    
  {L".sgi", L"image/x-sgi"},    
  {L".svg", L"image/svg-xml"},    
  {L".svgz", L"image/svg-xml"},    
  {L".targa", L"image/x-targa"},    
  {L".tif", L"image/x-tiff"},    
  {L".wbmp", L"image/vnd.wap.wbmp"},    
  {L".webp", L"image/webp"}, 
  {L".xbm", L"image/xbm"},    
  {L".xbm", L"image/x-xbitmap"},    
  {L".xpm", L"image/x-xpixmap"},    
  {L".xwd", L"image/x-xwindowdump"},    

  // { Text }
  {L".323", L"text/h323"},    

  {L".xml", L"text/xml"},    
  {L".uls", L"text/iuls"},    
  {L".txt", L"text/plain"},    
  {L".rtx", L"text/richtext"},    
  {L".wsc", L"text/scriptlet"},    
  {L".rt", L"text/vnd.rn-realtext"},    
  {L".htt", L"text/webviewhtml"},    
  {L".htc", L"text/x-component"},    
  {L".vcf", L"text/x-vcard"},    

  // { Video }
  {L".asf", L"video/x-ms-asf"},    
  {L".asx", L"video/x-ms-asf"},    
  {L".avi", L"video/x-msvideo"},    
  {L".dl", L"video/dl"},    
  {L".dv", L"video/dv"},  
  {L".flc", L"video/flc"},    
  {L".fli", L"video/fli"},    
  {L".gl", L"video/gl"},    
  {L".lsf", L"video/x-la-asf"},    
  {L".lsx", L"video/x-la-asf"},    
  {L".mng", L"video/x-mng"},    

  {L".mp2", L"video/mpeg"},    
  {L".mp3", L"video/mpeg"},    
  {L".mp4", L"video/mpeg"},    
  {L".mpeg", L"video/x-mpeg2a"},    
  {L".mpa", L"video/mpeg"},    
  {L".mpe", L"video/mpeg"},    
  {L".mpg", L"video/mpeg"},    
  {L".ogv", L"video/ogg"},    
  {L".moov", L"video/quicktime"},     
  {L".mov", L"video/quicktime"},    
  {L".mxu", L"video/vnd.mpegurl"},   
  {L".qt", L"video/quicktime"},    
  {L".qtc", L"video/x-qtc"}, 
  {L".rv", L"video/vnd.rn-realvideo"},    
  {L".ivf", L"video/x-ivf"},    
  {L".webm", L"video/webm"},    
  {L".wm", L"video/x-ms-wm"},    
  {L".wmp", L"video/x-ms-wmp"},    
  {L".wmv", L"video/x-ms-wmv"},    
  {L".wmx", L"video/x-ms-wmx"},    
  {L".wvx", L"video/x-ms-wvx"},    
  {L".rms", L"video/vnd.rn-realvideo-secure"},    
  {L".asx", L"video/x-ms-asf-plugin"},    
  {L".movie", L"video/x-sgi-movie"},    

  // { Application }
  {L".7z", L"application/x-7z-compressed"},   
  {L".a", L"application/x-archive"},   
  {L".aab", L"application/x-authorware-bin"},    
  {L".aam", L"application/x-authorware-map"},    
  {L".aas", L"application/x-authorware-seg"},    
  {L".abw", L"application/x-abiword"},    
  {L".ace", L"application/x-ace-compressed"},  
  {L".ai", L"application/postscript"},    
  {L".alz", L"application/x-alz-compressed"},    
  {L".ani", L"application/x-navi-animation"},   
  {L".arj", L"application/x-arj"},    
  {L".asf", L"application/vnd.ms-asf"},    
  {L".bat", L"application/x-msdos-program"},    
  {L".bcpio", L"application/x-bcpio"},    
  {L".boz", L"application/x-bzip2"},     
  {L".bz", L"application/x-bzip"},
  {L".bz2", L"application/x-bzip2"},    
  {L".cab", L"application/vnd.ms-cab-compressed"},    
  {L".cat", L"application/vnd.ms-pki.seccat"},    
  {L".ccn", L"application/x-cnc"},    
  {L".cco", L"application/x-cocoa"},    
  {L".cdf", L"application/x-cdf"},    
  {L".cer", L"application/x-x509-ca-cert"},    
  {L".chm", L"application/vnd.ms-htmlhelp"},    
  {L".chrt", L"application/vnd.kde.kchart"},    
  {L".cil", L"application/vnd.ms-artgalry"},    
  {L".class", L"application/java-vm"},    
  {L".com", L"application/x-msdos-program"},    
  {L".clp", L"application/x-msclip"},    
  {L".cpio", L"application/x-cpio"},    
  {L".cpt", L"application/mac-compactpro"},    
  {L".cqk", L"application/x-calquick"},    
  {L".crd", L"application/x-mscardfile"},    
  {L".crl", L"application/pkix-crl"},    
  {L".csh", L"application/x-csh"},    
  {L".dar", L"application/x-dar"},    
  {L".dbf", L"application/x-dbase"},    
  {L".dcr", L"application/x-director"},    
  {L".deb", L"application/x-debian-package"},    
  {L".dir", L"application/x-director"},    
  {L".dist", L"vnd.apple.installer+xml"},    
  {L".distz", L"vnd.apple.installer+xml"},    
  {L".dll", L"application/x-msdos-program"},    
  {L".dmg", L"application/x-apple-diskimage"},    
  {L".doc", L"application/msword"},    
  {L".dot", L"application/msword"},    
  {L".dvi", L"application/x-dvi"},    
  {L".dxr", L"application/x-director"},    
  {L".ebk", L"application/x-expandedbook"},    
  {L".eps", L"application/postscript"},    
  {L".evy", L"application/envoy"},    
  {L".exe", L"application/x-msdos-program"},    
  {L".fdf", L"application/vnd.fdf"},    
  {L".fif", L"application/fractals"},    
  {L".flm", L"application/vnd.kde.kivio"},    
  {L".fml", L"application/x-file-mirror-list"},    
  {L".gzip", L"application/x-gzip"},  
  {L".gnumeric", L"application/x-gnumeric"},    
  {L".gtar", L"application/x-gtar"},    
  {L".gz", L"application/x-gzip"},    
  {L".hdf", L"application/x-hdf"},    
  {L".hlp", L"application/winhlp"},    
  {L".hpf", L"application/x-icq-hpf"},    
  {L".hqx", L"application/mac-binhex40"},    
  {L".hta", L"application/hta"},    
  {L".ims", L"application/vnd.ms-ims"},    
  {L".ins", L"application/x-internet-signup"},    
  {L".iii", L"application/x-iphone"},    
  {L".iso", L"application/x-iso9660-image"},    
  {L".jar", L"application/java-archive"},    
  {L".karbon", L"application/vnd.kde.karbon"},    
  {L".kfo", L"application/vnd.kde.kformula"},    
  {L".kon", L"application/vnd.kde.kontour"},    
  {L".kpr", L"application/vnd.kde.kpresenter"},    
  {L".kpt", L"application/vnd.kde.kpresenter"},    
  {L".kwd", L"application/vnd.kde.kword"},    
  {L".kwt", L"application/vnd.kde.kword"},    
  {L".latex", L"application/x-latex"},    
  {L".lha", L"application/x-lzh"},    
  {L".lcc", L"application/fastman"},    
  {L".lrm", L"application/vnd.ms-lrm"},    
  {L".lz", L"application/x-lzip"},    
  {L".lzh", L"application/x-lzh"},    
  {L".lzma", L"application/x-lzma"},  
  {L".lzo", L"application/x-lzop"}, 
  {L".lzx", L"application/x-lzx"},
  {L".m13", L"application/x-msmediaview"},    
  {L".m14", L"application/x-msmediaview"},    
  {L".mpp", L"application/vnd.ms-project"},    
  {L".mvb", L"application/x-msmediaview"},    
  {L".man", L"application/x-troff-man"},    
  {L".mdb", L"application/x-msaccess"},    
  {L".me", L"application/x-troff-me"},    
  {L".ms", L"application/x-troff-ms"},    
  {L".msi", L"application/x-msi"},    
  {L".mpkg", L"vnd.apple.installer+xml"},    
  {L".mny", L"application/x-msmoney"},    
  {L".nix", L"application/x-mix-transfer"},    
  {L".o", L"application/x-object"},    
  {L".oda", L"application/oda"},    
  {L".odb", L"application/vnd.oasis.opendocument.database"},    
  {L".odc", L"application/vnd.oasis.opendocument.chart"},    
  {L".odf", L"application/vnd.oasis.opendocument.formula"},    
  {L".odg", L"application/vnd.oasis.opendocument.graphics"},    
  {L".odi", L"application/vnd.oasis.opendocument.image"},    
  {L".odm", L"application/vnd.oasis.opendocument.text-master"},    
  {L".odp", L"application/vnd.oasis.opendocument.presentation"},    
  {L".ods", L"application/vnd.oasis.opendocument.spreadsheet"},    
  {L".ogg", L"application/ogg"},    
  {L".odt", L"application/vnd.oasis.opendocument.text"},    
  {L".otg", L"application/vnd.oasis.opendocument.graphics-template"},    
  {L".oth", L"application/vnd.oasis.opendocument.text-web"},    
  {L".otp", L"application/vnd.oasis.opendocument.presentation-template"},    
  {L".ots", L"application/vnd.oasis.opendocument.spreadsheet-template"},    
  {L".ott", L"application/vnd.oasis.opendocument.text-template"},    
  {L".p10", L"application/pkcs10"},    
  {L".p12", L"application/x-pkcs12"},    
  {L".p7b", L"application/x-pkcs7-certificates"},    
  {L".p7m", L"application/pkcs7-mime"},    
  {L".p7r", L"application/x-pkcs7-certreqresp"},    
  {L".p7s", L"application/pkcs7-signature"},    
  {L".package", L"application/vnd.autopackage"},    
  {L".pfr", L"application/font-tdpfr"},    
  {L".pkg", L"vnd.apple.installer+xml"},    
  {L".pdf", L"application/pdf"},    
  {L".pko", L"application/vnd.ms-pki.pko"},    
  {L".pl", L"application/x-perl"},    
  {L".pnq", L"application/x-icq-pnq"},    
  {L".pot", L"application/mspowerpoint"},    
  {L".pps", L"application/mspowerpoint"},    
  {L".ppt", L"application/mspowerpoint"},    
  {L".ppz", L"application/mspowerpoint"},    
  {L".ps", L"application/postscript"},    
  {L".pub", L"application/x-mspublisher"},    
  {L".qpw", L"application/x-quattropro"},    
  {L".qtl", L"application/x-quicktimeplayer"},    
  {L".rar", L"application/rar"},    
  {L".rdf", L"application/rdf+xml"},    
  {L".rjs", L"application/vnd.rn-realsystem-rjs"},    
  {L".rm", L"application/vnd.rn-realmedia"},    
  {L".rmf", L"application/vnd.rmf"},    
  {L".rmp", L"application/vnd.rn-rn_music_package"},    
  {L".rmx", L"application/vnd.rn-realsystem-rmx"},    
  {L".rnx", L"application/vnd.rn-realplayer"},    
  {L".rpm", L"application/x-redhat-package-manager"},
  {L".rsml", L"application/vnd.rn-rsml"},    
  {L".rtsp", L"application/x-rtsp"},    
  {L".rss", L"application/rss+xml"},    
  {L".scm", L"application/x-icq-scm"},    
  {L".ser", L"application/java-serialized-object"},    
  {L".scd", L"application/x-msschedule"},    
  {L".sda", L"application/vnd.stardivision.draw"},    
  {L".sdc", L"application/vnd.stardivision.calc"},    
  {L".sdd", L"application/vnd.stardivision.impress"},    
  {L".sdp", L"application/x-sdp"},    
  {L".setpay", L"application/set-payment-initiation"},    
  {L".setreg", L"application/set-registration-initiation"},    
  {L".sh", L"application/x-sh"},    
  {L".shar", L"application/x-shar"},    
  {L".shw", L"application/presentations"},    
  {L".sit", L"application/x-stuffit"},    
  {L".sitx", L"application/x-stuffitx"},  
  {L".skd", L"application/x-koan"},    
  {L".skm", L"application/x-koan"},    
  {L".skp", L"application/x-koan"},    
  {L".skt", L"application/x-koan"},    
  {L".smf", L"application/vnd.stardivision.math"},    
  {L".smi", L"application/smil"},    
  {L".smil", L"application/smil"},    
  {L".spl", L"application/futuresplash"},    
  {L".ssm", L"application/streamingmedia"},    
  {L".sst", L"application/vnd.ms-pki.certstore"},    
  {L".stc", L"application/vnd.sun.xml.calc.template"},    
  {L".std", L"application/vnd.sun.xml.draw.template"},    
  {L".sti", L"application/vnd.sun.xml.impress.template"},    
  {L".stl", L"application/vnd.ms-pki.stl"},    
  {L".stw", L"application/vnd.sun.xml.writer.template"},    
  {L".svi", L"application/softvision"},    
  {L".sv4cpio", L"application/x-sv4cpio"},    
  {L".sv4crc", L"application/x-sv4crc"},    
  {L".swf", L"application/x-shockwave-flash"},    
  {L".swf1", L"application/x-shockwave-flash"},    
  {L".sxc", L"application/vnd.sun.xml.calc"},    
  {L".sxi", L"application/vnd.sun.xml.impress"},    
  {L".sxm", L"application/vnd.sun.xml.math"},    
  {L".sxw", L"application/vnd.sun.xml.writer"},    
  {L".sxg", L"application/vnd.sun.xml.writer.global"},    
  {L".t", L"application/x-troff"},    
  {L".tar", L"application/x-tar"},    
  {L".tcl", L"application/x-tcl"},    
  {L".tex", L"application/x-tex"},    
  {L".texi", L"application/x-texinfo"},    
  {L".texinfo", L"application/x-texinfo"},    
  {L".tbz", L"application/x-bzip-compressed-tar"},   
  {L".tbz2", L"application/x-bzip-compressed-tar"},   
  {L".tgz", L"application/x-compressed-tar"},    
  {L".tlz", L"application/x-lzma-compressed-tar"},    
  {L".tr", L"application/x-troff"},    
  {L".trm", L"application/x-msterminal"},    
  {L".troff", L"application/x-troff"},    
  {L".tsp", L"application/dsptype"},    
  {L".torrent", L"application/x-bittorrent"},    
  {L".ttz", L"application/t-time"},    
  {L".txz", L"application/x-xz-compressed-tar"}, 
  {L".udeb", L"application/x-debian-package"},    

  {L".uin", L"application/x-icq"},    
  {L".urls", L"application/x-url-list"},    
  {L".ustar", L"application/x-ustar"},    
  {L".vcd", L"application/x-cdlink"},    
  {L".vor", L"application/vnd.stardivision.writer"},    
  {L".vsl", L"application/x-cnet-vsl"},    
  {L".wcm", L"application/vnd.ms-works"},    
  {L".wb1", L"application/x-quattropro"},    
  {L".wb2", L"application/x-quattropro"},    
  {L".wb3", L"application/x-quattropro"},    
  {L".wdb", L"application/vnd.ms-works"},    
  {L".wks", L"application/vnd.ms-works"},    
  {L".wmd", L"application/x-ms-wmd"},    
  {L".wms", L"application/x-ms-wms"},    
  {L".wmz", L"application/x-ms-wmz"},    
  {L".wp5", L"application/wordperfect5.1"},    
  {L".wpd", L"application/wordperfect"},    
  {L".wpl", L"application/vnd.ms-wpl"},    
  {L".wps", L"application/vnd.ms-works"},    
  {L".wri", L"application/x-mswrite"},    
  {L".xfdf", L"application/vnd.adobe.xfdf"},    
  {L".xls", L"application/x-msexcel"},    
  {L".xlb", L"application/x-msexcel"},     
  {L".xpi", L"application/x-xpinstall"},    
  {L".xps", L"application/vnd.ms-xpsdocument"},    
  {L".xsd", L"application/vnd.sun.xml.draw"},    
  {L".xul", L"application/vnd.mozilla.xul+xml"},    
  {L".z", L"application/x-compress"},    
  {L".zoo", L"application/x-zoo"},    
  {L".zip", L"application/x-zip-compressed"},    
    
  // { WAP }
  {L".wbmp", L"image/vnd.wap.wbmp"},    
  {L".wml", L"text/vnd.wap.wml"},    
  {L".wmlc", L"application/vnd.wap.wmlc"},    
  {L".wmls", L"text/vnd.wap.wmlscript"},    
  {L".wmlsc", L"application/vnd.wap.wmlscriptc"},    

  // { Non-web text}
  /* {
  IMPORTANT!!

  You should not use a text MIME type definition unless you are
  extremely certain that the file will NOT be a binary.  Some browsers
  will display the text instead of saving to disk and it looks ugly
  if a web-browser shows all of the 8bit charactors.
  } */
  //of course, we have to add this :-).
  {L".asm", L"text/x-asm"},   
  {L".p", L"text/x-pascal"},    
  {L".pas", L"text/x-pascal"},    

  {L".cs", L"text/x-csharp"}, 

  {L".c", L"text/x-csrc"},    
  {L".c++", L"text/x-c++src"},    
  {L".cpp", L"text/x-c++src"},    
  {L".cxx", L"text/x-c++src"},    
  {L".cc", L"text/x-c++src"},    
  {L".h", L"text/x-chdr"}, 
  {L".h++", L"text/x-c++hdr"},    
  {L".hpp", L"text/x-c++hdr"},    
  {L".hxx", L"text/x-c++hdr"},    
  {L".hh", L"text/x-c++hdr"},    
  {L".java", L"text/x-java"},    

  // { WEB }
  {L".css", L"text/css"},    
  {L".js", L"text/javascript"},    
  {L".htm", L"text/html"},    
  {L".html", L"text/html"},    
  {L".xhtml", L"application/xhtml+xml"}, 
  {L".xht", L"application/xhtml+xml"}, 
  {L".rdf", L"application/rdf+xml"}, 
  {L".rss", L"application/rss+xml"}, 

  {L".ls", L"text/javascript"},    
  {L".mocha", L"text/javascript"},    
  {L".shtml", L"server-parsed-html"},    
  {L".xml", L"text/xml"},    
  {L".sgm", L"text/sgml"},    
  {L".sgml", L"text/sgml"},    

  // { Message }
  {L".mht", L"message/rfc822"}
};

enum {CTINITSIZE = 512, CTINCSIZE = 512};

static struct contenttype *dynamic_contenttype;
static int unsigned dcntotal, dcnmax;

static int addcontenttype(const wchar_t *ext, const wchar_t *value);

static int extcompare(const void *p1, const void *p2);

int initcontenttype(void)
{
	HKEY extkey;
	wchar_t key[] = L"\\";
	
	qsort(static_contenttype, sizeof(static_contenttype) / sizeof(static_contenttype[0]), sizeof(static_contenttype[0]), extcompare);

	if(RegOpenKeyEx(HKEY_CLASSES_ROOT, key, 0U, KEY_ENUMERATE_SUB_KEYS | KEY_READ, &extkey) != ERROR_SUCCESS)
		return -1;
	
	long int ret;
	wchar_t subkey[_MAX_FNAME];
	long unsigned int subkeysize_wchars, index;
	index = 0LU;
	subkeysize_wchars = sizeof(subkey) / sizeof(subkey[0]);
	wmemset(subkey, L'\0', subkeysize_wchars);
	while((ret = RegEnumKeyEx(extkey, index++,
		subkey, &subkeysize_wchars, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)	{
			if(subkey[0] != L'.')
				goto resetsize;

			HKEY hsubkey;
		    wchar_t ctvaluekey[_MAX_PATH], ctvalue[_MAX_PATH];
			unsigned long int ctvaluebytes;
			
			memset(ctvaluekey, 0, sizeof(ctvaluekey));
			memset(ctvalue, 0, ctvaluebytes = sizeof(ctvalue));

			_snwprintf(ctvaluekey, sizeof(ctvaluekey) / sizeof(ctvaluekey[0]), L"%s%s", key, subkey);
			if(RegOpenKeyEx(HKEY_CLASSES_ROOT, ctvaluekey, 0U, KEY_ENUMERATE_SUB_KEYS | KEY_READ, &hsubkey) != ERROR_SUCCESS)
				goto resetsize;
			if(RegQueryValueEx(hsubkey, _T("Content Type"), NULL, NULL, (unsigned char *)ctvalue, &ctvaluebytes) == ERROR_SUCCESS)
				addcontenttype(subkey, ctvalue);
			
			RegCloseKey(hsubkey);
resetsize:
			subkeysize_wchars = sizeof(subkey) / sizeof(subkey[0]);
	}
	if(ret != ERROR_NO_MORE_ITEMS) {
		wprintf(L"%lu\n", ret);
	}
	RegCloseKey(extkey);
	if(dynamic_contenttype)
		qsort(dynamic_contenttype, dcntotal, sizeof(struct contenttype), extcompare);
	return 0;
}

void destroycontenttype(void)
{
	while(dcntotal-- >= 1) {
		free((void *)dynamic_contenttype[dcntotal].ext);
		free((void *)dynamic_contenttype[dcntotal].value);
	}
	free(dynamic_contenttype);
	dynamic_contenttype = NULL;
	dcntotal = dcnmax = 0u;
}

const wchar_t *findcontenttype(const wchar_t *ext)
{
	const struct contenttype *ct;
	struct contenttype key;
	wchar_t *lwext;
	const wchar_t *res = NULL;

	lwext = _wcsdup(ext);
	key.ext = _wcslwr(lwext);
	key.value = NULL;
	if(dynamic_contenttype 
		&& (ct = (const struct contenttype *)bsearch(&key, dynamic_contenttype, dcntotal, 
		sizeof(struct contenttype), extcompare)) != NULL)
		res = ct->value;
	else {
	    ct = (const struct contenttype *)bsearch(&key, static_contenttype, sizeof(static_contenttype) / sizeof(static_contenttype[0]),
		    sizeof(static_contenttype[0]), extcompare);
		if(ct)
			res = ct->value;
	}
	free(lwext);
	return res;
}

static int extcompare(const void *p1, const void *p2)
{
	const struct contenttype *c1 = (const struct contenttype *)p1;
	const struct contenttype *c2 = (const struct contenttype *)p2;

	return wcscmp(c1->ext, c2->ext);
}

static int addcontenttype(const wchar_t *ext, const wchar_t *value)
{
	if(dynamic_contenttype == NULL) {
		if((dynamic_contenttype = (struct contenttype *)malloc(CTINITSIZE * sizeof(struct contenttype))) == NULL)
			return -1;
		dcnmax = CTINITSIZE;
		dcntotal = 0u;
	}
	if(dcntotal == dcnmax) { // ย๚มห
		struct contenttype *tmp;
		if((tmp = (struct contenttype *)realloc(dynamic_contenttype, dcnmax + CTINCSIZE)) == NULL)
			return -1;
		dcnmax += CTINCSIZE;
		dynamic_contenttype = tmp;
	}
	dynamic_contenttype[dcntotal].ext = _wcslwr(_wcsdup(ext));
	dynamic_contenttype[dcntotal].value = _wcsdup(value);
	dcntotal++;
	return 0;
}


// tmail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//{Generate a string 63 characters long (63 is a whim, not a requirement)...}
enum {BOUNDARYSIZE = 64, RFCMAXLINELENGTH = 76};

enum {INITSIZE = 512, INCSIZE = 512};

struct mail
{
	const unsigned char *__restrict from; 
	const unsigned char *__restrict to;
	const unsigned char *__restrict smtpserver;
	const unsigned char *__restrict username;
	const unsigned char *__restrict password;
	unsigned char * __restrict message;
	int unsigned leninbytes, sizeinbytes;
	unsigned char boundary[BOUNDARYSIZE];
};

struct mail_user_data
{
	const struct mail *mail;
	int unsigned bytesdone;
};

/* 这是一组测试代码 */
static int csum(int first, ...);
static void a(int i, int j, int k, int l);
static void wchartm(void);
static void chartm(void);

static unsigned int localeasctime(unsigned char *timestr, int unsigned sizebytes);
static int generateboundary(unsigned char *boundary, int sizebytes);
static int toquotedprintable(struct mail * __restrict mail, const unsigned char *__restrict src);
static int unsigned tobase64(unsigned char *dest, unsigned int destsizebytes, const unsigned char *src, unsigned int srcbytes);
static int unsigned debase64(unsigned char *dest, unsigned int destsizebytes, const unsigned char *src, unsigned int srcbytes);

static int initmail(struct mail *mail);
static void destroymail(struct mail *mail);
static int unsigned catmail(struct mail * __restrict mail, const unsigned char * __restrict format, ...);
static int unsigned catmailattachment(struct mail * __restrict mail, const wchar_t *attachmentfile);
// 返回实际字节数
static int unsigned tomailbase64w(unsigned char * __restrict dest, unsigned int destsizebytes, 
	const wchar_t *src, unsigned int srcwchars);
static int unsigned tomailbase64(unsigned char * __restrict dest, unsigned int destsizebytes, 
	const unsigned char *src, unsigned int srcbytes);
/* size 表示包含末尾 '\0' */
static int  incmailsize(struct mail * __restrict mail, unsigned int incsizebytes);
static int printmail(const struct mail *mail);
static int unsigned callbackmail(void *ptr, int unsigned size, int unsigned nmemb, void *userp);
static int sendmail(const struct mail * __restrict mail);

/* 以 NULL 表示可变参数的结束 */
static int makemail(struct mail *__restrict mail,
	const unsigned char *__restrict from, 
	const unsigned char *__restrict to, 
	const unsigned char *__restrict subject, 
	const unsigned char *__restrict smtpserver,
	const unsigned char *__restrict username,
	const unsigned char *__restrict password,
	const unsigned char *__restrict content, ...);

// 一下这两条content在本程序中的 quoted printable 编码和在 XE6 update1 中的不同
//(const unsigned char *)"TIdMessageBuilderPlain控件编辑人员这是为了使用户需求而开发的，嘻嘻！！未来在哪儿？未来",
//(const unsigned char *)"TIdMessageBuilderPlain控件编辑人员这是为了使用户需求而开发的，嘻嘻！！abcdefghijklmnopqrstuvwsyzabcdefghijklmnopqrstuv",

int _tmain(int argc, _TCHAR* argv[])
{
	struct mail mail;
	_wsetlocale(LC_ALL, L"chs_china.936");

	initcontenttype();
	initmail(&mail);
	makemail(&mail, 
		(const unsigned char *)"iamtomboy@163.com", 
		(const unsigned char *)"rubble@126.com", 
		(const unsigned char *)"中文标题", 
		(const unsigned char *)"smtp.163.com",
		(const unsigned char *)"iamtomboy",
		(const unsigned char *)"wangxixuan",
		//(const unsigned char *)"TIdMessageBuilderPlain控件编辑人员这是为了使用户需求而开发的，嘻嘻！！未来在哪儿？未来",
		(const unsigned char *)"TIdMessageBuilderPlain控件编辑人员这是为了使用户需求而开发的，嘻嘻！！abcdefghijklmnopqrstuvwsyzabcdefghijklmnopqrstuv",
		L"D:\\msys\\changelog.txt", L"D:\\照片.bmp", L"D:\\3.txt", NULL);
	printmail(&mail);
	sendmail(&mail);
	destroymail(&mail);
	unsigned char b[25], c[128] = "iamtomboy@163.com";
	tobase64(b, 25, c, strlen((const char *)c));
	wprintf(L"%S\n", b);
	memset(c, 0, sizeof(c));
	debase64(c, 128u, b, 24);
	wprintf(L"%S\n", c);
	wprintf(L".acrobatsecuritysettings : %s\n", findcontenttype(L".acrobatsecuritysettings"));
	wprintf(L".tar.gz : %s\n", findcontenttype(L".gz"));
	wprintf(L".png : %s\n", findcontenttype(L".png"));
	wprintf(L".txt : %s\n", findcontenttype(L".txt"));
	destroycontenttype();
	return 0;
}

static int initmail(struct mail *mail)
{
	int res = -1;

    memset(mail, 0, sizeof(struct mail));
	if((mail->message = (unsigned char *)malloc(INITSIZE)) != NULL) {
		res = 0;
		memset(mail->message, 0, mail->sizeinbytes = INITSIZE);
	}
	srand((unsigned int)time(NULL));
	return res;
}

static void destroymail(struct mail *mail)
{
	free(mail->message);
	memset(mail, 0, sizeof(struct mail));
}

static int unsigned catmail(struct mail * __restrict mail, const unsigned char * __restrict format, ...)
{
	int unsigned bytes, size;

	if(format == NULL)
		return 0;

	va_list arg_ptr;
	va_start(arg_ptr, format);
	bytes = _vscprintf((const char *)format, arg_ptr);
	va_end(arg_ptr);

	if(bytes == 0u) // 尽早返回
		return 0u;

	// +1 是为了给 '\0' 留出位置
	size = mail->leninbytes + bytes + 1;

	if(size > mail->sizeinbytes) {
		unsigned char *tmp;
		unsigned int alloc_size;

		alloc_size = size + INCSIZE;

		if((tmp = (unsigned char *)realloc(mail->message, alloc_size)) == NULL)
			return 0u;

		mail->message = tmp;
		mail->sizeinbytes = alloc_size;
		memset(mail->message + mail->leninbytes, 0, alloc_size - mail->leninbytes);
	}

	va_start(arg_ptr, format);
	mail->leninbytes += (bytes = _vsnprintf((char *)mail->message + mail->leninbytes,
		mail->sizeinbytes - mail->leninbytes - 1u, (const char *)format, arg_ptr));
	va_end(arg_ptr);
	return bytes;
}

static int unsigned catmailattachment(struct mail * __restrict mail, const wchar_t *attachmentfile)
{
	struct __stat64 st;
	FILE *fp;
	const wchar_t *filename, *ext, *ct;
	unsigned char *src, *base64dest, base64filename[_MAX_FNAME << 3];
	unsigned int srcbytes, destsizebytes, nblock;

	if(_wstat64(attachmentfile, &st) == -1 || (fp = _wfopen(attachmentfile, L"rb")) == NULL)
		return 0u;
	// 64-bit integer-suffix: i64 LL ll
	if((src = (unsigned char *)malloc((unsigned int)st.st_size + 1u)) == NULL) {
		fclose(fp);
		return 0u;
	}

	destsizebytes = (((unsigned int)st.st_size - 1u) / 3u + 1u) * 4u + 1u;
	if((base64dest = (unsigned char *)malloc(destsizebytes)) == NULL) {
		free(src);
		fclose(fp);
		return 0u;
	}
	if((srcbytes = fread(src, 1U, (unsigned int)st.st_size, fp)) == 0u) {
		free(src);
		fclose(fp);
		return 0u;
	}
	tobase64(base64dest, destsizebytes, src, srcbytes);
	free(src);
	if((filename = wcsrchr(attachmentfile, L'\\')) == NULL)
		filename = attachmentfile;
	else
		filename++;
	tomailbase64w(base64filename, sizeof(base64filename), filename, wcslen(filename));
	if((ext = wcsrchr(attachmentfile, L'.')) == NULL)
		ext = L".txt";
	if((ct = findcontenttype(ext)) == NULL)
		ct = findcontenttype(L".txt");

	/* 不知道为什么 Indy 10 发的邮件中，name 和 filename 都另起一行，而且以.开头，我比着它的样子做，导致邮件的附件名称不能正常显示。 */
	catmail(mail, (const unsigned char *)"--%s\r\nContent-Type: %S;\r\n\tname=\"%s\"\r\n"
	"Content-Transfer-Encoding: base64\r\n"
	"Content-Disposition: attachment;\r\n\tfilename=\"%s\"\r\n\r\n",
	mail->boundary, ct, base64filename, base64filename);
		
	nblock = (destsizebytes - 1u) / RFCMAXLINELENGTH + 1u;
	for(unsigned int i = 0u; i < nblock; i++)
		catmail(mail, (const unsigned char *)"%.*s\r\n", RFCMAXLINELENGTH, base64dest + i * RFCMAXLINELENGTH);
	catmail(mail, (const unsigned char *)"\r\n");
	free(base64dest);	
	return destsizebytes - 1u;
}

/* destsizebytes 至少为 ((srcwchars * 2 - 1) / 3 + 1) * 4 + 1 + strlen("=?GB18030?B??=") 	返回实际字节数 */
static int unsigned tomailbase64w(unsigned char * __restrict dest, unsigned int destsizebytes, 
	const wchar_t *src, unsigned int srcwchars)
{
	unsigned int srcbytes;
	unsigned char *buf, *p;

	srcbytes = wcstombs(NULL, src, 0u);
	if((buf = (unsigned char *)malloc(srcbytes + 1u)) == NULL)
		return 0;

	wcstombs((char *)buf, src, srcbytes + 1u);
	p = buf;
	while(*p) {
		if(_ismbblead(*p)) {
			unsigned int realbytes = _snprintf((char *)dest, destsizebytes - 1, "=?GB18030?B?");
			realbytes += tobase64(dest + realbytes, destsizebytes - realbytes, buf, srcbytes);
			free(buf);
			dest[realbytes++] = '?';
			dest[realbytes++] = '=';
			dest[realbytes] = '\0';
			return realbytes;
		}
		p++;
	}

	strcpy((char *)dest, (const char *)buf);
	free(buf);
	return srcbytes;
}

/* destsizebytes 至少为 ((srcbytes - 1) / 3 + 1) * 4 + 1 + strlen("=?GB18030?B??=") 	返回实际字节数 */
static int unsigned tomailbase64(unsigned char * __restrict dest, unsigned int destsizebytes, 
	const unsigned char *src, unsigned int srcbytes)
{
	const unsigned char *p;
	
	p = src;
	while(*p) {
		if(_ismbblead(*p)) {
			unsigned int realbytes = _snprintf((char *)dest, destsizebytes - 1, "=?GB18030?B?");
			realbytes += tobase64(dest + realbytes, destsizebytes - realbytes, src, srcbytes);
			dest[realbytes++] = '?';
			dest[realbytes++] = '=';
			dest[realbytes] = '\0';
			return realbytes;
		}
		p++;
	}
	strcpy((char *)dest, (const char *)src);
	return srcbytes;
}

static int  incmailsize(struct mail * __restrict mail, unsigned int incsize)
{
	unsigned int size;

	size = mail->leninbytes + incsize;

	if(size > mail->sizeinbytes) {
		unsigned char *tmp;
	
		if((tmp = (unsigned char *)realloc(mail->message, size)) == NULL)
			return -1;

		mail->message = tmp;
		mail->sizeinbytes = size;
		memset(mail->message + mail->leninbytes, 0, size - mail->leninbytes);
	}
	return 0;
}

static int printmail(const struct mail *mail)
{
	FILE *fp;
	int res = 0;
	if((fp = _wfopen(L"D:\\dump.txt", L"wb")) == NULL)
		return -1;
	res = fprintf(fp, "dump of a mail:\r\n\t容量：%u\r\n\t字节：%u\r\n\t长度：%u\r\n\t内容：\r\n",
		mail->sizeinbytes, mail->leninbytes, _mbslen(mail->message));
	fwrite(mail->message, 1u, mail->leninbytes, fp);
	fclose(fp);
	return res;
}

static int unsigned callbackmail(void *ptr, int unsigned size, int unsigned nmemb, void *userp)
{
	if((size == 0u) || (nmemb == 0u) || ((size * nmemb) < 1U))
		return 0U;
	struct mail_user_data *mud = (struct mail_user_data *)userp;
	unsigned int bytesleft = mud->mail->leninbytes - mud->bytesdone;
	unsigned int shouldcopybytes;

	shouldcopybytes = size * nmemb >= bytesleft ? bytesleft : size * nmemb;
	memcpy(ptr, mud->mail->message + mud->bytesdone, shouldcopybytes);
	mud->bytesdone += shouldcopybytes;
	return shouldcopybytes;
}

static int sendmail(const struct mail * __restrict mail)
{
	CURL *curl;
	CURLcode res = CURLE_OK;

	if((curl = curl_easy_init()) == NULL)
		return -1;
	
	struct mail_user_data mud = {mail, 0U};

	curl_easy_setopt(curl, CURLOPT_URL, (const char *)mail->smtpserver);
	curl_easy_setopt(curl, CURLOPT_MAIL_FROM, (const char *)mail->from);
	struct curl_slist *recipients = NULL;
	recipients = curl_slist_append(recipients, (const char *)mail->to);
	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
	curl_easy_setopt(curl, CURLOPT_USERNAME, (const char *)mail->username);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, (const char *)mail->password);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, callbackmail);
    curl_easy_setopt(curl, CURLOPT_READDATA, &mud);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    /* Send the message */ 
    res = curl_easy_perform(curl);
	const char *errmsg = curl_easy_strerror(res);
	curl_slist_free_all(recipients);
	curl_easy_cleanup(curl);

	return 0;
}

static int makemail(struct mail *__restrict mail,
	const unsigned char *__restrict from, 
	const unsigned char *__restrict to, 
	const unsigned char *__restrict subject, 
	const unsigned char *__restrict smtpserver,
	const unsigned char *__restrict username,
	const unsigned char *__restrict password,
	const unsigned char *__restrict content, ...)
{
	va_list arg_ptr;
	const wchar_t *attachmentfile;

	if(mail == NULL)
		return -1;

	mail->from = from;
	mail->to = to;
	mail->smtpserver = smtpserver;
	mail->username = username;
	mail->password = password;

	/* from */
	catmail(mail, (const unsigned char *)"From: \"%s\" <%s>\r\n", from, from);
	/* subject */
	unsigned char *base64subject;
	unsigned int base64subjectsizebytes, subjectbytes;
	subjectbytes = strlen((char *)subject);
	base64subjectsizebytes = (((subjectbytes - 1u) / 3u) + 1u) * 4u + 1u + 14u;
	if((base64subject = (unsigned char *)malloc(base64subjectsizebytes)) == NULL)
		return -1;
	tomailbase64(base64subject, base64subjectsizebytes, subject, subjectbytes);
	catmail(mail, (const unsigned char *)"Subject: %s\r\n", base64subject);
	free(base64subject);
	/* to */
	catmail(mail, (const unsigned char *)"To: %s\r\n", to);
	generateboundary(mail->boundary, sizeof(mail->boundary));
	/* content-type & boundary */
	catmail(mail, (const unsigned char *)"Content-Type: multipart/mixed; boundary=\"%s\"\r\n", mail->boundary);
	/* date */
	unsigned char now[32];
	localeasctime(now, 32);
	catmail(mail, (const unsigned char *)"MIME-Version: 1.0\r\nDate: %s\r\n", now);
    /* something contained in Indy10 SMTP message */
	catmail(mail, (const unsigned char *)"\r\nThis is a multi-part message in MIME format\r\n\r\n"
		"--%s\r\nContent-Type: text/plain; charset=\"GB18030\"\r\n"
		"Content-Transfer-Encoding: quoted-printable\r\n"
		"Content-Disposition: inline\r\n\r\n", mail->boundary);

	toquotedprintable(mail, content);
	catmail(mail, (const unsigned char *)"\r\n\r\n");

	va_start(arg_ptr, content);
	while((attachmentfile = va_arg(arg_ptr, const wchar_t *)) != NULL)
		catmailattachment(mail, attachmentfile);
	va_end(arg_ptr);

	catmail(mail, (const unsigned char *)"--%s--\r\n\r\n", mail->boundary);

	va_start(arg_ptr, content);
	while((attachmentfile = va_arg(arg_ptr, const wchar_t *)) != NULL)
		wprintf(L"%s\n", attachmentfile);
	va_end(arg_ptr);
	return 0;
}

static int csum(int first, ...)
{
	va_list arg_ptr;
	int count = 0, sum = 0, i = first;

	va_start(arg_ptr, first);

	while(i != -1) {
		sum += i;
		count++;
		i = va_arg(arg_ptr, int);
	}
	va_end(arg_ptr);
	return sum;
}

static void a(int i, int j, int k, int l)
{
	wprintf(L"%p %p %p %p\n", &i, &j, &k, &l);
}

static void wchartm(void)
{
     __int64 ltime;
    wchar_t tmp[26], buf[26];

	wmemset(tmp, L'\0', sizeof(tmp) / sizeof(tmp[0]));
	wmemset(buf, L'\0', sizeof(buf) / sizeof(buf[0]));
    _time64(&ltime);
	_wctime_s(tmp, sizeof(tmp) / sizeof(tmp[0]), &ltime);

	_snwprintf_s(buf, sizeof(buf) / sizeof(buf[0]) - 1, L"%.3s,%.*s", tmp, 21, &tmp[3]);
    wprintf(L"%s", buf);
}

static void chartm(void)
{
     __int64 ltime;
    char tmp[26], buf[26];

	memset(tmp, 0, sizeof(tmp));
	memset(buf, 0, sizeof(buf));
    _time64(&ltime);
	ctime_s(tmp, sizeof(tmp), &ltime);
	
	_snprintf(buf, sizeof(buf) - 1, "%.3s,%.*s", tmp, 21, &tmp[3]);
    wprintf(L"%S", buf);
}

static unsigned int localeasctime(unsigned char *timestr, int unsigned sizebytes)
{

    //  Sat, 2 Aug 2014 02:45:39 +0800
	if(sizebytes < 32)
		return -1;

	memset(timestr, 0, sizebytes);
	
	__int64 ltime;
	_time64(&ltime);

	struct tm tm;
	_localtime64_s(&tm, &ltime);

	int unsigned realbytes;
    _locale_t lc = _create_locale(LC_ALL, "C");
	realbytes = _strftime_l((char *)timestr, sizebytes, "%a, %#d %b %Y %X +0800", &tm, lc);
	_free_locale(lc);
	return realbytes;
}

#if 0
void RangedRandDemo( int range_min, int range_max, int n )
{
   // Generate random numbers in the half-closed interval
   // [range_min, range_max). In other words,
   // range_min <= random number < range_max
   int i;
   for ( i = 0; i < n; i++ )
   {
      int u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)
            + range_min;
      printf( "  %6d\n", u);
   }
}
#endif

static int generateboundary(unsigned char *boundary, int sizebytes)
{
	if(sizebytes < BOUNDARYSIZE)
		return -1;
	unsigned char randch;
	unsigned int i;

    // {Allow only digits (ASCII 48-57), upper-case letters (65-90) and lowercase letters (97-122), which is 62 possible chars...}
	for(i = 0u; i < BOUNDARYSIZE - 1; i++) {
		randch = rand() % 62 + 1; // [1, 62]
		randch += 47;             // [48, 109]
		if(randch > 83)
			randch += 13;         // {Move into lowercase letter range}
		else if(randch > 57)
			randch += 7;          // {Move into upper-case letter range}
		boundary[i] = randch;
	}
	boundary[i] = '\0';
	//CC2: RFC 2045 recommends including "=_" in the boundary, insert in random location...
	i = rand() % (BOUNDARYSIZE - 2);
	boundary[i] = '=';
	boundary[i + 1] = '_';
	return BOUNDARYSIZE - 1u;
}

//_itow((int)(*psrc & 0xFF), b, 16);
static int toquotedprintable(struct mail * __restrict mail, const unsigned char *__restrict src)
{
	const unsigned char * __restrict psrc;
	unsigned int linebytes = 0;

	// Ref: http://en.wikipedia.org/wiki/Quoted-printable 
	// Lines of Quoted-Printable encoded data must not be longer than 76 characters.
	psrc = src;
	linebytes = 0u;
	while(*psrc) {
		if(_ismbblead((unsigned int)(*psrc & 0xFF))) {
			if(linebytes + 6u >= RFCMAXLINELENGTH) {
				linebytes = 0u;
				catmail(mail, (const unsigned char *)"=\r\n");
			}
			linebytes += catmail(mail, (const unsigned char *)"=%02hX", *psrc & 0xFF);
			if(*++psrc == '\0')
				break;
            linebytes += catmail(mail, (const unsigned char *)"=%02hX", *psrc & 0xFF);
			psrc++;
		}
		else {
			if(linebytes + 1u >= RFCMAXLINELENGTH) {
				linebytes = 0u;
				catmail(mail, (const unsigned char *)"=\r\n");
			}
			if(*psrc == '\r' || *psrc == '\n' || *psrc == '=')
                linebytes += catmail(mail, (const unsigned char *)"=%02hX", *psrc & 0xFF);
			else
			    linebytes += catmail(mail, (const unsigned char *)"%hc", *psrc);
			psrc++;
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------*
函数名字           tobase64 - 加密函数
函数原型           int unsigned tobase64(unsigned char *dest, unsigned int destsizebytes, const unsigned char *src, unsigned int srcbytes)
函数所在头文件      crypt.h
函数的详细描述      src：等待被加密的数据；
                  dest：存放加密后的数据；
				  srcbytes：被加密数据的字节数，也就是长度；
				  destsizebytes：dest指向的缓冲区的长度，根据Base64编码的特点，
				            destsizebytes = ((src - 1) / 3 + 1) * 4 + 1; 函数检查
			     	这个关系，如果 destsizebytes 的值小于 ((src - 1) / 3 + 1) * 4 + 1，则返回0。
函数返回值         成功返回实际长度，失败返回 0。
*--------------------------------------------------------------------------*/
static int unsigned tobase64(unsigned char *dest, unsigned int destsizebytes, const unsigned char *src, unsigned int srcbytes)
{
	const unsigned char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//const char EnBase64Tab[] = "#$abcdefghijklmNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMnopqrstuvwxyz";
    unsigned char c1, c2, c3; /* 三个字节 */
    int n_div, n_mod, i;
	unsigned char *pdest;

    if(src == NULL || dest == NULL || ((srcbytes - 1u) / 3u + 1u) * 4u + 1u > destsizebytes)
		return 0;

    n_div = srcbytes / 3;
	n_mod = srcbytes % 3;

	pdest = dest;

    for(i = 0; i < n_div && pdest - dest + 1u <= destsizebytes; i++) {
	     c1 = *src++;
		 c2 = *src++;
		 c3 = *src++;
		 
		 *pdest++ = EnBase64Tab[c1 >> 2];
		 *pdest++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3F];
		 *pdest++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3F];
		 *pdest++ = EnBase64Tab[c3 & 0x3F];
	}

	if(pdest - dest + 1u > destsizebytes) {
		*pdest = '\0';
		return 0;
	}

	if(n_mod == 1) { /* 对余下的一个字节编码 */
	    c1 = *src++;
        *pdest++ = EnBase64Tab[c1 >> 2];
		*pdest++ = EnBase64Tab[(c1 & 0x03) << 4];
		*pdest++ = '=';
		*pdest++ = '=';
 	} else if(n_mod == 2) { /* 对余下的二个字节编码 */
       c1 = *src++;
	   c2 = *src++;
	   *pdest++ = EnBase64Tab[c1 >> 2];
	   *pdest++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3F];
	   *pdest++ = EnBase64Tab[(c2 << 2) & 0x3F];
	   *pdest++ = '=';
	}

	if(pdest - dest + 1u > destsizebytes) {
		*pdest = '\0';
		return 0;
	}

	*pdest = '\0';
	return pdest - dest;
} /* tobase64 */

static int unsigned debase64(unsigned char *dest, unsigned int destsizebytes, const unsigned char *src, unsigned int srcbytes)
{
	const unsigned char DeBase64Tab[] = {-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
		                                 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	                                     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
										 -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
										 -1,  -1,  -1,  62,  -1,  -1,  -1,  63,  52,  53,
										 54,  55,  56,  57,  58,  59,  60,  61,  -1,  -1,
										 -1,   0,  -1,  -1,  -1,   0,   1,   2,   3,   4,
										  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
										 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
										 25,  -1,  -1,  -1,  -1,  -1,  -1,  26,  27,  28,
										 29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
										 29,  40,  41,  42,  43,  44,  45,  46,  47,  48,
										 49,  50,  51
	}; 

	unsigned char p1, p2, p3, p4, *pdest;
	unsigned int div, i;

	if(src == NULL || dest == NULL || srcbytes % 4 != 0u || destsizebytes < (srcbytes >> 2u) * 3u + 1u)
		return 0;

	div = srcbytes >> 2;
	pdest = dest;
	for(i = 0u; i < div; i++) {
		p1 = DeBase64Tab[*src++];
		p2 = DeBase64Tab[*src++];
		p3 = DeBase64Tab[*src++];
		p4 = DeBase64Tab[*src++];

		*pdest++ = (p1 << 2) | (p2 >> 4);
		*pdest++ = (p2 << 4) | (p3 >> 2);
		*pdest++ = ((p3 & 0x03) << 6) | p4;
	}
	*pdest++ = '\0';
	return pdest - dest;
}


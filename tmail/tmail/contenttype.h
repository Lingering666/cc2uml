#ifndef __CONTENTTYPE_H__
#define __CONTENTTYPE_H__

#ifdef __cplusplus
extern "C" {
#endif
	void destroycontenttype(void);
	int initcontenttype(void);
	const wchar_t *findcontenttype(const wchar_t *ext);
#ifdef __cplusplus
}
#endif

#endif
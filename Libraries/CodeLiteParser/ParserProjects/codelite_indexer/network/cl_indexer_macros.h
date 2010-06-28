#ifndef __INDEXER_MACROS_H__
#define __INDEXER_MACROS_H__

#define PACK_INT(p, i) {\
	memcpy((void*)p, (void*)&i, sizeof(i));\
	p += sizeof(i);\
}

#define UNPACK_INT(i, p){\
	memcpy((void*)&i, p, sizeof(i));\
	p += sizeof(i);\
}

#define PACK_STD_STRING(p, s) {\
	size_t l = s.length();\
	memcpy((void*)p, (void*)&l, sizeof(l));\
	p += sizeof(l);\
	if(!s.empty()){\
		memcpy((void*)p, (void*)s.c_str(), l);\
		p += l;\
	}\
}

#define UNPACK_STD_STRING(s, p){\
	size_t len(0);\
	memcpy((void*)&len, p, sizeof(len));\
	p += sizeof(len);\
	if(len > 0){\
		char *tmp = new char[len+1];\
		memcpy(tmp, p, len);\
		tmp[len] = 0;\
		p += len;\
		s = tmp;\
		delete []tmp;\
	}\
}

#endif

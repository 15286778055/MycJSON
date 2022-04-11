#ifndef MY_CJSON__h
#define MY_CJSON__h

#include <string>
#include <cstring>

using namespace std;

/* begin of namespace __MyCJSON__ */
namespace __MyCJSON__ {

/* MycJSON value types */
#define MycJSON_Object 0
#define MycJSON_Array 1
#define MycJSON_String 2
#define MycJSON_Number 3
#define MycJSON_True 4
#define MycJSON_False 5
#define MycJSON_Null 6

/* a flag pointer to error */
static const char* ep;

/* memory manage */
static void* (*MycJSON_malloc)(size_t sz) = malloc;
static void (*MycJSON_free)(void* ptr) = free;

typedef struct {
    char* buffer;
    int length;
    int offset;
} printbuffer;


/* MycJSON Object 对象 */
class MycJSONObject {

public:
    /* value typename */
    typedef MycJSONObject object_type;
    typedef MycJSONObject array_type;
    typedef char* string_type;
    typedef int numberint_type;
    typedef double numberdouble_type;
    typedef bool true_type;
    typedef bool false_type;
    typedef char null_type;

    /* 键值的类型别名 */
    typedef string key_type;
    typedef union value_type {
        object_type* value_object;
        array_type* value_array;
        string_type value_string;
        int value_int;
        double value_double;
        bool value_true;
        bool value_false;
        char value_null;
    } value_type;

    /* parse to json object*/
    static const char* parse_value(MycJSONObject* item, const char* value);
    static const char* parse_object(MycJSONObject* item, const char* value);
    static const char* parse_array(MycJSONObject* item, const char* value);
    static const char* parse_string(MycJSONObject* item, const char* str);
    static const char* parse_number(MycJSONObject* item, const char* value);

    /* to json string */
    char* Print(MycJSONObject* item);
    static char* print_value(MycJSONObject* item, int depth, int fmt, printbuffer* p);
    static char* print_object(MycJSONObject* item, int depth, int fmt, printbuffer* p);
    static char* print_string(MycJSONObject* item, printbuffer* p);
    static char* print_string_ptr(const char* str, printbuffer* p);
    static char* ensure(printbuffer* p, int needed);
    static int update(printbuffer *p);
    static int pow2gt (int x);


    /* construcotr destructor */
    // MycJSONObject() {}
    // ~MycJSONObject() {
    //     delete next;
    //     delete prev;
    //     if (type == MycJSON_Object) delete value.value_object;
    //     if (type == MycJSON_Array) delete value.value_array;
    // }


private:
    
    /* member data */
    int type; /* The type of the item */
    key_type key;
    
    value_type value;
    
    MycJSONObject* next;
    MycJSONObject* prev;
    // MycJSONObject* child;

    char *string;       /* the item's name string */
    
};

/* declare some utility functions */
static const char* erase_space(const char* value);
static unsigned parse_hex4(const char* str);



/* define funcitons belong to MycJSONObject */
const char* MycJSONObject::parse_value(MycJSONObject* item, const char* value) {
    
    if (!value) return nullptr; /* 空串 */
    if ('{' == *value) { return parse_object(item, value); }
    if ('[' == *value) { return parse_array(item, value); }
    if ('-' == *value || ('0' <= *value && '9' >= *value)) { return parse_number(item, value); }
    if ('\"' == *value) { return parse_string(item, value); }
    if (!strncmp(value, "true", 4)) { item->type = MycJSON_True; item->value.value_int = 1; return value + 4; }
    if (!strncmp(value, "false", 5)) { item->type = MycJSON_False; return value + 5; }
    if (!strncmp(value, "null", 4)) { item->type = MycJSON_Null; return value + 4;}
    
    //

}

const char* MycJSONObject::parse_object(MycJSONObject* item, const char* value) {
    
    if ('{' != *value) { ep = value; return nullptr; };     /* not an object */

    item->type = MycJSON_Object;
    value = erase_space(value+1);
    if (*value == '}') return value+1;      /* empty object */

    MycJSONObject* child = (MycJSONObject*)MycJSON_malloc(sizeof(MycJSONObject));
    item->value.value_object = child;
    if (!item->value.value_object) return 0;
    value = erase_space(parse_string(child, erase_space(value)));
    if (!value) return 0;
    child->key = child->value.value_string; child->value.value_string = nullptr;     /* parse key */
    if (*value != ':') { ep = value; return 0; }
    value = erase_space(parse_value(child,erase_space(value+1)));        /* parse value */
    if (!value) return 0;


    while (',' == *value) {
        MycJSONObject* new_item = (MycJSONObject*)MycJSON_malloc(sizeof(MycJSONObject));
        if (new_item) memset(new_item, 0, sizeof(MycJSONObject));
        child->next = new_item; new_item->prev = child; child = new_item;
        value = erase_space(parse_string(child, erase_space(value+1)));
        if (!value) return 0;
        child->key = child->value.value_string; child->value.value_string = nullptr;       /* parse key */
        if (*value != ';') { ep = value; return 0; }
        value = erase_space(parse_value(child, erase_space(value+1)));       /* parse value */
        if (!value) return 0;
    }

    if (*value == '}') return value + 1;
    ep = value;
    return 0;

}

static unsigned parse_hex4(const char* str) {
    unsigned h = 0;
    // 1
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    h = h << 4; ++str;
    // 2
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    h = h << 4; ++str;
    // 3
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    h = h << 4; ++str;
    // 4
    if (*str >= '0' && *str <= '9') h += (*str) - '0';
    else if (*str >= 'A' && *str <= 'F') h += 10 + (*str) - 'A';
    else if (*str >= 'a' && *str <= 'f') h += 10 + (*str) - 'a';
    else return 0;
    return h;
}

static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
const char* MycJSONObject::parse_string(MycJSONObject* item, const char* str) {
    
    if ('\"' != *str) { ep = str; return 0; };      /* not a string */
    
    const char* ptr = str+1;
    char* ptr2;
    char* out;
    int len = 0;
    unsigned uc, uc2;

    while('\"' != *ptr  && *ptr && ++len) if ('\\' == *ptr++) ++ptr; /* skip escaped quotes */

    out = (char*)MycJSON_malloc(len + 1);       /* this is how long we need for the string, roughly */
    if (!out) return 0;

    ptr = str + 1;
    ptr2 = out;
    while ('\"' != *ptr && *ptr) {
        if ('\\' != *ptr) *ptr2++ = *ptr++;
        else {      /* escaped quotes need special action*/
            ++ptr;
            switch (*ptr) {
                case 'b': *ptr2++ = '\b'; break;
                case 'f': *ptr2++ = '\f'; break;
                case 'n': *ptr2++ = '\n'; break;
                case 'r': *ptr2++ = '\r'; break;
                case 't': *ptr2++ = '\t'; break;
                case 'u': {     /* transcode utf16 to utf8. */
                    uc = parse_hex4(ptr+1); ptr += 4;       /* get the unicode char */

                    if ((uc >= 0xDC00 && uc <= 0xDFFF) || uc == 0) break;       /* check for invalid */

                    if (uc >= 0xD800 && uc <= 0xDBFF) {
                        if (ptr[1] != '\\' || ptr[2] != 'u') break;
                        uc2 = parse_hex4(ptr+3); ptr += 6;
                        if (uc < 0xDc00 || uc > 0xDFFF) uc = 0x10000 + (((uc&0x3FF)<<10) | (uc2&0x3FF));
                    }

                    len = 4;
                    if (uc<0x80) len=1;
                    else if (uc < 0x800) len = 2;
                    else if (uc < 0x10000) len = 3;
                    ptr2 += len;

                    switch (len) {
                        case 4: *--ptr2 = ((uc | 0x80) & 0xBF); uc >>= 6;
                        case 3: *--ptr2 = ((uc | 0x80) & 0xBF); uc >>= 6;
                        case 2: *--ptr2 = ((uc | 0x80) & 0xBF); uc >>= 6;
                        case 1: *--ptr2 = (uc | firstByteMark[len]);
                    }
                    ptr2 += len;
                    break;
                }
                default: *ptr2++ = *ptr; break;
            }
        }
        ++ptr;
    }
    *ptr2 = 0;
    if (*ptr == '\"') ++ptr;
    item->value.value_string = out;
    item->type = MycJSON_String;
    return ptr;
}

const char* MycJSONObject::parse_number(MycJSONObject* item, const char* value) {

}

const char* MycJSONObject::parse_array(MycJSONObject* item, const char* value) {

}


/* Utility to jump whitespace and cl/lr */
static const char* erase_space(const char* value) {
    /* 0 ~ 31 为控制字符（非打印），32 为 空格 */
    while (value && *value && 32 >= (unsigned char)*value) ++value;
    return value;
}


/* functions to translate object to stirng */

/* translate for value */
char* MycJSONObject::print_value(MycJSONObject* item, int depth, int fmt, printbuffer* p) {
    char* out = nullptr;
    if (!item) return 0;
    if (p) {
        switch ((item->type) & 255) {
            case MycJSON_Object: out = print_object(item, depth, fmt, p); break;
            case MycJSON_String: out = print_string(item, p); break;
        }
    } else {
        switch ((item->type) & 255) {
            case MycJSON_Object: out = print_object(item, depth, fmt, 0); break;
            // case MycJSON_Array: break;
            case MycJSON_String: out = print_string(item, 0); break;
            // case MycJSON_Number: break;
            // case MycJSON_True: break;
            // case MycJSON_False: break;
            // case MycJSON_Null: break;
        }
    }
    return out;
}

/* translate for object */
char* MycJSONObject::print_object(MycJSONObject* item, int depth, int fmt, printbuffer* p) {
    char **entries = 0, **names = 0;
    char *out = 0, *ptr, *ret, *str;
    int len = 7, i = 0, j;
    MycJSONObject *child = item->value.value_object;
    int numentries = 0, fail = 0;
    size_t tmplen = 0;
    /* cout the number of entries */
    while (child) {
        numentries++;
        child = child->next;
    }
    /* explicitly handle empty object case */
    if (!numentries) {
        if (p) out = ensure(p, fmt ? depth+4 : 3);
        else out = (char *)MycJSON_malloc(fmt ? depth+4 : 3);
        if (!out) return 0;
        ptr = out; *ptr++ = '{';
        if (fmt) {
            *ptr++='\n';
            for (i=0; i < depth-1; ++i) *ptr++ = '\t';
        }
        *ptr++ = '}'; *ptr++ = 0;
        return out;
    }
    if (p) {
        /* coompose the output */
        i = p->offset;
        len = fmt?2:1; ptr = ensure(p, len+1); if(!ptr) return 0;
        *ptr++ = '{'; if (fmt) *ptr++ = '\n'; *ptr = 0; p->offset += len;
        child = item->value.value_object; ++depth;
        while (child) {
            if (fmt) {
                ptr = ensure(p, depth); if(!ptr) return 0;
                for (j = 0; j < depth; ++j) {
                    *ptr++ = '\t';
                }
                p->offset += depth;
            }
            print_string_ptr(child->value.value_string, p);     /* parse key */
            p->offset = update(p);

            len = fmt ? 2 : 1;
            ptr = ensure(p, len);
            if (fmt) *ptr++ = '\t';
            p->offset += len;

            print_value(child, depth, fmt, p);      /* parse value */
            p->offset = update(p);

            len = (fmt?1:0) + (child->next?1:0);
            ptr = ensure(p, len+1); if(!ptr) return 0;
            if (child->next) *ptr++ = ',';
            if (fmt) *ptr++ = '\n'; *ptr = 0;
            p->offset += len;
            child = child->next;
        }
        ptr = ensure(p, fmt?(depth+1):2); if (!ptr) return 0;
        if (fmt) for (i = 0; i < depth - 1; ++i) *ptr++ = '\t';
        *ptr++ = '}'; *ptr = 0;
        out = (p->buffer)+i;
    } else {
        /* allocate space for the names and the objects */
        entries = (char **)MycJSON_malloc(numentries * sizeof(char *));
        if (!entries) return 0;
        names = (char **) MycJSON_malloc(numentries * sizeof(char *));
        if (!names) return 0;
        memset(entries, 0, sizeof(char *) * numentries);
        memset(names, 0, sizeof(char *) * numentries);

        /* collect all the results into our arrays: */
        child = item->value.value_object; ++depth; if (fmt) len += depth;
        while (child)
        {
            names[i] = str = print_string_ptr(child->string, 0);
            entries[i++] = ret = print_value(child, depth, fmt, 0);
            if (str && ret) len += strlen(ret) + strlen(str) + 2 + (fmt?2+depth:0); else fail = 1;
        }

        /* try to allocate the output string */
        if (!fail) out = (char*)MycJSON_malloc(len);
        if (!out) fail = 1;

        /* handle failure */
        if (fail) {
            for (i = 0; i < numentries; ++i) {
                if (names[i]) MycJSON_free(names[i]);
                if (entries[i]) MycJSON_free(entries[i]);
            }
            MycJSON_free(names);
            MycJSON_free(entries);
            return 0;
        }

        /* compose the output */
        *out = '{'; ptr = out + 1; if (fmt) *ptr++ = '\n'; *ptr = 0;
        for (i = 0; i < numentries; ++i) {
            if (fmt) for (j = 0; j < depth; j++) *ptr++ = '\t';
            tmplen = strlen(names[i]); memcpy(ptr, names[i], tmplen); ptr += tmplen;
            *ptr++ = ':'; if (fmt) *ptr++ = '\t';
            strcpy(ptr, entries[i]); ptr += strlen(entries[i]);
            if (i != numentries - 1) *ptr++ = ',';
            if (fmt) *ptr++ = '\n'; *ptr = 0;
            MycJSON_free(names[i]); MycJSON_free(entries[i]);
        }

        MycJSON_free(names); MycJSON_free(entries);
        if (fmt) for (i = 0; i < depth - 1; ++i) *ptr++ = '\t';
        *ptr++ = '}'; *ptr++ = 0;
    }
    return out;
}

/* translate for string */
char* MycJSONObject::print_string(MycJSONObject* item, printbuffer* p) { return print_string_ptr(item->value.value_string, p); }

char* MycJSONObject::print_string_ptr(const char* str, printbuffer* p) {
    const char* ptr;
    char* ptr2;
    char* out;
    int len = 0;
    int flag = 0;
    unsigned char token;

    for (ptr=str; *ptr; ++ptr) flag |= ((*ptr>0 && *ptr<32) || (*ptr == '\"') || (*ptr == '\\')) ? 1 : 0;
    if (!flag) {        /* have no no-printable character */
        len = ptr - str;
        if (p) out = ensure(p, len+3);      /* 3 means quotes and terminate character */
        else out = (char *)MycJSON_malloc(len+3);
        if (!out) return 0;
        ptr2 = out; *ptr2++ = '\"';
        strcpy(ptr2, str);
        ptr2[len] = '\"';
        ptr2[len+1] = 0;
        return out;
    }

    if (!str) {     /* str has no strings */
        if (p) out = ensure(p, 3);
        else out = (char *)MycJSON_malloc(3);
        if (!out) return 0;
        strcpy(out, "\"\"");
        return out;
    }

    /* str has no-printable characters */
    ptr = str;
    while ((token=*ptr) && ++len) {     /* count the length of no-printable string */
        if (strchr(("\"\\\b\f\n\r\t"), token)) len++;
        else if (token < 32) len += 5;
        ptr++;
    }

    if (p) out = ensure(p, len+3);
    else out = (char *)MycJSON_malloc(len+3);
    if (!out) return 0;

    ptr2 = out; ptr = str;
    *ptr2++ = '\"';
    while (*ptr) {
        if ((unsigned char)*ptr>31 && *ptr!='\"' && *ptr!='\\') *ptr2++ = *ptr++;
        else {
            *ptr2++ = '\\';
            switch (token = *ptr++) {
                case '\\': *ptr2++ = '\\'; break;
                case '\"': *ptr2++ = '\"'; break;
                case '\b': *ptr2++ = 'b'; break;
                case '\f': *ptr2++ = 'f'; break;
                case '\n': *ptr2++ = 'n'; break;
                case '\r': *ptr2++ = 'r"'; break;
                case '\t': *ptr2++ = 't'; break;
                default: sprintf(ptr2, "u%04x", token); ptr2 += 5; break;
            }
        }
    }
    *ptr2++ = '\"'; *ptr2++ = 0;
    return out;
}

char* MycJSONObject::ensure(printbuffer* p, int needed) {
    char *newbuffer; int newsize;
    if (!p || !p->buffer) return 0;
    needed += p->offset;
    if (needed <= p->length) return p->buffer + p->offset;

    newsize = pow2gt(needed);
    newbuffer = (char *)MycJSON_malloc(newsize);
    if (!newbuffer) { MycJSON_free(p->buffer); p->length = 0; p->buffer = 0; return 0; }
    if (newbuffer) memcpy(newbuffer, p->buffer, p->length);
    MycJSON_free(p->buffer);
    p->length = newsize;
    p->buffer = newbuffer;
    return newbuffer + p->offset;
}

static int update(printbuffer *p) {
    char *str;
    if (!p || !p->buffer) return 0;
    str = p->buffer + p->offset;
    return p->offset + strlen(str);
}

static int pow2gt (int x) {	--x;	x|=x>>1;	x|=x>>2;	x|=x>>4;	x|=x>>8;	x|=x>>16;	return x+1;	}



} /* end of namespace __MyCJSON__ */






#endif
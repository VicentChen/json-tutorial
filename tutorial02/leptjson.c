#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL, strtod() */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/**
* s: string "null" "true" or "false"
* t: the correct type
*/
static int lept_parse_literal(lept_context* c, lept_value* v, const char* s, lept_type t)
{
	for (*s; *s != 0; s++) {
		if (*c->json != *s)
			return LEPT_PARSE_INVALID_VALUE;
		(c->json)++;
	}
	v->type = t;
	return LEPT_PARSE_OK;
}

static int lept_parse_number(lept_context* c, lept_value* v) {
    char* end = c->json;
    /* \TODO validate number */
	/* phase 1 */
	if (*end == '-') end++;
	if (*end == '0')
		end++;
	else if (ISDIGIT1TO9(*end))
		for (*end; ISDIGIT(*end); end++);
	else
		return LEPT_PARSE_INVALID_VALUE;

	/* phase 2 */
	if (*end == '.') {
		end++;
		if (!ISDIGIT(*end)) return LEPT_PARSE_INVALID_VALUE;
		for (*end; ISDIGIT(*end); end++);
	}	

	/* phase 3 */
	if (*end == 'e' || *end == 'E') {
		end++;
		if (*end == '+' || *end == '-')
			end++;
		if (!ISDIGIT(*end)) return LEPT_PARSE_INVALID_VALUE;
		for (*end; ISDIGIT(*end); end++);
	}

	v->n = strtod(c->json, NULL);
	c->json = end;
	v->type = LEPT_NUMBER;
	return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
		case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", LEPT_FALSE);
        case 'n':  return lept_parse_literal(c, v, "null", LEPT_NULL);
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}

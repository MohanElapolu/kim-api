#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pdjson.h"

#if _WIN32
#  define C_RED(s)   s
#  define C_GREEN(s) s
#  define C_BOLD(s)  s
#else
#  define C_RED(s)   "\033[31;1m" s "\033[0m"
#  define C_GREEN(s) "\033[32;1m" s "\033[0m"
#  define C_BOLD(s)  "\033[1m"    s "\033[0m"
#endif

struct expect {
    enum json_type type;
    const char *str;
};

#define countof(a) (sizeof(a) / sizeof(*a))

#define TEST(name, stream) \
    do { \
        int r = test(name, stream, seq, countof(seq), str, sizeof(str) - 1); \
        if (r) \
            count_pass++; \
        else \
            count_fail++; \
    } while (0)

const char json_typename[][12] = {
    "",
    "ERROR",
    "DONE",
    "OBJECT",
    "OBJECT_END",
    "ARRAY",
    "ARRAY_END",
    "STRING",
    "NUMBER",
    "TRUE",
    "FALSE",
    "NULL",
};

static int
has_value(enum json_type type)
{
    return type == JSON_STRING || type == JSON_NUMBER;
}

struct buffer {
    const void *buf;
    size_t len;
    size_t pos;
};

static int
buffer_fgetc(void *arg)
{
    const unsigned char *buf;
    struct buffer *b = arg;
    if (b->pos == b->len)
        return -1;
    buf = b->buf;
    return buf[b->pos++];
}

static int
test(const char *name,
     int stream,
     struct expect *seq,
     size_t seqlen,
     const char *buf,
     size_t len)
{
    size_t i;
    int success = 1;
    struct buffer buffer;
    struct json_stream json[1];
    enum json_type expect, actual;
    const char *expect_str, *actual_str;

    buffer.buf = buf;
    buffer.len = len;
    buffer.pos = 0;
    json_open(json, buffer_fgetc, &buffer, JSON_FLAG_STREAMING);

    for (i = 0; success && i < seqlen; i++) {
        expect = seq[i].type;
        actual = json_next(json);
        actual_str = has_value(actual) ? json_get_string(json, 0) : "";
        expect_str = seq[i].str ? seq[i].str : "";

        if (actual != expect)
            success = 0;
        else if (seq[i].str && !!strcmp(expect_str, actual_str))
            success = 0;
        else if (stream && actual == JSON_DONE)
            json_reset(json);
    }

    if (success) {
        printf(C_GREEN("PASS") " %s\n", name);
    } else {
        printf(C_RED("FAIL") " %s: "
               "expect " C_BOLD("%s") " %s / "
               "actual " C_BOLD("%s") " %s\n",
               name,
               json_typename[expect], expect_str,
               json_typename[actual], actual_str);
    }
    json_close(json);
    return success;
}

int
main(void)
{
    int count_pass = 0;
    int count_fail = 0;

    {
        const char str[] = "  1024\n";
        struct expect seq[] = {
            {JSON_NUMBER, "1024"},
            {JSON_DONE, 0},
        };
        TEST("number", 0);
    }

    {
        const char str[] = "  true \n";
        struct expect seq[] = {
            {JSON_TRUE, 0},
            {JSON_DONE, 0},
        };
        TEST("true", 0);
    }

    {
        const char str[] = "\nfalse\r\n";
        struct expect seq[] = {
            {JSON_FALSE, 0},
            {JSON_DONE, 0},
        };
        TEST("false", 0);
    }

    {
        const char str[] = "\tnull";
        struct expect seq[] = {
            {JSON_NULL, 0},
            {JSON_DONE, 0},
        };
        TEST("null", 0);
    }

    {
        const char str[] = "\"foo\"";
        struct expect seq[] = {
            {JSON_STRING, "foo"},
            {JSON_DONE, 0},
        };
        TEST("string", 0);
    }

    {
        const char str[] = "\"Tim \\\"The Tool Man\\\" Taylor\"";
        struct expect seq[] = {
            {JSON_STRING, "Tim \"The Tool Man\" Taylor"},
            {JSON_DONE, 0},
        };
        TEST("string quotes", 0);
    }

    {
        const char str[] = "{\"abc\": -1}";
        struct expect seq[] = {
            {JSON_OBJECT, 0},
            {JSON_STRING, "abc"},
            {JSON_NUMBER, "-1"},
            {JSON_OBJECT_END, 0},
            {JSON_DONE, 0},
        };
        TEST("object", 0);
    }

    {
        const char str[] = "[1, \"two\", true, null]";
        struct expect seq[] = {
            {JSON_ARRAY, 0},
            {JSON_NUMBER, "1"},
            {JSON_STRING, "two"},
            {JSON_TRUE, 0},
            {JSON_NULL, 0},
            {JSON_ARRAY_END, 0},
            {JSON_DONE, 0},
        };
        TEST("array", 0);
    }

    {
        const char str[] = "1 10 100 2002";
        struct expect seq[] = {
            {JSON_NUMBER, "1"},
            {JSON_DONE, 0},
            {JSON_NUMBER, "10"},
            {JSON_DONE, 0},
            {JSON_NUMBER, "100"},
            {JSON_DONE, 0},
            {JSON_NUMBER, "2002"},
            {JSON_DONE, 0},
            {JSON_ERROR, 0},
        };
        TEST("number stream", 1);
    }

    {
        const char str[] = "{\"foo\": [1, 2, 3]}\n[]\n\"name\"";
        struct expect seq[] = {
            {JSON_OBJECT, 0},
            {JSON_STRING, "foo"},
            {JSON_ARRAY, 0},
            {JSON_NUMBER, "1"},
            {JSON_NUMBER, "2"},
            {JSON_NUMBER, "3"},
            {JSON_ARRAY_END, 0},
            {JSON_OBJECT_END, 0},
            {JSON_DONE, 0},
            {JSON_ARRAY, 0},
            {JSON_ARRAY_END, 0},
            {JSON_DONE, 0},
            {JSON_STRING, "name"},
            {JSON_DONE, 0},
            {JSON_ERROR, 0},
        };
        TEST("mixed stream", 1);
    }

    {
        const char str[] = "[1, 2, 3";
        struct expect seq[] = {
            {JSON_ARRAY, 0},
            {JSON_NUMBER, "1"},
            {JSON_NUMBER, "2"},
            {JSON_NUMBER, "3"},
            {JSON_ERROR, 0},
        };
        TEST("incomplete array", 0);
    }

    {
        const char str[] = "\"\\u0068\\u0065\\u006c\\u006c\\u006F\"";
        struct expect seq[] = {
            {JSON_STRING, "hello"},
            {JSON_DONE, 0},
        };
        TEST("\\uXXXX", 0);
    }

    {
        /* This surrogate half must precede another half */
        const char str[] = "\"\\uD800\\u0065\"";
        struct expect seq[] = {
            {JSON_ERROR, 0}
        };
        TEST("invalid surrogate pair", 0);
    }

    {
        /* This surrogate half cannot be alone */
        const char str[] = "\"\\uDC00\"";
        struct expect seq[] = {
            {JSON_ERROR, 0}
        };
        TEST("invalid surrogate half", 0);
    }

    {
        /* Surrogate halves are in the wrong order */
        const char str[] = "\":\\uDc00\\uD800\"";
        struct expect seq[] = {
            {JSON_ERROR, 0}
        };
        TEST("surrogate misorder", 0);
    }

    {
        /* This is a valid encoding for U+10000 */
        const char str[] = "\":\\uD800\\uDC00\"";
        struct expect seq[] = {
            {JSON_STRING, ":\xf0\x90\x80\x80"}, /* UTF-8 for U+10000 */
            {JSON_DONE, 0},
        };
        TEST("surrogate pair", 0);
    }

    printf("%d pass, %d fail\n", count_pass, count_fail);
    exit(count_fail ? EXIT_FAILURE : EXIT_SUCCESS);
}

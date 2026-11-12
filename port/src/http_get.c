// PORT_HttpGet (launcher_core.h), the launcher's one network call (its cover art: launcher_cover.c). macOS: the system's
// libcurl (the SDK's, -lcurl; nothing vendored). iOS has its own in ios_http.m (NSURLSession). Windows: WinHTTP. Linux: the system's
// libcurl opened with dlopen. Android (HttpURLConnection through JNI) is owed: its stub says "not supported yet".
#include <stdio.h>
#include <string.h>
#include "launcher_core.h"
#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__APPLE__) && !TARGET_OS_IPHONE
#include <pthread.h>
#include <curl/curl.h>

#define MAX_BODY (10L * 1024 * 1024)

typedef struct {
    FILE *f;
    long size;
} Body;

static size_t Write(char *p, size_t size, size_t count, void *user) {
    Body *b = user;
    size_t len = size * count;
    if (b->size + (long)len > MAX_BODY) {
        return 0;    // (curl stops: CURLE_WRITE_ERROR)
    }
    b->size += (long)len;
    return fwrite(p, 1, len, b->f);
}

static pthread_once_t sOnce = PTHREAD_ONCE_INIT;

static void Init(void) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

int PORT_HttpGet(const char *url, const char *path, char *err, size_t n) {
    char why[CURL_ERROR_SIZE] = "";
    Body b = {NULL, 0};
    long status = 0;
    CURLcode c;
    CURL *h;
    if (strncmp(url, "https://", 8) != 0) {
        snprintf(err, n, "https only");
        return -1;
    }
    pthread_once(&sOnce, Init);
    if ((b.f = fopen(path, "wb")) == NULL) {
        snprintf(err, n, "can't write %s", path);
        return -1;
    }
    if ((h = curl_easy_init()) == NULL) {
        fclose(b.f);
        snprintf(err, n, "libcurl didn't start");
        return -1;
    }
    curl_easy_setopt(h, CURLOPT_URL, url);
    curl_easy_setopt(h, CURLOPT_PROTOCOLS_STR, "https");
    curl_easy_setopt(h, CURLOPT_REDIR_PROTOCOLS_STR, "https");
    curl_easy_setopt(h, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(h, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(h, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(h, CURLOPT_NOSIGNAL, 1L);    // (a thread: no SIGALRM for the resolver's timeout)
    curl_easy_setopt(h, CURLOPT_USERAGENT, LC_USER_AGENT);
    curl_easy_setopt(h, CURLOPT_MAXFILESIZE_LARGE, (curl_off_t)MAX_BODY);
    curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, Write);
    curl_easy_setopt(h, CURLOPT_WRITEDATA, &b);
    curl_easy_setopt(h, CURLOPT_ERRORBUFFER, why);
    c = curl_easy_perform(h);
    curl_easy_getinfo(h, CURLINFO_RESPONSE_CODE, &status);
    curl_easy_cleanup(h);
    if (fclose(b.f) != 0 && c == CURLE_OK) {
        c = CURLE_WRITE_ERROR;
    }
    if (c != CURLE_OK) {
        snprintf(err, n, "%s", why[0] ? why : curl_easy_strerror(c));
        return -1;
    }
    return status == 200 ? 0 : (int)(status > 0 ? status : 1);
}

#elif defined(_WIN32)
// Windows: WinHTTP (system DLL, no dependency to ship). https only (WinHTTP refuses a redirect to http by default, and the
// request is made with WINHTTP_FLAG_SECURE), 15 s in all, 10 MB, LC_USER_AGENT.
#include <windows.h>
#include <winhttp.h>

#define MAX_BODY (10L * 1024 * 1024)
#define TIMEOUT_MS 15000

static void LastError(char *err, size_t n, const char *what) {
    DWORD e = GetLastError();
    if (e == ERROR_WINHTTP_TIMEOUT) {
        snprintf(err, n, "%s: timed out", what);
    } else {
        snprintf(err, n, "%s: WinHTTP error %lu", what, (unsigned long)e);
    }
}

int PORT_HttpGet(const char *url, const char *path, char *err, size_t n) {
    wchar_t wurl[2100], wagent[200], host[260], urlpath[1900];
    URL_COMPONENTS uc;
    HINTERNET ses = NULL, con = NULL, req = NULL;
    FILE *f = NULL;
    DWORD status = 0, size = sizeof(status), got;
    ULONGLONG start = GetTickCount64();
    long total = 0;
    int rc = -1;
    static char buf[16384];

    if (strncmp(url, "https://", 8) != 0) {
        snprintf(err, n, "https only");
        return -1;
    }
    if (MultiByteToWideChar(CP_UTF8, 0, url, -1, wurl, 2100) == 0 || MultiByteToWideChar(CP_UTF8, 0, LC_USER_AGENT, -1, wagent, 200) == 0) {
        snprintf(err, n, "the address is too long");
        return -1;
    }
    memset(&uc, 0, sizeof(uc));
    uc.dwStructSize = sizeof(uc);
    uc.lpszHostName = host;
    uc.dwHostNameLength = 260;
    uc.lpszUrlPath = urlpath;
    uc.dwUrlPathLength = 1900;
    uc.dwExtraInfoLength = (DWORD)-1;    // (the query stays in the path)
    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) {
        snprintf(err, n, "bad address");
        return -1;
    }
    if (uc.dwExtraInfoLength != 0 && uc.lpszExtraInfo != NULL) {
        size_t have = wcslen(urlpath);
        if (have + uc.dwExtraInfoLength + 1 < 1900) {
            wcsncat(urlpath, uc.lpszExtraInfo, uc.dwExtraInfoLength);
        }
    }
    if ((f = fopen(path, "wb")) == NULL) {
        snprintf(err, n, "can't write %s", path);
        return -1;
    }
    if ((ses = WinHttpOpen(wagent, WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0)) == NULL) {
        LastError(err, n, "WinHTTP didn't start");
        goto out;
    }
    WinHttpSetTimeouts(ses, TIMEOUT_MS, TIMEOUT_MS, TIMEOUT_MS, TIMEOUT_MS);
    if ((con = WinHttpConnect(ses, host, uc.nPort, 0)) == NULL) {
        LastError(err, n, "connect");
        goto out;
    }
    if ((req = WinHttpOpenRequest(con, L"GET", urlpath, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE)) == NULL) {
        LastError(err, n, "request");
        goto out;
    }
    {
        DWORD policy = WINHTTP_OPTION_REDIRECT_POLICY_DISALLOW_HTTPS_TO_HTTP, maxRedirects = 5;
        WinHttpSetOption(req, WINHTTP_OPTION_REDIRECT_POLICY, &policy, sizeof(policy));
        WinHttpSetOption(req, WINHTTP_OPTION_MAX_HTTP_AUTOMATIC_REDIRECTS, &maxRedirects, sizeof(maxRedirects));
    }
    if (!WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) || !WinHttpReceiveResponse(req, NULL)) {
        LastError(err, n, "no answer");
        goto out;
    }
    WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &size,
                        WINHTTP_NO_HEADER_INDEX);
    if (status == 200) {
        for (;;) {
            if (GetTickCount64() - start > TIMEOUT_MS) {
                snprintf(err, n, "timed out");
                goto out;
            }
            if (!WinHttpReadData(req, buf, sizeof(buf), &got)) {
                LastError(err, n, "read");
                goto out;
            }
            if (got == 0) {
                break;
            }
            total += (long)got;
            if (total > MAX_BODY) {
                snprintf(err, n, "the answer is over 10 MB");
                goto out;
            }
            if (fwrite(buf, 1, got, f) != got) {
                snprintf(err, n, "can't write %s", path);
                goto out;
            }
        }
    }
    rc = status == 200 ? 0 : (int)(status > 0 ? status : 1);
out:
    if (f != NULL && fclose(f) != 0 && rc == 0) {
        snprintf(err, n, "can't write %s", path);
        rc = -1;
    }
    if (rc != 0) {
        remove(path);    // (an HTTP status or a failure leaves no body behind)
    }
    if (req != NULL) WinHttpCloseHandle(req);
    if (con != NULL) WinHttpCloseHandle(con);
    if (ses != NULL) WinHttpCloseHandle(ses);
    return rc;
}

#elif defined(__linux__) && !defined(__ANDROID__)
// Linux: the system's libcurl, opened at run time (dlopen "libcurl.so.4": no development package to build against, and a machine
// without libcurl just keeps the gradient). The option numbers are curl's stable ABI (curl.h).
#include <dlfcn.h>
#include <pthread.h>
#include <stdint.h>

#define MAX_BODY (10L * 1024 * 1024)

enum {
    O_WRITEDATA = 10001, O_URL = 10002, O_ERRORBUFFER = 10010, O_WRITEFUNCTION = 20011, O_TIMEOUT = 13, O_FOLLOWLOCATION = 52,
    O_USERAGENT = 10018, O_MAXREDIRS = 68, O_NOSIGNAL = 99, O_PROTOCOLS = 181, O_REDIR_PROTOCOLS = 182, O_PROTOCOLS_STR = 10318,
    O_REDIR_PROTOCOLS_STR = 10319, O_MAXFILESIZE_LARGE = 30117, I_RESPONSE_CODE = 0x200002
};

static struct {
    int (*global_init)(long);
    void *(*easy_init)(void);
    int (*setopt)(void *, int, ...);
    int (*perform)(void *);
    int (*getinfo)(void *, int, ...);
    void (*cleanup)(void *);
    const char *(*strerror)(int);
} sCurl;
static pthread_once_t sOnce = PTHREAD_ONCE_INIT;

static void Init(void) {
    void *h = dlopen("libcurl.so.4", RTLD_NOW);
    if (h == NULL) {
        h = dlopen("libcurl-gnutls.so.4", RTLD_NOW);
    }
    if (h == NULL) {
        return;
    }
    sCurl.global_init = (int (*)(long))dlsym(h, "curl_global_init");
    sCurl.easy_init = (void *(*)(void))dlsym(h, "curl_easy_init");
    sCurl.setopt = (int (*)(void *, int, ...))dlsym(h, "curl_easy_setopt");
    sCurl.perform = (int (*)(void *))dlsym(h, "curl_easy_perform");
    sCurl.getinfo = (int (*)(void *, int, ...))dlsym(h, "curl_easy_getinfo");
    sCurl.cleanup = (void (*)(void *))dlsym(h, "curl_easy_cleanup");
    sCurl.strerror = (const char *(*)(int))dlsym(h, "curl_easy_strerror");
    if (sCurl.global_init == NULL || sCurl.easy_init == NULL || sCurl.setopt == NULL || sCurl.perform == NULL ||
        sCurl.getinfo == NULL || sCurl.cleanup == NULL || sCurl.strerror == NULL) {
        memset(&sCurl, 0, sizeof(sCurl));
        return;
    }
    sCurl.global_init(3);    // CURL_GLOBAL_DEFAULT
}

typedef struct {
    FILE *f;
    long size;
} Body;

static size_t Write(char *p, size_t size, size_t count, void *user) {
    Body *b = user;
    size_t len = size * count;
    if (b->size + (long)len > MAX_BODY) {
        return 0;    // (curl stops: CURLE_WRITE_ERROR)
    }
    b->size += (long)len;
    return fwrite(p, 1, len, b->f);
}

int PORT_HttpGet(const char *url, const char *path, char *err, size_t n) {
    char why[256] = "";
    Body b = {NULL, 0};
    long status = 0;
    int c;
    void *h;
    if (strncmp(url, "https://", 8) != 0) {
        snprintf(err, n, "https only");
        return -1;
    }
    pthread_once(&sOnce, Init);
    if (sCurl.easy_init == NULL) {
        snprintf(err, n, "libcurl is not installed");
        return -1;
    }
    if ((b.f = fopen(path, "wb")) == NULL) {
        snprintf(err, n, "can't write %s", path);
        return -1;
    }
    if ((h = sCurl.easy_init()) == NULL) {
        fclose(b.f);
        snprintf(err, n, "libcurl didn't start");
        return -1;
    }
    sCurl.setopt(h, O_URL, url);
    if (sCurl.setopt(h, O_PROTOCOLS_STR, "https") != 0) {    // (before curl 7.85: the number form)
        sCurl.setopt(h, O_PROTOCOLS, 2L);
        sCurl.setopt(h, O_REDIR_PROTOCOLS, 2L);
    } else {
        sCurl.setopt(h, O_REDIR_PROTOCOLS_STR, "https");
    }
    sCurl.setopt(h, O_FOLLOWLOCATION, 1L);
    sCurl.setopt(h, O_MAXREDIRS, 5L);
    sCurl.setopt(h, O_TIMEOUT, 15L);
    sCurl.setopt(h, O_NOSIGNAL, 1L);    // (a thread: no SIGALRM for the resolver's timeout)
    sCurl.setopt(h, O_USERAGENT, LC_USER_AGENT);
    sCurl.setopt(h, O_MAXFILESIZE_LARGE, (int64_t)MAX_BODY);
    sCurl.setopt(h, O_WRITEFUNCTION, Write);
    sCurl.setopt(h, O_WRITEDATA, &b);
    sCurl.setopt(h, O_ERRORBUFFER, why);
    c = sCurl.perform(h);
    sCurl.getinfo(h, I_RESPONSE_CODE, &status);
    sCurl.cleanup(h);
    if (fclose(b.f) != 0 && c == 0) {
        c = 23;    // CURLE_WRITE_ERROR
    }
    if (c != 0) {
        snprintf(err, n, "%s", why[0] ? why : sCurl.strerror(c));
        return -1;
    }
    return status == 200 ? 0 : (int)(status > 0 ? status : 1);
}

#elif !defined(__APPLE__) && !defined(__ANDROID__)    // (Android: android_launcher.c, HttpURLConnection over JNI)

int PORT_HttpGet(const char *url, const char *path, char *err, size_t n) {
    (void)url;
    (void)path;
    snprintf(err, n, "not supported yet on this system");
    return -1;
}

#endif

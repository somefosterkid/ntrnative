// The launcher's cover art (docs/launcher.md, "Cover art"): each game's front cover fetched on the user's device into the
// game's folder as cover.jpg, as other launchers do (TWiLight Menu++ from GameTDB, RetroArch from libretro-thumbnails). Nothing
// is shipped or bundled. The UIs show the user's artwork.jpg first, then cover.jpg, then their gradient.
//   1. GameTDB (https://www.gametdb.com), by the header's 4-letter code: art.gametdb.com/ds/<kind>/<region>/<code>.jpg, the
//      kinds largest first (coverHQ 768x680 for IRBO, coverM, cover), the regions from the code's last letter. GameTDB's FAQ
//      ("Developers"): software may use its artwork; add a link to GameTDB so users know where to add games and artwork.
//   2. libretro-thumbnails (github.com/libretro-thumbnails/Nintendo_-_Nintendo_DS, Named_Boxarts), by the table's No-Intro
//      name, through RetroArch's own server, thumbnails.libretro.com (512 px wide PNGs).
// One try per game per process (LC_CoverWanted); a failure is logged, quiet, and tried again at the next start. The fetch is
// blocking: the UIs run LC_FetchCover on a thread of their own and reload when it returns.
// Test: BWPORT_COVER_FAIL=gametdb (GameTDB skipped, as if it failed: the libretro fallback), =offline (every URL on a host that
// does not resolve, as with no network).
//
// Images: stb_image and stb_image_write (Sean Barrett, public domain / MIT, port/src/stb_image*.h, unchanged); their code is
// compiled here (launcher_gui.cpp uses them too).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "launcher_core.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define LONG_SIDE 1500

int LC_WriteImage(const unsigned char *rgb, int w, int h, const char *dst) {
    char tmp[LC_PATH + 8];
    unsigned char *out;
    int ow = w, oh = h, lng = w > h ? w : h, x, y, ok;
    if (lng > LONG_SIDE) {
        ow = (int)((long)w * LONG_SIDE / lng);
        oh = (int)((long)h * LONG_SIDE / lng);
        ow = ow < 1 ? 1 : ow;
        oh = oh < 1 ? 1 : oh;
    }
    if ((out = malloc((size_t)ow * oh * 3)) == NULL) {
        return -1;
    }
    for (y = 0; y < oh; y++) {    // a box filter: each pixel the mean of the source pixels under it
        int y0 = (int)((long)y * h / oh), y1 = (int)((long)(y + 1) * h / oh);
        y1 = y1 <= y0 ? y0 + 1 : y1;
        for (x = 0; x < ow; x++) {
            int x0 = (int)((long)x * w / ow), x1 = (int)((long)(x + 1) * w / ow), sx, sy, k;
            unsigned sum[3] = {0, 0, 0}, cnt = 0;
            x1 = x1 <= x0 ? x0 + 1 : x1;
            for (sy = y0; sy < y1; sy++) {
                for (sx = x0; sx < x1; sx++) {
                    const unsigned char *p = &rgb[((size_t)sy * w + sx) * 3];
                    sum[0] += p[0];
                    sum[1] += p[1];
                    sum[2] += p[2];
                    cnt++;
                }
            }
            for (k = 0; k < 3; k++) {
                out[((size_t)y * ow + x) * 3 + k] = (unsigned char)(sum[k] / cnt);
            }
        }
    }
    snprintf(tmp, sizeof(tmp), "%s.tmp", dst);
    ok = stbi_write_jpg(tmp, ow, oh, 3, out, 85) != 0 && rename(tmp, dst) == 0;
    free(out);
    if (!ok) {
        unlink(tmp);
    }
    return ok ? 0 : -1;
}

static char sTried[16];    // by the table's row: tried in this process

int LC_CoverWanted(const LC_Game *g) {
    char dir[LC_PATH], path[LC_PATH + 16];
    struct stat st;
    int count, i;
    const LC_Game *t = LC_Games(&count);
    i = (int)(g - t);
    if (i < 0 || i >= count || i >= (int)sizeof(sTried) || sTried[i] || !LC_CoverDownloads()) {
        return 0;
    }
    LC_GameDir(g, dir, sizeof(dir));
    snprintf(path, sizeof(path), "%s/artwork.jpg", dir);
    if (stat(path, &st) == 0) {
        return 0;
    }
    snprintf(path, sizeof(path), "%s/cover.jpg", dir);
    if (stat(path, &st) == 0) {
        return 0;
    }
    sTried[i] = 1;
    return 1;
}

// One URL: 0 = a good image written to dst; 1 = not there or not an image (the next URL); -1 = no answer (stop: offline)
static int Try(const char *url, const char *tmp, const char *dst, char *err, size_t n) {
    char why[300] = "";
    unsigned char *rgb;
    int w = 0, h = 0, c = 0, r = PORT_HttpGet(url, tmp, why, sizeof(why));
    if (r != 0) {
        fprintf(stderr, "bwport: cover: %s -> %d%s%s\n", url, r, why[0] ? ": " : "", why);
        unlink(tmp);
        if (r < 0) {
            snprintf(err, n, "No answer from %.80s: %s", url, why);
            return -1;
        }
        snprintf(err, n, "Not found (HTTP %d)", r);
        return 1;
    }
    rgb = stbi_load(tmp, &w, &h, &c, 3);
    unlink(tmp);
    if (rgb == NULL || w < 16 || h < 16) {
        fprintf(stderr, "bwport: cover: %s -> 200 but not a picture (%s)\n", url, rgb == NULL ? stbi_failure_reason() : "too small");
        stbi_image_free(rgb);
        snprintf(err, n, "The download wasn't a picture");
        return 1;
    }
    r = LC_WriteImage(rgb, w, h, dst);
    fprintf(stderr, "bwport: cover: %s -> 200, %dx%d%s\n", url, w, h, r == 0 ? ", cover.jpg written" : ", cover.jpg NOT written");
    stbi_image_free(rgb);
    if (r != 0) {
        snprintf(err, n, "The cover couldn't be saved (is the disk full?)");
        return -1;
    }
    return 0;
}

static void WriteSource(const char *dir, const char *name, const char *link) {
    char path[LC_PATH + 16];
    FILE *f;
    snprintf(path, sizeof(path), "%s/cover.txt", dir);
    if ((f = fopen(path, "w")) != NULL) {
        fprintf(f, "%s\n%s\n", name, link);
        fclose(f);
    }
}

int LC_FetchCover(const LC_Game *g, char *err, size_t n) {
    // GameTDB's DS regions by the code's last letter (O: the USA and Europe in one, as Black's IRBO)
    static const struct { char c; const char *r[2]; } kRegions[] = {
        {'E', {"US", NULL}}, {'O', {"US", "EN"}}, {'P', {"EN", NULL}}, {'J', {"JA", NULL}}, {'K', {"KO", NULL}},
        {'D', {"DE", "EN"}}, {'F', {"FR", "EN"}}, {'S', {"ES", "EN"}}, {'I', {"IT", "EN"}}, {'H', {"NL", "EN"}},
        {'U', {"AU", "EN"}}, {'C', {"ZH", NULL}},
    };
    static const char *kKinds[] = {"coverHQ", "coverM", "cover"};
    const char *regions[2] = {"EN", NULL}, *fail = getenv("BWPORT_COVER_FAIL");
    const char *host = fail != NULL && strcmp(fail, "offline") == 0 ? "offline.invalid" : NULL;
    char dir[LC_PATH], tmp[LC_PATH + 16], dst[LC_PATH + 16], url[1024], link[256], why[400] = "";
    int i, k, r = 1;

    if (!LC_CoverDownloads()) {
        snprintf(err, n, "Download Artwork is off");
        return -1;
    }
    LC_GameDir(g, dir, sizeof(dir));
    snprintf(tmp, sizeof(tmp), "%s/cover.download", dir);
    snprintf(dst, sizeof(dst), "%s/cover.jpg", dir);
    fprintf(stderr, "bwport: cover: fetching %s's (%s)\n", g->title, g->code);
    for (i = 0; i < (int)(sizeof(kRegions) / sizeof(kRegions[0])); i++) {
        if (strlen(g->code) == 4 && g->code[3] == kRegions[i].c) {
            regions[0] = kRegions[i].r[0];
            regions[1] = kRegions[i].r[1];
        }
    }
    if (fail != NULL && strcmp(fail, "gametdb") == 0) {
        fprintf(stderr, "bwport: cover: GameTDB skipped (BWPORT_COVER_FAIL=gametdb)\n");
    } else {
        for (i = 0; i < 2 && regions[i] != NULL && r > 0; i++) {
            for (k = 0; k < 3 && r > 0; k++) {
                snprintf(url, sizeof(url), "https://%s/ds/%s/%s/%s.jpg", host ? host : "art.gametdb.com", kKinds[k], regions[i], g->code);
                r = Try(url, tmp, dst, why, sizeof(why));
            }
        }
        if (r == 0) {
            snprintf(link, sizeof(link), "https://www.gametdb.com/DS/%s", g->code);
            WriteSource(dir, "GameTDB", link);
            return 0;
        }
    }
    if (r >= 0 && g->noIntro != NULL) {
        // the file name as libretro-thumbnails spells it (&*/:`<>?\|" become _), then percent-encoded
        char *o = url + snprintf(url, sizeof(url), "https://%s/Nintendo%%20-%%20Nintendo%%20DS/Named_Boxarts/",
                                 host ? host : "thumbnails.libretro.com");
        const char *s;
        for (s = g->noIntro; *s != 0 && o < url + sizeof(url) - 8; s++) {
            unsigned char ch = strchr("&*/:`<>?\\|\"", *s) != NULL ? '_' : (unsigned char)*s;
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || strchr("-_.~()", ch) != NULL) {
                *o++ = (char)ch;
            } else {
                o += sprintf(o, "%%%02X", ch);
            }
        }
        strcpy(o, ".png");
        r = Try(url, tmp, dst, why, sizeof(why));
        if (r == 0) {
            WriteSource(dir, "libretro", "https://github.com/libretro-thumbnails/Nintendo_-_Nintendo_DS");
            return 0;
        }
    }
    fprintf(stderr, "bwport: cover: none for %s (%s); tried again at the next start\n", g->title, why);
    snprintf(err, n, "%s", why);
    return -1;
}

int LC_CoverSource(const LC_Game *g, char *name, size_t nameN, char *link, size_t linkN) {
    char dir[LC_PATH], path[LC_PATH + 16], line[300];
    struct stat st;
    FILE *f;
    LC_GameDir(g, dir, sizeof(dir));
    snprintf(path, sizeof(path), "%s/cover.jpg", dir);
    if (stat(path, &st) != 0) {
        return -1;
    }
    snprintf(name, nameN, "GameTDB");    // (a cover.txt lost: GameTDB is the first source)
    snprintf(link, linkN, "https://www.gametdb.com");
    snprintf(path, sizeof(path), "%s/cover.txt", dir);
    if ((f = fopen(path, "r")) != NULL) {
        if (fgets(line, sizeof(line), f) != NULL) {
            line[strcspn(line, "\r\n")] = 0;
            snprintf(name, nameN, "%s", line);
        }
        if (fgets(line, sizeof(line), f) != NULL) {
            line[strcspn(line, "\r\n")] = 0;
            snprintf(link, linkN, "%s", line);
        }
        fclose(f);
    }
    return 0;
}

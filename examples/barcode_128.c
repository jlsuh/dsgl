#include "dsgl.h"

#include <stdbool.h>

#define BUFFER_SIZE 65
int get_max_input_length(void)
{
    return BUFFER_SIZE - 1;
}

char data_buffer[BUFFER_SIZE];
char *get_data_buffer(void)
{
    return data_buffer;
}

int canvas_width = 0;
int get_width(void)
{
    return canvas_width;
}

int canvas_height = 0;
int get_height(void)
{
    return canvas_height;
}

int dpr = 1;
void set_dpr(int user_dpr)
{
    if (user_dpr < 1)
        user_dpr = 1;
    if (user_dpr > 4)
        user_dpr = 4;
    dpr = user_dpr;
}

#define MAX_WIDTH 13000
#define MAX_HEIGHT 1200

uint32_t pixels[MAX_WIDTH * MAX_HEIGHT];
uint32_t *get_pixel_buffer(void)
{
    return pixels;
}

#define BASE_BAR_HEIGHT_PX 160
#define BASE_MODULE_WIDTH_PX 4
#define BASE_VERTICAL_QUIET_ZONE_PX 30
#define MODULES_PER_SYMBOL 11
#define START_CHECK_STOP_SYMBOLS 3

#define ANY_SUBSET -1
#define SUBSET_A 0
#define SUBSET_B 1
#define SUBSET_C 2

#define CODE_A 101
#define CODE_B 100
#define CODE_C 99
#define FNC_1 102
#define FNC_2 97
#define FNC_3 96
#define FNC_4 107
#define SHIFT 98
#define START_A 103
#define START_B 104
#define START_C 105
#define STOP 106

#define PATTERN_WIDTHS_LEN 107

int symbol_buffer[BUFFER_SIZE * 2];

const char *PATTERN_WIDTHS[PATTERN_WIDTHS_LEN] = {
    "212222", "222122", "222221", "121223", "121322", "131222", "122213",
    "122312", "132212", "221213", "221312", "231212", "112232", "122132",
    "122231", "113222", "123122", "123221", "223211", "221132", "221231",
    "213212", "223112", "312131", "311222", "321122", "321221", "312212",
    "322112", "322211", "212123", "212321", "232121", "111323", "131123",
    "131321", "112313", "132113", "132311", "211313", "231113", "231311",
    "112133", "112331", "132131", "113123", "113321", "133121", "313121",
    "211331", "231131", "213113", "213311", "213131", "311123", "311321",
    "331121", "312113", "312311", "332111", "314111", "221411", "431111",
    "111224", "111422", "121124", "121421", "141122", "141221", "112214",
    "112412", "122114", "122411", "142112", "142211", "241211", "221114",
    "413111", "241112", "134111", "111242", "121142", "121241", "114212",
    "124112", "124211", "411212", "421112", "421211", "212141", "214121",
    "412121", "111143", "111341", "131141", "114113", "114311", "411113",
    "411311", "113141", "114131", "311141", "411131", "211412", "211214",
    "211232", "233111"};

int kernighan_ritchie_strlen(const char *s)
{
    int i = 0;
    while (s[i] != '\0')
        ++i;
    return i;
}

bool kernighan_ritchie_strncmp(const char *s1, const char *s2, int n)
{
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0')
            return false;
    }
    return true;
}

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool check_digits(const char *buf, int index, int count, int total_len)
{
    if (index + count > total_len)
        return false;
    for (int i = 0; i < count; i++) {
        if (!is_digit(buf[index + i]))
            return false;
    }
    return true;
}

static int draw_barcode(Dsgl_Canvas *c, int x, int y, const char *pattern_width,
                        int module_width, int h)
{
    int curr_x = x;
    int is_bar = 1;
    for (int i = 0; pattern_width[i] != '\0'; i++) {
        int width_px = (pattern_width[i] - '0') * module_width;
        if (is_bar) {
            dsgl_fill_rect(c, curr_x, y, width_px, h, 0xFF000000);
        }
        curr_x += width_px;
        is_bar = !is_bar;
    }
    return curr_x - x;
}

typedef struct {
    const char *key;
    int len;
    int value;
    int dest_subset;
} Keyword;

#define KEYWORDS_LEN 40
Keyword KEYWORDS[KEYWORDS_LEN] = {
    {"NUL", 3, 64, SUBSET_A},         {"SOH", 3, 65, SUBSET_A},
    {"STX", 3, 66, SUBSET_A},         {"ETX", 3, 67, SUBSET_A},
    {"EOT", 3, 68, SUBSET_A},         {"ENQ", 3, 69, SUBSET_A},
    {"ACK", 3, 70, SUBSET_A},         {"BEL", 3, 71, SUBSET_A},
    {"BS", 2, 72, SUBSET_A},          {"HT", 2, 73, SUBSET_A},
    {"LF", 2, 74, SUBSET_A},          {"VT", 2, 75, SUBSET_A},
    {"FF", 2, 76, SUBSET_A},          {"CR", 2, 77, SUBSET_A},
    {"SO", 2, 78, SUBSET_A},          {"SI", 2, 79, SUBSET_A},
    {"DLE", 3, 80, SUBSET_A},         {"DC1", 3, 81, SUBSET_A},
    {"DC2", 3, 82, SUBSET_A},         {"DC3", 3, 83, SUBSET_A},
    {"DC4", 3, 84, SUBSET_A},         {"NAK", 3, 85, SUBSET_A},
    {"SYN", 3, 86, SUBSET_A},         {"ETB", 3, 87, SUBSET_A},
    {"CAN", 3, 88, SUBSET_A},         {"EM", 2, 89, SUBSET_A},
    {"SUB", 3, 90, SUBSET_A},         {"ESC", 3, 91, SUBSET_A},
    {"FS", 2, 92, SUBSET_A},          {"GS", 2, 93, SUBSET_A},
    {"RS", 2, 94, SUBSET_A},          {"US", 2, 95, SUBSET_A},
    {"DEL", 3, 95, SUBSET_B},         {"FNC1", 4, FNC_1, ANY_SUBSET},
    {"FNC2", 4, FNC_2, ANY_SUBSET},   {"FNC3", 4, FNC_3, ANY_SUBSET},
    {"FNC4", 4, FNC_4, ANY_SUBSET},   {"CODEA", 5, CODE_A, ANY_SUBSET},
    {"CODEB", 5, CODE_B, ANY_SUBSET}, {"CODEC", 5, CODE_C, ANY_SUBSET}};

int match_keyword(const char *data_buffer, int idx)
{
    if (data_buffer[idx] != '^')
        return -1;
    for (int k = 0; k < KEYWORDS_LEN; k++) {
        if (kernighan_ritchie_strncmp(&data_buffer[idx + 1], KEYWORDS[k].key,
                                      KEYWORDS[k].len)) {
            return k;
        }
    }
    return -1;
}

void render(void)
{
    int module_width_px = BASE_MODULE_WIDTH_PX * dpr;
    int bar_height_px = BASE_BAR_HEIGHT_PX * dpr;
    int vertical_quiet_zone_px = BASE_VERTICAL_QUIET_ZONE_PX * dpr;
    int horizontal_quiet_zone_px = 10 * module_width_px;
    int data_len = kernighan_ritchie_strlen(data_buffer);
    int next_symbol_idx = 0;
    int next_input_idx = 0;
    int curr_subset = SUBSET_B;
    if (check_digits(data_buffer, 0, 4, data_len)) {
        curr_subset = SUBSET_C;
        symbol_buffer[next_symbol_idx++] = START_C;
    } else {
        int keyword_idx = match_keyword(data_buffer, 0);
        if (keyword_idx != -1 &&
            KEYWORDS[keyword_idx].dest_subset == SUBSET_A) {
            curr_subset = SUBSET_A;
            symbol_buffer[next_symbol_idx++] = START_A;
        } else {
            curr_subset = SUBSET_B;
            symbol_buffer[next_symbol_idx++] = START_B;
        }
    }
    while (next_input_idx < data_len) {
        int keyword_idx = match_keyword(data_buffer, next_input_idx);
        if (keyword_idx != -1) {
            Keyword k = KEYWORDS[keyword_idx];
            if (k.dest_subset != ANY_SUBSET && k.dest_subset != curr_subset) {
                if (k.dest_subset == SUBSET_A) {
                    symbol_buffer[next_symbol_idx++] = CODE_A;
                    curr_subset = SUBSET_A;
                } else if (k.dest_subset == SUBSET_B) {
                    symbol_buffer[next_symbol_idx++] = CODE_B;
                    curr_subset = SUBSET_B;
                }
            }
            if (k.value == CODE_A)
                curr_subset = SUBSET_A;
            if (k.value == CODE_B)
                curr_subset = SUBSET_B;
            if (k.value == CODE_C)
                curr_subset = SUBSET_C;
            if (k.value == FNC_4) {
                if (curr_subset == SUBSET_A) {
                    symbol_buffer[next_symbol_idx++] = 101;
                } else if (curr_subset == SUBSET_B) {
                    symbol_buffer[next_symbol_idx++] = 100;
                } else {
                    symbol_buffer[next_symbol_idx++] = CODE_B;
                    curr_subset = SUBSET_B;
                    symbol_buffer[next_symbol_idx++] = 100;
                }
            } else {
                symbol_buffer[next_symbol_idx++] = k.value;
            }
            next_input_idx += (1 + k.len);
            continue;
        }
        if (curr_subset == SUBSET_C) {
            if (check_digits(data_buffer, next_input_idx, 2, data_len)) {
                int d1 = data_buffer[next_input_idx] - '0';
                int d2 = data_buffer[next_input_idx + 1] - '0';
                symbol_buffer[next_symbol_idx++] = (d1 * 10) + d2;
                next_input_idx += 2;
            } else {
                symbol_buffer[next_symbol_idx++] = CODE_B;
                curr_subset = SUBSET_B;
            }
        } else {
            bool is_escape_ahead =
                (match_keyword(data_buffer, next_input_idx) != -1);
            if (!is_escape_ahead &&
                check_digits(data_buffer, next_input_idx, 4, data_len)) {
                symbol_buffer[next_symbol_idx++] = CODE_C;
                curr_subset = SUBSET_C;
                continue;
            }
            char c = data_buffer[next_input_idx];
            if (curr_subset == SUBSET_B) {
                if (c >= 0 && c <= 31) {
                    bool next_is_ctrl = false;
                    if (next_input_idx + 1 < data_len) {
                        char next_c = data_buffer[next_input_idx + 1];
                        if (next_c >= 0 && next_c <= 31)
                            next_is_ctrl = true;
                    }
                    if (next_is_ctrl) {
                        symbol_buffer[next_symbol_idx++] = CODE_A;
                        curr_subset = SUBSET_A;
                        symbol_buffer[next_symbol_idx++] = c + 64;
                    } else {
                        symbol_buffer[next_symbol_idx++] = SHIFT;
                        symbol_buffer[next_symbol_idx++] = c + 64;
                    }
                    next_input_idx++;
                } else if (c >= 32 && c <= 127) {
                    symbol_buffer[next_symbol_idx++] = c - 32;
                    next_input_idx++;
                } else {
                    symbol_buffer[next_symbol_idx++] = 0;
                    next_input_idx++;
                }
            } else if (curr_subset == SUBSET_A) {
                if (c >= 97 && c <= 122) {
                    bool next_is_lower = false;
                    if (next_input_idx + 1 < data_len) {
                        char next_c = data_buffer[next_input_idx + 1];
                        if (next_c >= 97 && next_c <= 122)
                            next_is_lower = true;
                    }
                    if (next_is_lower) {
                        symbol_buffer[next_symbol_idx++] = CODE_B;
                        curr_subset = SUBSET_B;
                        symbol_buffer[next_symbol_idx++] = c - 32;
                    } else {
                        symbol_buffer[next_symbol_idx++] = SHIFT;
                        symbol_buffer[next_symbol_idx++] = c - 32;
                    }
                    next_input_idx++;
                } else if (c >= 32 && c <= 95) {
                    symbol_buffer[next_symbol_idx++] = c - 32;
                    next_input_idx++;
                } else if (c >= 0 && c <= 31) {
                    symbol_buffer[next_symbol_idx++] = c + 64;
                    next_input_idx++;
                } else if (c >= 96 && c <= 127) {
                    symbol_buffer[next_symbol_idx++] = CODE_B;
                    curr_subset = SUBSET_B;
                    continue;
                } else {
                    next_input_idx++;
                }
            }
        }
    }
    long dividend = symbol_buffer[0];
    for (int i = 1; i < next_symbol_idx; i++) {
        dividend += (symbol_buffer[i] * i);
    }
    symbol_buffer[next_symbol_idx++] = dividend % 103;
    symbol_buffer[next_symbol_idx++] = STOP;
    int total_modules = (next_symbol_idx * MODULES_PER_SYMBOL) + 2;
    canvas_width =
        total_modules * module_width_px + (2 * horizontal_quiet_zone_px);
    canvas_height = bar_height_px + (2 * vertical_quiet_zone_px);
    Dsgl_Canvas c = dsgl_create_canvas(pixels, canvas_width, canvas_height);
    dsgl_fill_rect(&c, 0, 0, canvas_width, canvas_height, 0xFFFFFFFF);
    int curr_x = horizontal_quiet_zone_px;
    int curr_y = vertical_quiet_zone_px;
    for (int i = 0; i < next_symbol_idx; i++) {
        int value = symbol_buffer[i];
        curr_x += draw_barcode(&c, curr_x, curr_y, PATTERN_WIDTHS[value],
                               module_width_px, bar_height_px);
    }
    dsgl_fill_rect(&c, curr_x, curr_y, 2 * module_width_px, bar_height_px,
                   0xFF000000);
}

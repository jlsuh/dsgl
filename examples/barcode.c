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
void set_dpr(int val)
{
    if (val < 1)
        val = 1;
    if (val > 4)
        val = 4;
    dpr = val;
}

#define MAX_WIDTH 14000
#define MAX_HEIGHT 1200

uint32_t pixels[MAX_WIDTH * MAX_HEIGHT];
uint32_t *get_pixel_buffer(void)
{
    return pixels;
}

#define BASE_MODULE_WIDTH 4
#define BASE_BARCODE_HEIGHT 160
#define BASE_QUIET_V 30
#define MODULES_PER_SYMBOL 11
#define START_CHECK_STOP_SYMBOLS 3
#define SUBSET_B 1
#define SUBSET_C 2
#define WIDTHS_LEN 107

int symbol_buffer[BUFFER_SIZE * 2];

const char *WIDTHS[WIDTHS_LEN] = {
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

int kernighan_ritchie_strlen(char s[])
{
    int i = 0;
    while (s[i] != '\0')
        ++i;
    return i;
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

static int draw_barcode(Dsgl_Canvas *c, int x, int y, const char *pattern,
                        int mod_w, int h)
{
    int current_x = x;
    int is_bar = 1;
    for (int i = 0; pattern[i] != '\0'; i++) {
        int width_px = (pattern[i] - '0') * mod_w;
        if (is_bar) {
            dsgl_fill_rect(c, current_x, y, width_px, h, 0xFF000000);
        }
        current_x += width_px;
        is_bar = !is_bar;
    }
    return current_x - x;
}

void render(void)
{
    int mod_w = BASE_MODULE_WIDTH * dpr;
    int bar_h = BASE_BARCODE_HEIGHT * dpr;
    int quiet_v = BASE_QUIET_V * dpr;
    int quiet_h = (MODULES_PER_SYMBOL * mod_w);
    int data_len = kernighan_ritchie_strlen(data_buffer);
    int next_symbol_idx = 0;
    int next_input_idx = 0;
    int current_subset = SUBSET_B;
    if (check_digits(data_buffer, 0, 4, data_len)) {
        current_subset = SUBSET_C;
        symbol_buffer[next_symbol_idx++] = 105;
    } else {
        current_subset = SUBSET_B;
        symbol_buffer[next_symbol_idx++] = 104;
    }
    while (next_input_idx < data_len) {
        if (current_subset == SUBSET_B) {
            if (check_digits(data_buffer, next_input_idx, 4, data_len)) {
                symbol_buffer[next_symbol_idx++] = 99;
                current_subset = SUBSET_C;
            } else {
                char c = data_buffer[next_input_idx];
                if (c >= 32 && c <= 127) {
                    symbol_buffer[next_symbol_idx++] = c - 32;
                } else {
                    symbol_buffer[next_symbol_idx++] = 0;
                }
                next_input_idx++;
            }
        } else if (current_subset == SUBSET_C) {
            if (check_digits(data_buffer, next_input_idx, 2, data_len)) {
                int d1 = data_buffer[next_input_idx] - '0';
                int d2 = data_buffer[next_input_idx + 1] - '0';
                int value = (d1 * 10) + d2;
                symbol_buffer[next_symbol_idx++] = value;
                next_input_idx += 2;
            } else {
                symbol_buffer[next_symbol_idx++] = 100;
                current_subset = SUBSET_B;
            }
        }
    }
    long checksum = symbol_buffer[0];
    for (int i = 1; i < next_symbol_idx; i++) {
        checksum += (symbol_buffer[i] * i);
    }
    symbol_buffer[next_symbol_idx++] = checksum % 103;
    symbol_buffer[next_symbol_idx++] = 106;
    int data_symbols = next_symbol_idx - START_CHECK_STOP_SYMBOLS;
    int total_modules = 57 + (data_symbols * MODULES_PER_SYMBOL);
    int calculated_width = total_modules * mod_w;
    int calculated_height = bar_h + (2 * quiet_v);
    if (calculated_width > MAX_WIDTH || calculated_height > MAX_HEIGHT) {
        canvas_width = 0;
        canvas_height = 0;
        return;
    }
    canvas_width = calculated_width;
    canvas_height = calculated_height;
    Dsgl_Canvas c = dsgl_create_canvas(pixels, canvas_width, canvas_height);
    dsgl_fill_rect(&c, 0, 0, canvas_width, canvas_height, 0xFFFFFFFF);
    int cursor_x = quiet_h;
    int cursor_y = quiet_v;
    for (int i = 0; i < next_symbol_idx; i++) {
        int value = symbol_buffer[i];
        cursor_x +=
            draw_barcode(&c, cursor_x, cursor_y, WIDTHS[value], mod_w, bar_h);
    }
    dsgl_fill_rect(&c, cursor_x, cursor_y, 2 * mod_w, bar_h, 0xFF000000);
}

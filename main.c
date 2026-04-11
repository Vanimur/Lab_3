#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char *convertStrToLongBv(char *str, size_t *cells){
    if (str == NULL || cells == NULL) return NULL;
    size_t len = 0, ix = 0;
    len = strlen(str);

    if (len == 0) {
        *cells = 0;
        return NULL;
    }

    *cells = ((len - 1) / 8) + 1;

    unsigned char *vec = (unsigned char *)calloc(*cells, sizeof(unsigned char));
    if (vec == NULL) return NULL;

    for (size_t i = 0; i < *cells; i++) {
        unsigned char mask = 1;

        for (int j = 0; (j < 8) && (ix < len); j++){
            if (str[ix] != '0'){
                vec[i] = vec[i] | mask;
            }
            mask = mask << 1;
            ix++;
        }
    }
    return vec;
}


unsigned char *convertLongBvToStr(unsigned char *vec, size_t sz){
    size_t ix = 0;
    if (vec == NULL) return NULL;

    if (sz == 0) {
        return NULL;
    }

    size_t len = 8 * sz + 1;
    unsigned char *str = (unsigned char*)calloc(len, sizeof(unsigned char));
    for (size_t i = 0; i < sz; i++){
        unsigned char mask = 1;
        for (int j = 0; (j < 8) && (ix < len); j++){
            if ((vec[i] & mask) != 0)
                str[ix] = '1';
            else
                str[ix] = '0';
            mask = mask << 1;
            ix++;
        }
    }
    return str;
}


void set_bit_1(unsigned char *vec, size_t len, size_t k){
    size_t byte = 0, bit =0;
    unsigned char mask = 1;

    if (k >= len) return;

    byte = k / 8;
    bit = k % 8;

    mask = mask << bit;

    vec[byte] = vec[byte] | mask;
}


void set_bit_0(unsigned char *vec, size_t len, size_t k){
    size_t byte = 0, bit =0;
    unsigned char mask = 1;

    if (k >= len) return;

    byte = k / 8;
    bit = k % 8;

    mask = ~(mask << bit);

    vec[byte] = vec[byte] & mask;
}


int main()
{
    char str[20] = "";
    size_t vector_cells;
    unsigned char *vector = convertStrToLongBv(str, &vector_cells);
    unsigned char *str_vector = convertLongBvToStr(vector, vector_cells);
    printf("%.*s", ((vector_cells + 7) / 8), str_vector);

    free(vector);
    vector = NULL;
    free(str_vector);
    str_vector = NULL;
    return 0;
}

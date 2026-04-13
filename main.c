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


unsigned char *convertLongBvToStr(unsigned char *vec, size_t cells){
    size_t ix = 0;
    if (vec == NULL) return NULL;

    if (cells == 0) {
        return NULL;
    }

    size_t len = 8 * cells + 1;
    unsigned char *str = (unsigned char*)calloc(len, sizeof(unsigned char));
    for (size_t i = 0; i < cells; i++){
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


unsigned char *logMul(unsigned char *vecA, size_t lenA, unsigned char *vecB, size_t lenB){
    if (vecA == NULL || vecB == NULL) return NULL;
    if (lenA != lenB) return NULL;

    size_t cells = (lenA + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = vecA[i] & vecB[i];
    }
    return result;
}


unsigned char *logSub(unsigned char *vecA, size_t lenA, unsigned char *vecB, size_t lenB){
    if (vecA == NULL || vecB == NULL) return NULL;
    if (lenA != lenB) return NULL;

    size_t cells = (lenA + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = vecA[i] | vecB[i];
    }
    return result;
}


unsigned char *logXor(unsigned char *vecA, size_t lenA, unsigned char *vecB, size_t lenB){
    if (vecA == NULL || vecB == NULL) return NULL;
    if (lenA != lenB) return NULL;

    size_t cells = (lenA + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = vecA[i] ^ vecB[i];
    }
    return result;
}


unsigned char *logNot(unsigned char *vec, size_t len){
    if (vec == NULL) return NULL;

    size_t cells = (len + 7) / 8;

    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    for (size_t i = 0; i < cells; i++){
        result[i] = ~vec[i];
    }

    size_t xvost_bit = len % 8;
    if (xvost_bit != 0){
        unsigned char mask = 1;
        mask = (mask << xvost_bit) - 1;
        result[cells - 1] = result[cells - 1] & mask;
    }

    return result;
}


unsigned char *logShiftLeft(unsigned char *vec, size_t len, size_t n) {
    if (vec == NULL) return NULL;

    size_t cells = (len + 7) / 8;
    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    if (n >= len) return result;

    size_t byte_shift = n / 8;
    size_t bit_shift = n % 8;

    for (size_t i = 0; i < cells; i++) {
        unsigned char current = 0;

        if (i + byte_shift < cells) {
            current = vec[i + byte_shift] >> bit_shift;
        }

        if (bit_shift > 0 && (i + byte_shift + 1 < cells)) {
            unsigned char transfer = vec[i + byte_shift + 1] << (8 - bit_shift);
            current = current | transfer;
        }

        result[i] = current;
    }

    size_t xvost_bit = len % 8;
    if (xvost_bit != 0) {
        unsigned char mask = (1 << xvost_bit) - 1;
        result[cells - 1] = result[cells - 1] & mask;
    }

    return result;
}


unsigned char *logShiftRight(unsigned char *vec, size_t len, size_t n) {
    if (vec == NULL) return NULL;

    size_t cells = (len + 7) / 8;
    unsigned char *result = (unsigned char*)calloc(cells, sizeof(unsigned char));
    if (result == NULL) return NULL;

    if (n >= len) return result;

    size_t byte_shift = n / 8;
    size_t bit_shift = n % 8;

    for (size_t i = cells - 1; i < cells; i--) {
        unsigned char current = 0;

        if (i >= byte_shift) {
            current = vec[i - byte_shift] << bit_shift;
        }

        if (bit_shift > 0 && (i > byte_shift)) {
            unsigned char transfer = vec[i - byte_shift - 1] >> (8 - bit_shift);
            current = current | transfer;
        }

        result[i] = current;
    }

    size_t xvost_bit = len % 8;
    if (xvost_bit != 0) {
        unsigned char mask = (1 << xvost_bit) - 1;
        result[cells - 1] = result[cells - 1] & mask;
    }

    return result;
}

int main()
{
    char str[20] = "1111111111";
    size_t vector_cells;
    unsigned char *vector = convertStrToLongBv(str, &vector_cells);
    //unsigned char *sdvig ="%s" logShiftLeft(vector, strlen(str), 1);
    unsigned char *sdvig = logShiftRight(vector, strlen(str), 0);
    set_bit_0(sdvig, strlen(str), 3);
    unsigned char *str_vector = convertLongBvToStr(sdvig, vector_cells);
    printf("%s", str_vector);

    free(vector);
    vector = NULL;
    free(str_vector);
    str_vector = NULL;
    free(sdvig);
    sdvig = NULL;
    return 0;
}
